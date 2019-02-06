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

#include "object_void.h"

namespace g0at
{
    namespace model
    {
        object_void::object_void(object_list *list)
            : object(list)
        {
            proto.push_back(list->get_generic_proto_instance());
        }

        object_void *object_void::to_object_void()
        {
            return this;
        }

        std::wstring object_void::to_string() const
        {
            return L"void";
        }
    };
};