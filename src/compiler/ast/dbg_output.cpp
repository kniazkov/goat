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

#include "dbg_output.h"
#include "lib/utils.h"
#include "root.h"
#include "binary.h"
#include "function.h"
#include "identifier.h"
#include "variable.h"
#include "bracket.h"
#include "static_string.h"
#include "brackets_pair.h"
#include "function_call.h"
#include "statement_expression.h"
#include "custom_operator.h"
#include "addition.h"
#include "integer.h"
#include "subtraction.h"
#include "negation.h"
#include "declare_variable.h"
#include "assignment.h"
#include "real.h"
#include "declare_function.h"
#include "statement_return.h"
#include "token_object.h"
#include "property.h"
#include "is_equal_to.h"
#include "is_not_equal_to.h"
#include "statement_while.h"
#include "method_call.h"
#include "token_array.h"
#include "statement_block.h"
#include "statement_if.h"
#include "statement_throw.h"
#include "statement_try.h"
#include "inheritance.h"
#include "character.h"
#include "statement_for.h"
#include "is_less_than.h"
#include "operator_new.h"
#include "prefix_increment.h"
#include "statement_lock.h"
#include "index_access.h"
#include "variable_in.h"
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
#include "exclamation.h"
#include "tilde.h"
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
#include "assignment_by_sum.h"
#include "assignment_by_difference.h"
#include "assignment_by_product.h"
#include "assignment_by_quotient.h"
#include "assignment_by_remainder.h"
#include "assignment_by_left_shift.h"
#include "assignment_by_signed_right_shift.h"
#include "assignment_by_zero_fill_right_shift.h"
#include "assignment_by_bitwise_and.h"
#include "assignment_by_bitwise_or.h"
#include "assignment_by_bitwise_xor.h"
#include "ternary.h"
#include "parenthesized_expression.h"

namespace g0at
{
    namespace ast
    {
        dbg_output::dbg_output(visitor_data &_data)
            : data(_data)
        {
            id = data.uid++;
        }

        std::wstring dbg_output::to_string(token *obj)
        {
            int uid = 0;
            std::wstringstream tmp;
            tmp << L"digraph abstract_syntax_tree" << std::endl <<
                L"{" << std::endl <<
                L"  node [fontname=\"serif\", fontsize=11 shape=box style=rounded penwidth=0.5]" << std::endl << 
                L"  edge [fontname=\"serif\", fontsize=11 penwidth=0.5]" << std::endl;
            visitor_data data(tmp, uid);
            dbg_output dbg(data);
            obj->accept(&dbg);
            tmp << L"}";
            return tmp.str();
        }

        void dbg_output::print(const wchar_t *title)
        {
            data.stream << L"  node_" << id << L" [label=<" << title << L">]" << std::endl;
        }

        void dbg_output::print(const wchar_t *title, const wchar_t *content)
        {
            data.stream << L"  node_" << id << L" [label=<" << title << L"<br/><font color=\"blue\">" << content << "</font>>]" << std::endl;
        }

        void dbg_output::print(const wchar_t *title, std::wstring content)
        {
            data.stream << L"  node_" << id << L" [label=<" << title << L"<br/><font color=\"blue\">" << content << "</font>>]" << std::endl;
        }

        void dbg_output::link(int pred_id, int succ_id, bool dashed)
        {
            data.stream << L"  node_" << pred_id << L" -> node_" << succ_id;
            if (dashed)
                data.stream << L" [style=dashed]";
            data.stream << std::endl;
        }

        void dbg_output::link(int pred_id, int succ_id, bool dashed, const wchar_t *label)
        {
            data.stream << L"  node_" << pred_id << L" -> node_" << succ_id << L" [label=\"" << label << L"\"";
            if (dashed)
                data.stream << L" style=dashed";
            data.stream << L"]" << std::endl;
        }

        void dbg_output::link_child(const dbg_output &child)
        {
            link(id, child.id, false);
        }

        void dbg_output::link_child(const dbg_output &child, const wchar_t *label)
        {
            link(id, child.id, false, label);
        }

        void dbg_output::print_token_list(token_list *list, const wchar_t *title)
        {
            auto tok = list->first;
            if (!tok)
                return;
            dbg_output head(data);
            head.print(title);
            link_child(head);
            int pred_id = head.id;
            bool dashed = false;
            while(tok)
            {
                dbg_output element(data);
                tok->accept(&element);
                link(pred_id, element.id, dashed);
                pred_id = element.id;
                dashed = true;
                tok = tok->next;
            }
        }

        void dbg_output::print_binary(binary *ref, const wchar_t *title, const wchar_t* content)
        {
            print(title, content);
            dbg_output left(data);
            ref->get_left()->accept(&left);
            link_child(left, L"left");
            dbg_output right(data);
            ref->get_right()->accept(&right);
            link_child(right, L"right");
        }

        void dbg_output::visit(function *ref)
        {
            print(ref->get_type() == function_type::thread ? L"thread" : L"function");
            print_token_list(ref->get_raw_args_list(), L"args (raw)");
            print_token_list(ref->get_args_list(), L"args");
            print_token_list(ref->get_raw_list(), L"body (raw)");
            print_token_list(ref->get_body(), L"body");
        }

        void dbg_output::visit(identifier *ref)
        {
            print(L"identifier", ref->get_name());
        }

        void dbg_output::visit(variable *ref)
        {
            print(L"variable", ref->get_name());
        }

        void dbg_output::visit(bracket *ref)
        {
            wchar_t tmp[2];
            tmp[0] = ref->get_symbol();
            tmp[1] = 0;
            print(L"bracket", tmp);
        }

        void dbg_output::visit(static_string *ref)
        {
            std::wstring text = lib::escape_html_entities(lib::escape_special_chars(ref->get_text()));
            print(L"static text", text.length() > 0 ? text : L"&nbsp;");
        }

        void dbg_output::visit(semicolon *ref)
        {
            print(L"semicolon", L";");
        }

        void dbg_output::visit(brackets_pair *ref)
        {
            wchar_t tmp[3];
            tmp[0] = ref->get_symbol();
            tmp[1] = ref->get_inverse_symbol();
            tmp[2] = 0;
            print(L"pair of brackets", tmp);
        }

        void dbg_output::visit(function_call *ref)
        {
            print(L"function call");
            dbg_output child(data);
            ref->get_func_object()->accept(&child);
            link_child(child, L"object");
            print_token_list(ref->get_raw_list(), L"raw");
            print_token_list(ref->get_args_list(), L"args");
        }

        void dbg_output::visit(statement_expression *ref)
        {
            print(L"statement");
            dbg_output child(data);
            ref->get_expression()->accept(&child);
            link_child(child);
        }

        void dbg_output::visit(plus *ref)
        {
            print(L"plus", L"+");
        }

        void dbg_output::visit(custom_operator *ref)
        {
            print(L"custom operator", ref->get_oper());
        }

        void dbg_output::visit(addition *ref)
        {
            print_binary(ref, L"addition", L"+");
        }

        void dbg_output::visit(integer *ref)
        {
            print(L"integer", std::to_wstring(ref->get_value()));
        }

        void dbg_output::visit(minus *ref)
        {
            print(L"minus", L"-");
        }

        void dbg_output::visit(subtraction *ref)
        {
            print_binary(ref, L"subtraction", L"-");
        }

        void dbg_output::visit(negation *ref)
        {
            print(L"negation", L"-");
            dbg_output right(data);
            ref->get_right()->accept(&right);
            link_child(right);
        }

        void dbg_output::visit(value_void *ref)
        {
            print(L"void");
        }

        void dbg_output::visit(value_undefined *ref)
        {
            print(L"undefined");
        }

        void dbg_output::visit(value_null *ref)
        {
            print(L"null");
        }

        void dbg_output::visit(keyword_var *ref)
        {
            print(L"keyword", L"var");
        }

        void dbg_output::visit(declare_variable *ref)
        {
            print(L"declare variable");
            int pred_id = id;
            bool dashed = false;
            for (int i = 0, cnt = ref->get_count(); i < cnt; i++)
            {
                variable_info info = ref->get_variable(i);
                dbg_output var(data);
                var.print(L"variable", info.name);
                link(pred_id, var.id, dashed);
                if (info.init_val)
                {
                    dbg_output init(data);
                    info.init_val->accept(&init);
                    var.link_child(init, L"init");
                }
                pred_id = var.id;
                dashed = false;
            }
        }

        void dbg_output::visit(assign *ref)
        {
            print(L"assign", L"=");
        }

        void dbg_output::visit(assignment *ref)
        {
            print_binary(ref, L"assignment", L"=");
        }

        void dbg_output::visit(comma *ref)
        {
            print(L"comma", L",");
        }

        void dbg_output::visit(real *ref)
        {
            print(L"real number", lib::double_to_wstring(ref->get_value()));
        }

        void dbg_output::visit(keyword_function *ref)
        {
            print(L"keyword", L"function");
        }

        void dbg_output::visit(declare_function *ref)
        {
            ref->get_func()->accept(this);
        }

        void dbg_output::visit(keyword_return *ref)
        {
            print(L"keyword", L"return");
        }

        void dbg_output::visit(statement_return *ref)
        {
            print(L"return");
            auto expr = ref->get_expression();
            if (expr)
            {
                dbg_output child(data);
                expr->accept(&child);
                link_child(child);
            }
        }

        void dbg_output::visit(token_object *ref)
        {
            print(L"object", L"{}");
            print_token_list(ref->get_raw_list(), L"raw");
            int count = ref->get_items_count();
            if (count > 0)
            {
                for (int i = 0; i < count; i++)
                {
                    auto item = ref->get_item(i);

                    dbg_output pair(data);
                    pair.print(L"pair");
                    link_child(pair);

                    dbg_output key(data);
                    item.first->accept(&key);
                    pair.link_child(key, L"key");

                    dbg_output value(data);
                    item.second->accept(&value);
                    pair.link_child(value, L"value");
                }
            }
        }

        void dbg_output::visit(colon *ref)
        {
            print(L"colon", L":");
        }

        void dbg_output::visit(dot *ref)
        {
            print(L"dot", L".");
        }

        void dbg_output::visit(property *ref)
        {
            print(L"property", ref->get_name());
            dbg_output child(data);
            ref->get_left()->accept(&child);
            link_child(child);
        }

        void dbg_output::visit(value_true *ref)
        {
            print(L"true");
        }

        void dbg_output::visit(value_false *ref)
        {
            print(L"false");
        }

        void dbg_output::visit(equals *ref)
        {
            print(L"equals", L"==");
        }

        void dbg_output::visit(not_equal *ref)
        {
            print(L"not equal", L"==");
        }

        void dbg_output::visit(is_equal_to *ref)
        {
            print_binary(ref, L"is equal to", L"==");
        }

        void dbg_output::visit(is_not_equal_to *ref)
        {
            print_binary(ref, L"is not equal to", L"!=");
        }

        void dbg_output::visit(keyword_while *ref)
        {
            print(L"keyword", L"while");
        }

        void dbg_output::visit(statement_while *ref)
        {
            print(L"while");
            dbg_output condition(data);
            ref->get_expression()->accept(&condition);
            link_child(condition, L"condition");
            dbg_output stmt(data);
            ref->get_statement()->accept(&stmt);
            link_child(stmt, L"statement");
        }

        void dbg_output::visit(method_call *ref)
        {
            print(L"method call", ref->get_name());
            dbg_output child(data);
            ref->get_left()->accept(&child);
            link_child(child, L"object");
            print_token_list(ref->get_raw_list(), L"raw");
            print_token_list(ref->get_args_list(), L"args");
        }

        void dbg_output::visit(this_ptr *ref)
        {
            print(L"this");
        }

        void dbg_output::visit(token_array *ref)
        {
            print(L"array", L"[]");
            print_token_list(ref->get_raw_list(), L"raw");
            print_token_list(ref->get_object_list(), L"objects");
        }

        void dbg_output::visit(statement_block *ref)
        {
            print(L"block");
            print_token_list(ref->get_raw_list(), L"body (raw)");
            print_token_list(ref->get_body(), L"body");
        }

        void dbg_output::visit(keyword_if *ref)
        {
            print(L"keyword", L"if");
        }

        void dbg_output::visit(keyword_else *ref)
        {
            print(L"keyword", L"else");
        }

        void dbg_output::visit(statement_if *ref)
        {
            print(L"if");
            dbg_output condition(data);
            ref->get_expression()->accept(&condition);
            link_child(condition, L"condition");
            dbg_output out_if(data);
            ref->get_stmt_if()->accept(&out_if);
            link_child(out_if, L"stmt if");
            auto stmt_else = ref->get_stmt_else();
            if (stmt_else)
            {
                dbg_output out_else(data);
                stmt_else->accept(&out_else);
                link_child(out_else, L"stmt else");
            }
        }

        void dbg_output::visit(keyword_throw *ref)
        {
            print(L"keyword", L"throw");
        }

        void dbg_output::visit(statement_throw *ref)
        {
            print(L"throw");
            auto expr = ref->get_expression();
            if (expr)
            {
                dbg_output child(data);
                expr->accept(&child);
                link_child(child);
            }
        }

        void dbg_output::visit(keyword_try *ref)
        {
            print(L"keyword", L"try");
        }

        void dbg_output::visit(keyword_catch *ref)
        {
            print(L"keyword", L"catch");
        }

        void dbg_output::visit(keyword_finally *ref)
        {
            print(L"keyword", L"finally");
        }

        void dbg_output::visit(statement_try *ref)
        {
            if (ref->has_var())
            {
                print(L"try", ref->get_var_name());
            }
            else
            {
                print(L"try");
            }
            dbg_output out_try(data);
            ref->get_stmt_try()->accept(&out_try);
            link_child(out_try, L"stmt try");
            auto stmt_catch = ref->get_stmt_catch();
            if (stmt_catch)
            {
                dbg_output out_catch(data);
                stmt_catch->accept(&out_catch);
                link_child(out_catch, L"stmt catch");
            }
            auto stmt_finally = ref->get_stmt_finally();
            if (stmt_finally)
            {
                dbg_output out_finally(data);
                stmt_finally->accept(&out_finally);
                link_child(out_finally, L"stmt finally");
            }
        }

        void dbg_output::visit(inherit *ref)
        {
            print(L"inherit", L"-&gt;");
        }

        void dbg_output::visit(inheritance *ref)
        {
            print(L"inheritance", L"-&gt;");
            dbg_output left(data);
            ref->get_left()->accept(&left);
            link_child(left, L"ancestor");
            dbg_output right(data);
            ref->get_right()->accept(&right);
            link_child(right, L"successor");
        }

        void dbg_output::visit(character *ref)
        {
            wchar_t tmp[] = { ref->get_value(), 0 };
            std::wstring text = lib::escape_html_entities(lib::escape_special_chars(tmp, 1));
            print(L"char", text);
        }

        void dbg_output::visit(keyword_for *ref)
        {
            print(L"keyword", L"for");
        }

        void dbg_output::visit(statement_for *ref)
        {
            print(L"for");
            auto stmt_init = ref->get_stmt_init();
            if (stmt_init)
            {
                dbg_output out_stmt_init(data);
                stmt_init->accept(&out_stmt_init);
                link_child(out_stmt_init, L"init");
            }
            auto condition = ref->get_condition();
            if (condition)
            {
                dbg_output out_condition(data);
                condition->accept(&out_condition);
                link_child(out_condition, L"condition");
            }
            auto increment = ref->get_increment();
            if (increment)
            {
                dbg_output out_increment(data);
                increment->accept(&out_increment);
                link_child(out_increment, L"increment");
            }
            dbg_output out_body(data);
            ref->get_body()->accept(&out_body);
            link_child(out_body, L"body");
        }

        void dbg_output::visit(less *ref)
        {
            print(L"less", L"&lt;");
        }

        void dbg_output::visit(is_less_than *ref)
        {
            print_binary(ref, L"is less than", L"&lt;");
        }

        void dbg_output::visit(statement_empty *ref)
        {
            print(L"do nothing");
        }

        void dbg_output::visit(keyword_new *ref)
        {
            print(L"keyword", L"new");
        }

        void dbg_output::visit(operator_new *ref)
        {
            print(L"new");
            dbg_output out_proto(data);
            ref->get_proto()->accept(&out_proto);
            link_child(out_proto, L"proto");
            print_token_list(ref->get_raw_list(), L"raw");
            print_token_list(ref->get_args_list(), L"args");
        }

        void dbg_output::visit(increment *ref)
        {
            print(L"increment", L"++");
        }

        void dbg_output::visit(prefix_increment *ref)
        {
            print(L"prefix increment", L"++");
            dbg_output right(data);
            ref->get_right()->accept(&right);
            link_child(right);
        }

        void dbg_output::visit(keyword_thread *ref)
        {
            print(L"keyword", L"thread");
        }

        void dbg_output::visit(keyword_lock *ref)
        {
            print(L"keyword", L"lock");
        }

        void dbg_output::visit(statement_lock *ref)
        {
            print(L"lock");
            dbg_output out_stmt(data);
            ref->get_statement()->accept(&out_stmt);
            link_child(out_stmt);
        }

        void dbg_output::visit(index_access *ref)
        {
            print(L"access by index");
            dbg_output child(data);
            ref->get_expression()->accept(&child);
            link_child(child, L"object");
            print_token_list(ref->get_raw_list(), L"raw");
            print_token_list(ref->get_args_list(), L"args");
        }

        void dbg_output::visit(keyword_in *ref)
        {
            print(L"keyword", L"in");
        }

        void dbg_output::visit(variable_in *ref)
        {
            if (ref->is_declared())
                print(L"declared variable in", ref->get_name());
            else
                print(L"variable in", ref->get_name());
        }

        void dbg_output::visit(statement_for_in *ref)
        {
            std::wstringstream wss;
            if (ref->is_declared())
                wss << L"var ";
            wss << ref->get_name() << L" in";
            print(L"for", wss.str());
            dbg_output out_body(data);
            ref->get_body()->accept(&out_body);
            link_child(out_body, L"body");
        }

        void dbg_output::visit(keyword_do *ref)
        {
            print(L"keyword", L"do");
        }

        void dbg_output::visit(statement_do_while *ref)
        {
            print(L"do while");
            dbg_output condition(data);
            ref->get_expression()->accept(&condition);
            link_child(condition, L"condition");
            dbg_output stmt(data);
            ref->get_statement()->accept(&stmt);
            link_child(stmt, L"statement");
        }

        void dbg_output::visit(keyword_break *ref)
        {
            print(L"keyword", L"break");
        }

        void dbg_output::visit(keyword_continue *ref)
        {
            print(L"keyword", L"continue");
        }

        void dbg_output::visit(statement_break *ref)
        {
            print(L"break");
        }

        void dbg_output::visit(statement_continue *ref)
        {
            print(L"continue");
        }

        void dbg_output::visit(keyword_switch *ref)
        {
            print(L"keyword", L"switch");
        }

        void dbg_output::visit(keyword_case *ref)
        {
            print(L"keyword", L"case");
        }

        void dbg_output::visit(keyword_default *ref)
        {
            print(L"keyword", L"default");
        }

        void dbg_output::visit(statement_switch *ref)
        {
            print(L"switch");

            int pred_id = id;
            dbg_output out_expr(data);
            ref->get_expression()->accept(&out_expr);
            link_child(out_expr, L"expression");
            print_token_list(ref->get_raw_list(), L"raw");
            print_token_list(ref->get_default_block(), L"default");
            bool dashed = false;
            for (int i = 0, cnt = ref->get_count(); i < cnt; i++)
            {
                auto block = ref->get_block(i);
                dbg_output out_block(data);
                out_block.print(L"case");
                link(pred_id, out_block.id, dashed);
                dbg_output out_block_expr(data);
                block->get_expression()->accept(&out_block_expr);
                link(out_block.id, out_block_expr.id, false, L"expression");
                int pred_id_2 = out_block.id;
                bool dashed_2 = false;
                auto stmt = block->get_body()->first;
                while(stmt)
                {
                    dbg_output out_stmt(data);
                    stmt->accept(&out_stmt);
                    link(pred_id_2, out_stmt.id, dashed_2);
                    pred_id_2 = out_stmt.id;
                    dashed_2 = true;
                    stmt = stmt->next;
                }
                pred_id = out_block.id;
                dashed = true;
            }
        }

        void dbg_output::visit(suffix_increment *ref)
        {
            print(L"suffix increment", L"++");
            dbg_output left(data);
            ref->get_left()->accept(&left);
            link_child(left);
        }

        void dbg_output::visit(decrement *ref)
        {
            print(L"decrement", L"--");
        }

        void dbg_output::visit(prefix_decrement *ref)
        {
            print(L"prefix decrement", L"--");
            dbg_output right(data);
            ref->get_right()->accept(&right);
            link_child(right);
        }

        void dbg_output::visit(suffix_decrement *ref)
        {
            print(L"suffix decrement", L"--");
            dbg_output left(data);
            ref->get_left()->accept(&left);
            link_child(left);
        }

        void dbg_output::visit(keyword_import *ref)
        {
            print(L"keyword", L"import");
        }

        void dbg_output::visit(asterisk *ref)
        {
            print(L"asterisk", L"*");
        }

        void dbg_output::visit(double_asterisk *ref)
        {
            print(L"double asterisk", L"**");
        }

        void dbg_output::visit(slash *ref)
        {
            print(L"slash", L"/");
        }

        void dbg_output::visit(percent *ref)
        {
            print(L"percent", L"%");
        }

        void dbg_output::visit(multiplication *ref)
        {
            print_binary(ref, L"multiplication", L"*");
        }

        void dbg_output::visit(exponentiation *ref)
        {
            print_binary(ref, L"exponentiation", L"**");
        }

        void dbg_output::visit(division *ref)
        {
            print_binary(ref, L"division", L"/");
        }

        void dbg_output::visit(remainder *ref)
        {
            print_binary(ref, L"remainder", L"%");
        }

        void dbg_output::visit(unary_plus *ref)
        {
            print(L"unary plus", L"+");
            dbg_output right(data);
            ref->get_right()->accept(&right);
            link_child(right);
        }

        void dbg_output::visit(exclamation *ref)
        {
            print(L"exclamation", L"!");
        }

        void dbg_output::visit(tilde *ref)
        {
            print(L"tilde", L"~");
        }

        void dbg_output::visit(logical_not *ref)
        {
            print(L"logical not", L"!");
            dbg_output right(data);
            ref->get_right()->accept(&right);
            link_child(right);
        }

        void dbg_output::visit(bitwise_not *ref)
        {
            print(L"bitwise not", L"~");
            dbg_output right(data);
            ref->get_right()->accept(&right);
            link_child(right);
        }

        void dbg_output::visit(double_exclamation *ref)
        {
            print(L"double exclamation", L"!!");
        }

        void dbg_output::visit(operator_bool *ref)
        {
            print(L"operator bool", L"!!");
            dbg_output right(data);
            ref->get_right()->accept(&right);
            link_child(right);
        }

        void dbg_output::visit(left_shift *ref)
        {
            print_binary(ref, L"left shift", L"&lt;&lt;");
        }

        void dbg_output::visit(signed_right_shift *ref)
        {
            print_binary(ref, L"signed right shift", L"&gt;&gt;");
       }

        void dbg_output::visit(zero_fill_right_shift *ref)
        {
            print_binary(ref, L"zero fill right shift", L"&gt;&gt;&gt;");
       }

        void dbg_output::visit(double_less *ref)
        {
            print(L"double less", L"&lt;&lt;");
        }

        void dbg_output::visit(double_greater *ref)
        {
            print(L"double greater", L"&gt;&gt;");
        }

        void dbg_output::visit(triple_greater *ref)
        {
            print(L"triple greater", L"&gt;&gt;&gt;");
        }

        void dbg_output::visit(is_less_than_or_equal_to *ref)
        {
            print_binary(ref, L"is less than or equal to", L"&lt;=");
        }

        void dbg_output::visit(is_greater_than *ref)
        {
            print_binary(ref, L"is greater than", L"&gt;");
        }

        void dbg_output::visit(is_greater_than_or_equal_to *ref)
        {
            print_binary(ref, L"is greater than or equal to", L"&gt;=");
        }

        void dbg_output::visit(less_or_equal *ref)
        {
            print(L"less or equal", L"&lt;=");
        }

        void dbg_output::visit(greater *ref)
        {
            print(L"greater", L"&gt;");
        }

        void dbg_output::visit(greater_or_equal *ref)
        {
            print(L"greater or equal", L"&gt;=");
        }

        void dbg_output::visit(bitwise_and *ref)
        {
            print_binary(ref, L"bitwise and", L"&amp;");
        }

        void dbg_output::visit(bitwise_or *ref)
        {
            print_binary(ref, L"bitwise or", L"|");
        }

        void dbg_output::visit(bitwise_xor *ref)
        {
            print_binary(ref, L"bitwise xor", L"^");
        }

        void dbg_output::visit(logical_and *ref)
        {
            print_binary(ref, L"logical and", L"&amp;&amp;");
        }

        void dbg_output::visit(logical_or *ref)
        {
            print_binary(ref, L"logical or", L"||");
        }

        void dbg_output::visit(ampersand *ref)
        {
            print(L"ampersand", L"&amp;");
        }

        void dbg_output::visit(double_ampersand *ref)
        {
            print(L"double ampersand", L"&amp;");
        }

        void dbg_output::visit(vertical_bar *ref)
        {
            print(L"vertical bar", L"|");
        }

        void dbg_output::visit(double_vertical_bar *ref)
        {
            print(L"double vertical bar", L"||");
        }

        void dbg_output::visit(caret *ref)
        {
            print(L"caret", L"^");
        }

        void dbg_output::visit(assignment_by_sum *ref)
        {
            print_binary(ref, L"assignment by sum", L"+=");
        }

        void dbg_output::visit(assignment_by_difference *ref)
        {
            print_binary(ref, L"assignment by difference", L"-=");
        }

        void dbg_output::visit(assignment_by_product *ref)
        {
            print_binary(ref, L"assignment by product", L"*=");
        }

        void dbg_output::visit(assignment_by_quotient *ref)
        {
            print_binary(ref, L"assignment by quotient", L"/=");
        }

        void dbg_output::visit(assignment_by_remainder *ref)
        {
            print_binary(ref, L"assignment by remainder", L"%=");
        }

        void dbg_output::visit(assignment_by_left_shift *ref)
        {
            print_binary(ref, L"assignment by left shift", L"&lt;&lt;=");
        }

        void dbg_output::visit(assignment_by_signed_right_shift *ref)
        {
            print_binary(ref, L"assignment by signed right shift", L"&gt;&gt;=");
        }

        void dbg_output::visit(assignment_by_zero_fill_right_shift *ref)
        {
            print_binary(ref, L"assignment by zero fill right shift", L"&gt;&gt;&gt;=");
        }

        void dbg_output::visit(assignment_by_bitwise_and *ref)
        {
            print_binary(ref, L"assignment by bitwise and", L"&amp;=");
        }

        void dbg_output::visit(assignment_by_bitwise_or *ref)
        {
            print_binary(ref, L"assignment by bitwise or", L"|=");
        }

        void dbg_output::visit(assignment_by_bitwise_xor *ref)
        {
            print_binary(ref, L"assignment by bitwise xor", L"^=");
        }
        
        void dbg_output::visit(plus_assign *ref)
        {
            print(L"plus assign", L"+=");
        }
        
        void dbg_output::visit(minus_assign *ref)
        {
            print(L"minus assign", L"-=");
        }
        
        void dbg_output::visit(asterisk_assign *ref)
        {
            print(L"asterisk assign", L"*=");
        }
        
        void dbg_output::visit(slash_assign *ref)
        {
            print(L"slash assign", L"/=");
        }
        
        void dbg_output::visit(percent_assign *ref)
        {
            print(L"percent assign", L"%=");
        }
        
        void dbg_output::visit(double_less_assign *ref)
        {
            print(L"double less assign", L"&lt;&lt;=");
        }
        
        void dbg_output::visit(double_greater_assign *ref)
        {
            print(L"double greater assign", L"&gt;&gt;=");
        }
        
        void dbg_output::visit(triple_greater_assign *ref)
        {
            print(L"triple greater assign", L"&gt;&gt;&gt;=");
        }
        
        void dbg_output::visit(ampersand_assign *ref)
        {
            print(L"ampersand assign", L"&amp;=");
        }
        
        void dbg_output::visit(vertical_bar_assign *ref)
        {
            print(L"vertical bar assign", L"|=");
        }
        
        void dbg_output::visit(caret_assign *ref)
        {
            print(L"caret assign", L"^=");
        }

        void dbg_output::visit(question_mark *ref)
        {
            print(L"question mark", L"?");
        }

        void dbg_output::visit(ternary *ref)
        {
            print(L"ternary");
            dbg_output condition(data);
            ref->get_condition()->accept(&condition);
            link_child(condition, L"condition");
            dbg_output out_true(data);
            ref->get_expr_true()->accept(&out_true);
            link_child(out_true, L"expr if true");
            dbg_output out_false(data);
            ref->get_expr_false()->accept(&out_false);
            link_child(out_false, L"expr if false");
        }

        void dbg_output::visit(parenthesized_expression *ref)
        {
            print(L"parenthesized expression");
            print_token_list(ref->get_raw_list(), L"body (raw)");
            auto expr = ref->get_expression();
            if (expr)
            {
                dbg_output out_expr(data);
                expr->accept(&out_expr);
                link_child(out_expr, L"expr");
            }
        }
    };
};
