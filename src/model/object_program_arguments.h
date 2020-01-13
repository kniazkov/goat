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

#include "object_array_like.h"
#include <vector>

namespace g0at
{
    namespace model
    {
        class object_program_arguments : public object_array_like
        {
        public:
            object_program_arguments(object_pool *pool, std::vector<char*> *args);
            void trace() override;
            void trace_parallel(object_pool *pool) override;

            int get_length() const override;
            variable get_item(int idx) const override;
            variable get_item(int idx) override;

        protected:
            std::vector<object*> data;
        };
    };
};
