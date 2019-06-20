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

namespace g0at
{
    namespace code
    {
        /**
         * @brief Identifier of instruction
         * 
         * The wrapper of primitive integer type.
         * I use this class as an identifier instead of a primitive type for type safety.
         */
        class iid_t
        {
        public:
            iid_t()
                : value(-1)
            {
            }

            iid_t(int _value)
                : value(_value)
            {
            }

            bool operator<(const iid_t &iid) const
            {
                return value < iid.value;
            }

            iid_t operator++()
            {
                ++value;
                return *this;
            }

            iid_t operator++(int)
            {
                iid_t old(*this);
                ++value;
                return old;
            }

            int as_int() { return value; }
            bool valid() { return value >= 0; }
        
        private:
            int value;
        };

        /**
         * @brief Pointer to identifier of instruction
         * 
         * The wrapper of raw pointer type.
         * I use this class instead of a raw pointer for type safety.
         */
        class iid_ptr_t
        {
        public:
            iid_ptr_t()
                : ptr(nullptr)
            {
            }

            iid_ptr_t(iid_t *_ptr)
                : ptr(_ptr)
            {
            }

            operator bool()
            {
                return ptr != nullptr;
            }
            
            void set(iid_t iid)
            {
                if (ptr)
                    *ptr = iid;
            }

        private:
            iid_t *ptr;
        };
    };
};
