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
#include "object_cache.h"
#include "lib/new.h"
#include <string>
#include <cstdint>

namespace g0at
{
    namespace model
    {
        class object;
        class object_pool;
        class generic_object;
        class generic_proto;
        class context;
        class object_string;
        class object_integer;
        class object_real;
        class object_boolean;
        class object_array;
        class object_char;

        template <int Factor, int Count> class object_pool_typed
        {
        public:
            object_pool_typed()
                : alive(0)
            {
            }

            inline bool destroy_or_cache(object *obj, object_pool *pool);
            inline object *get(object_pool *pool);

            int alive;
            object_list dead;
            static const int factor = Factor;
            static const int min_count = Count;

        protected:
            bool next_object_should_be_destroyed()
            {
                return dead.count > min_count && dead.count > alive * factor;
            }
        };

        struct object_pool_report
        {
            uint64_t new_count;
            uint64_t reinit_count;

            object_pool_report()
                : new_count(0), reinit_count(0)
            {
            }
        };

        class object_pool
        {
        public:
            object_pool(std::vector<std::wstring> &identifiers_list);
            void add(object *item);
            void destroy_all();

            int get_next_id() { return id++; }
            object_pool_report &get_report() { return report; }

            object *get_generic_proto_instance() { return generic_proto_instance; }
            object *get_void_instance() { return void_instance; }
            object *get_undefined_instance() { return undefined_instance; };
            object *get_null_instance() { return null_instance; }
            object *get_string_proto_instance() { return string_proto_instance; }
            object *get_number_proto_instance() { return number_proto_instance; }
            object *get_integer_proto_instance() { return integer_proto_instance; }
            object *get_function_proto_instance() { return function_proto_instance; }
            object *get_thread_proto_instance() { return thread_proto_instance; }
            object *get_boolean_proto_instance() { return boolean_proto_instance; }
            object *get_real_proto_instance() { return real_proto_instance; }
            object *get_array_proto_instance() { return array_proto_instance; }
            object *get_char_proto_instance() { return char_proto_instance; }
            object *get_exception_proto_instance() { return exception_proto_instance; }
            object *get_exception_illegal_argument_instance() { return exception_illegal_argument_instance; }
            object *get_exception_illegal_context_instance() { return exception_illegal_context_instance; }

            generic_object *create_generic_object();
            context *create_context();
            context *create_context(context *proto);
            context *create_context(context *proto, context *parent);
            context *create_context(object *this_ptr, context *proto, context *parent);
            object_string *create_object_string(std::wstring data);
            object_string *create_object_string(std::wstring data, int id);
            object_integer *create_object_integer(int64_t value);
            object_real *create_object_real(double value);
            object_boolean *create_object_boolean(bool value);
            object_array *create_object_array();
            object_char *create_object_char(wchar_t value);

            void mark_all_static_strings() { static_strings.mark_all(); }
            object_string *get_static_string(std::wstring name) { return static_strings.get_object(name, this); }
            object_string *get_static_string(int id) { return static_strings.get_object(id); }

            object_list population;
            object_pool_typed<4, 256> generic_objects;
            object_pool_typed<8, 4096> contexts;
            object_pool_typed<2, 128> strings;
            object_pool_typed<1, 64> integers;
            object_pool_typed<1, 64> real_numbers;
            object_pool_typed<1, 64> booleans;
            object_pool_typed<2, 128> arrays;
            object_pool_typed<1, 64> chars;

        private:
            object_pool(const object_pool &) { }
            void operator=(const object_pool &) { }
 
            int id;
            object_pool_report report;

            object *generic_proto_instance;
            object *void_instance;
            object *undefined_instance;
            object *null_instance;
            object *string_proto_instance;
            object *number_proto_instance;
            object *integer_proto_instance;
            object *function_proto_instance;
            object *thread_proto_instance;
            object *boolean_proto_instance;
            object *real_proto_instance;
            object *array_proto_instance;
            object *char_proto_instance;
            object *exception_proto_instance;
            object *exception_illegal_argument_instance;
            object *exception_illegal_context_instance;

            object_cache static_strings;
        };

        template <int Factor, int Count> bool object_pool_typed<Factor, Count>::destroy_or_cache(object *obj, object_pool *pool)
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
                lib::it_is_a_cached_block(obj);
                return true;
            }
        }

        template <int Factor, int Count> object *object_pool_typed<Factor, Count>::get(object_pool *pool)
        {
            alive++;
            if (dead.count > 0)
            {
                object *obj = dead.remove();
                pool->population.add(obj);
                pool->get_report().reinit_count++;
                lib::it_is_a_not_cached_block(obj);
                return obj;
            }
            return nullptr;
        }
    };
};
