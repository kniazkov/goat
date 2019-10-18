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

#include "object_function_built_in.h"
#include "object_exception.h"

namespace g0at
{
    namespace model
    {
        struct wrap_add
        {
            static void call(object *obj, thread *thr)
            {
                obj->op_add(thr);
            }
        };

        struct wrap_sub
        {
            static void call(object *obj, thread *thr)
            {
                obj->op_sub(thr);
            }
        };

        struct wrap_pos
        {
            static void call(object *obj, thread *thr)
            {
                obj->op_pos(thr);
            }
        };

        struct wrap_neg
        {
            static void call(object *obj, thread *thr)
            {
                obj->op_neg(thr);
            }
        };

        struct wrap_inc
        {
            static void call(object *obj, thread *thr)
            {
                obj->op_inc(thr);
            }
        };

        struct wrap_dec
        {
            static void call(object *obj, thread *thr)
            {
                obj->op_dec(thr);
            }
        };

        struct wrap_not
        {
            static void call(object *obj, thread *thr)
            {
                obj->op_not(thr);
            }
        };

        struct wrap_bool
        {
            static void call(object *obj, thread *thr)
            {
                obj->op_bool(thr);
            }
        };

        struct wrap_inv
        {
            static void call(object *obj, thread *thr)
            {
                obj->op_inv(thr);
            }
        };

        struct wrap_mul
        {
            static void call(object *obj, thread *thr)
            {
                obj->op_mul(thr);
            }
        };

        struct wrap_exp
        {
            static void call(object *obj, thread *thr)
            {
                obj->op_exp(thr);
            }
        };

        struct wrap_div
        {
            static void call(object *obj, thread *thr)
            {
                obj->op_div(thr);
            }
        };

        struct wrap_mod
        {
            static void call(object *obj, thread *thr)
            {
                obj->op_mod(thr);
            }
        };

        struct wrap_eq
        {
            static void call(object *obj, thread *thr)
            {
                obj->op_eq(thr);
            }
        };

        struct wrap_neq
        {
            static void call(object *obj, thread *thr)
            {
                obj->op_neq(thr);
            }
        };

        struct wrap_shl
        {
            static void call(object *obj, thread *thr)
            {
                obj->op_shl(thr);
            }
        };

        struct wrap_shr
        {
            static void call(object *obj, thread *thr)
            {
                obj->op_shr(thr);
            }
        };

        struct wrap_shrz
        {
            static void call(object *obj, thread *thr)
            {
                obj->op_shrz(thr);
            }
        };

        struct wrap_less
        {
            static void call(object *obj, thread *thr)
            {
                obj->op_less(thr);
            }
        };

        struct wrap_leq
        {
            static void call(object *obj, thread *thr)
            {
                obj->op_leq(thr);
            }
        };

        struct wrap_great
        {
            static void call(object *obj, thread *thr)
            {
                obj->op_great(thr);
            }
        };

        struct wrap_greq
        {
            static void call(object *obj, thread *thr)
            {
                obj->op_greq(thr);
            }
        };

        struct wrap_inherit
        {
            static void call(object *obj, thread *thr)
            {
                obj->op_inherit(thr);
            }
        };

        struct wrap_and
        {
            static void call(object *obj, thread *thr)
            {
                obj->op_and(thr);
            }
        };

        struct wrap_or
        {
            static void call(object *obj, thread *thr)
            {
                obj->op_or(thr);
            }
        };

        struct wrap_bitand
        {
            static void call(object *obj, thread *thr)
            {
                obj->op_bitand(thr);
            }
        };

        struct wrap_bitor
        {
            static void call(object *obj, thread *thr)
            {
                obj->op_bitor(thr);
            }
        };

        struct wrap_xor
        {
            static void call(object *obj, thread *thr)
            {
                obj->op_xor(thr);
            }
        };

        struct wrap_protect
        {
            static void call(object *obj, thread *thr)
            {
                obj->op_protect(thr);
            }
        };

        template <typename W> class unary_operator_adapter : public object_function_built_in
        {
        public:
            unary_operator_adapter(object_pool *pool)
                : object_function_built_in(pool)
            {
            }
            
            void call(thread *thr, int arg_count, call_mode mode) override
            {
                if (mode != call_mode::as_method)
                {
                    thr->raise_exception(new object_exception_illegal_context(thr->pool));
                    return;
                }
                if (arg_count != 0)
                {
                    thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                    return;
                }
                object *this_ptr = thr->peek().get_object();
                W::call(this_ptr, thr);
            }
        };

        template <typename W> class binary_operator_adapter : public object_function_built_in
        {
        public:
            binary_operator_adapter(object_pool *pool)
                : object_function_built_in(pool)
            {
            }
            
            void call(thread *thr, int arg_count, call_mode mode) override
            {
                if (mode != call_mode::as_method)
                {
                    thr->raise_exception(new object_exception_illegal_context(thr->pool));
                    return;
                }
                if (arg_count != 1)
                {
                    thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                    return;
                }
                object *this_ptr = thr->peek().get_object();
                W::call(this_ptr, thr);
            }
        };

        template <typename W0, typename W1> class unary_binary_operator_adapter : public object_function_built_in
        {
        public:
            unary_binary_operator_adapter(object_pool *pool)
                : object_function_built_in(pool)
            {
            }
            
            void call(thread *thr, int arg_count, call_mode mode) override
            {
                if (mode != call_mode::as_method)
                {
                    thr->raise_exception(new object_exception_illegal_context(thr->pool));
                    return;
                }
                object *this_ptr = thr->peek().get_object();
                if (arg_count == 0)
                {
                    W0::call(this_ptr, thr);
                }
                else if (arg_count == 1)
                {
                    W1::call(this_ptr, thr);
                }
                else
                {
                    thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                }
            }
        };
    };
};