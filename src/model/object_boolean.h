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
        class object_boolean : public object
        {
        public:
            object_boolean(object_pool *pool, bool _value);
            object_type get_type() const override;
            object_boolean *to_object_boolean() override;
            bool less(const object *obj) const override;
            std::wstring to_string() const override;
            void clear() override;
            bool get_boolean(bool *pval) override;

            bool get_value() { return value; }

        protected:
            bool value;

            template <template<typename R, typename A> class F> void unary_operation(thread *thr);
            template <template<typename R, typename X, typename Y> class F> void binary_operation(thread *thr);
        };

        class object_boolean_proto : public object
        {
        friend class object_pool;
        protected:
            object_boolean_proto(object_pool *pool);
        };
    };
};