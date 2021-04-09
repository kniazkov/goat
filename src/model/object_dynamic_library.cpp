/*

Copyright (C) 2021 Ivan Kniazkov

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

#include "object_dynamic_library.h"
#include "object_string.h"
#include "object_exception.h"
#include "object_function_built_in.h"
#include "object_function_dll.h"
#include "lib/utils.h"
#include "lib/assert.h"
#include "resource/strings.h"

namespace g0at
{
    namespace model
    {
        object_dynamic_library::object_dynamic_library(object_pool *_pool, lib::dll_loader *_loader)
            : object(_pool, _pool->get_dynamic_library_proto_instance()), loader(_loader)
        {
        }

        object_dynamic_library::~object_dynamic_library()
        {
            delete loader;
        }

        object_dynamic_library *object_dynamic_library::to_object_dynamic_library()
        {
            return this;
        }

        std::wstring object_dynamic_library::to_string() const
        {
            return resource::str_dll;
        }

        /*
            Prototype
        */

        class object_dynamic_library_method : public object_function_built_in
        {
        public:
            object_dynamic_library_method(object_pool *_pool)
                : object_function_built_in(_pool)
            {
            }
            
            void call(thread *thr, int arg_count, call_mode mode) override
            {
                if (mode != call_mode::as_method)
                {
                    thr->raise_exception(new object_exception_illegal_context(thr->pool));
                    return;
                }
                object *this_ptr = thr->pop().get_object();
                assert(this_ptr != nullptr);
                object_dynamic_library *this_ptr_dll = this_ptr->to_object_dynamic_library();
                if (!this_ptr_dll)
                {
                    thr->raise_exception(new object_exception_illegal_context(thr->pool));
                    return;
                }
                variable result;
                if (payload(thr, arg_count, this_ptr_dll, &result))
                {
                    thr->pop(arg_count);
                    thr->push(result);
                }
            }

            virtual bool payload(thread *thr, int arg_count, object_dynamic_library *library, variable *result) = 0;
        };

        class object_dynamic_library_find : public object_dynamic_library_method
        {
        public:
            object_dynamic_library_find(object_pool *_pool)
                : object_dynamic_library_method(_pool)
            {
            }
            
            bool payload(thread *thr, int arg_count, object_dynamic_library *library, variable *result) override
            {
                if (arg_count > 0)
                {
                    variable &arg_fname = thr->peek(0);
                    object *arg_fname_obj = arg_fname.get_object();
                    if (arg_fname_obj)
                    {
                        object_string *arg_fname_obj_str = arg_fname_obj->to_object_string();
                        if (arg_fname_obj_str)
                        {
                            std::wstring func_name = arg_fname_obj_str->get_data();
                            bool is_ascii;
                            std::string func_name_ascii = lib::wstring_to_ascii_string(func_name, &is_ascii);
                            if (is_ascii)
                            {
                                void *func = library->get_loader()->load(func_name_ascii.c_str());
                                if (func)
                                    result->set_object(new object_function_dll(thr->pool, library, func));
                                else
                                    result->set_object(thr->pool->get_null_instance());
                                return true;
                            }
                        }
                    }
                }
                thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                return false;
            }
        };

        object_dynamic_library_proto::object_dynamic_library_proto(object_pool *pool)
            : object(pool)
        {
        }

        void object_dynamic_library_proto::init(object_pool *pool)
        {
            add_object(pool->get_static_string(resource::str_find), new object_dynamic_library_find(pool));
            lock();
        }

        void object_dynamic_library_proto::op_new(thread *thr, int arg_count)
        {
            thr->raise_exception(new object_exception_illegal_operation(thr->pool));
        }
    };
};
