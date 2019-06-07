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

#include "exception.h"

namespace g0at
{
    namespace lib
    {
        template <typename T> class buffer
        {
        public:
            buffer()
                : length(0), data(nullptr)
            {
            }

            ~buffer()
            {
                delete[] data;
            }

            void init(int size)
            {
                if (data)
                {
                    delete[] data;
                    data = nullptr;
                }
                if (size)
                    data = new T[size];
                length = size;
            }

            int size()
            {
                return length;
            }

            bool empty()
            {
                return length == 0;
            }

            T &at(int idx)
            {
                if (idx < 0 || idx >= length)
                    throw index_out_of_bounds();

                return data[idx];
            }

            T &operator[](int idx)
            {
                return at(idx);
            }

        protected:
            buffer(const buffer& ) { }
            void operator=(const buffer &) { }

            int length;
            T * data;
        };
    };
};
