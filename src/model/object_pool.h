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

        class object_pool_typed
        {
        public:
            object_pool_typed()
                : alive_count(0), factor(2), min_count(128)
            {
            }

            void destroy_or_cache_object(object *obj);

            bool object_should_be_destroyed()
            {
            }

            int alive_count;
            object_list dead;
            int factor;
            int min_count;
        };

        class object_pool
        {
        public:
            object_pool();

            void add(object *item)
            {
                population.add(item);
            }

/*
            void remove(object *item)
            {
                population.remove(item);
            }
*/

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

            object *get_generic_proto_instance();
            object *get_void_instance();
            object *get_undefined_instance();
            object *get_null_instance();
            object *get_string_proto_instance();
            object *get_integer_proto_instance();
            object *get_function_proto_instance();
            object *get_boolean_proto_instance();
            object *get_real_proto_instance();

            object_pool_typed *get_pool_generic_objects() { return &generic_objects; }
            object_pool_typed *get_pool_contexts() { return &contexts; }
            object_pool_typed *get_pool_strings() { return &strings; }
            object_pool_typed *get_pool_integers() { return &integers; }
            object_pool_typed *get_pool_real_numbers() { return &real_numbers; }
            object_pool_typed *get_pool_booleans() { return &booleans; }

        private:
            object_pool(const object_pool &) { }
            void operator=(const object_pool &) { }
            void init();
            
            object_list population;
            object_pool_typed generic_objects;
            object_pool_typed contexts;
            object_pool_typed strings;
            object_pool_typed integers;
            object_pool_typed real_numbers;
            object_pool_typed booleans;

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
    };
};
