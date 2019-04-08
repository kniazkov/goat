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
#include "variable.h"
#include "function.h"
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
#include "this_ptr.h"

namespace g0at
{
    namespace pt
    {
        dbg_output::dbg_output(environment &env)
            : env(env)
        {
            id = env.uid++;
        }

        std::wstring dbg_output::to_string(node* obj)
        {
            std::wstringstream stream;
            int uid = 0;
            std::map<scope*, int> scope_nodes;
            environment env(stream, uid, scope_nodes);
            stream << L"digraph parse_tree" << std::endl <<
                L"{" << std::endl <<
                L"  node [fontname=\"serif\", fontsize=11 shape=box style=rounded penwidth=0.5]" << std::endl << 
                L"  edge [fontname=\"serif\", fontsize=11 penwidth=0.5]" << std::endl;
            dbg_output dbg(env);
            obj->accept(&dbg);
            stream << L"}";
            return stream.str();
        }

        void dbg_output::print(node *leaf, const wchar_t *title)
        {
            env.stream << L"  node_" << id << L" [label=<" << title << L">]" << std::endl;
            if (leaf)
                link_node_common_info(leaf);
        }

        void dbg_output::print(node *leaf, const wchar_t *title, const wchar_t *content)
        {
            env.stream << L"  node_" << id << L" [label=<" << title
                << L"<br/><font color=\"blue\">" << content << L"</font>"
                << L">]" << std::endl;
            if (leaf)
                link_node_common_info(leaf);
        }

        void dbg_output::print(node *leaf, const wchar_t *title, std::wstring content)
        {
            env.stream << L"  node_" << id << L" [label=<" << title
                << L"<br/><font color=\"blue\">" << content << L"</font>"
                << L">]" << std::endl;
            if (leaf)
                link_node_common_info(leaf);
        }

        void dbg_output::link_node_common_info(node *leaf)
        {
            auto sk = leaf->get_scope();
            if (sk)
            {
                auto iter = env.scope_nodes.find(sk.get());
                if (iter != env.scope_nodes.end())
                {
                    link(iter->second, id, false);
                }
                else
                {
                    dbg_output out_sk(env);
                    std::wstring symbols = sk->get_symbols_list();
                    if (symbols.size() > 0)
                        out_sk.print(nullptr, L"scope", symbols);
                    else
                        out_sk.print(nullptr, L"scope");
                    link(out_sk.id, id, false);
                    env.scope_nodes[sk.get()] = out_sk.id;
                }
            }
        }

        void dbg_output::link(int pred_id, int succ_id, bool dashed)
        {
            env.stream << L"  node_" << pred_id << L" -> node_" << succ_id;
            if (dashed)
                env.stream << L" [style=dashed]";
            env.stream << std::endl;
        }

        void dbg_output::link(int pred_id, int succ_id, bool dashed, const wchar_t *label)
        {
            env.stream << L"  node_" << pred_id << L" -> node_" << succ_id << L" [label=\"" << label << L"\"";
            if (dashed)
                env.stream << L" style=dashed";
            env.stream << L"]" << std::endl;
        }

        void dbg_output::link_child(const dbg_output &child)
        {
            link(id, child.id, false);
        }

        void dbg_output::link_child(const dbg_output &child, const wchar_t *label)
        {
            link(id, child.id, false, label);
        }

        void dbg_output::visit(variable *ref)
        {
            print(ref, L"variable", ref->get_name());
        }

        void dbg_output::visit(function *ref)
        {
            int args_count = ref->get_args_count();
            if (args_count > 0)
            {
                std::wstringstream tmp;
                for (int i = 0; i < args_count; i++)
                {
                    if (i)
                        tmp << L", ";
                    tmp << ref->get_arg(i);
                }
                print(ref, L"function", tmp.str());
            }
            else
            {
                print(ref, L"function");
            }

            int code_size = ref->get_code_size();
            if (code_size > 0)
            {
                dbg_output body(env);
                body.print(ref, L"body");
                link_child(body);
                int pred_id = body.id;
                bool dashed = false;
                for (int i = 0; i < code_size; i++)
                {
                    dbg_output stmt(env);
                    ref->get_stmt(i)->accept(&stmt);
                    link(pred_id, stmt.id, dashed);
                    pred_id = stmt.id;
                    dashed = true;
                }
            }
        }

        void dbg_output::visit(static_string *ref)
        {
            std::wstring text = lib::escape_special_chars(ref->get_text());
            print(ref, L"static text", text.length() > 0 ? text : L"&nbsp;");
        }

        void dbg_output::visit(function_call *ref)
        {
            print(ref, L"function call");
            dbg_output child(env);
            ref->get_func_object()->accept(&child);
            link_child(child, L"object");
            int args_count = ref->get_args_count();
            if (args_count > 0)
            {
                dbg_output args(env);
                args.print(ref, L"args");
                link_child(args);
                int pred_id = args.id;
                bool dashed = false;
                for (int i = 0; i < args_count; i++)
                {
                    dbg_output arg(env);
                    ref->get_arg(i)->accept(&arg);
                    link(pred_id, arg.id, dashed);
                    pred_id = arg.id;
                    dashed = true;
                }
            }
        }

        void dbg_output::visit(statement_expression *ref)
        {
            print(ref, L"statement");
            dbg_output child(env);
            ref->get_expression()->accept(&child);
            link_child(child);
        }

        void dbg_output::visit(addition *ref)
        {
            print(ref, L"addition", L"+");
            dbg_output left(env);
            ref->get_left()->accept(&left);
            link_child(left, L"left");
            dbg_output right(env);
            ref->get_right()->accept(&right);
            link_child(right, L"right");
        }

        void dbg_output::visit(subtraction *ref)
        {
            print(ref, L"subtraction", L"-");
            dbg_output left(env);
            ref->get_left()->accept(&left);
            link_child(left, L"left");
            dbg_output right(env);
            ref->get_right()->accept(&right);
            link_child(right, L"right");
        }

        void dbg_output::visit(negation *ref)
        {
            print(ref, L"negation", L"-");
            dbg_output right(env);
            ref->get_right()->accept(&right);
            link_child(right);
        }

        void dbg_output::visit(integer *ref)
        {
            print(ref, L"integer", std::to_wstring(ref->get_value()));
        }

        void dbg_output::visit(value_void *ref)
        {
            print(ref, L"void");
        }

        void dbg_output::visit(value_undefined *ref)
        {
            print(ref, L"undefined");
        }

        void dbg_output::visit(value_null *ref)
        {
            print(ref, L"null");
        }

        void dbg_output::visit(declare_variable *ref)
        {
            print(ref, L"declare variable");
            int pred_id = id;
            bool dashed = false;
            for (int i = 0, cnt = ref->get_count(); i < cnt; i++)
            {
                variable_info info = ref->get_variable(i);
                dbg_output var(env);
                var.print(ref, L"variable", info.name);
                link(pred_id, var.id, dashed);
                if (info.init_val)
                {
                    dbg_output init(env);
                    info.init_val->accept(&init);
                    var.link_child(init);
                }
                pred_id = var.id;
                dashed = false;
            }
        }

        void dbg_output::visit(assignment *ref)
        {
            print(ref, L"assignment", L"=");
            dbg_output left(env);
            ref->get_left()->accept(&left);
            link_child(left, L"left");
            dbg_output right(env);
            ref->get_right()->accept(&right);
            link_child(right, L"right");
       }

        void dbg_output::visit(real *ref)
        {
            print(ref, L"real number", lib::double_to_wstring(ref->get_value()));
        }

        void dbg_output::visit(declare_function *ref)
        {
            ref->get_func()->accept(this);
        }

        void dbg_output::visit(statement_return *ref)
        {
            print(ref, L"return");
            auto expr = ref->get_expression();
            if (expr)
            {
                dbg_output child(env);
                expr->accept(&child);
                link_child(child);
            }
        }

        void dbg_output::visit(node_object *ref)
        {
            print(ref, L"object", L"{}");
            int count = ref->get_items_count();
            if (count > 0)
            {
                for (int i = 0; i < count; i++)
                {
                    auto item = ref->get_item(i);

                    dbg_output pair(env);
                    pair.print(ref, L"pair");
                    link_child(pair);

                    dbg_output key(env);
                    item.first->accept(&key);
                    pair.link_child(key, L"key");

                    dbg_output value(env);
                    item.second->accept(&value);
                    pair.link_child(value, L"value");
                }
            }
        }

        void dbg_output::visit(property *ref)
        {
            print(ref, L"property", ref->get_name());
            dbg_output child(env);
            ref->get_left()->accept(&child);
            link_child(child);
        }

        void dbg_output::visit(value_true *ref)
        {
            print(ref, L"true");
        }

        void dbg_output::visit(value_false *ref)
        {
            print(ref, L"false");
        }

        void dbg_output::visit(is_equal_to *ref)
        {
            print(ref, L"is equal to", L"==");
            dbg_output left(env);
            ref->get_left()->accept(&left);
            link_child(left, L"left");
            dbg_output right(env);
            ref->get_right()->accept(&right);
            link_child(right, L"right");
        }

        void dbg_output::visit(is_not_equal_to *ref)
        {
            print(ref, L"is not equal to", L"!=");
            dbg_output left(env);
            ref->get_left()->accept(&left);
            link_child(left, L"left");
            dbg_output right(env);
            ref->get_right()->accept(&right);
            link_child(right, L"right");
        }

        void dbg_output::visit(statement_while *ref)
        {
            print(ref, L"while");
            dbg_output condition(env);
            ref->get_expression()->accept(&condition);
            link_child(condition, L"condition");
            dbg_output stmt(env);
            ref->get_statement()->accept(&stmt);
            link_child(stmt, L"statement");
        }

        void dbg_output::visit(method_call *ref)
        {
            print(ref, L"method call", ref->get_name());
            dbg_output child(env);
            ref->get_left()->accept(&child);
            link_child(child, L"object");
            int args_count = ref->get_args_count();
            if (args_count > 0)
            {
                dbg_output args(env);
                args.print(ref, L"args");
                link_child(args);
                int pred_id = args.id;
                bool dashed = false;
                for (int i = 0; i < args_count; i++)
                {
                    dbg_output arg(env);
                    ref->get_arg(i)->accept(&arg);
                    link(pred_id, arg.id, dashed);
                    pred_id = arg.id;
                    dashed = true;
                }
            }
        }

        void dbg_output::visit(this_ptr *ref)
        {
            print(ref, L"this");
        }

        void dbg_output::visit(node_array *ref)
        {
            print(ref, L"array", L"[]");
            int objects_count = ref->get_objects_count();
            if (objects_count > 0)
            {
                dbg_output objects(env);
                objects.print(ref, L"objects");
                link_child(objects);
                int pred_id = objects.id;
                bool dashed = false;
                for (int i = 0; i < objects_count; i++)
                {
                    dbg_output obj(env);
                    ref->get_object(i)->accept(&obj);
                    link(pred_id, obj.id, dashed);
                    pred_id = obj.id;
                    dashed = true;
                }
            }
        }

        void dbg_output::visit(statement_block *ref)
        {
            print(ref, L"block");
            int code_size = ref->get_code_size();
            if (code_size > 0)
            {
                int pred_id = id;
                bool dashed = false;
                for (int i = 0; i < code_size; i++)
                {
                    dbg_output stmt(env);
                    ref->get_stmt(i)->accept(&stmt);
                    link(pred_id, stmt.id, dashed);
                    pred_id = stmt.id;
                    dashed = true;
                }
            }
        }

        void dbg_output::visit(statement_if *ref)
        {
            print(ref, L"if");
            dbg_output condition(env);
            ref->get_expression()->accept(&condition);
            link_child(condition, L"condition");
            dbg_output out_if(env);
            ref->get_stmt_if()->accept(&out_if);
            link_child(out_if, L"stmt if");
            auto stmt_else = ref->get_stmt_else();
            if (stmt_else)
            {
                dbg_output out_else(env);
                stmt_else->accept(&out_else);
                link_child(out_else, L"stmt else");
            }
        }

        void dbg_output::visit(statement_throw *ref)
        {
            print(ref, L"throw");
            auto expr = ref->get_expression();
            if (expr)
            {
                dbg_output child(env);
                expr->accept(&child);
                link_child(child);
            }
        }

        void dbg_output::visit(statement_try *ref)
        {
            if (ref->has_var())
            {
                print(ref, L"try", ref->get_var_name());
            }
            else
            {
                print(ref, L"try");
            }
            dbg_output out_try(env);
            ref->get_stmt_try()->accept(&out_try);
            link_child(out_try, L"stmt try");
            auto stmt_catch = ref->get_stmt_catch();
            if (stmt_catch)
            {
                dbg_output out_catch(env);
                stmt_catch->accept(&out_catch);
                link_child(out_catch, L"stmt catch");
            }
            auto stmt_finally = ref->get_stmt_finally();
            if (stmt_finally)
            {
                dbg_output out_finally(env);
                stmt_finally->accept(&out_finally);
                link_child(out_finally, L"stmt finally");
            }
        }

        void dbg_output::visit(inheritance *ref)
        {
            print(ref, L"inheritance", L"-&gt;");
            dbg_output left(env);
            ref->get_left()->accept(&left);
            link_child(left, L"ancestor");
            dbg_output right(env);
            ref->get_right()->accept(&right);
            link_child(right, L"successor");
        }

        void dbg_output::visit(character *ref)
        {
            wchar_t tmp[] = { ref->get_value(), 0 };
            print(ref, L"char", tmp);
        }
    };
};