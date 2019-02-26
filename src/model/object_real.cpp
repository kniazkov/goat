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
#include "lib/functional.h"
#include "lib/utils.h"
#include "thread.h"
#include <assert.h>

namespace g0at
{
    namespace model
    {
        object_real::object_real(object_list *list, double _value)
            : object(list), value(_value)
        {
            proto.push_back(list->get_integer_proto_instance());
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

        void object_real::op_eq(thread *thr)
        {
            binary_logical_operation<lib::func::equals, false>(thr);
        }

        void object_real::op_neq(thread *thr)
        {
            binary_logical_operation<lib::func::not_equal, true>(thr);
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
            assert(right_is_real);
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

        object_real_proto::object_real_proto(object_list *list)
            : object(list)
        {
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

            object *to_object(variable *var, object_list *list) override
            {
                object *obj = new object_real(list, var->data.r);
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

            void op_eq(variable *var, thread *thr)  override
            {
                binary_logical_operation<lib::func::equals, false>(var, thr);
            }

            void op_neq(variable *var, thread *thr)  override
            {
                binary_logical_operation<lib::func::not_equal, false>(var, thr);
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
                assert(right_is_real);
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
