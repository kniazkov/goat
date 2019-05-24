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

namespace g0at
{
    namespace ast
    {
        dbg_output::dbg_output(std::wstringstream &_stream, int &_uid)
            : stream(_stream), uid(_uid)
        {
            id = uid++;
        }

        std::wstring dbg_output::to_string(token *obj)
        {
            int uid = 0;
            std::wstringstream tmp;
            tmp << L"digraph abstract_syntax_tree" << std::endl <<
                L"{" << std::endl <<
                L"  node [fontname=\"serif\", fontsize=11 shape=box style=rounded penwidth=0.5]" << std::endl << 
                L"  edge [fontname=\"serif\", fontsize=11 penwidth=0.5]" << std::endl;
            dbg_output dbg(tmp, uid);
            obj->accept(&dbg);
            tmp << L"}";
            return tmp.str();
        }

        void dbg_output::print(const wchar_t *title)
        {
            stream << L"  node_" << id << L" [label=<" << title << L">]" << std::endl;
        }

        void dbg_output::print(const wchar_t *title, const wchar_t *content)
        {
            stream << L"  node_" << id << L" [label=<" << title << L"<br/><font color=\"blue\">" << content << "</font>>]" << std::endl;
        }

        void dbg_output::print(const wchar_t *title, std::wstring content)
        {
            stream << L"  node_" << id << L" [label=<" << title << L"<br/><font color=\"blue\">" << content << "</font>>]" << std::endl;
        }

        void dbg_output::link(int pred_id, int succ_id, bool dashed)
        {
            stream << L"  node_" << pred_id << L" -> node_" << succ_id;
            if (dashed)
                stream << L" [style=dashed]";
            stream << std::endl;
        }

        void dbg_output::link(int pred_id, int succ_id, bool dashed, const wchar_t *label)
        {
            stream << L"  node_" << pred_id << L" -> node_" << succ_id << L" [label=\"" << label << L"\"";
            if (dashed)
                stream << L" style=dashed";
            stream << L"]" << std::endl;
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
            dbg_output head(stream, uid);
            head.print(title);
            link_child(head);
            int pred_id = head.id;
            bool dashed = false;
            while(tok)
            {
                dbg_output element(stream, uid);
                tok->accept(&element);
                link(pred_id, element.id, dashed);
                pred_id = element.id;
                dashed = true;
                tok = tok->next;
            }
        }

        void dbg_output::visit(function *ref)
        {
            print(L"function");
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
            std::wstring text = lib::escape_special_chars(ref->get_text());
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
            dbg_output child(stream, uid);
            ref->get_func_object()->accept(&child);
            link_child(child, L"object");
            print_token_list(ref->get_raw_list(), L"raw");
            print_token_list(ref->get_args_list(), L"args");
        }

        void dbg_output::visit(statement_expression *ref)
        {
            print(L"statement");
            dbg_output child(stream, uid);
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
            print(L"addition", L"+");
            dbg_output left(stream, uid);
            ref->get_left()->accept(&left);
            link_child(left, L"left");
            dbg_output right(stream, uid);
            ref->get_right()->accept(&right);
            link_child(right, L"right");
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
            print(L"subtraction", L"-");
            dbg_output left(stream, uid);
            ref->get_left()->accept(&left);
            link_child(left, L"left");
            dbg_output right(stream, uid);
            ref->get_right()->accept(&right);
            link_child(right, L"right");
        }

        void dbg_output::visit(negation *ref)
        {
            print(L"negation", L"-");
            dbg_output right(stream, uid);
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
                dbg_output var(stream, uid);
                var.print(L"variable", info.name);
                link(pred_id, var.id, dashed);
                if (info.init_val)
                {
                    dbg_output init(stream, uid);
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
            print(L"assignment", L"=");
            dbg_output left(stream, uid);
            ref->get_left()->accept(&left);
            link_child(left, L"left");
            dbg_output right(stream, uid);
            ref->get_right()->accept(&right);
            link_child(right, L"right");
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
                dbg_output child(stream, uid);
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

                    dbg_output pair(stream, uid);
                    pair.print(L"pair");
                    link_child(pair);

                    dbg_output key(stream, uid);
                    item.first->accept(&key);
                    pair.link_child(key, L"key");

                    dbg_output value(stream, uid);
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
            dbg_output child(stream, uid);
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
            print(L"is equal to", L"==");
            dbg_output left(stream, uid);
            ref->get_left()->accept(&left);
            link_child(left, L"left");
            dbg_output right(stream, uid);
            ref->get_right()->accept(&right);
            link_child(right, L"right");
        }

        void dbg_output::visit(is_not_equal_to *ref)
        {
            print(L"is not equal to", L"!=");
            dbg_output left(stream, uid);
            ref->get_left()->accept(&left);
            link_child(left, L"left");
            dbg_output right(stream, uid);
            ref->get_right()->accept(&right);
            link_child(right, L"right");
        }

        void dbg_output::visit(keyword_while *ref)
        {
            print(L"keyword", L"while");
        }

        void dbg_output::visit(statement_while *ref)
        {
            print(L"while");
            dbg_output condition(stream, uid);
            ref->get_expression()->accept(&condition);
            link_child(condition, L"condition");
            dbg_output stmt(stream, uid);
            ref->get_statement()->accept(&stmt);
            link_child(stmt, L"statement");
        }

        void dbg_output::visit(method_call *ref)
        {
            print(L"method call", ref->get_name());
            dbg_output child(stream, uid);
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
            dbg_output condition(stream, uid);
            ref->get_expression()->accept(&condition);
            link_child(condition, L"condition");
            dbg_output out_if(stream, uid);
            ref->get_stmt_if()->accept(&out_if);
            link_child(out_if, L"stmt if");
            auto stmt_else = ref->get_stmt_else();
            if (stmt_else)
            {
                dbg_output out_else(stream, uid);
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
                dbg_output child(stream, uid);
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
            dbg_output out_try(stream, uid);
            ref->get_stmt_try()->accept(&out_try);
            link_child(out_try, L"stmt try");
            auto stmt_catch = ref->get_stmt_catch();
            if (stmt_catch)
            {
                dbg_output out_catch(stream, uid);
                stmt_catch->accept(&out_catch);
                link_child(out_catch, L"stmt catch");
            }
            auto stmt_finally = ref->get_stmt_finally();
            if (stmt_finally)
            {
                dbg_output out_finally(stream, uid);
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
            dbg_output left(stream, uid);
            ref->get_left()->accept(&left);
            link_child(left, L"ancestor");
            dbg_output right(stream, uid);
            ref->get_right()->accept(&right);
            link_child(right, L"successor");
        }

        void dbg_output::visit(character *ref)
        {
            wchar_t tmp[] = { ref->get_value(), 0 };
            print(L"char", tmp);
        }
    };
};