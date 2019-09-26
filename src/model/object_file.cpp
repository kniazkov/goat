/*

Copyright (C) 17..19 Ivan Kniazkov

This file is part of interpreter of programming language
codenamed "Goat" ("Goat interpreter").

Goat interpreter is free software: you can redistribute it and/or
modify it under the terms of the GNU General Public License as published
by the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Goat interpreter is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with Goat interpreter.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "object_file.h"
#include "object_string.h"
#include "object_function_built_in.h"
#include "object_byte_array.h"
#include "lib/utils.h"
#include "lib/assert.h"
#include "resource/strings.h"
#include <cstdio>

namespace g0at
{
    namespace model
    {
        object_file::object_file(object_pool *_pool, file_descriptor _descr)
            : object(_pool, _pool->get_file_proto_instance()), descr(_descr)
        {
        }

        object_file::~object_file()
        {
            if (descr.sysctl)
            {
                FILE *stream = (FILE*)descr.sysctl;
                fclose(stream);
            }
        }

        object_file *object_file::to_object_file()
        {
            return this;
        }

        std::wstring object_file::to_string() const
        {
            return resource::str_file;
        }

        /*
            Prototype
        */

        class object_file_method : public object_function_built_in
        {
        public:
            object_file_method(object_pool *_pool)
                : object_function_built_in(_pool)
            {
            }
            
            void call(thread *thr, int arg_count, call_mode mode) override
            {
                if (mode != call_mode::as_method)
                {
                    thr->raise_exception(thr->pool->get_exception_illegal_context_instance());
                    return;
                }
                object *this_ptr = thr->pop().get_object();
                assert(this_ptr != nullptr);
                object_file *this_ptr_file = this_ptr->to_object_file();
                if (!this_ptr_file)
                {
                    thr->raise_exception(thr->pool->get_exception_illegal_context_instance());
                    return;
                }
                file_descriptor *descr = this_ptr_file->get_descriptor();
                FILE *stream = (FILE*)descr->sysctl;
                variable result;
                if (payload(thr, arg_count, descr, stream, &result))
                {
                    thr->pop(arg_count);
                    thr->push(result);
                }
            }

            virtual bool payload(thread *thr, int arg_count, file_descriptor *descr, FILE *stream, variable *result) = 0;
        };


        class object_file_read : public object_file_method
        {
        public:
            object_file_read(object_pool *_pool)
                : object_file_method(_pool)
            {
            }
            
            bool payload(thread *thr, int arg_count, file_descriptor *descr, FILE *stream, variable *result) override
            {
                if (stream && descr->mode == file_access_mode::read || descr->mode == file_access_mode::full)
                {
                    if (arg_count < 1)
                    {
                        thr->raise_exception(thr->pool->get_exception_illegal_argument_instance());
                        return false;
                    }
                    variable &count = thr->peek();
                    int64_t count_int64;
                    if (!count.get_integer(&count_int64) || count_int64 > INT32_MAX)
                    {
                        thr->raise_exception(thr->pool->get_exception_illegal_argument_instance());
                        return false;
                    }
                    if (count_int64 <= 0)
                    {
                        result->set_object(thr->pool->get_undefined_instance());
                        return true;
                    }
                    size_t bytes_to_read = (size_t)count_int64;
                    uint8_t *buff = new uint8_t[bytes_to_read];
                    size_t actually_read = std::fread(buff, 1, bytes_to_read, stream);
                    result->set_object(new object_byte_array(thr->pool, buff, actually_read));
                    delete[] buff;
                }
                assert(false);
            }
        };

        object_file_proto::object_file_proto(object_pool *pool)
            : object(pool)
        {
        }

        void object_file_proto::init(object_pool *pool)
        {
            add_object(pool->get_static_string(resource::str_read), new object_file_read(pool));
        }
    };
};
