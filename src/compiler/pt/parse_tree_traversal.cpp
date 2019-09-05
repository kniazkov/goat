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

#include "parse_tree_traversal.h"
#include "node.h"
#include <algorithm>
#include "function.h"
#include "variable.h"
#include "static_string.h"
#include "function_call.h"
#include "statement_expression.h"
#include "addition.h"
#include "integer.h"
#include "subtraction.h"
#include "negation.h"
#include "declare_variable.h"
#include "assignment.h"
#include "real.h"
#include "declare_function.h"
#include "statement_return.h"
#include "node_object.h"
#include "property.h"
#include "is_equal_to.h"
#include "is_not_equal_to.h"
#include "statement_while.h"
#include "method_call.h"
#include "this_ptr.h"
#include "node_array.h"
#include "statement_block.h"
#include "statement_if.h"
#include "statement_throw.h"
#include "statement_try.h"
#include "inheritance.h"
#include "character.h"
#include "value_void.h"
#include "value_undefined.h"
#include "value_null.h"
#include "value_true.h"
#include "value_false.h"
#include "statement_for.h"
#include "is_less_than.h"
#include "operator_new.h"
#include "prefix_increment.h"
#include "statement_lock.h"
#include "index_access.h"
#include "statement_for_in.h"
#include "statement_do_while.h"
#include "statement_switch.h"
#include "suffix_increment.h"
#include "prefix_decrement.h"
#include "suffix_decrement.h"
#include "multiplication.h"
#include "exponentiation.h"
#include "division.h"
#include "remainder.h"
#include "unary_plus.h"
#include "logical_not.h"
#include "bitwise_not.h"
#include "operator_bool.h"
#include "left_shift.h"
#include "signed_right_shift.h"
#include "zero_fill_right_shift.h"
#include "is_less_than_or_equal_to.h"
#include "is_greater_than.h"
#include "is_greater_than_or_equal_to.h"
#include "bitwise_and.h"
#include "bitwise_or.h"
#include "bitwise_xor.h"
#include "logical_and.h"
#include "logical_or.h"

namespace g0at
{
    namespace pt
    {
        parse_tree_traversal::parse_tree_traversal()
        {
        }

        void parse_tree_traversal::traverse(node *root)
        {
            root->accept(this);
            while(!queue.empty())
            {
                node *next = queue.back();
                next->accept(this);
                queue.pop_back();
            }
        }

        bool parse_tree_traversal::is_postponed(node *n)
        {
            auto iter_end = queue.end();
            return std::find(queue.begin(), iter_end, n) != iter_end;
        }

        void parse_tree_traversal::visit(function *ref)
        {
            for (int i = 0, code_size = ref->get_code_size(); i < code_size; i++)
            {
                ref->get_stmt(i)->accept(this);
            }
            payload(ref);
        }

        void parse_tree_traversal::payload(function *ref)
        {
        }

        void parse_tree_traversal::visit(static_string *ref)
        {
            payload(ref);
        }

        void parse_tree_traversal::payload(static_string *ref)
        {
        }

        void parse_tree_traversal::visit(variable *ref)
        {
            payload(ref);
        }

        void parse_tree_traversal::payload(variable *ref)
        {
        }

        void parse_tree_traversal::visit(function_call *ref)
        {
            int args_count = ref->get_args_count();
            for (int i = args_count - 1; i > -1; i--)
            {
                ref->get_arg(i)->accept(this);
            }
            ref->get_func_object()->accept(this);
            payload(ref);
        }

        void parse_tree_traversal::payload(function_call *ref)
        {
        }

        void parse_tree_traversal::visit(statement_expression *ref)
        {
            ref->get_expression()->accept(this);
            payload(ref);
        }

        void parse_tree_traversal::payload(statement_expression *ref)
        {
        }

        void parse_tree_traversal::visit(addition *ref)
        {
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
            payload(ref);
        }

        void parse_tree_traversal::payload(addition *ref)
        {
        }

        void parse_tree_traversal::visit(integer *ref)
        {
            payload(ref);
        }

        void parse_tree_traversal::payload(integer *ref)
        {
        }

        void parse_tree_traversal::visit(subtraction *ref)
        {
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
            payload(ref);
        }

        void parse_tree_traversal::payload(subtraction *ref)
        {
        }

        void parse_tree_traversal::visit(negation *ref)
        {
            ref->get_right()->accept(this);
            payload(ref);
        }

        void parse_tree_traversal::payload(negation *ref)
        {
        }

        void parse_tree_traversal::visit(value_void *ref)
        {
            payload(ref);
        }

        void parse_tree_traversal::payload(value_void *ref)
        {
        }

        void parse_tree_traversal::visit(value_undefined *ref)
        {
            payload(ref);
        }

        void parse_tree_traversal::payload(value_undefined *ref)
        {
        }

        void parse_tree_traversal::visit(value_null *ref)
        {
            payload(ref);
        }

        void parse_tree_traversal::payload(value_null *ref)
        {
        }

        void parse_tree_traversal::visit(declare_variable *ref)
        {
            for (int i = 0, count = ref->get_count(); i < count; i++)
            {
                variable_info info = ref->get_variable(i);
                if (info.init_val)
                    info.init_val->accept(this);
            }
            payload(ref);
        }

        void parse_tree_traversal::payload(declare_variable *ref)
        {
        }

        void parse_tree_traversal::visit(assignment *ref)
        {
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
            payload(ref);
        }

        void parse_tree_traversal::payload(assignment *ref)
        {
        }

        void parse_tree_traversal::visit(real *ref)
        {
            payload(ref);
        }

        void parse_tree_traversal::payload(real *ref)
        {
        }

        void parse_tree_traversal::visit(declare_function *ref)
        {
            ref->get_func()->accept(this);
            payload(ref);
        }

        void parse_tree_traversal::payload(declare_function *ref)
        {
        }

        void parse_tree_traversal::visit(statement_return *ref)
        {
            auto expr = ref->get_expression();
            if (expr)
                expr->accept(this);
            payload(ref);
        }

        void parse_tree_traversal::payload(statement_return *ref)
        {
        }

        void parse_tree_traversal::visit(node_object *ref)
        {
            int count = ref->get_items_count();
            for (int i = count - 1; i > -1; i--)
            {
                auto item = ref->get_item(i);
                item.second->accept(this);
                item.first->accept(this);
            }
            payload(ref);
        }

        void parse_tree_traversal::payload(node_object *ref)
        {
        }

        void parse_tree_traversal::visit(property *ref)
        {
            ref->get_left()->accept(this);
            payload(ref);
        }

        void parse_tree_traversal::payload(property *ref)
        {
        }

        void parse_tree_traversal::visit(value_true *ref)
        {
            payload(ref);
        }

        void parse_tree_traversal::payload(value_true *ref)
        {
        }

        void parse_tree_traversal::visit(value_false *ref)
        {
            payload(ref);
        }

        void parse_tree_traversal::payload(value_false *ref)
        {
        }

        void parse_tree_traversal::visit(is_equal_to *ref)
        {
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
            payload(ref);
        }

        void parse_tree_traversal::payload(is_equal_to *ref)
        {
        }

        void parse_tree_traversal::visit(is_not_equal_to *ref)
        {
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
            payload(ref);
        }

        void parse_tree_traversal::payload(is_not_equal_to *ref)
        {
        }

        void parse_tree_traversal::visit(statement_while *ref)
        {
            ref->get_expression()->accept(this);
            ref->get_statement()->accept(this);
            payload(ref);
        }

        void parse_tree_traversal::payload(statement_while *ref)
        {
        }

        void parse_tree_traversal::visit(method_call *ref)
        {
            int args_count = ref->get_args_count();
            for (int i = args_count - 1; i > -1; i--)
            {
                ref->get_arg(i)->accept(this);
            }
            ref->get_left()->accept(this);
            payload(ref);
        }

        void parse_tree_traversal::payload(method_call *ref)
        {
        }

        void parse_tree_traversal::visit(this_ptr *ref)
        {
            payload(ref);
        }

        void parse_tree_traversal::payload(this_ptr *ref)
        {
        }

        void parse_tree_traversal::visit(node_array *ref)
        {
            for (int i = 0, count = ref->get_objects_count(); i < count; i++)
            {
                auto item = ref->get_object(i);
                item->accept(this);
            }
            payload(ref);
        }

        void parse_tree_traversal::payload(node_array *ref)
        {
        }

        void parse_tree_traversal::visit(statement_block *ref)
        {
            int code_size = ref->get_code_size();
            for (int i = 0; i < code_size; i++)
            {
                ref->get_stmt(i)->accept(this);
            }
            payload(ref);
        }

        void parse_tree_traversal::payload(statement_block *ref)
        {
        }

        void parse_tree_traversal::visit(statement_if *ref)
        {
            ref->get_expression()->accept(this);
            ref->get_stmt_if()->accept(this);
            auto stmt_else = ref->get_stmt_else();
            if (stmt_else)
            {
                stmt_else->accept(this);
            }
            payload(ref);
        }

        void parse_tree_traversal::payload(statement_if *ref)
        {
        }

        void parse_tree_traversal::visit(statement_throw *ref)
        {
            auto expr = ref->get_expression();
            if (expr)
            {
                expr->accept(this);
            }
            payload(ref);
        }

        void parse_tree_traversal::payload(statement_throw *ref)
        {
        }

        void parse_tree_traversal::visit(statement_try *ref)
        {
            ref->get_stmt_try()->accept(this);
            auto stmt_catch = ref->get_stmt_catch();
            auto stmt_finally = ref->get_stmt_finally();

            if (stmt_catch)
            {
                stmt_catch->accept(this);
            }

            if (stmt_finally)
            {
                stmt_finally->accept(this);
            }
            payload(ref);
        }

        void parse_tree_traversal::payload(statement_try *ref)
        {
        }

        void parse_tree_traversal::visit(inheritance *ref)
        {
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
            payload(ref);
        }

        void parse_tree_traversal::payload(inheritance *ref)
        {
        }

        void parse_tree_traversal::visit(character *ref)
        {
            payload(ref);
        }

        void parse_tree_traversal::payload(character *ref)
        {
        }

        void parse_tree_traversal::visit(statement_for *ref)
        {
            auto stmt_init = ref->get_stmt_init();
            if (stmt_init)
            {
                stmt_init->accept(this);
            }
            auto condition = ref->get_condition();
            if (condition)
            {
                condition->accept(this);
            }
            auto increment = ref->get_increment();
            if (increment)
            {
                increment->accept(this);
            }
            ref->get_body()->accept(this);
            payload(ref);
        }

        void parse_tree_traversal::payload(statement_for *ref)
        {
        }

        void parse_tree_traversal::visit(is_less_than *ref)
        {
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
            payload(ref);
        }

        void parse_tree_traversal::payload(is_less_than *ref)
        {
        }

        void parse_tree_traversal::visit(statement_empty *ref)
        {
            payload(ref);
        }

        void parse_tree_traversal::payload(statement_empty *ref)
        {
        }

        void parse_tree_traversal::visit(operator_new *ref)
        {
            int args_count = ref->get_args_count();
            for (int i = args_count - 1; i > -1; i--)
            {
                ref->get_arg(i)->accept(this);
            }
            ref->get_proto()->accept(this);
            payload(ref);
        }

        void parse_tree_traversal::payload(operator_new *ref)
        {
        }

        void parse_tree_traversal::visit(prefix_increment *ref)
        {
            ref->get_right()->accept(this);
            payload(ref);
        }

        void parse_tree_traversal::payload(prefix_increment *ref)
        {
        }

        void parse_tree_traversal::visit(statement_lock *ref)
        {
            ref->get_statement()->accept(this);
            payload(ref);
        }

        void parse_tree_traversal::payload(statement_lock *ref)
        {
        }

        void parse_tree_traversal::visit(index_access *ref)
        {
            int args_count = ref->get_args_count();
            for (int i = args_count - 1; i > -1; i--)
            {
                ref->get_arg(i)->accept(this);
            }
            ref->get_object()->accept(this);
            payload(ref);
        }

        void parse_tree_traversal::payload(index_access *ref)
        {
        }

        void parse_tree_traversal::visit(statement_for_in *ref)
        {
            ref->get_expression()->accept(this);
            ref->get_body()->accept(this);
            payload(ref);
        }

        void parse_tree_traversal::payload(statement_for_in *ref)
        {
        }

        void parse_tree_traversal::visit(statement_do_while *ref)
        {
            ref->get_expression()->accept(this);
            ref->get_statement()->accept(this);
            payload(ref);
        }

        void parse_tree_traversal::payload(statement_do_while *ref)
        {
        }

        void parse_tree_traversal::visit(statement_break *ref)
        {
            payload(ref);
        }

        void parse_tree_traversal::payload(statement_break *ref)
        {
        }

        void parse_tree_traversal::visit(statement_continue *ref)
        {
            payload(ref);
        }

        void parse_tree_traversal::payload(statement_continue *ref)
        {
        }

        void parse_tree_traversal::visit(statement_switch *ref)
        {
            int i, n, j, k;
            ref->get_expression()->accept(this);
            for (i = 0, n = ref->get_count(); i < n; i++)
            {
                auto block = ref->get_block(i);
                block->get_expression()->accept(this);
                for (j = 0, k = block->get_code_size(); j < k; j++)
                {
                    block->get_statement(j)->accept(this);
                }
            }
            if (ref->has_default_block())
            {
                auto block = ref->get_default_block();
                for (j = 0, k = block->get_code_size(); j < k; j++)
                {
                    block->get_statement(j)->accept(this);
                }
            }
            payload(ref);
        }

        void parse_tree_traversal::payload(statement_switch *ref)
        {
        }

        void parse_tree_traversal::visit(suffix_increment *ref)
        {
            ref->get_left()->accept(this);
            payload(ref);
        }

        void parse_tree_traversal::payload(suffix_increment *ref)
        {
        }

        void parse_tree_traversal::visit(prefix_decrement *ref)
        {
            ref->get_right()->accept(this);
            payload(ref);
        }

        void parse_tree_traversal::payload(prefix_decrement *ref)
        {
        }

        void parse_tree_traversal::visit(suffix_decrement *ref)
        {
            ref->get_left()->accept(this);
            payload(ref);
        }

        void parse_tree_traversal::payload(suffix_decrement *ref)
        {
        }
        
        void parse_tree_traversal::visit(multiplication *ref)
        {
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
            payload(ref);
        }

        void parse_tree_traversal::payload(multiplication *ref)
        {
        }
        
        void parse_tree_traversal::visit(exponentiation *ref)
        {
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
            payload(ref);
        }

        void parse_tree_traversal::payload(exponentiation *ref)
        {
        }
        
        void parse_tree_traversal::visit(division *ref)
        {
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
            payload(ref);
        }

        void parse_tree_traversal::payload(division *ref)
        {
        }
        
        void parse_tree_traversal::visit(remainder *ref)
        {
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
            payload(ref);
        }

        void parse_tree_traversal::payload(remainder *ref)
        {
        }

        void parse_tree_traversal::visit(unary_plus *ref)
        {
            ref->get_right()->accept(this);
            payload(ref);
        }

        void parse_tree_traversal::payload(unary_plus *ref)
        {
        }

        void parse_tree_traversal::visit(logical_not *ref)
        {
            ref->get_right()->accept(this);
            payload(ref);
        }

        void parse_tree_traversal::payload(logical_not *ref)
        {
        }

        void parse_tree_traversal::visit(bitwise_not *ref)
        {
            ref->get_right()->accept(this);
            payload(ref);
        }

        void parse_tree_traversal::payload(bitwise_not *ref)
        {
        }
        
        void parse_tree_traversal::visit(operator_bool *ref)
        {
            ref->get_right()->accept(this);
            payload(ref);
        }

        void parse_tree_traversal::payload(operator_bool *ref)
        {
        }
        
        void parse_tree_traversal::visit(left_shift *ref)
        {
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
            payload(ref);
        }

        void parse_tree_traversal::payload(left_shift *ref)
        {
        }
        
        void parse_tree_traversal::visit(signed_right_shift *ref)
        {
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
            payload(ref);
        }

        void parse_tree_traversal::payload(signed_right_shift *ref)
        {
        }

        
        void parse_tree_traversal::visit(zero_fill_right_shift *ref)
        {
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
            payload(ref);
        }

        void parse_tree_traversal::payload(zero_fill_right_shift *ref)
        {
        }

        
        void parse_tree_traversal::visit(is_less_than_or_equal_to *ref)
        {
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
            payload(ref);
        }

        void parse_tree_traversal::payload(is_less_than_or_equal_to *ref)
        {
        }

        
        void parse_tree_traversal::visit(is_greater_than *ref)
        {
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
            payload(ref);
        }

        void parse_tree_traversal::payload(is_greater_than *ref)
        {
        }
      
        void parse_tree_traversal::visit(is_greater_than_or_equal_to *ref)
        {
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
            payload(ref);
        }

        void parse_tree_traversal::payload(is_greater_than_or_equal_to *ref)
        {
        }
      
        void parse_tree_traversal::visit(bitwise_and *ref)
        {
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
            payload(ref);
        }

        void parse_tree_traversal::payload(bitwise_and *ref)
        {
        }
      
        void parse_tree_traversal::visit(bitwise_or *ref)
        {
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
            payload(ref);
        }

        void parse_tree_traversal::payload(bitwise_or *ref)
        {
        }
      
        void parse_tree_traversal::visit(bitwise_xor *ref)
        {
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
            payload(ref);
        }

        void parse_tree_traversal::payload(bitwise_xor *ref)
        {
        }
      
        void parse_tree_traversal::visit(logical_and *ref)
        {
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
            payload(ref);
        }

        void parse_tree_traversal::payload(logical_and *ref)
        {
        }
      
        void parse_tree_traversal::visit(logical_or *ref)
        {
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
            payload(ref);
        }

        void parse_tree_traversal::payload(logical_or *ref)
        {
        }
    };
};
