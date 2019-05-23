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

#include "root_scope.h"

namespace g0at
{
    namespace pt
    {
        namespace root_scope
        {
            root_scope::root_scope()
            {
                /*
                    Object
                */
                type_object = new pt::type();
                add_type(type_object);
                lib::pointer<pt::symbol> symbol_object = new pt::symbol(L"Object", type_object.get());
                add_symbol(symbol_object);

                /*
                    Integer
                */
                lib::pointer<pt::type> type_integer_proto = new pt::type(type_object);
                add_type(type_integer_proto);
                type_integer = new pt::type(type_integer_proto.get());
                add_type(type_integer);
                lib::pointer<pt::symbol> symbol_integer = new pt::symbol(L"Integer", type_integer_proto.get());
                add_symbol(symbol_integer);
              }
        };
    };
};
