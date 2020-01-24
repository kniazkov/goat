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

#include "settings.h"
#include "gpio.h"

#ifdef GPIO_DEBUG
#include "utils.h"
#include <iostream>
#endif

namespace g0at
{
    namespace lib
    {
#ifdef GPIO_ENABLE

#ifdef GPIO_EMULATION
#define GPIO_IMPLEMENTED
        static const unsigned int gpio_port_numbers[] = {0, 1, 2, 3, 4, 5, 6, 7};
        static const unsigned int gpio_count = sizeof(gpio_port_numbers) / sizeof(unsigned int);
#ifdef GPIO_DEBUG
        static int64_t gpio_time_start;
#endif

        bool gpio_values[gpio_count];

        void gpio_init()
        {
            for (unsigned int i = 0; i < gpio_count; i++)
                gpio_values[i] = false;
#ifdef GPIO_DEBUG
            gpio_time_start = get_time_ns();
#endif
        }

        gpio_info gpio_get_info()
        {
            return gpio_info(gpio_count, gpio_port_numbers);
        }

        void gpio_set_value(unsigned int port, bool value)
        {
            if (port < gpio_count)
            {
#ifdef GPIO_DEBUG
                if (gpio_values[port] != value)
                {
                    int64_t gpio_time = get_time_ns() - gpio_time_start;
                    std::cout << "\n*** gpio " << port << '\t' << (value ? '1' : '0') << '\t' << gpio_time;
                }
#endif
                gpio_values[port] = value;
            }
        }

        bool gpio_get_value(unsigned int port)
        {
            return port < gpio_count ? gpio_values[port] : false;
        }
#endif // GPIO_EMULATION

#endif // GPIO_ENABLE

#ifndef GPIO_IMPLEMENTED
        void gpio_init()
        {
        }

        gpio_info gpio_get_info()
        {
            return gpio_info(0, nullptr);
        }

        void gpio_set_value(unsigned int port, bool value)
        {
        }

        bool gpio_get_value(unsigned int port)
        {
            return false;
        }
#endif
    };
};
