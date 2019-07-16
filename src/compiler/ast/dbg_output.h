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
        protected:
            struct visitor_data
            {
                std::wstringstream &stream;
                int &uid;

                visitor_data(std::wstringstream &_stream, int &_uid)
                    : stream(_stream), uid(_uid)
                {
                }
            };

        public:
            static std::wstring to_string(token *obj);
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
            void visit(dot *ref) override;
            void visit(property *ref) override;
            void visit(value_true *ref) override;
            void visit(value_false *ref) override;
            void visit(equals *ref) override;
            void visit(not_equal *ref) override;
            void visit(is_equal_to *ref) override;
            void visit(is_not_equal_to *ref) override;
            void visit(keyword_while *ref) override;
            void visit(statement_while *ref) override;
            void visit(method_call *ref) override;
            void visit(this_ptr *ref) override;
            void visit(token_array *ref) override;
            void visit(statement_block *ref) override;
            void visit(keyword_if *ref) override;
            void visit(keyword_else *ref) override;
            void visit(statement_if *ref) override;
            void visit(keyword_throw *ref) override;
            void visit(statement_throw *ref) override;
            void visit(keyword_try *ref) override;
            void visit(keyword_catch *ref) override;
            void visit(keyword_finally *ref) override;
            void visit(statement_try *ref) override;
            void visit(inherit *ref) override;
            void visit(inheritance *ref) override;
            void visit(character *ref) override;
            void visit(keyword_for *ref) override;
            void visit(statement_for *ref) override;
            void visit(less *ref) override;
            void visit(is_less_than *ref) override;
            void visit(statement_empty *ref) override;
            void visit(keyword_new *ref) override;
            void visit(operator_new *ref) override;
            void visit(increment *ref) override;
            void visit(prefix_increment *ref) override;
            void visit(keyword_thread *ref) override;
            void visit(keyword_lock *ref) override;
            void visit(statement_lock *ref) override;
            void visit(index_access *ref) override;
            void visit(keyword_in *ref) override;
            void visit(variable_in *ref) override;
            void visit(statement_for_in *ref) override;
            void visit(keyword_do *ref) override;
            void visit(statement_do_while *ref) override;
            void visit(keyword_break *ref) override;
            void visit(keyword_continue *ref) override;
            void visit(statement_break *ref) override;
            void visit(statement_continue *ref) override;
            void visit(keyword_switch *ref) override;
            void visit(keyword_case *ref) override;
            void visit(keyword_default *ref) override;
            void visit(statement_switch *ref) override;
            void visit(suffix_increment *ref) override;

        protected:
            dbg_output(visitor_data &_data);
            void print(const wchar_t *title);
            void print(const wchar_t *title, const wchar_t* content);
            void print(const wchar_t *title, std::wstring content);
            void link(int pred_id, int succ_id, bool dashed);
            void link(int pred_id, int succ_id, bool dashed, const wchar_t *label);
            void link_child(const dbg_output &child);
            void link_child(const dbg_output &child, const wchar_t *label);
            void print_token_list(token_list *list, const wchar_t *title);

            visitor_data &data;
            int id;
        };
    };
};
