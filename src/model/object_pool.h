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

#pragma once

#include "object_list.h"

namespace g0at
{
    namespace model
    {
        class object;
        class object_pool;

        template <int Factor, int Count> class object_pool_typed
        {
        public:
            object_pool_typed()
                : alive(0)
            {
            }

            inline bool destroy_or_cache_object(object *obj, object_pool *pool);

            int alive;
            object_list dead;
            static const int factor = Factor;
            static const int min_count = Count;

        protected:
            bool next_object_should_be_destroyed()
            {
                int dead_count = dead.get_count();
                return dead_count > min_count && dead_count > alive * factor;
            }
        };

        class object_pool
        {
        public:
            object_pool();

            void add(object *item)
            {
                population.add(item);
            }

            void destroy_all()
            {
                population.destroy_all();
                generic_objects.dead.destroy_all();
                contexts.dead.destroy_all();
                strings.dead.destroy_all();
                integers.dead.destroy_all();
                real_numbers.dead.destroy_all();
                booleans.dead.destroy_all();
            }

            object *get_generic_proto_instance() { return generic_proto_instance; }
            object *get_void_instance() { return void_instance; }
            object *get_undefined_instance() { return undefined_instance; };
            object *get_null_instance() { return null_instance; }
            object *get_string_proto_instance() { return string_proto_instance; }
            object *get_integer_proto_instance() { return integer_proto_instance; }
            object *get_function_proto_instance() { return function_proto_instance; }
            object *get_boolean_proto_instance() { return boolean_proto_instance; }
            object *get_real_proto_instance() { return real_proto_instance; }

            object_list population;
            object_pool_typed<2, 128> generic_objects;
            object_pool_typed<8, 1024> contexts;
            object_pool_typed<2, 64> strings;
            object_pool_typed<2, 64> integers;
            object_pool_typed<2, 64> real_numbers;
            object_pool_typed<2, 64> booleans;

        private:
            object_pool(const object_pool &) { }
            void operator=(const object_pool &) { }
 
            object *generic_proto_instance;
            object *void_instance;
            object *undefined_instance;
            object *null_instance;
            object *string_proto_instance;
            object *integer_proto_instance;
            object *function_proto_instance;
            object *boolean_proto_instance;
            object *real_proto_instance;
        };

        template <int Factor, int Count> bool object_pool_typed<Factor, Count>::destroy_or_cache_object(object *obj, object_pool *pool)
        {
            pool->population.remove(obj);
            alive--;
            if (next_object_should_be_destroyed())
            {
                delete obj;
                return false;
            }
            else
            {
                dead.add(obj);
                return true;
            }
        }
    };
};
