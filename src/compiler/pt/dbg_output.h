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

#pragma once

#include "node_visitor.h"
#include <sstream>
#include <string>
#include <map>
#include "lib/pointer.h"

namespace g0at
{
    namespace pt
    {
        class node;
        class function;
        class scope;
        class binary;

        class dbg_output : public node_visitor
        {
        protected:
            struct environment
            {
                std::wstringstream &stream;
                int &uid;
                std::map<scope*, int> &scope_nodes;

                environment(std::wstringstream &_stream, int &_uid, std::map<scope*, int> &_scope_nodes)
                    : stream(_stream), uid(_uid), scope_nodes(_scope_nodes)
                {
                }
            };

            enum class edge_style
            {
                normal,
                node_to_next_one,
                scope_to_node,
                scope_to_child
            };

        public:
            static std::wstring to_string(node* obj);
            void visit(variable *ref) override;
            void visit(function *ref) override;
            void visit(static_string *ref) override;
            void visit(function_call *ref) override;
            void visit(statement_expression *ref) override;
            void visit(addition *ref) override;
            void visit(integer *ref) override;
            void visit(subtraction *ref) override;
            void visit(negation *ref) override;
            void visit(value_void *ref) override;
            void visit(value_undefined *ref) override;
            void visit(value_null *ref) override;
            void visit(declare_variable *ref) override;
            void visit(assignment *ref) override;
            void visit(real *ref) override;
            void visit(declare_function *ref) override;
            void visit(statement_return *ref) override;
            void visit(node_object *ref) override;
            void visit(property *ref) override;
            void visit(value_true *ref) override;
            void visit(value_false *ref) override;
            void visit(is_equal_to *ref) override;
            void visit(is_not_equal_to *ref) override;
            void visit(statement_while *ref) override;
            void visit(method_call *ref) override;
            void visit(this_ptr *ref) override;
            void visit(node_array *ref) override;
            void visit(statement_block *ref) override;
            void visit(statement_if *ref) override;
            void visit(statement_throw *ref) override;
            void visit(statement_try *ref) override;
            void visit(inheritance *ref) override;
            void visit(character *ref) override;
            void visit(statement_for *ref) override;
            void visit(is_less_than *ref) override;
            void visit(statement_empty *ref) override;
            void visit(operator_new *ref) override;
            void visit(prefix_increment *ref) override;
            void visit(statement_lock *ref) override;
            void visit(index_access *ref) override;
            void visit(statement_for_in *ref) override;
            void visit(statement_do_while *ref) override;
            void visit(statement_switch *ref) override;
            void visit(suffix_increment *ref) override;
            void visit(prefix_decrement *ref) override;
            void visit(suffix_decrement *ref) override;
            void visit(multiplication *ref) override;
            void visit(exponentiation *ref) override;
            void visit(division *ref) override;
            void visit(remainder *ref) override;
            void visit(unary_plus *ref) override;
            void visit(logical_not *ref) override;
            void visit(bitwise_not *ref) override;
            void visit(operator_bool *ref) override;
            void visit(left_shift *ref) override;
            void visit(signed_right_shift *ref) override;
            void visit(zero_fill_right_shift *ref) override;
            void visit(is_less_than_or_equal_to *ref) override;
            void visit(is_greater_than *ref) override;
            void visit(is_greater_than_or_equal_to *ref) override;
            void visit(bitwise_and *ref) override;
            void visit(bitwise_or *ref) override;
            void visit(bitwise_xor *ref) override;
            void visit(logical_and *ref) override;
            void visit(logical_or *ref) override;
            void visit(assignment_by_sum *ref) override;
            void visit(assignment_by_difference *ref) override;
            void visit(assignment_by_product *ref) override;
            void visit(assignment_by_quotient *ref) override;
            void visit(assignment_by_remainder *ref) override;
            void visit(assignment_by_left_shift *ref) override;
            void visit(assignment_by_signed_right_shift *ref) override;
            void visit(assignment_by_zero_fill_right_shift *ref) override;
            void visit(assignment_by_bitwise_and *ref) override;
            void visit(assignment_by_bitwise_or *ref) override;
            void visit(assignment_by_bitwise_xor *ref) override;
            void visit(ternary *ref) override;
            void visit(statement_break *ref) override;
            void visit(statement_continue *ref) override;
            void visit(statement_debug *ref) override;
            void visit(protection *ref) override;

        protected:
            dbg_output(environment &env);
            void print(node *leaf, const wchar_t *title);
            void print(node *leaf, const wchar_t *title, const wchar_t* content);
            void print(node *leaf, const wchar_t *title, std::wstring content);
            void link_node_common_info(node *leaf);
            int print_scope_node_if_needed(scope *sk);
            void link(int pred_id, int succ_id, edge_style style);
            void link(int pred_id, int succ_id, edge_style style, const wchar_t *label);
            void link_child(const dbg_output &child);
            void link_child(const dbg_output &child, const wchar_t *label);
            void print_binary(binary *leaf, const wchar_t *title, std::wstring content);

            environment &env;
            int id;
        };
    };
};
