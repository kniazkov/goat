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

#include "object.h"
#include <assert.h>
#include <sstream>

namespace g0at
{
    namespace model
    {
        /* 
            Generic object
        */

        object::object(object_list *list)
        {
            list->add(this);
        }

        object::~object()
        {
        }

        object_type object::get_type() const
        {
            return GENERIC;
        }

        object_string *object::to_object_string()
        {
            return nullptr;
        }

        object_function *object::to_object_function()
        {
            return nullptr;
        }

        bool object::less(const object *obj) const
        {
            return this < obj;
        }
    
        std::wstring object::to_string() const
        {
            std::wstringstream stream;
            stream << '{';

            int k = 0;
            for (auto pair : objects)
            {
                if (k)
                    stream << ',';
                k++;
                stream << pair.first->to_string() << ':' << pair.second.to_string();
            }

            stream << '}';
            return stream.str();
        }

        void object::add_object(object *key, object *value)
        {
            variable var;
            var.set_object(value);
            objects[key] = var;
        }

        variable *object::find_object(object *key)
        {
            auto iter = objects.find(key);
            if (iter != objects.end())
            {
                return &iter->second;
            }
            
            variable *var;
            for (object *pt : proto)
            {
                var = pt->find_object(key);
                if (var != nullptr)
                    break;
            }
            return var;
        }

        void object::op_add(thread *thr)
        {
            assert(false); // not implemented
        }

        /* 
            Generic handler
        */

        handler::~handler()
        {
        }

        class generic_handler : public handler
        {
        protected:
            generic_handler()
            {
            }

        public:
            static handler *get_instance()
            {
                static generic_handler instance;
                return &instance;
            }

            std::wstring to_string(const variable *var) const override
            {
                return var->data.obj->to_string();
            }

            object *to_object(variable *var, object_list *list) override
            {
                return var->data.obj;
            }

            void op_add(variable *var, thread *thr)
            {
                var->data.obj->op_add(thr);
            }
        };

        handler *handler::get_generic_instance()
        {
            return generic_handler::get_instance();
        }
    };
};
