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

#include "symbol_builder.h"
#include "lib/assert.h"
#include "compiler/pt/declare_variable.h"
#include "compiler/pt/integer.h"

namespace g0at
{
    namespace analyzer
    {
        symbol_builder::symbol_builder(pt::root_scope::root_scope *_root_scope)
            : uid(0), root_scope(_root_scope)
        {
        }

        void symbol_builder::payload(pt::declare_variable *ref)
        {
            auto scope = ref->get_scope();
            if (scope == nullptr)
                return;
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
            ref->ret_type = root_scope->get_type_integer();
        }
    };
};