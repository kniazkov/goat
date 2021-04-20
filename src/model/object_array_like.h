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

namespace g0at
{
    namespace model
    {
        class object_array_like : public object
        {
        public:
            object_array_like(object_pool *pool, object *proto);
            object_array_like *to_object_array_like() override;
            std::wstring to_string() const override;
            goat_value * get_value(goat_ext_environment *env) override;

            void m_get(thread *thr, int arg_count) override;
            void m_set(thread *thr, int arg_count) override;
            void m_iterator(thread *thr, int arg_count) override;

            virtual int get_length() const = 0;
            virtual variable get_item(int idx) const = 0;
            virtual variable get_item(int idx) = 0;
        };

        class object_array_like_proto : public object
        {
        friend class object_pool;
        protected:
            object_array_like_proto(object_pool *pool);
            void init(object_pool *pool);
        public:
            void op_new(thread *thr, int arg_count) override;
        };
    };
};
