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
#include <map>
#include "lib/pointer.h"

namespace g0at
{
    namespace pt
    {
        class node;
        class function;
        class scope;

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

        protected:
            dbg_output(environment &env);
            void print(node *leaf, const wchar_t *title);
            void print(node *leaf, const wchar_t *title, const wchar_t* content);
            void print(node *leaf, const wchar_t *title, std::wstring content);
            void link_node_common_info(node *leaf);
            void link(int pred_id, int succ_id, bool dashed);
            void link(int pred_id, int succ_id, bool dashed, const wchar_t *label);
            void link_child(const dbg_output &child);
            void link_child(const dbg_output &child, const wchar_t *label);

            environment &env;
            int id;
        };
    };
};
