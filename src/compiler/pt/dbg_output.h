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

#include "node_visitor.h"
#include <sstream>
#include <string>
#include "lib/pointer.h"

namespace g0at
{
    namespace pt
    {
        class function;

        class dbg_output : public node_visitor
        {
        public:
            dbg_output(std::wstringstream &_stream);
            dbg_output(std::wstringstream &_stream, int _indent);
            static std::wstring to_string(lib::pointer<function> obj);
            void visit(function *ref) override;
            void visit(static_string *ref) override;
            void visit(function_call *ref) override;
            void visit(statement_expression *ref) override;
            void visit(addition *ref) override;
            void visit(integer *ref) override;
            void visit(subtraction *ref) override;

        protected:
            void add_indent();

            std::wstringstream &stream;
            int indent;
        };
    };
};
