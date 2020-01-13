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

#include "compiler/pt/scope.h"

namespace g0at
{
    namespace pt
    {
        namespace root_scope
        {
            class root_scope : public pt::scope
            {
            public:
                root_scope();
                pt::type_object* get_type_object() { return type_object.get(); }
                pt::type* get_type_integer() { return type_integer.get(); }

            private:
                lib::pointer<pt::type_object> type_object;
                lib::pointer<pt::type_function_proto> type_function_proto;
                lib::pointer<pt::type> type_integer;
            };
        };
    };
};
