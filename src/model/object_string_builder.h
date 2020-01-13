/*

Copyright (C) 2017-2020 Ivan Kniazkov

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
#include <sstream>

namespace g0at
{
    namespace model
    {
        class object_string_builder : public object
        {
        public:
            object_string_builder(object_pool *pool);
            object_string_builder *to_object_string_builder() override;
            std::wstring to_string() const override;
            std::wstring to_string_notation() const override;
            void op_shl(thread *thr) override;

            std::wstring get_data() { return data.str(); }
            int64_t get_length() { return length; }
            void append(std::wstring str)
            {
                data << str;
                length += str.size();
            }

        protected:
            std::wstringstream data;
            int64_t length;
        };

        class object_string_builder_proto : public object
        {
        friend class object_pool;
        protected:
            object_string_builder_proto(object_pool *pool);
            void init(object_pool *pool);
        public:
            void op_new(thread *thr, int arg_count) override;
        };
    };
};