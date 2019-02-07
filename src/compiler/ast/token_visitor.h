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

namespace g0at
{
    namespace ast
    {
        class function;
        class identifier;
        class variable;
        class bracket;
        class static_string;
        class semicolon;
        class brackets_pair;
        class function_call;
        class statement_expression;
        class plus;
        class custom_operator;
        class addition;
        class integer;
        class minus;
        class subtraction;
        class negation;
        class value_void;
        class value_undefined;
        class value_null;
        class keyword_var;
        class declare_variable;

        class token_visitor
        {
        public:
            token_visitor();
            virtual ~token_visitor();
            virtual void visit(function *ref);
            virtual void visit(identifier *ref);
            virtual void visit(variable *ref);
            virtual void visit(bracket *ref);
            virtual void visit(static_string *ref);
            virtual void visit(semicolon *ref);
            virtual void visit(brackets_pair *ref);
            virtual void visit(function_call *ref);
            virtual void visit(statement_expression *ref);
            virtual void visit(plus *ref);
            virtual void visit(custom_operator *ref);
            virtual void visit(addition *ref);
            virtual void visit(integer *ref);
            virtual void visit(minus *ref);
            virtual void visit(subtraction *ref);
            virtual void visit(negation *ref);
            virtual void visit(value_void *ref);
            virtual void visit(value_undefined *ref);
            virtual void visit(value_null *ref);
            virtual void visit(keyword_var *ref);
            virtual void visit(declare_variable *ref);
        };
    };
};
