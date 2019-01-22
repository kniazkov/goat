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
#include <map>
#include <vector>
#include <string>

namespace g0at
{
    namespace model
    {
        class object;
        class object_string;
        class object_function;
        class handler;

        enum object_type
        {
            GENERIC,
            STRING,
            FUNCTION
        };

        class object_comparator
        {
        public:
            inline bool operator() (const object *a, const object *b) const;
        };

        class variable
        {
        public:
            variable()
            {
                hndl = nullptr;
                data.obj = nullptr;
            }

            inline void set_object(object *obj);
            inline std::wstring to_string() const;

            handler *hndl;
            union
            {
                object *obj;
            } data;
        };

        class object
        {
        public:
            object(object_list *list);
            virtual ~object();
            virtual object_type get_type() const;
            virtual object_string *to_object_string();
            virtual object_function *to_object_function();

            virtual bool less(const object *obj) const;
            virtual std::wstring to_string() const;

            void add_object(object *key, object *value)
            {
                variable var;
                var.set_object(value);
                objects[key] = var;
            }

            object *prev;
            object *next;

        protected:
            std::map<object*, variable, object_comparator> objects;
            std::vector<object*> proto;
        };

        class handler
        {
        public:
            virtual ~handler();
            static handler *get_generic_instance();
            virtual std::wstring to_string(const variable *var) const = 0;
        };

        bool object_comparator::operator ()(const object *a, const object *b) const
        {
            if (a == b)
                return false;

            object_type x = a->get_type();
            object_type y = b->get_type();

            if (x == y)
                return a->less(b);

            return x < y;
        }

        void variable::set_object(object *obj)
        {
            hndl = handler::get_generic_instance();
            data.obj = obj;
        }

        std::wstring variable::to_string() const
        {
            return hndl->to_string(this);
        }
    };
};
