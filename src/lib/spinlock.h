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

#include <atomic>

namespace g0at
{
    namespace lib
    {
        class spinlock
        {
        public:
            spinlock()
            {
                 flag.clear();
            }

            spinlock(const spinlock&) = delete;
            ~spinlock() = default;

            void lock()
            {
                while (flag.test_and_set(std::memory_order_acquire))
                ;
            }

            bool try_lock()
            {
                return !flag.test_and_set(std::memory_order_acquire);
            }

            void unlock()
            {
                flag.clear(std::memory_order_release);
            }

        private:
            std::atomic_flag flag;
        };
    }
}