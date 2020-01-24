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

namespace g0at
{
    namespace lib
    {
        struct gpio_info
        {
            gpio_info(const unsigned int _count, const unsigned int *_port_numbers)
                : count(_count), port_numbers(_port_numbers)
            {
            }

            const unsigned int count;
            const unsigned int *port_numbers; 
        };

        void gpio_init();
        gpio_info gpio_get_info();
        void gpio_set_value(unsigned int port, bool value);
        bool gpio_get_value(unsigned int port);
    };
};