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

#include "context_factory.h"
#include "model/object_function_built_in.h"
#include "model/object_string.h"
#include "model/object_dynamic_library.h"
#include "model/object_exception.h"
#include "resource/strings.h"
#include "lib/utils.h"
#include "lib/assert.h"

namespace g0at
{
    namespace model
    {
        namespace built_in
        {
            class dlopen : public object_function_built_in
            {
            public:
                dlopen(object_pool *_pool)
                    : object_function_built_in(_pool)
                {
                }
                
                void call(thread *thr, int arg_count, call_mode fcall_mode) override
                {
                    if (arg_count > 0)
                    {
                        if (fcall_mode == call_mode::as_method)
                            thr->pop();
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
                                    lib::dll_loader *loader = lib::dll_loader::open(file_name_ascii.c_str());
                                    if (loader)
                                        result.set_object(new object_dynamic_library(thr->pool, loader));
                                    else
                                        result.set_object(thr->pool->get_undefined_instance());
                                    thr->push(result);
                                    return;
                                } // is_ascii
                            } // arg_fname_obj_str
                        } // arg_fname_obj
                    } // arg_count > 0
                    thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                }
            };

            object *context_factory::create_function_dlopen()
            {
                return new dlopen(pool);
            }
        };
    };
};
