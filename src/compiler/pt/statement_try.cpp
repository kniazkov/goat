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

#include "statement_try.h"

namespace g0at
{
    namespace pt
    {
        statement_try::statement_try(fragment _frag, lib::pointer<statement> _stmt_try, lib::pointer<statement> _stmt_catch, std::wstring _var_name)
            : statement(_frag), stmt_try(_stmt_try), stmt_catch(_stmt_catch), var_name(_var_name)
        {
        }

        statement_try::statement_try(fragment _frag, lib::pointer<statement> _stmt_try, lib::pointer<statement> _stmt_finally)
            : statement(_frag), stmt_try(_stmt_try), stmt_finally(_stmt_finally)
        {
        }

        statement_try::statement_try(fragment _frag, lib::pointer<statement> _stmt_try, lib::pointer<statement> _stmt_catch, std::wstring _var_name, lib::pointer<statement> _stmt_finally)
            : statement(_frag), stmt_try(_stmt_try), stmt_catch(_stmt_catch), var_name(_var_name), stmt_finally(_stmt_finally)
        {
        }

        void statement_try::accept(node_visitor *visitor)
        {
            visitor->visit(this);
        }

        statement_try *statement_try::to_statement_try()
        {
            return this;
        }
    };
};
