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

#include "global/global.h"
#include <exception>
#include <string>

namespace g0at
{
    namespace lib
    {
        class exception : public std::exception
        {
        public:
            exception(std::wstring message);
            const char* what() const throw() override;

        protected:
            std::string data;
        };

        class file_not_found : public lib::exception
        {
        public:
            file_not_found(const char *file_name)
                : exception(global::resource->file_not_found(file_name))
            {
            }
        };
    };
};