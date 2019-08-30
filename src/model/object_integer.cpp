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
#include "object_string.h"
#include "object_function_built_in.h"
#include "resource/strings.h"
#include "lib/functional.h"
#include "thread.h"
#include "lib/assert.h"

namespace g0at
{
    namespace model
    {
        object_integer::object_integer(object_pool *pool, int64_t value)
            : variable_wrapper(pool, pool->get_integer_proto_instance())
        {
            var.set_integer(value);
        }

        void object_integer::kill(object_pool *pool)
        {
            if (pool->integers.destroy_or_cache(this, pool))
            {
                objects.clear();
            }
        }

        void object_integer::reinit(int64_t value)
        {
            var.set_integer(value);
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
            return var.data.i < obj_int->var.data.i;
        }

        void object_integer::m_iterator(thread *thr, int arg_count)
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

        object_integer_proto::object_integer_proto(object_pool *pool)
            : object(pool, pool->get_number_proto_instance())
        {
        }

        void object_integer_proto::init(object_pool *pool)
        {
            add_object(pool->get_static_string(resource::str_oper_plus_plus), pool->get_wrap_inc_instance());
            add_object(pool->get_static_string(resource::str_oper_minus_minus), pool->get_wrap_dec_instance());
            add_object(pool->get_static_string(resource::str_oper_tilde), pool->get_wrap_inv_instance());
            add_object(pool->get_static_string(resource::str_oper_plus), pool->get_wrap_pos_add_instance());
            add_object(pool->get_static_string(resource::str_oper_minus), pool->get_wrap_neg_sub_instance());
            add_object(pool->get_static_string(resource::str_oper_asterisk), pool->get_wrap_mul_instance());
            add_object(pool->get_static_string(resource::str_oper_double_asterisk), pool->get_wrap_exp_instance());
            add_object(pool->get_static_string(resource::str_oper_slash), pool->get_wrap_div_instance());
            add_object(pool->get_static_string(resource::str_oper_percent), pool->get_wrap_mod_instance());
            add_object(pool->get_static_string(resource::str_oper_exclamation), pool->get_wrap_not_instance());
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

            void op_pos(variable *var, thread *thr)  override
            {
                unary_operation<lib::func::pos>(var, thr);
            }

            void op_neg(variable *var, thread *thr)  override
            {
                unary_operation<lib::func::neg>(var, thr);
            }

            void op_inc(variable *var, thread *thr)  override
            {
                unary_operation<lib::func::inc>(var, thr);
            }

            void op_dec(variable *var, thread *thr)  override
            {
                unary_operation<lib::func::dec>(var, thr);
            }

            void op_not(variable *var, thread *thr)  override
            {
                thr->pop();
                variable result;
                result.set_boolean(var->data.i == 0);
                thr->push(result);
            }

            void op_inv(variable *var, thread *thr)  override
            {
                unary_operation<lib::func::inv>(var, thr);
            }

            void op_mul(variable *var, thread *thr)  override
            {
                binary_math_operation<lib::func::mul>(var, thr);
            }

            void op_exp(variable *var, thread *thr)  override
            {
                binary_math_operation<lib::func::exp>(var, thr);
            }

            void op_div(variable *var, thread *thr)  override
            {
                binary_math_operation<lib::func::div>(var, thr);
            }

            void op_mod(variable *var, thread *thr)  override
            {
                binary_math_operation<lib::func::mod>(var, thr);
            }

            void op_eq(variable *var, thread *thr)  override
            {
                binary_logical_operation<lib::func::equals, false>(var, thr);
            }

            void op_neq(variable *var, thread *thr)  override
            {
                binary_logical_operation<lib::func::not_equal, true>(var, thr);
            }

            void op_less(variable *var, thread *thr)  override
            {
                binary_logical_operation<lib::func::less, true>(var, thr);
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
                variable result;
                thr->pop();
                variable right = thr->pop();
                int64_t right_int_value;
                bool right_is_integer = right.get_integer(&right_int_value);
                if(right_is_integer)
                {
                    result.set_integer(F<int64_t, int64_t, int64_t>::calculate(var->data.i, right_int_value));
                    thr->push(result);
                    return;
                }
                double right_real_value;
                bool right_is_real = right.get_real(&right_real_value);
                if (!right_is_real)
                {
                    thr->raise_exception(thr->pool->get_exception_illegal_argument_instance());
                    return;
                }
                result.set_real(F<double, int64_t, double>::calculate(var->data.i, right_real_value));
                thr->push(result);
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
