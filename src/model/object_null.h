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

#pragma once

#include "object.h"

namespace g0at
{
    namespace model
    {
        class object_null : public object
        {
        friend class object_pool;
        protected:
            object_null(object_pool *pool);
            void init(object_pool *pool);
            bool is_void() override;

        public:
            object_null *to_object_null() override;
            std::wstring to_string() const override;
            goat_value * get_value(const goat_allocator *allocator) override;
            void op_not(thread *thr) override;
            void op_bool(thread *thr) override;
            void op_and(thread *thr) override;
            void op_or(thread *thr) override;
            void op_eq(thread *thr) override;
            void op_neq(thread *thr) override;
            void op_new(thread *thr, int arg_count) override;
            void m_iterator(thread *thr, int arg_count) override;
        };
    };
};