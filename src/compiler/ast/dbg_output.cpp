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

#include "dbg_output.h"
#include "function.h"
#include "identifier.h"
#include "bracket.h"
#include "static_string.h"

namespace g0at
{
    namespace ast
    {
        dbg_output::dbg_output(std::wstringstream &_stream)
            : stream(_stream), indent(0)
        {
        }

        dbg_output::dbg_output(std::wstringstream &_stream, int _indent)
            : stream(_stream), indent(_indent)
        {
        }

        void dbg_output::visit(function *ref)
        {
            auto tok_list = ref->get_list();
            auto tok = tok_list->first;
            while(tok)
            {
                tok->accept(this);
                tok = tok->next;
            }
        }

        void dbg_output::visit(identifier *ref)
        {
            stream << ref->get_name();
        }

        void dbg_output::visit(bracket *ref)
        {
            stream << ref->get_symbol();
        }

        void dbg_output::visit(static_string *ref)
        {
            stream << L'\"' << ref->get_text() << L'\"';
        }

        void dbg_output::visit(semicolon *ref)
        {
            stream << L';';
        }
    };
};