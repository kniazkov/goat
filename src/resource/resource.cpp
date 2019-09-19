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

#include "resource.h"
#include <cstring>

namespace g0at
{
    namespace resource
    {
        resource * resource::get_instance(const char *lang)
        {
            if (lang)
            {
                if (0 == std::strcmp(lang, "ru") || 0 == std::strcmp(lang, "russian"))
                    return get_intance_ru();

                if (0 == std::strcmp(lang, "en") || 0 == std::strcmp(lang, "english"))
                    return get_intance_en();
            }

            // default:

            return get_intance_en();
        }
    };
};