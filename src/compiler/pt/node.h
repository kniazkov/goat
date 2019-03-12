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
#include "compiler/source/position.h"
#include "lib/ref_counter.h"
#include "lib/pointer.h"

namespace g0at
{
    namespace pt
    {
        class expression;
        class statement;
        class variable;
        class static_string;
        class function_call;
        class statement_expression;
        class function;
        class binary;
        class addition;
        class integer;
        class subtraction;
        class unary_prefix;
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

        class node : public lib::ref_counter
        {
        public:
            node(lib::pointer<position> _pos);
            virtual ~node();
            virtual void accept(node_visitor *visitor) = 0;
            virtual expression *to_expression();
            virtual statement *to_statement();
            virtual variable *to_variable();
            virtual static_string *to_static_string();
            virtual function_call *to_function_call();
            virtual statement_expression *to_statement_expression();
            virtual function *to_function();
            virtual binary *to_binary();
            virtual addition *to_addition();
            virtual integer *to_integer();
            virtual subtraction *to_subtraction();
            virtual unary_prefix *to_unary_prefix();
            virtual negation *to_negation();
            virtual value_void *to_value_void();
            virtual value_undefined *to_value_undefined();
            virtual value_null *to_value_null();
            virtual declare_variable *to_declare_variable();
            virtual assignment *to_assignment();
            virtual real *to_real();
            virtual declare_function *to_declare_function();
            virtual statement_return *to_statement_return();
            virtual node_object *to_node_object();
            virtual property *to_property();
            virtual value_true *to_value_true();
            virtual value_false *to_value_false();
            virtual is_equal_to *to_is_equal_to();
            virtual is_not_equal_to *to_is_not_equal_to();
            virtual statement_while *to_statement_while();
            virtual method_call *to_method_call();
            virtual this_ptr *to_this_ptr();

            lib::pointer<position> get_position() { return pos; }

        protected:
            lib::pointer<position> pos;
        };
    };
};
