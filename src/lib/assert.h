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

#include "exception.h"

namespace g0at
{
    namespace lib
    {
        class assertion_failed : public lib::exception
        {
        public:
            assertion_failed(const char *file, int line, const char *assertion)
                : exception(global::resource->assertion_failed(file, line, assertion))
            {
            }
        };
    };
};

#ifdef assert
#undef assert
#endif

#ifdef NDEBUG
#define assert(expr) ((void) 0)
#else
#define assert(expr) if (!(expr)) throw g0at::lib::assertion_failed(__FILE__, __LINE__, #expr)
#endif
