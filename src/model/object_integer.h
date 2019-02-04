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
        class object_integer : public object
        {
        public:
            object_integer(object_list *list, int64_t _value);
            object_type get_type() const override;
            object_integer *to_object_integer() override;
            bool less(const object *obj) const override;
            std::wstring to_string() const override;
            bool get_integer(int64_t *pval) override;
            void op_add(thread *thr) override;
            void op_sub(thread *thr) override;
            void op_neg(thread *thr) override;

            int64_t get_value() { return value; }

        protected:
            int64_t value;
        };

        class object_integer_proto : public object
        {
        friend class object_list;
        protected:
            object_integer_proto(object_list *list);
        };
    };
};