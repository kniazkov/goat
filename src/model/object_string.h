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
        class object_string : public object
        {
        friend class object_pool;
        protected:
            object_string(object_pool *pool, std::wstring _data);
            object_string(object_pool *pool, std::wstring _data, int _id);
            void reinit(std::wstring _data);
            void reinit(std::wstring _data, int _id);
            
        public:
            void kill(object_pool *pool) override;
            object_type get_type() const override;
            object_string *to_object_string() override;
            bool less(const object *obj) const override;
            std::wstring to_string() const override;
            std::wstring to_string_notation() const override;
            void op_add(thread *thr) override;
            void op_less(thread *thr) override;
            void m_get(thread *thr, int arg_count) override;
            void m_set(thread *thr, int arg_count) override;

            std::wstring get_data() { return data; }

        protected:
            std::wstring data;
            int id;
        };

        class object_string_proto : public object
        {
        friend class object_pool;
        protected:
            object_string_proto(object_pool *pool);
            void init(object_pool *pool);
        };
    };
};