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
        class object_real : public object
        {
        friend class object_pool;
        protected:
            object_real(object_pool *pool, double _value);
            void reinit(double _value);

        public:
            void kill(object_pool *pool) override;
            object_type get_type() const override;
            object_real *to_object_real() override;
            bool less(const object *obj) const override;
            std::wstring to_string() const override;
            bool get_real(double *pval) override;

            void op_add(thread *thr) override;
            void op_sub(thread *thr) override;
            void op_neg(thread *thr) override;
            void op_eq(thread *thr) override;
            void op_neq(thread *thr) override;
            void op_less(thread *thr) override;

            double get_value() { return value; }

        protected:
            double value;

            template <template<typename R, typename A> class F> void unary_operation(thread *thr);
            template <template<typename R, typename X, typename Y> class F> void binary_math_operation(thread *thr);
            template <template<typename R, typename X, typename Y> class F, bool Def> void binary_logical_operation(thread *thr);
        };

        class object_real_proto : public object
        {
        friend class object_pool;
        protected:
            object_real_proto(object_pool *pool);
        };
    };
};