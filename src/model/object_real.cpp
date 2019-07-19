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
#include "thread.h"
#include "lib/functional.h"
#include "lib/utils.h"
#include "lib/assert.h"

namespace g0at
{
    namespace model
    {
        object_real::object_real(object_pool *pool, double _value)
            : object(pool, pool->get_real_proto_instance()), value(_value)
        {
        }

        void object_real::reinit(double _value)
        {
            value = _value;
        }

        void object_real::kill(object_pool *pool)
        {
            if (pool->real_numbers.destroy_or_cache(this, pool))
            {
                objects.clear();
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
            return value < obj_real->value;
        }

        std::wstring object_real::to_string() const
        {
            return lib::double_to_wstring(value);
        }

        bool object_real::get_real(double *pval)
        {
            *pval = value;
            return true;
        }

        void object_real::op_add(thread *thr)
        {
            binary_math_operation<lib::func::plus>(thr);
        }

        void object_real::op_sub(thread *thr)
        {
            binary_math_operation<lib::func::minus>(thr);
        }

        void object_real::op_neg(thread *thr)
        {
            unary_operation<lib::func::neg>(thr);
        }

        void object_real::op_inc(thread *thr)
        {
            unary_operation<lib::func::inc>(thr);
        }

        void object_real::op_dec(thread *thr)
        {
            unary_operation<lib::func::dec>(thr);
        }

        void object_real::op_eq(thread *thr)
        {
            binary_logical_operation<lib::func::equals, false>(thr);
        }

        void object_real::op_neq(thread *thr)
        {
            binary_logical_operation<lib::func::not_equal, true>(thr);
        }

        void object_real::op_less(thread *thr)
        {
            binary_logical_operation<lib::func::less, true>(thr);
        }

        void object_real::m_iterator(thread *thr, int arg_count)
        {
            thr->pop();
            thr->pop(arg_count);
            variable tmp;
            tmp.set_object(thr->pool->get_iterator_proto_instance());
            thr->push(tmp);
        }

        template <template<typename R, typename A> class F> void object_real::unary_operation(thread *thr)
        {
            thr->pop();
            variable result;
            result.set_real(F<double, double>::calculate(value));
            thr->push(result);
        }

        template <template<typename R, typename X, typename Y> class F> void object_real::binary_math_operation(thread *thr)
        {
            thr->pop();
            variable right = thr->pop();
            double right_value;
            bool right_is_real = right.get_real(&right_value);
            if (!right_is_real)
            {
                thr->raise_exception(thr->pool->get_exception_illegal_argument_instance());
                return;
            }
            variable result;
            result.set_real(F<double, double, double>::calculate(value, right_value));
            thr->push(result);
        }

        template <template<typename R, typename X, typename Y> class F, bool Def> void object_real::binary_logical_operation(thread *thr)
        {
            thr->pop();
            variable right = thr->pop();
            double right_value;
            bool right_is_real = right.get_real(&right_value);
            variable result;
            if(right_is_real)
            {
                result.set_boolean(F<bool, double, double>::calculate(value, right_value));
            }
            else
            {
                result.set_boolean(Def);
            }
            thr->push(result);
        }

        /*
            Prototype
        */

        template <template<typename R, typename A> class F> class object_real_unary_operator : public object_function_built_in
        {
        public:
            object_real_unary_operator(object_pool *_pool)
                : object_function_built_in(_pool)
            {
            }
            
            void call(thread *thr, int arg_count, call_mode mode) override
            {
                if (mode != call_mode::as_method)
                {
                    thr->raise_exception(thr->pool->get_exception_illegal_context_instance());
                    return;
                }
                object *this_ptr = thr->pop().get_object();
                assert(this_ptr != nullptr);
                object_real *this_ptr_real = this_ptr->to_object_real();
                if (!this_ptr_real)
                {
                    thr->raise_exception(thr->pool->get_exception_illegal_context_instance());
                    return;
                }
                thr->pop(arg_count);
                variable result;
                result.set_real(F<double, double>::calculate(this_ptr_real->get_value()));
                thr->push(result);
            }
        };

        template <template<typename R, typename X, typename Y> class F1, template<typename R, typename A> class F2>
        class object_real_binary_unary_math_operator : public object_function_built_in
        {
        public:
            object_real_binary_unary_math_operator(object_pool *_pool)
                : object_function_built_in(_pool)
            {
            }
            
            void call(thread *thr, int arg_count, call_mode mode) override
            {
                if (mode != call_mode::as_method)
                {
                    thr->raise_exception(thr->pool->get_exception_illegal_context_instance());
                    return;
                }
                object *this_ptr = thr->pop().get_object();
                assert(this_ptr != nullptr);
                object_real *this_ptr_real = this_ptr->to_object_real();
                if (!this_ptr_real)
                {
                    thr->raise_exception(thr->pool->get_exception_illegal_context_instance());
                    return;
                }
                variable result;
                if (arg_count < 1)
                {
                    result.set_real(F2<double, double>::calculate(this_ptr_real->get_value()));
                    thr->push(result);
                    return;
                }
                variable right = thr->peek();
                thr->pop(arg_count);
                double right_real_value;
                bool right_is_real = right.get_real(&right_real_value);
                if (!right_is_real)
                {
                    thr->raise_exception(thr->pool->get_exception_illegal_argument_instance());
                    return;
                }
                result.set_real(F1<double, double, double>::calculate(this_ptr_real->get_value(), right_real_value));
                thr->push(result);
            }
        };

        template <template<typename R, typename X, typename Y> class F> class object_real_binary_math_operator : public object_function_built_in
        {
        public:
            object_real_binary_math_operator(object_pool *_pool)
                : object_function_built_in(_pool)
            {
            }
            
            void call(thread *thr, int arg_count, call_mode mode) override
            {
                if (mode != call_mode::as_method)
                {
                    thr->raise_exception(thr->pool->get_exception_illegal_context_instance());
                    return;
                }
                if (arg_count < 1)
                {
                    thr->raise_exception(thr->pool->get_exception_illegal_argument_instance());
                    return;
                }
                object *this_ptr = thr->pop().get_object();
                assert(this_ptr != nullptr);
                object_real *this_ptr_real = this_ptr->to_object_real();
                if (!this_ptr_real)
                {
                    thr->raise_exception(thr->pool->get_exception_illegal_context_instance());
                    return;
                }
                variable result;
                variable right = thr->peek();
                thr->pop(arg_count);
                double right_real_value;
                bool right_is_real = right.get_real(&right_real_value);
                if (!right_is_real)
                {
                    thr->raise_exception(thr->pool->get_exception_illegal_argument_instance());
                    return;
                }
                result.set_real(F<double, double, double>::calculate(this_ptr_real->get_value(), right_real_value));
                thr->push(result);
            }
        };

        object_real_proto::object_real_proto(object_pool *pool)
            : object(pool, pool->get_number_proto_instance())
        {
        }

        void object_real_proto::init(object_pool *pool)
        {
            add_object(pool->get_static_string(L"++"), new object_real_unary_operator<lib::func::inc>(pool));
            add_object(pool->get_static_string(L"--"), new object_real_unary_operator<lib::func::dec>(pool));
            add_object(pool->get_static_string(L"+"), new object_real_binary_math_operator<lib::func::plus>(pool));
            add_object(pool->get_static_string(L"-"), new object_real_binary_unary_math_operator<lib::func::minus, lib::func::neg>(pool));
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
                    thr->raise_exception(thr->pool->get_exception_illegal_argument_instance());
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
