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

#include "object_real.h"
#include "object_string.h"
#include "object_function_built_in.h"
#include "object_exception.h"
#include "thread.h"
#include "resource/strings.h"
#include "lib/functional.h"
#include "lib/utils.h"
#include "lib/assert.h"

namespace g0at
{
    namespace model
    {
        static bool convert_to_real(variable &var, variable *result)
        {
            double real_value;
            if (var.get_real(&real_value))
            {
                result->set_real(real_value);
                return true;
            }
            bool boolean_value;
            if (var.get_boolean(&boolean_value))
            {
                result->set_real(boolean_value ? 1 : 0);
                return true;
            }
            wchar_t char_value;
            if (var.get_char(&char_value))
            {
                result->set_real(char_value);
                return true;
            }
            object *obj = var.get_object();
            if (obj)
            {
                object_string *obj_string = obj->to_object_string();
                if (obj_string)
                {
                    std::wstring string_value = obj_string->get_data();
                    if (lib::wstring_to_double(string_value, &real_value))
                    {
                        result->set_real(real_value);
                        return true;
                    }
                }
            }
            return false;
        }

        object_real::object_real(object_pool *pool, double value)
            : variable_wrapper(pool, pool->get_real_proto_instance())
        {
            var.set_real(value);
        }

        void object_real::reinit(double value)
        {
            var.set_real(value);
        }

        void object_real::kill(object_pool *pool)
        {
            if (pool->real_numbers.destroy_or_cache(this, pool))
            {
                objects.clear();
                immutable = false;
            }
        }

        object_type object_real::get_type() const
        {
            return object_type::real;
        }

        object_real *object_real::to_object_real()
        {
            return this;
        }

        bool object_real::less(const object *obj) const
        {
            assert(obj->get_type() == object_type::real);
            const object_real *obj_real = static_cast<const object_real*>(obj);
            return var.data.r < obj_real->var.data.r;
        }

        void object_real::m_iterator(thread *thr, int arg_count)
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

        object_real_proto::object_real_proto(object_pool *pool)
            : object(pool, pool->get_number_proto_instance())
        {
        }

        class object_real_valueof : public object_function_built_in
        {
        public:
            object_real_valueof(object_pool *_pool)
                : object_function_built_in(_pool)
            {
            }
            
            void call(thread *thr, int arg_count, call_mode mode) override
            {
                if (arg_count > 0)
                {
                    if (mode == call_mode::as_method)
                        thr->pop();
                    variable arg = thr->peek();
                    thr->pop(arg_count);
                    variable result;
                    if (!convert_to_real(arg, &result))
                    {
                        result.set_object(thr->pool->get_null_instance());
                    }
                    thr->push(result);
                    return;
                }
                thr->raise_exception(new object_exception_illegal_argument(thr->pool));
            }
        };

        void object_real_proto::init(object_pool *pool)
        {
            add_object(pool->get_static_string(resource::str_valueof), new object_real_valueof(pool));
            add_object(pool->get_static_string(resource::str_oper_plus_plus), pool->get_wrap_inc_instance());
            add_object(pool->get_static_string(resource::str_oper_minus_minus), pool->get_wrap_dec_instance());
            add_object(pool->get_static_string(resource::str_oper_plus), pool->get_wrap_pos_add_instance());
            add_object(pool->get_static_string(resource::str_oper_minus), pool->get_wrap_neg_sub_instance());
            add_object(pool->get_static_string(resource::str_oper_asterisk), pool->get_wrap_mul_instance());
            add_object(pool->get_static_string(resource::str_oper_double_asterisk), pool->get_wrap_exp_instance());
            add_object(pool->get_static_string(resource::str_oper_slash), pool->get_wrap_div_instance());
            add_object(pool->get_static_string(resource::str_oper_exclamation), pool->get_wrap_not_instance());
            add_object(pool->get_static_string(resource::str_oper_double_exclamation), pool->get_wrap_bool_instance());
            add_object(pool->get_static_string(resource::str_oper_less), pool->get_wrap_less_instance());
            add_object(pool->get_static_string(resource::str_oper_less_equal), pool->get_wrap_leq_instance());
            add_object(pool->get_static_string(resource::str_oper_greater), pool->get_wrap_great_instance());
            add_object(pool->get_static_string(resource::str_oper_greater_equal), pool->get_wrap_greq_instance());
            add_object(pool->get_static_string(resource::str_oper_double_ampersand), pool->get_wrap_and_instance());
            add_object(pool->get_static_string(resource::str_oper_double_vertical_bar), pool->get_wrap_or_instance());
            lock();
        }

        void object_real_proto::op_new(thread *thr, int arg_count)
        {
            variable result;
            if (arg_count > 0)
            {
                variable arg = thr->peek();
                thr->pop(arg_count);
                if (!convert_to_real(arg, &result))
                {
                    result.set_object(thr->pool->get_null_instance());
                }
            }
            else
            {
                result.set_real(0);
            }
            thr->push(result);
        }

        /*
            Primitive handler
        */

        class real_handler : public handler
        {
        public:
            static handler *get_instance()
            {
                static real_handler instance;
                return &instance;
            }

            std::wstring to_string(const variable *var) const override
            {
                return lib::double_to_wstring(var->data.r);
            }

            std::wstring to_string_notation(const variable *var) const override
            {
                return lib::double_to_wstring(var->data.r);
            }

            object *to_object(variable *var, object_pool *pool) override
            {
                object *obj = pool->create_object_real(var->data.r);
                var->set_object(obj);
                return obj;
            }

            bool get_integer(variable *var, int64_t *pval)
            {
                return false;
            }

            bool get_real(variable *var, double *pval)
            {
                *pval = var->data.r;
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

            void op_not(variable *var, thread *thr)  override
            {
                thr->pop();
                variable result;
                result.set_boolean(var->data.r == 0);
                thr->push(result);
            }

            void op_bool(variable *var, thread *thr)  override
            {
                thr->pop();
                variable result;
                result.set_boolean(var->data.r != 0);
                thr->push(result);
            }

            void op_inc(variable *var, thread *thr)  override
            {
                unary_operation<lib::func::inc>(var, thr);
            }

            void op_dec(variable *var, thread *thr)  override
            {
                unary_operation<lib::func::dec>(var, thr);
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
                thr->pop();
                variable right = thr->pop();
                double right_value;
                bool right_is_real = right.get_real(&right_value);
                if (!right_is_real)
                {
                    thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                    return;
                }
                if (right_value == 0)
                {
                    thr->raise_exception(new object_exception_division_by_zero(thr->pool));
                    return;
                }
                variable result;
                result.set_real(var->data.r / right_value);
                thr->push(result);
            }

            void op_eq(variable *var, thread *thr)  override
            {
                binary_logical_operation<lib::func::equals, false>(var, thr);
            }

            void op_neq(variable *var, thread *thr)  override
            {
                binary_logical_operation<lib::func::not_equal, false>(var, thr);
            }

            void op_less(variable *var, thread *thr)  override
            {
                binary_logical_operation<lib::func::less, false>(var, thr);
            }

            void op_leq(variable *var, thread *thr)  override
            {
                binary_logical_operation<lib::func::less_or_equal, false>(var, thr);
            }

            void op_great(variable *var, thread *thr)  override
            {
                binary_logical_operation<lib::func::greater, false>(var, thr);
            }

            void op_greq(variable *var, thread *thr)  override
            {
                binary_logical_operation<lib::func::greater_or_equal, false>(var, thr);
            }

            void op_and(variable *var, thread *thr)  override
            {
                thr->pop();
                if (var->data.r == 0)
                {
                    thr->pop();
                    thr->push(*var);
                }
            }

            void op_or(variable *var, thread *thr)  override
            {
                thr->pop();
                if (var->data.r != 0)
                {
                    thr->pop();
                    thr->push(*var);
                }
            }

            void m_instance_of(variable *var, thread *thr, int arg_count) override
            {
                thr->pop();
                object *base = thr->peek().get_object();
                thr->pop(arg_count);
                variable result;
                if (base)
                    result.set_boolean(base == thr->pool->get_real_proto_instance()
                        || base == thr->pool->get_number_proto_instance()
                        || base == thr->pool->get_generic_proto_instance());
                else
                    result.set_boolean(false);
                thr->push(result);
            }

        protected:
            real_handler()
            {
            }

            template <template<typename R, typename A> class F> void unary_operation(variable *var, thread *thr)
            {
                thr->pop();
                variable result;
                result.set_real(F<double, double>::calculate(var->data.r));
                thr->push(result);
            }

            template <template<typename R, typename X, typename Y> class F> void binary_math_operation(variable *var, thread *thr)
            {
                thr->pop();
                variable right = thr->pop();
                double right_value;
                bool right_is_real = right.get_real(&right_value);
                if (!right_is_real)
                {
                    thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                    return;
                }
                variable result;
                result.set_real(F<double, double, double>::calculate(var->data.r, right_value));
                thr->push(result);
            }

            template <template<typename R, typename X, typename Y> class F, bool Def> void binary_logical_operation(variable *var, thread *thr)
            {
                thr->pop();
                variable right = thr->pop();
                double right_value;
                bool right_is_real = right.get_real(&right_value);
                variable result;
                if(right_is_real)
                {
                    result.set_boolean(F<bool, double, double>::calculate(var->data.r, right_value));
                }
                else
                {
                    result.set_boolean(Def);
                }
                thr->push(result);
            }
        };

        handler *handler::get_instance_real()
        {
            return real_handler::get_instance();
        }
    };
};
