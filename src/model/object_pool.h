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

        private:
            object_pool(const object_pool &) { }
            void operator=(const object_pool &) { }
            void init();
            
            object_list population;

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
