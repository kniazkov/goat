/*

Copyright (C) 2017-2020 Ivan Kniazkov

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
#include "object_exception.h"
#include "resource/strings.h"
#include "lib/functional.h"
#include "thread.h"
#include "lib/utils.h"
#include "lib/assert.h"

namespace g0at
{
    namespace model
    {
        static bool convert_to_integer(variable &var, variable *result, int radix)
        {
            int64_t int_value;
            if (var.get_integer(&int_value))
            {
                result->set_integer(int_value);
                return true;
            }
            double real_value;
            if (var.get_real(&real_value))
            {
                result->set_integer((int64_t)real_value);
                return true;
            }
            bool boolean_value;
            if (var.get_boolean(&boolean_value))
            {
                result->set_integer(boolean_value ? 1 : 0);
                return true;
            }
            wchar_t char_value;
            if (var.get_char(&char_value))
            {
                result->set_integer(char_value);
                return true;
            }
            object *obj = var.get_object();
            if (obj)
            {
                object_string *obj_string = obj->to_object_string();
                if (obj_string)
                {
                    std::wstring string_value = obj_string->get_data();
                    if (lib::wstring_to_int64(string_value, &int_value, radix))
                    {
                        result->set_integer(int_value);
                        return true;
                    }
                }
            }
            return false;
        }

        object_integer::object_integer(object_pool *pool, int64_t value)
            : variable_wrapper(pool, pool->get_integer_proto_instance())
        {
            var.set_integer(value);
        }

#ifndef GC_DEBUG
        void object_integer::kill(object_pool *pool)
        {
            if (pool->integers.destroy_or_cache(this, pool))
            {
                objects.clear();
                immutable = false;
            }
        }
#endif

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

        class object_integer_valueof : public object_function_built_in
        {
        public:
            object_integer_valueof(object_pool *_pool)
                : object_function_built_in(_pool)
            {
            }
            
            void call(thread *thr, int arg_count, call_mode mode) override
            {
                if (arg_count > 0)
                {
                    if (mode == call_mode::as_method)
                        thr->pop();
                    variable arg_value = thr->peek(0);
                    int64_t arg_radix = 10;
                    if (arg_count > 1)
                    {
                        if (!thr->peek(1).get_integer(&arg_radix) || (arg_radix != 2 && arg_radix != 10 && arg_radix != 16))
                        {
                            thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                            return;
                        }
                    }
                    thr->pop(arg_count);
                    variable result;
                    if (!convert_to_integer(arg_value, &result, (int)arg_radix))
                    {
                        result.set_object(thr->pool->get_null_instance());
                    }
                    thr->push(result);
                    return;
                }
                thr->raise_exception(new object_exception_illegal_argument(thr->pool));
            }
        };

        class object_integer_string : public object_function_built_in
        {
        public:
            object_integer_string(object_pool *_pool)
                : object_function_built_in(_pool)
            {
            }
            
            void call(thread *thr, int arg_count, call_mode mode) override
            {
                if (mode != call_mode::as_method)
                {
                    thr->raise_exception(new object_exception_illegal_context(thr->pool));
                    return;
                }
                int64_t value;
                if (!thr->pop().get_integer(&value))
                {
                    thr->raise_exception(new object_exception_illegal_context(thr->pool));
                    return;
                }
                int64_t arg_radix = 10;
                if (arg_count > 0)
                {
                    if (!thr->peek().get_integer(&arg_radix) || (arg_radix != 2 && arg_radix != 10 && arg_radix != 16))
                    {
                        thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                        return;
                    }
                }
                thr->pop(arg_count);
                variable result;
                result.set_object(thr->pool->create_object_string(lib::int64_to_wstring(value, (int)arg_radix)));
                thr->push(result);
            }
        };

        void object_integer_proto::init(object_pool *pool)
        {
            add_object(pool->get_static_string(resource::str_valueof), new object_integer_valueof(pool));
            add_object(pool->get_static_string(resource::str_string), new object_integer_string(pool));
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
            add_object(pool->get_static_string(resource::str_oper_double_exclamation), pool->get_wrap_bool_instance());
            add_object(pool->get_static_string(resource::str_oper_double_less), pool->get_wrap_shl_instance());
            add_object(pool->get_static_string(resource::str_oper_double_greater), pool->get_wrap_shr_instance());
            add_object(pool->get_static_string(resource::str_oper_triple_greater), pool->get_wrap_shrz_instance());
            add_object(pool->get_static_string(resource::str_oper_less), pool->get_wrap_less_instance());
            add_object(pool->get_static_string(resource::str_oper_less_equal), pool->get_wrap_leq_instance());
            add_object(pool->get_static_string(resource::str_oper_greater), pool->get_wrap_great_instance());
            add_object(pool->get_static_string(resource::str_oper_greater_equal), pool->get_wrap_greq_instance());
            add_object(pool->get_static_string(resource::str_oper_ampersand), pool->get_wrap_bitand_instance());
            add_object(pool->get_static_string(resource::str_oper_vertical_bar), pool->get_wrap_bitor_instance());
            add_object(pool->get_static_string(resource::str_oper_caret), pool->get_wrap_xor_instance());
            add_object(pool->get_static_string(resource::str_oper_double_ampersand), pool->get_wrap_and_instance());
            add_object(pool->get_static_string(resource::str_oper_double_vertical_bar), pool->get_wrap_or_instance());
            lock();
        }

        void object_integer_proto::op_new(thread *thr, int arg_count)
        {
            variable result;
            if (arg_count > 0)
            {
                variable arg = thr->peek();
                thr->pop(arg_count);
                if (!convert_to_integer(arg, &result, 10))
                {
                    result.set_object(thr->pool->get_null_instance());
                }
            }
            else
            {
                result.set_integer(0);
            }
            thr->push(result);
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

            bool get_integer(variable *var, int64_t *pval) override
            {
                *pval = var->data.i;
                return true;
            }

            bool get_real(variable *var, double *pval) override
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

            void op_bool(variable *var, thread *thr)  override
            {
                thr->pop();
                variable result;
                result.set_boolean(var->data.i != 0);
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
                variable result;
                thr->pop();
                variable right = thr->pop();
                int64_t right_int_value;
                bool right_is_integer = right.get_integer(&right_int_value);
                if(right_is_integer)
                {
                    if (right_int_value == 0)
                    {
                        thr->raise_exception(new object_exception_division_by_zero(thr->pool));
                        return;
                    }
                    result.set_integer(var->data.i / right_int_value);
                    thr->push(result);
                    return;
                }
                double right_real_value;
                bool right_is_real = right.get_real(&right_real_value);
                if (!right_is_real)
                {
                    thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                    return;
                }
                if (right_real_value == 0)
                {
                    thr->raise_exception(new object_exception_division_by_zero(thr->pool));
                    return;
                }
                result.set_real(var->data.i / right_real_value);
                thr->push(result);
            }

            void op_mod(variable *var, thread *thr)  override
            {
                binary_math_integer_operation<lib::func::mod>(var, thr);
            }

            void op_eq(variable *var, thread *thr)  override
            {
                binary_logical_operation<lib::func::equals, false>(var, thr);
            }

            void op_neq(variable *var, thread *thr)  override
            {
                binary_logical_operation<lib::func::not_equal, true>(var, thr);
            }

            void op_shl(variable *var, thread *thr)  override
            {
                variable result;
                thr->pop();
                variable right = thr->pop();
                int64_t right_int_value;
                bool right_is_integer = right.get_integer(&right_int_value);
                if(right_is_integer)
                {
                    if (right_int_value > 63 || right_int_value < 0)
                        result.set_integer(0);
                    else
                        result.set_integer(var->data.i << right_int_value);
                    thr->push(result);
                }
                else
                {
                    thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                }
            }

            void op_shr(variable *var, thread *thr)  override
            {
                variable result;
                thr->pop();
                variable right = thr->pop();
                int64_t right_int_value;
                bool right_is_integer = right.get_integer(&right_int_value);
                if(right_is_integer)
                {
                    if (right_int_value < 0)
                        result.set_integer(0);
                    else if (right_int_value > 63)
                        result.set_integer(var->data.i < 0 ? -1 : 0);
                    else
                        result.set_integer(var->data.i >> right_int_value);
                    thr->push(result);
                }
                else
                {
                    thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                }
            }

            void op_shrz(variable *var, thread *thr)  override
            {
                variable result;
                thr->pop();
                variable right = thr->pop();
                int64_t right_int_value;
                bool right_is_integer = right.get_integer(&right_int_value);
                if(right_is_integer)
                {
                    if (right_int_value > 63 || right_int_value < 0)
                        result.set_integer(0);
                    else
                        result.set_integer((int64_t)((uint64_t)var->data.i >> (uint64_t)right_int_value));
                    thr->push(result);
                }
                else
                {
                    thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                }
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
                if (var->data.i == 0)
                {
                    thr->pop();
                    thr->push(*var);
                }
            }

            void op_or(variable *var, thread *thr)  override
            {
                thr->pop();
                if (var->data.i != 0)
                {
                    thr->pop();
                    thr->push(*var);
                }
            }

            void op_bitand(variable *var, thread *thr)  override
            {
                binary_math_integer_operation<lib::func::bitwise_and>(var, thr);
            }

            void op_bitor(variable *var, thread *thr)  override
            {
                binary_math_integer_operation<lib::func::bitwise_or>(var, thr);
            }

            void op_xor(variable *var, thread *thr)  override
            {
                binary_math_integer_operation<lib::func::bitwise_xor>(var, thr);
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
                    thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                    return;
                }
                result.set_real(F<double, int64_t, double>::calculate(var->data.i, right_real_value));
                thr->push(result);
            }

            template <template<typename R, typename X, typename Y> class F> void binary_math_integer_operation(variable *var, thread *thr)
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
                thr->raise_exception(new object_exception_illegal_argument(thr->pool));
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
