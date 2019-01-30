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
        class thread;
        class object;
        class object_string;
        class object_function;
        class object_integer;
        class handler;

        enum object_type
        {
            GENERIC,
            STRING,
            FUNCTION,
            INTEGER
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
            inline void set_integer(int64_t value);

            inline std::wstring to_string() const;
            inline object *to_object(object_list *list);
            inline bool get_integer(int64_t *pval);

            inline void op_add(thread *thr);

            handler *hndl;
            union
            {
                object *obj;
                int64_t i;
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
            virtual object_integer *to_object_integer();

            virtual bool less(const object *obj) const;
            virtual std::wstring to_string() const;

            void add_object(object *key, object *value);
            variable *find_object(object *key);

            virtual bool get_integer(int64_t *pval);

            virtual void op_add(thread *thr);

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
            static handler *get_instance_generic();
            static handler *get_instance_integer();
            virtual std::wstring to_string(const variable *var) const = 0;
            virtual object *to_object(variable *var, object_list *list) = 0;
            virtual bool get_integer(variable *var, int64_t *val) = 0;

            virtual void op_add(variable *var, thread *thr) = 0;
        };

        class generic_handler : public handler
        {
        public:
            static handler *get_instance();
            std::wstring to_string(const variable *var) const override;
            object *to_object(variable *var, object_list *list) override;
            bool get_integer(variable *var, int64_t *pval) override;
            void op_add(variable *var, thread *thr)  override;

        protected:
            generic_handler();
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

        /*
            Variable
        */

        void variable::set_object(object *obj)
        {
            hndl = handler::get_instance_generic();
            data.obj = obj;
        }

        void variable::set_integer(int64_t value)
        {
            hndl = handler::get_instance_integer();
            data.i = value;
        }

        std::wstring variable::to_string() const
        {
            return hndl->to_string(this);
        }

        object *variable::to_object(object_list *list)
        {
            return hndl->to_object(this, list);
        }

        bool variable::get_integer(int64_t *pval)
        {
            return hndl->get_integer(this, pval);
        }

        void variable::op_add(thread *thr)
        {
            hndl->op_add(this, thr);
        }
    };
};
