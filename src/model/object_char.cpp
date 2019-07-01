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

#include "object_char.h"
#include "lib/functional.h"
#include "thread.h"
#include "lib/assert.h"
#include "lib/utils.h"
#include <sstream>

namespace g0at
{
    namespace model
    {
        object_char::object_char(object_pool *pool, wchar_t _value)
            : object(pool, pool->get_char_proto_instance()), value(_value)
        {
        }

        void object_char::reinit(wchar_t _value)
        {
            value = _value;
        }

        void object_char::kill(object_pool *pool)
        {
            if (pool->chars.destroy_or_cache(this, pool))
            {
                objects.clear();
            }
        }

        object_type object_char::get_type() const
        {
            return object_type::charact;
        }

        object_char *object_char::to_object_char()
        {
            return this;
        }

        bool object_char::less(const object *obj) const
        {
            assert(obj->get_type() == object_type::charact);
            const object_char *obj_char = static_cast<const object_char*>(obj);
            return value < obj_char->value;
        }

        std::wstring object_char::to_string() const
        {
            wchar_t tmp[] = { value, 0 };
            return std::wstring(tmp);
        }

        std::wstring object_char::to_string_notation() const
        {
            std::wstringstream wss;
            wss << L'\'' << lib::escape_special_chars(&value, 1) << L'\'';
            return wss.str();
        }

        bool object_char::get_char(wchar_t *pval)
        {
            *pval = value;
            return true;
        }

        void object_char::m_iterator(thread *thr, int arg_count)
        {
            thr->pop();
            thr->pop(arg_count);
            variable tmp;
            tmp.set_object(thr->pool->get_iterator_proto_instance());
            thr->push(tmp);
        }

        /*
            Prototype
        */

        object_char_proto::object_char_proto(object_pool *pool)
            : object(pool)
        {
        }

        /*
            Primitive handler
        */

        class char_handler : public handler
        {
        public:
            static handler *get_instance()
            {
                static char_handler instance;
                return &instance;
            }

            std::wstring to_string(const variable *var) const override
            {
                wchar_t tmp[] = { var->data.c, 0 };
                return std::wstring(tmp);
            }

            std::wstring to_string_notation(const variable *var) const override
            {
                std::wstringstream wss;
                wss << L'\'' << lib::escape_special_chars(&var->data.c, 1) << L'\'';
                return wss.str();
            }

            object *to_object(variable *var, object_pool *pool) override
            {
                object *obj = pool->create_object_char(var->data.b);
                var->set_object(obj);
                return obj;
            }

            bool get_char(variable *var, wchar_t *pval)
            {
                *pval = var->data.c;
                return true;
            }

            void m_instance_of(variable *var, thread *thr, int arg_count) override
            {
                thr->pop();
                object *base = thr->peek().get_object();
                thr->pop(arg_count);
                variable result;
                if (base)
                    result.set_boolean(base == thr->pool->get_char_proto_instance() || base == thr->pool->get_generic_proto_instance());
                else
                    result.set_boolean(false);
                thr->push(result);
            }

        protected:
            char_handler()
            {
            }
        };

        handler *handler::get_instance_char()
        {
            return char_handler::get_instance();
        }
    };
};
