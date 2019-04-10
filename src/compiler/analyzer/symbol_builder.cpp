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

#include "symbol_builder.h"
#include "lib/assert.h"
#include "compiler/pt/declare_variable.h"
#include "compiler/pt/integer.h"

namespace g0at
{
    namespace analyzer
    {
        built_in_types::built_in_types()
            : type_integer(nullptr)
        {
        }

        symbol_builder::symbol_builder()
            : uid(0)
        {
        }

        void symbol_builder::payload(pt::declare_variable *ref)
        {
            auto scope = ref->get_scope();
            assert(scope != nullptr);
            for (int i = 0, count = ref->get_count(); i < count; i++)
            {
                auto info = ref->get_variable(i);
                lib::pointer<pt::symbol> symbol;
                if (info.init_val)
                    symbol = new pt::symbol(uid++, info.name, info.init_val->ret_type);
                else
                    symbol = new pt::symbol(uid++, info.name);
                scope->add_symbol(symbol);
            }
        }

        void symbol_builder::payload(pt::integer *ref)
        {
            ref->ret_type = get_type_integer();
        }
    };
};