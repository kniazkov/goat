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

#include "object_function.h"

namespace g0at
{
    namespace model
    {
        object_function::object_function(object_list *list)
            : object(list)
        {
        } 

        object_type object_function::get_type() const
        {
            return FUNCTION;
        }

        object_function *object_function::to_object_function()
        {
            return this;
        }

        std::wstring object_function::to_string() const
        {
            return L"function";
        }
    };
};
