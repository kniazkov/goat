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

#include "statement_try.h"

namespace g0at
{
    namespace ast
    {
        statement_try::statement_try(keyword_try *_kw, lib::pointer<statement> _stmt_try, lib::pointer<statement> _stmt_catch, std::wstring _var_name)
            : stmt_try(_stmt_try), stmt_catch(_stmt_catch), var_name(_var_name)
        {
            frag.begin = _kw->get_fragment().begin;
            frag.end = _stmt_catch->get_fragment().end;
        }

        statement_try::statement_try(keyword_try *_kw, lib::pointer<statement> _stmt_try, lib::pointer<statement> _stmt_finally)
            : stmt_try(_stmt_try), stmt_finally(_stmt_finally)
        {
            frag.begin = _kw->get_fragment().begin;
            frag.end = _stmt_finally->get_fragment().end;
        }

        statement_try::statement_try(keyword_try *_kw, lib::pointer<statement> _stmt_try, lib::pointer<statement> _stmt_catch, std::wstring _var_name, lib::pointer<statement> _stmt_finally)
            : stmt_try(_stmt_try), stmt_catch(_stmt_catch), var_name(_var_name), stmt_finally(_stmt_finally)
        {
            frag.begin = _kw->get_fragment().begin;
            frag.end = _stmt_finally->get_fragment().end;
        }

        void statement_try::accept(token_visitor *visitor)
        {
            visitor->visit(this);
        }

        statement_try *statement_try::to_statement_try()
        {
            return this;
        }
    };
};
