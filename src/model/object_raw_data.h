/*

Copyright (C) 2021 Ivan Kniazkov

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
        class object_raw_data : public object
        {
        public:
            object_raw_data(object_pool *_pool, void *_raw_data, const char *_descriptor);
            object_raw_data *to_object_raw_data() override;
            std::wstring to_string() const override;
            goat_value * get_value(const goat_allocator *allocator) override;

        private:
            void *raw_data;
            const char *descriptor;
        };

        class object_raw_data_proto : public object
        {
        friend class object_pool;
        protected:
            object_raw_data_proto(object_pool *pool);
        public:
            void op_new(thread *thr, int arg_count) override;
        };
    };
};