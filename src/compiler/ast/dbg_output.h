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

#include "token_visitor.h"
#include "token_list.h"
#include <sstream>
#include <string>
#include "lib/pointer.h"

namespace g0at
{
    namespace ast
    {
        class root;

        class dbg_output : public token_visitor
        {
        public:
            dbg_output(std::wstringstream &_stream);
            dbg_output(std::wstringstream &_stream, int _indent);
            static std::wstring to_string(lib::pointer<root> obj);
            void visit(function *ref) override;
            void visit(identifier *ref) override;
            void visit(variable *ref) override;
            void visit(bracket *ref) override;
            void visit(static_string *ref) override;
            void visit(semicolon *ref) override;
            void visit(brackets_pair *ref) override;
            void visit(function_call *ref) override;
            void visit(statement_expression *ref) override;
            void visit(plus *ref) override;
            void visit(custom_operator *ref) override;
            void visit(addition *ref) override;
            void visit(integer *ref) override;
            void visit(minus *ref) override;
            void visit(subtraction *ref) override;
            void visit(negation *ref) override;
            void visit(value_void *ref) override;
            void visit(value_undefined *ref) override;
            void visit(value_null *ref) override;
            void visit(keyword_var *ref) override;
            void visit(declare_variable *ref) override;
            void visit(assign *ref) override;
            void visit(assignment *ref) override;
            void visit(comma *ref) override;
            void visit(real *ref) override;
            void visit(keyword_function *ref) override;
            void visit(declare_function *ref) override;
            void visit(keyword_return *ref) override;
            void visit(statement_return *ref) override;
            void visit(token_object *ref) override;
            void visit(colon *ref) override;

        protected:
            void add_indent();
            void add_indent(int value);
            void print_token_list(token_list *list, const wchar_t *title);

            std::wstringstream &stream;
            int indent;
        };
    };
};
