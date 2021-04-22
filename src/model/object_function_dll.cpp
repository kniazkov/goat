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
#include "object_string.h"
#include "object_array.h"
#include "object_thread.h"
#include "process.h"
#include "lib/assert.h"
#include "lib/fast_allocator.h"

namespace g0at
{
    namespace model
    {
        object_function_dll::object_function_dll(object_pool *_pool, object_dynamic_library *_library, goat_ext_function _ext_func)
            : object_function(_pool), library(_library), ext_func(_ext_func), runner_data(nullptr)
        {
            lock();
        }

        object_function_dll::~object_function_dll()
        {
            delete runner_data;
        }

        void object_function_dll::trace()
        {
            library->mark();
        }

        void object_function_dll::trace_parallel(object_pool *pool)
        {
            library->mark_parallel(pool);
        }

        static void value_to_variable(object_pool *pool, goat_value *src, variable *dst)
        {
            if (!src)
            {
                dst->set_object(pool->get_undefined_instance());
                return;
            }

            switch (src->type)
            {
                case goat_type_null:
                    dst->set_object(pool->get_null_instance());
                    break;
                case goat_type_boolean:
                    dst->set_boolean(((goat_boolean*)src)->value);
                    break;
                case goat_type_integer:
                    dst->set_integer(((goat_integer*)src)->value);
                    break;
                case goat_type_real:
                    dst->set_real(((goat_real*)src)->value);
                    break;
                case goat_type_char:
                    dst->set_char(((goat_char*)src)->value);
                    break;
                case goat_type_string:
                    dst->set_object(pool->create_object_string(((goat_string*)src)->value));
                    break;
                case goat_type_array:
                {
                    object_array *array = pool->create_object_array();
                    goat_array_item *item = ((goat_array*)src)->first;
                    while(item)
                    {
                        variable var;
                        value_to_variable(pool, item->data, &var);
                        array->add_item(var);
                        item = item->next;
                    }
                    dst->set_object(array);
                    break;
                }
                case goat_type_object:
                {
                    object *object = pool->create_generic_object();
                    goat_object_record *rec  = ((goat_object*)src)->first;
                    while(rec)
                    {
                        variable var;
                        value_to_variable(pool, rec->data, &var);
                        object->add_object(pool->create_object_string(rec->key), var);
                        rec = rec->next;
                    }
                    dst->set_object(object);
                    break;                
                }
                case goat_type_thread:
                {
                    object *obj = (object*)(((goat_thread*)src)->ir_ptr);
                    if (pool->population.contains(obj) && obj->to_object_thread() != nullptr)
                        dst->set_object((object*)((goat_thread*)src)->ir_ptr);
                    else
                        dst->set_object(pool->get_undefined_instance());
                    break;
                }
                default:
                    dst->set_object(pool->get_undefined_instance());
            }
        }

        static void * ext_allocator(size_t size, lib::fast_allocator *memory_map)
        {
            return memory_map->alloc(size);
        }

        static bool ext_thread_runner(void *thread_ptr, void *thread_runner_data, int argc, goat_value **argv)
        {
            ext_thread_runner_data *data = (ext_thread_runner_data*)thread_runner_data;
            assert(data != nullptr);
            object *obj = (object*)thread_ptr;
            if (!data->pool->population.contains(obj))
                return false;
            object_thread *obj_thread = obj->to_object_thread();
            if (!obj_thread)
                return false;
            context *ctx = data->pool->create_context(obj_thread->get_proto_ctx());
            ctx->address_type = context_address_type::stop;
            int decl_arg_count = obj_thread->get_arg_names_count();
            for (int i = 0; i < decl_arg_count; i++)
            {
                object *key = obj_thread->get_arg_name(i);
                if (i < argc)
                {
                    variable arg;
                    value_to_variable(data->pool, argv[i], &arg);
                    ctx->add_object(key, arg);
                }
                else
                    ctx->add_object(key, data->pool->get_undefined_instance());
            }

            thread *new_thr = data->proc->active_threads->create_thread(ctx, nullptr);
            new_thr->state = thread_state::ok;
            new_thr->iid = obj_thread->get_first_iid();
            return true;
        }

        void object_function_dll::call(thread *thr, int arg_count, call_mode mode)
        {
            lib::fast_allocator tmp_memory(1024);

            goat_ext_environment env;
            env.allocator = (goat_allocator)ext_allocator;
            env.memory_map = (void*)(&tmp_memory);
            env.thread_runner = ext_thread_runner;
            if (runner_data == nullptr)
                runner_data = new ext_thread_runner_data(thr->pool, thr->get_process());
            env.thread_runner_data = (void*)runner_data;

            if (mode == call_mode::as_method)
                thr->pop();
            goat_value **args = nullptr;
            if (arg_count > 0)
            {
                args = (goat_value**)tmp_memory.alloc(sizeof(goat_value*) * arg_count);
                for (int i = 0; i < arg_count; i++)
                {
                    args[i] = thr->peek(i).get_value(&env);
                }
                thr->pop(arg_count);
            }
            goat_value *ret_val = ext_func(&env, arg_count, args);
            variable result;
            value_to_variable(thr->pool, ret_val, &result);
            thr->push(result);
        }
    };
};
