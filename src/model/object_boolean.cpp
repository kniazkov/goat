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

#include "object_boolean.h"
#include "lib/functional.h"
#include "thread.h"
#include <assert.h>

namespace g0at
{
    namespace model
    {
        object_boolean::object_boolean(object_pool *pool, bool _value)
            : object(pool, pool->get_boolean_proto_instance()), value(_value)
        {
        }

        void object_boolean::reinit(bool _value)
        {
            value = _value;
        }

        void object_boolean::kill(object_pool *pool)
        {
            if (pool->booleans.destroy_or_cache(this, pool))
            {
                objects.clear();
            }
        }

        object_type object_boolean::get_type() const
        {
            return object_type::boolean;
        }

        object_boolean *object_boolean::to_object_boolean()
        {
            return this;
        }

        bool object_boolean::less(const object *obj) const
        {
            assert(obj->get_type() == object_type::boolean);
            const object_boolean *obj_bool = static_cast<const object_boolean*>(obj);
            return (value ? 0 : 1) < (obj_bool->value ? 1 : 0);
        }

        std::wstring object_boolean::to_string() const
        {
            return value ? L"true" : L"false";
        }

        bool object_boolean::get_boolean(bool *pval)
        {
            *pval = value;
            return true;
        }

        template <template<typename R, typename A> class F> void object_boolean::unary_operation(thread *thr)
        {
            thr->pop();
            variable result;
            result.set_boolean(F<bool, bool>::calculate(value));
            thr->push(result);
        }

        template <template<typename R, typename X, typename Y> class F> void object_boolean::binary_operation(thread *thr)
        {
            thr->pop();
            variable right = thr->pop();
            bool right_value;
            bool right_is_boolean = right.get_boolean(&right_value);
            assert(right_is_boolean);
            variable result;
            result.set_boolean(F<bool, bool, bool>::calculate(value, right_value));
            thr->push(result);
        }

        /*
            Prototype
        */

        object_boolean_proto::object_boolean_proto(object_pool *pool)
            : object(pool)
        {
        }

        /*
            Primitive handler
        */

        class boolean_handler : public handler
        {
        public:
            static handler *get_instance()
            {
                static boolean_handler instance;
                return &instance;
            }

            std::wstring to_string(const variable *var) const override
            {
                return var->data.b ? L"true" : L"false";
            }

            std::wstring to_string_notation(const variable *var) const override
            {
                return var->data.b ? L"true" : L"false";
            }

            object *to_object(variable *var, object_pool *pool) override
            {
                object *obj = pool->create_object_boolean(var->data.b);
                var->set_object(obj);
                return obj;
            }

            bool get_boolean(variable *var, bool *pval)
            {
                *pval = var->data.b;
                return true;
            }

            void m_instance_of(variable *var, thread *thr, int arg_count) override
            {
                thr->pop();
                object *base = thr->peek().get_object();
                thr->pop(arg_count);
                variable result;
                if (base)
                    result.set_boolean(base == thr->pool->get_boolean_proto_instance());
                else
                    result.set_boolean(false);
                thr->push(result);
            }

        protected:
            boolean_handler()
            {
            }

            template <template<typename R, typename A> class F> void unary_operation(variable *var, thread *thr)
            {
                thr->pop();
                variable result;
                result.set_boolean(F<bool, bool>::calculate(var->data.i));
                thr->push(result);
            }

            template <template<typename R, typename X, typename Y> class F> void binary_operation(variable *var, thread *thr)
            {
                thr->pop();
                variable right = thr->pop();
                bool right_value;
                bool right_is_boolean = right.get_boolean(&right_value);
                assert(right_is_boolean);
                variable result;
                result.set_boolean(F<bool, bool, bool>::calculate(var->data.b, right_value));
                thr->push(result);
            }
        };

        handler *handler::get_instance_boolean()
        {
            return boolean_handler::get_instance();
        }
    };
};
