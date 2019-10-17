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

#include "context_factory.h"
#include "model/object_function_built_in.h"
#include "model/object_string.h"
#include "model/object_file.h"
#include "model/object_exception.h"
#include "resource/strings.h"
#include "lib/utils.h"
#include "lib/assert.h"
#include <cstdio>

namespace g0at
{
    namespace model
    {
        namespace built_in
        {
            class open : public object_function_built_in
            {
            public:
                open(object_pool *_pool)
                    : object_function_built_in(_pool)
                {
                }
                
                void call(thread *thr, int arg_count, call_mode fcall_mode) override
                {
                    if (arg_count > 0)
                    {
                        file_access_mode access_mode;
                        if (fcall_mode == call_mode::as_method)
                            thr->pop();
                        if (arg_count > 1)
                        {
                            access_mode = file_access_mode::invalid;
                            variable &arg_mode = thr->peek(1);
                            object *arg_mode_obj = arg_mode.get_object();
                            if (arg_mode_obj)
                            {
                                object_string *arg_mode_obj_str = arg_mode_obj->to_object_string();
                                if (arg_mode_obj_str)
                                {
                                    std::wstring mode_wstr = arg_mode_obj_str->get_data();
                                    if (mode_wstr == resource::str_READ)
                                        access_mode = file_access_mode::read;
                                    else if (mode_wstr == resource::str_WRITE)
                                        access_mode = file_access_mode::write;
                                    else if (mode_wstr == resource::str_APPEND)
                                        access_mode = file_access_mode::append;
                                    else if (mode_wstr == resource::str_FULL)
                                        access_mode = file_access_mode::full;
                                }
                            }
                        }
                        else
                        {
                            access_mode = file_access_mode::full;
                        }
                        if (access_mode != file_access_mode::invalid)
                        {
                            variable &arg_fname = thr->peek(0);
                            thr->pop(arg_count);
                            object *arg_fname_obj = arg_fname.get_object();
                            if (arg_fname_obj)
                            {
                                object_string *arg_fname_obj_str = arg_fname_obj->to_object_string();
                                if (arg_fname_obj_str)
                                {
                                    std::wstring file_name = arg_fname_obj_str->get_data();
                                    bool is_ascii;
                                    std::string file_name_ascii = lib::wstring_to_ascii_string(file_name, &is_ascii);
                                    if (is_ascii)
                                    {
                                        variable result;
                                        FILE *stream;
                                        switch(access_mode)
                                        {
                                            case file_access_mode::read:
                                                stream = std::fopen(file_name_ascii.c_str(), "r");
                                                break;
                                            case file_access_mode::write:
                                                stream = std::fopen(file_name_ascii.c_str(), "w");
                                                break;
                                            case file_access_mode::append:
                                                stream = std::fopen(file_name_ascii.c_str(), "a");
                                                break;
                                            case file_access_mode::full:
                                                stream = std::fopen(file_name_ascii.c_str(), "r+");
                                                break;
                                            default:
                                                assert(false);
                                        }
                                        if (!stream)
                                        {
                                            result.set_object(thr->pool->get_null_instance());
                                        }
                                        else
                                        {
                                            file_descriptor descr;
                                            descr.sysctl = (void*)stream;
                                            descr.name = file_name_ascii;
                                            descr.mode = access_mode;
                                            result.set_object(new object_file(thr->pool, descr));
                                        } // stream
                                        thr->push(result);
                                        return;
                                    } // is_ascii
                                } // arg_fname_obj_str
                            } // arg_fname_obj
                        } // access_mode != file_access_mode::invalid
                    } // arg_count > 0
                    thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                }
            };

            object *context_factory::create_function_open()
            {
                return new open(pool);
            }
        };
    };
};
