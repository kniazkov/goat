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

#include "object_integer.h"
#include "lib/functional.h"
#include "thread.h"
#include "lib/assert.h"

namespace g0at
{
    namespace model
    {
        object_integer::object_integer(object_pool *pool, int64_t _value)
            : object(pool, pool->get_integer_proto_instance()), value(_value)
        {
        }

        void object_integer::kill(object_pool *pool)
        {
            if (pool->integers.destroy_or_cache(this, pool))
            {
                objects.clear();
            }
        }

        void object_integer::reinit(int64_t _value)
        {
            value = _value;
        }

        object_type object_integer::get_type() const
        {
            return object_type::integer;
        }

        object_integer *object_integer::to_object_integer()
        {
            return this;
        }

        bool object_integer::less(const object *obj) const
        {
            assert(obj->get_type() == object_type::integer);
            const object_integer *obj_int = static_cast<const object_integer*>(obj);
            return value < obj_int->value;
        }

        std::wstring object_integer::to_string() const
        {
            return std::to_wstring(value);
        }

        bool object_integer::get_integer(int64_t *pval)
        {
            *pval = value;
            return true;
        }

        bool object_integer::get_real(double *pval)
        {
            *pval = (double)value;
            return true;
        }

        void object_integer::op_add(thread *thr)
        {
            binary_math_operation<lib::func::plus>(thr);
        }

        void object_integer::op_sub(thread *thr)
        {
            binary_math_operation<lib::func::minus>(thr);
        }

        void object_integer::op_neg(thread *thr)
        {
            unary_operation<lib::func::neg>(thr);
        }

        void object_integer::op_eq(thread *thr)
        {
            binary_logical_operation<lib::func::equals, false>(thr);
        }

        void object_integer::op_neq(thread *thr)
        {
            binary_logical_operation<lib::func::not_equal, true>(thr);
        }

        template <template<typename R, typename A> class F> void object_integer::unary_operation(thread *thr)
        {
            thr->pop();
            variable result;
            result.set_integer(F<int64_t, int64_t>::calculate(value));
            thr->push(result);
        }

        template <template<typename R, typename X, typename Y> class F> void object_integer::binary_math_operation(thread *thr)
        {
            thr->pop();
            variable right = thr->pop();
            int64_t right_int_value;
            bool right_is_integer = right.get_integer(&right_int_value);
            if (right_is_integer)
            {
                variable result;
                result.set_integer(F<int64_t, int64_t, int64_t>::calculate(value, right_int_value));
                thr->push(result);
            }
            else
            {
                double right_real_value;
                bool right_is_real = right.get_real(&right_real_value);
                assert(right_is_real);
                variable result;
                result.set_real(F<double, int64_t, double>::calculate(value, right_real_value));
                thr->push(result);
            }
        }

        template <template<typename R, typename X, typename Y> class F, bool Def> void object_integer::binary_logical_operation(thread *thr)
        {
            thr->pop();
            variable right = thr->pop();
            int64_t right_int_value;
            bool right_is_integer = right.get_integer(&right_int_value);
            variable result;
            if (right_is_integer)
            {
                result.set_boolean(F<bool, int64_t, int64_t>::calculate(value, right_int_value));
            }
            else
            {
                double right_real_value;
                bool right_is_real = right.get_real(&right_real_value);
                if(right_is_real)
                {
                    result.set_boolean(F<bool, int64_t, double>::calculate(value, right_real_value));
                }
                else
                {
                    result.set_boolean(Def);
                }
            }
            thr->push(result);
        }

        /*
            Prototype
        */

        object_integer_proto::object_integer_proto(object_pool *pool)
            : object(pool, pool->get_number_proto_instance())
        {
        }

        /*
            Primitive handler
        */

        class integer_handler : public handler
        {
        public:
            static handler *get_instance()
            {
                static integer_handler instance;
                return &instance;
            }

            std::wstring to_string(const variable *var) const override
            {
                return std::to_wstring(var->data.i);
            }

            std::wstring to_string_notation(const variable *var) const override
            {
                return std::to_wstring(var->data.i);
            }

            object *to_object(variable *var, object_pool *pool) override
            {
                object *obj = pool->create_object_integer(var->data.i);
                var->set_object(obj);
                return obj;
            }

            bool get_integer(variable *var, int64_t *pval)
            {
                *pval = var->data.i;
                return true;
            }

            bool get_real(variable *var, double *pval)
            {
                *pval = (double)var->data.i;
                return true;
            }

            void op_add(variable *var, thread *thr)  override
            {
                binary_math_operation<lib::func::plus>(var, thr);
            }

            void op_sub(variable *var, thread *thr)  override
            {
                binary_math_operation<lib::func::minus>(var, thr);
            }

            void op_neg(variable *var, thread *thr)  override
            {
                unary_operation<lib::func::neg>(var, thr);
            }

            void op_eq(variable *var, thread *thr)  override
            {
                binary_logical_operation<lib::func::equals, false>(var, thr);
            }

            void op_neq(variable *var, thread *thr)  override
            {
                binary_logical_operation<lib::func::not_equal, true>(var, thr);
            }

            void m_instance_of(variable *var, thread *thr, int arg_count) override
            {
                thr->pop();
                object *base = thr->peek().get_object();
                thr->pop(arg_count);
                variable result;
                if (base)
                    result.set_boolean(base == thr->pool->get_integer_proto_instance()
                        || base == thr->pool->get_number_proto_instance()
                        || base == thr->pool->get_generic_proto_instance());
                else
                    result.set_boolean(false);
                thr->push(result);
            }

        protected:
            integer_handler()
            {
            }

            template <template<typename R, typename A> class F> void unary_operation(variable *var, thread *thr)
            {
                thr->pop();
                variable result;
                result.set_integer(F<int64_t, int64_t>::calculate(var->data.i));
                thr->push(result);
            }

            template <template<typename R, typename X, typename Y> class F> void binary_math_operation(variable *var, thread *thr)
            {
                thr->pop();
                variable right = thr->pop();
                int64_t right_int_value;
                bool right_is_integer = right.get_integer(&right_int_value);
                if(right_is_integer)
                {
                    variable result;
                    result.set_integer(F<int64_t, int64_t, int64_t>::calculate(var->data.i, right_int_value));
                    thr->push(result);
                }
                else
                {
                    double right_real_value;
                    bool right_is_real = right.get_real(&right_real_value);
                    assert(right_is_real);                    
                    variable result;
                    result.set_real(F<double, int64_t, double>::calculate(var->data.i, right_real_value));
                    thr->push(result);
                }
            }

            template <template<typename R, typename X, typename Y> class F, bool Def> void binary_logical_operation(variable *var, thread *thr)
            {
                thr->pop();
                variable right = thr->pop();
                int64_t right_int_value;
                bool right_is_integer = right.get_integer(&right_int_value);
                variable result;
                if(right_is_integer)
                {
                    result.set_boolean(F<bool, int64_t, int64_t>::calculate(var->data.i, right_int_value));
                }
                else
                {
                    double right_real_value;
                    bool right_is_real = right.get_real(&right_real_value);
                    if(right_is_real)
                    {                    
                        result.set_boolean(F<bool, int64_t, double>::calculate(var->data.i, right_real_value));
                    }
                    else
                    {
                        result.set_boolean(Def);
                    }
                }
                thr->push(result);
            }
        };

        handler *handler::get_instance_integer()
        {
            return integer_handler::get_instance();
        }
    };
};
