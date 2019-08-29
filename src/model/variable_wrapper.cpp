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

#include "variable_wrapper.h"

namespace g0at
{
    namespace model
    {
        variable_wrapper::variable_wrapper(object_pool *_pool, object *_proto, variable _var)
            : object(_pool, _proto), var(_var)
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

        void variable_wrapper::op_less(thread *thr)
        {
            var.op_less(thr);
        }

        void variable_wrapper::op_inherit(thread *thr)
        {
            var.op_inherit(thr);
        }
    };
};
