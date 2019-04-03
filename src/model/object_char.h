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
        class object_char : public object
        {
        friend class object_pool;
        protected:
            object_char(object_pool *pool, wchar_t _value);
            void reinit(wchar_t _value);

        public:
            void kill(object_pool *pool) override;
            object_type get_type() const override;
            object_char *to_object_char() override;
            bool less(const object *obj) const override;
            std::wstring to_string() const override;
            std::wstring to_string_notation() const override;
            bool get_char(wchar_t *pval) override;

            wchar_t get_value() { return value; }

        protected:
            wchar_t value;
        };

        class object_char_proto : public object
        {
        friend class object_pool;
        protected:
            object_char_proto(object_pool *pool);
        };
    };
};