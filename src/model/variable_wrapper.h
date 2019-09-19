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

#include "object.h"

namespace g0at
{
    namespace model
    {
        class variable_wrapper : public object
        {
        public:
            variable_wrapper(object_pool *pool, object *proto);

            std::wstring to_string() const override;
            std::wstring to_string_notation() const override;

            bool get_integer(int64_t *pval) override;
            bool get_real(double *pval) override;
            bool get_boolean(bool *pval) override;
            bool get_char(wchar_t *pval) override;

            void op_add(thread *thr) override;
            void op_sub(thread *thr) override;
            void op_pos(thread *thr) override;
            void op_neg(thread *thr) override;
            void op_inc(thread *thr) override;
            void op_dec(thread *thr) override;
            void op_not(thread *thr) override;
            void op_bool(thread *thr) override;
            void op_inv(thread *thr) override;
            void op_mul(thread *thr) override;
            void op_exp(thread *thr) override;
            void op_div(thread *thr) override;
            void op_mod(thread *thr) override;
            void op_eq(thread *thr) override;
            void op_neq(thread *thr) override;
            void op_shl(thread *thr) override;
            void op_shr(thread *thr) override;
            void op_shrz(thread *thr) override;
            void op_less(thread *thr) override;
            void op_leq(thread *thr) override;
            void op_great(thread *thr) override;
            void op_greq(thread *thr) override;
            void op_inherit(thread *thr) override;
            void op_and(thread *thr) override;
            void op_or(thread *thr) override;
            void op_bitand(thread *thr) override;
            void op_bitor(thread *thr) override;
            void op_xor(thread *thr) override;

        protected:
            variable var;
        };
    };
};