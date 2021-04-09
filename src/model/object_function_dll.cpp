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

#include "object_function_dll.h"
#include "object_dynamic_library.h"
#include "lib/fast_allocator.h"

namespace g0at
{
    namespace model
    {
        object_function_dll::object_function_dll(object_pool *_pool, object_dynamic_library *_library, goat_ext_function _ext_func)
            : object_function(_pool), library(_library), ext_func(_ext_func)
        {
            lock();
        }

        void object_function_dll::trace()
        {
            library->mark();
        }

        void object_function_dll::trace_parallel(object_pool *pool)
        {
            library->mark_parallel(pool);
        }

        static void * ext_allocator(size_t size, lib::fast_allocator *mem_info)
        {
            return mem_info->alloc(size);
        }

        void object_function_dll::call(thread *thr, int arg_count, call_mode mode)
        {
            lib::fast_allocator tmp_memory(1024);

            goat_ext_environment env;
            env.allocator = (goat_allocator)ext_allocator;
            env.mem_info = (void*)(&tmp_memory);

            if (mode == call_mode::as_method)
                thr->pop();
            goat_value **args = nullptr;
            if (arg_count > 0)
            {
                args = (goat_value**)tmp_memory.alloc(sizeof(goat_value*) + arg_count);
                for (int i = 0; i < arg_count; i++)
                {
                    args[i] = thr->peek(i).get_value(&env);
                }
                thr->pop(arg_count);
            }
            goat_value *ret_val = ext_func(&env, arg_count, args);
            if (ret_val)
            {
                variable result;
                switch (ret_val->type)
                {
                    case goat_type_integer:
                        result.set_integer(((goat_integer*)ret_val)->value);
                        break;
                    default:
                        result.set_object(thr->pool->get_undefined_instance());
                }
                thr->push(result);
            }
            else
            {            
                thr->push_undefined();
            }
        }
    };
};
