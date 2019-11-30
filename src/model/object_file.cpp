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
#include "object_exception.h"
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
                    thr->raise_exception(new object_exception_illegal_context(thr->pool));
                    return;
                }
                object *this_ptr = thr->pop().get_object();
                assert(this_ptr != nullptr);
                object_file *this_ptr_file = this_ptr->to_object_file();
                if (!this_ptr_file)
                {
                    thr->raise_exception(new object_exception_illegal_context(thr->pool));
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
                if (stream && (descr->mode == file_access_mode::read || descr->mode == file_access_mode::full))
                {
                    if (arg_count < 1)
                    {
                        thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                        return false;
                    }
                    variable &count = thr->peek();
                    int64_t count_int64;
                    if (!count.get_integer(&count_int64) || count_int64 > INT32_MAX)
                    {
                        thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                        return false;
                    }
                    if (count_int64 <= 0)
                    {
                        result->set_object(thr->pool->get_null_instance());
                        return true;
                    }
                    size_t bytes_to_read = (size_t)count_int64;
                    uint8_t *buff = new uint8_t[bytes_to_read];
                    size_t actually_read = std::fread(buff, 1, bytes_to_read, stream);
                    result->set_object(new object_byte_array(thr->pool, buff, actually_read));
                    delete[] buff;
                    return true;
                }
                else
                {
                    thr->raise_exception(new object_exception_illegal_operation(thr->pool));
                    return false;
                }
            }
        };

        class object_file_write : public object_file_method
        {
        public:
            object_file_write(object_pool *_pool)
                : object_file_method(_pool)
            {
            }
            
            bool payload(thread *thr, int arg_count, file_descriptor *descr, FILE *stream, variable *result) override
            {
                if (stream && (descr->mode == file_access_mode::write ||
                    descr->mode == file_access_mode::append || descr->mode == file_access_mode::full))
                {
                    if (arg_count > 0)
                    {
                        object *data = thr->peek().get_object();
                        if (data) {
                            object_byte_array *byte_array = data->to_object_byte_array();
                            if (byte_array) {
                                size_t actually_written = std::fwrite(byte_array->get_data(), 1, byte_array->get_length(), stream);
                                result->set_integer(actually_written);
                                return true;
                            }
                        }
                    }
                    thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                }
                else
                {
                    thr->raise_exception(new object_exception_illegal_operation(thr->pool));
                }
                return false;
            }
        };

        class object_file_flush : public object_file_method
        {
        public:
            object_file_flush(object_pool *_pool)
                : object_file_method(_pool)
            {
            }
            
            bool payload(thread *thr, int arg_count, file_descriptor *descr, FILE *stream, variable *result) override
            {
                if (stream && (descr->mode == file_access_mode::write ||
                    descr->mode == file_access_mode::append || descr->mode == file_access_mode::full))
                {
                    int flush_result = fflush(stream);
                    result->set_boolean(flush_result == 0);
                    return true;
                }
                else
                {
                    thr->raise_exception(new object_exception_illegal_operation(thr->pool));
                    return false;
                }
            }
        };

        class object_file_close : public object_file_method
        {
        public:
            object_file_close(object_pool *_pool)
                : object_file_method(_pool)
            {
            }
            
            bool payload(thread *thr, int arg_count, file_descriptor *descr, FILE *stream, variable *result) override
            {
                if (stream)
                {
                    std::fclose(stream);
                    descr->mode == file_access_mode::closed;
                    descr->sysctl = nullptr;
                }
                result->set_object(thr->pool->get_undefined_instance());
                return true;
            }
        };

        class object_file_seek : public object_file_method
        {
        public:
            object_file_seek(object_pool *_pool)
                : object_file_method(_pool)
            {
            }
            
            bool payload(thread *thr, int arg_count, file_descriptor *descr, FILE *stream, variable *result) override
            {
                bool ret_val = false;
                if (stream && descr->mode != file_access_mode::closed && descr->mode != file_access_mode::append)
                {
                    if (arg_count < 1)
                    {
                        thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                        return false;
                    }
                    variable &position = thr->peek(0);                    
                    int64_t position_int64;
                    if (!position.get_integer(&position_int64) || position_int64 < 0)
                    {
                        thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                        return false;
                    }
                    bool has_origin; 
                    int origin;
                    if (arg_count > 1)
                    {
                        has_origin = false;
                        variable &origin_arg = thr->peek(1);
                        object *origin_obj = origin_arg.get_object();
                        if (origin_obj)
                        {
                            object_string *origin_obj_str = origin_obj->to_object_string();
                            if (origin_obj_str)
                            {
                                std::wstring origin_str = origin_obj_str->get_data();
                                if (origin_str == resource::str_BEGIN)
                                {
                                    origin = SEEK_SET;
                                    has_origin = true;
                                }
                                else if (origin_str == resource::str_END)
                                {
                                    origin = SEEK_END;
                                    has_origin = true;
                                }
                                else if (origin_str == resource::str_CURRENT)
                                {
                                    origin = SEEK_CUR;
                                    has_origin = true;
                                }
                            }
                        }
                    }
                    else
                    {
                        has_origin = true;
                        origin = SEEK_SET;
                    }
                    if (!has_origin)
                    {
                        thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                        return false;
                    }
                    int seek_result = std::fseek(stream, position_int64, origin);
                    ret_val = (seek_result == 0);
                }
                result->set_boolean(ret_val);
                return true;
            }
        };

        class object_file_position : public object_file_method
        {
        public:
            object_file_position(object_pool *_pool)
                : object_file_method(_pool)
            {
            }
            
            bool payload(thread *thr, int arg_count, file_descriptor *descr, FILE *stream, variable *result) override
            {
                int64_t position = -1;
                if (stream)
                {
                    position = std::ftell(stream);
                }
                result->set_integer(position);
                return true;
            }
        };

        class object_file_eof : public object_file_method
        {
        public:
            object_file_eof(object_pool *_pool)
                : object_file_method(_pool)
            {
            }
            
            bool payload(thread *thr, int arg_count, file_descriptor *descr, FILE *stream, variable *result) override
            {
                bool ret_val = true;
                if (stream)
                {
                    ret_val = (std::feof(stream) != 0);
                }
                result->set_boolean(ret_val);
                return true;
            }
        };

        class object_file_getc : public object_file_method
        {
        public:
            object_file_getc(object_pool *_pool)
                : object_file_method(_pool)
            {
            }
            
            bool payload(thread *thr, int arg_count, file_descriptor *descr, FILE *stream, variable *result) override
            {
                if (stream)
                {
                    int ret_val = std::fgetc(stream);
                    if (ret_val != EOF)
                    {
                        result->set_integer(ret_val & 0xFF);
                        return true;
                    }
                }
                result->set_object(thr->pool->get_null_instance());
                return true;
            }
        };

        object_file_proto::object_file_proto(object_pool *pool)
            : object(pool)
        {
        }

        void object_file_proto::init(object_pool *pool)
        {
            object *mode = pool->create_generic_object();
            mode->add_object(pool->get_static_string(resource::str_READ));
            mode->add_object(pool->get_static_string(resource::str_WRITE));
            mode->add_object(pool->get_static_string(resource::str_APPEND));
            mode->add_object(pool->get_static_string(resource::str_FULL));
            mode->lock();

            object *origin = pool->create_generic_object();
            origin->add_object(pool->get_static_string(resource::str_BEGIN));
            origin->add_object(pool->get_static_string(resource::str_END));
            origin->add_object(pool->get_static_string(resource::str_CURRENT));
            origin->lock();

            add_object(pool->get_static_string(resource::str_Mode), mode);
            add_object(pool->get_static_string(resource::str_Origin), origin);
            add_object(pool->get_static_string(resource::str_eof), new object_file_eof(pool));
            add_object(pool->get_static_string(resource::str_read), new object_file_read(pool));
            add_object(pool->get_static_string(resource::str_write), new object_file_write(pool));
            add_object(pool->get_static_string(resource::str_flush), new object_file_flush(pool));
            add_object(pool->get_static_string(resource::str_getc), new object_file_getc(pool));
            add_object(pool->get_static_string(resource::str_seek), new object_file_seek(pool));
            add_object(pool->get_static_string(resource::str_position), new object_file_position(pool));
            add_object(pool->get_static_string(resource::str_close), new object_file_close(pool));
            lock();
        }

        void object_file_proto::op_new(thread *thr, int arg_count)
        {
            thr->raise_exception(new object_exception_illegal_operation(thr->pool));
        }
    };
};
