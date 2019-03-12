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
    namespace pt
    {
        class variable;
        class static_string;
        class function_call;
        class statement_expression;
        class function;
        class addition;
        class integer;
        class subtraction;
        class negation;
        class value_void;
        class value_undefined;
        class value_null;
        class declare_variable;
        class assignment;
        class real;
        class declare_function;
        class statement_return;
        class node_object;
        class property;
        class value_true;
        class value_false;
        class is_equal_to;
        class is_not_equal_to;
        class statement_while;
        class method_call;
        class this_ptr;

        class node_visitor
        {
        public:
            node_visitor();
            virtual ~node_visitor();
            virtual void visit(variable *ref);
            virtual void visit(static_string *ref);
            virtual void visit(function_call *ref);
            virtual void visit(statement_expression *ref);
            virtual void visit(function *ref);
            virtual void visit(addition *ref);
            virtual void visit(integer *ref);
            virtual void visit(subtraction *ref);
            virtual void visit(negation *ref);
            virtual void visit(value_void *ref);
            virtual void visit(value_undefined *ref);
            virtual void visit(value_null *ref);
            virtual void visit(declare_variable *ref);
            virtual void visit(assignment *ref);
            virtual void visit(real *ref);
            virtual void visit(declare_function *ref);
            virtual void visit(statement_return *ref);
            virtual void visit(node_object *ref);
            virtual void visit(property *ref);
            virtual void visit(value_true *ref);
            virtual void visit(value_false *ref);
            virtual void visit(is_equal_to *ref);
            virtual void visit(is_not_equal_to *ref);
            virtual void visit(statement_while *ref);
            virtual void visit(method_call *ref);
            virtual void visit(this_ptr *ref);
        };
    };
};
