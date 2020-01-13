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
#include <vector>
#include <cstdint>

namespace g0at
{
    namespace model
    {
        class object_byte_array : public object
        {
        public:
            object_byte_array(object_pool *pool);
            object_byte_array(object_pool *pool, std::string data);
            object_byte_array(object_pool *pool, uint8_t *data, size_t size);
            object_byte_array *to_object_byte_array() override;
            std::wstring to_string() const override;

            void m_get(thread *thr, int arg_count) override;
            void m_set(thread *thr, int arg_count) override;
            void m_iterator(thread *thr, int arg_count) override;

            void add_byte(uint8_t byte) { vector.push_back(byte); }
            int get_length() { return (int)vector.size(); }
            uint8_t &get_byte_at(int idx) { return vector.at(idx); }
            uint8_t *get_data() { return &vector[0]; }

        protected:
            std::vector<uint8_t> vector;
        };

        class object_byte_array_proto : public object
        {
        friend class object_pool;
        protected:
            object_byte_array_proto(object_pool *pool);
            void init(object_pool *pool);
        public:
            void op_new(thread *thr, int arg_count) override;
        };
    };
};
