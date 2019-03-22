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
        class assign;
        class assignment;
        class comma;
        class real;
        class keyword_function;
        class declare_function;
        class keyword_return;
        class statement_return;
        class token_object;
        class colon;
        class dot;
        class property;
        class value_true;
        class value_false;
        class equals;
        class not_equal;
        class is_equal_to;
        class is_not_equal_to;
        class keyword_while;
        class statement_while;
        class method_call;
        class this_ptr;
        class token_array;
        class statement_block;
        class keyword_if;
        class keyword_else;
        class statement_if;
        class keyword_throw;

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
            virtual void visit(assign *ref);
            virtual void visit(assignment *ref);
            virtual void visit(comma *ref);
            virtual void visit(real *ref);
            virtual void visit(keyword_function *ref);
            virtual void visit(declare_function *ref);
            virtual void visit(keyword_return *ref);
            virtual void visit(statement_return *ref);
            virtual void visit(token_object *ref);
            virtual void visit(colon *ref);
            virtual void visit(dot *ref);
            virtual void visit(property *ref);
            virtual void visit(value_true *ref);
            virtual void visit(value_false *ref);
            virtual void visit(equals *ref);
            virtual void visit(not_equal *ref);
            virtual void visit(is_equal_to *ref);
            virtual void visit(is_not_equal_to *ref);
            virtual void visit(keyword_while *ref);
            virtual void visit(statement_while *ref);
            virtual void visit(method_call *ref);
            virtual void visit(this_ptr *ref);
            virtual void visit(token_array *ref);
            virtual void visit(statement_block *ref);
            virtual void visit(keyword_if *ref);
            virtual void visit(keyword_else *ref);
            virtual void visit(statement_if *ref);
            virtual void visit(keyword_throw *ref);
        };
    };
};
