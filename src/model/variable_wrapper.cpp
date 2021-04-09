/*

Copyright (C) 2017-2021 Ivan Kniazkov

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

#include "variable_wrapper.h"

namespace g0at
{
    namespace model
    {
        variable_wrapper::variable_wrapper(object_pool *pool, object *proto)
            : object(pool, proto)
        {
        }

        std::wstring variable_wrapper::to_string() const
        {
            return var.to_string();
        }

        std::wstring variable_wrapper::to_string_notation() const
        {
            return var.to_string_notation();
        }

        bool variable_wrapper::get_integer(int64_t *pval)
        {
            return var.get_integer(pval);
        }

        bool variable_wrapper::get_real(double *pval)
        {
            return var.get_real(pval);
        }

        bool variable_wrapper::get_boolean(bool *pval)
        {
            return var.get_boolean(pval);
        }

        bool variable_wrapper::get_char(wchar_t *pval)
        {
            return var.get_char(pval);
        }

        goat_value * variable_wrapper::get_value(goat_native_environment *env)
        {
            return var.get_value(env);
        }

        void variable_wrapper::op_add(thread *thr)
        {
            var.op_add(thr);
        }

        void variable_wrapper::op_sub(thread *thr)
        {
            var.op_sub(thr);
        }

        void variable_wrapper::op_pos(thread *thr)
        {
            var.op_pos(thr);
        }

        void variable_wrapper::op_neg(thread *thr)
        {
            var.op_neg(thr);
        }

        void variable_wrapper::op_inc(thread *thr)
        {
            var.op_inc(thr);
        }

        void variable_wrapper::op_dec(thread *thr)
        {
            var.op_dec(thr);
        }

        void variable_wrapper::op_not(thread *thr)
        {
            var.op_not(thr);
        }

        void variable_wrapper::op_bool(thread *thr)
        {
            var.op_bool(thr);
        }

        void variable_wrapper::op_inv(thread *thr)
        {
            var.op_inv(thr);
        }

        void variable_wrapper::op_mul(thread *thr)
        {
            var.op_mul(thr);
        }

        void variable_wrapper::op_exp(thread *thr)
        {
            var.op_exp(thr);
        }

        void variable_wrapper::op_div(thread *thr)
        {
            var.op_div(thr);
        }

        void variable_wrapper::op_mod(thread *thr)
        {
            var.op_mod(thr);
        }

        void variable_wrapper::op_eq(thread *thr)
        {
            var.op_eq(thr);
        }

        void variable_wrapper::op_neq(thread *thr)
        {
            var.op_neq(thr);
        }

        void variable_wrapper::op_shl(thread *thr)
        {
            var.op_shl(thr);
        }

        void variable_wrapper::op_shr(thread *thr)
        {
            var.op_shr(thr);
        }

        void variable_wrapper::op_shrz(thread *thr)
        {
            var.op_shrz(thr);
        }

        void variable_wrapper::op_less(thread *thr)
        {
            var.op_less(thr);
        }

        void variable_wrapper::op_leq(thread *thr)
        {
            var.op_leq(thr);
        }

        void variable_wrapper::op_great(thread *thr)
        {
            var.op_great(thr);
        }

        void variable_wrapper::op_greq(thread *thr)
        {
            var.op_greq(thr);
        }

        void variable_wrapper::op_inherit(thread *thr)
        {
            var.op_inherit(thr);
        }

        void variable_wrapper::op_and(thread *thr)
        {
            var.op_and(thr);
        }

        void variable_wrapper::op_or(thread *thr)
        {
            var.op_or(thr);
        }

        void variable_wrapper::op_bitand(thread *thr)
        {
            var.op_bitand(thr);
        }

        void variable_wrapper::op_bitor(thread *thr)
        {
            var.op_bitor(thr);
        }

        void variable_wrapper::op_xor(thread *thr)
        {
            var.op_xor(thr);
        }

        void variable_wrapper::op_protect(thread *thr)
        {
            var.op_protect(thr);
        }
    };
};
