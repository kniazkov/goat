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

#include "dbg_output.h"
#include "lib/utils.h"
#include "lib/assert.h"
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
#include "statement_for.h"
#include "is_less_than.h"
#include "statement_empty.h"
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
#include "statement_break.h"
#include "statement_continue.h"
#include "statement_debug.h"
#include "protection.h"

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
                L"  node [fontname=\"sans-serif\", fontsize=11 shape=box style=rounded penwidth=0.7]" << std::endl << 
                L"  edge [fontname=\"sans-serif\", fontsize=11 penwidth=0.7]" << std::endl;
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

        void dbg_output::print_binary(binary *leaf, const wchar_t *title, std::wstring content)
        {
            print(leaf, title, content);
            dbg_output left(env);
            leaf->get_left()->accept(&left);
            link_child(left, L"left");
            dbg_output right(env);
            leaf->get_right()->accept(&right);
            link_child(right, L"right");
        }

        void dbg_output::link_node_common_info(node *leaf)
        {
#if 0            
            auto sk = leaf->get_scope();
            if (sk)
            {
                int node_sk_id = print_scope_node_if_needed(sk.get());
                link(node_sk_id, id, edge_style::scope_to_node);
            }
#endif
        }

#if 0            
        int dbg_output::print_scope_node_if_needed(scope *sk)
        {
            auto iter = env.scope_nodes.find(sk);
            if (iter != env.scope_nodes.end())
            {
                return iter->second;
            }
            else
            {
                dbg_output out_sk(env);
                auto symbols = sk->get_symbol_table();
                /* it does not work
                std::sort(symbols.begin(), symbols.end(), [](const scope::descriptor &x, const scope::descriptor &y) -> bool
                { 
                    return x.item->get_name() > y.item->get_name(); 
                });
                */
                if (symbols.size() > 0)
                {
                    env.stream << L"  node_" << out_sk.id << L" [label=<scope"
                        << L"<br/><font color=\"blue\">";
                    int j = 0;
                    for (int i = 0, size = (int)symbols.size(); i <  size; i++)
                    {
                        auto descr = symbols[i];
                        if (descr.sl->get_id() < 0 && !descr.defined)
                            continue;
                        if (j > 0)
                            env.stream << L", ";
                        if (descr.redefined)
                            env.stream << L"<font color=\"red\">";
                        if (descr.defined)
                            env.stream << L"<b>";
                        env.stream << descr.sl->get_name();
                        if (descr.defined)
                        {
                            int pr_cnt = descr.sl->get_proto_count();
                            if (pr_cnt > 0)
                            {
                                env.stream << L"-&gt;";
                                if (pr_cnt > 1)
                                    env.stream << L'[';
                                for (int k = 0; k < pr_cnt; k++)
                                {
                                    type *proto = descr.sl->get_proto(k);
                                    assert(proto != nullptr);
                                    symbol *sl_proto = sk->find_symbol_by_type(proto);
                                    if (sl_proto)
                                        env.stream << sl_proto->get_name();
                                    else
                                        env.stream << L"?";
                                }
                                if (pr_cnt > 1)
                                    env.stream << L']';
                            }
                            env.stream << L"</b>";
                        }
                        if (descr.redefined)
                            env.stream << L"</font>";
                        j++;
                    }
                    if (!j)
                        env.stream << L'*';
                    env.stream << L"</font>" << L">]" << std::endl;
                }
                else
                {
                    env.stream << L"  node_" << out_sk.id << L" [label=<scope>]" << std::endl;
                }
                env.scope_nodes[sk] = out_sk.id;
                for (int i = 0, count = sk->get_parents_count(); i < count; i++)
                {
                    scope *par_sk = sk->get_parent(i);
                    int node_par_sk_id = print_scope_node_if_needed(par_sk);
                    link(node_par_sk_id, out_sk.id, edge_style::scope_to_child);
                }
                return out_sk.id;
            }
        }
#endif

        void dbg_output::link(int pred_id, int succ_id, edge_style style)
        {
            env.stream << L"  node_" << pred_id << L" -> node_" << succ_id;
            switch(style)
            {
                case edge_style::node_to_next_one:
                    env.stream << L" [style=dashed]";
                    break;
                case edge_style::scope_to_node:
                    env.stream << L" [color=gray]";
                    break;
                case edge_style::scope_to_child:
                    env.stream << L" [color=gray style=dashed]";
                    break;
                default:
                    break;
            }
            env.stream << std::endl;
        }

        void dbg_output::link(int pred_id, int succ_id, edge_style style, const wchar_t *label)
        {
            env.stream << L"  node_" << pred_id << L" -> node_" << succ_id << L" [label=\"" << label << L"\"";
            switch(style)
            {
                case edge_style::node_to_next_one:
                    env.stream << L" style=dashed";
                    break;
                case edge_style::scope_to_node:
                    env.stream << L" color=gray";
                    break;
                case edge_style::scope_to_child:
                    env.stream << L" color=gray style=dashed";
                    break;
                default:
                    break;
            }
            env.stream << L"]" << std::endl;
        }

        void dbg_output::link_child(const dbg_output &child)
        {
            link(id, child.id, edge_style::normal);
        }

        void dbg_output::link_child(const dbg_output &child, const wchar_t *label)
        {
            link(id, child.id, edge_style::normal, label);
        }

        void dbg_output::visit(variable *ref)
        {
            print(ref, L"variable", ref->get_name());
        }

        void dbg_output::visit(function *ref)
        {
            const wchar_t *type = ref->get_type() == function_type::thread ? L"thread" : L"function";
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
                print(ref, type, tmp.str());
            }
            else
            {
                print(ref, type);
            }

            int code_size = ref->get_code_size();
            if (code_size > 0)
            {
                dbg_output body(env);
                body.print(ref, L"body");
                link_child(body);
                int pred_id = body.id;
                edge_style style = edge_style::normal;
                for (int i = 0; i < code_size; i++)
                {
                    dbg_output stmt(env);
                    ref->get_stmt(i)->accept(&stmt);
                    link(pred_id, stmt.id, style);
                    pred_id = stmt.id;
                    style = edge_style::node_to_next_one;
                }
            }
        }

        void dbg_output::visit(static_string *ref)
        {
            std::wstring text = lib::escape_html_entities(lib::escape_special_chars(ref->get_text()));
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
                edge_style style = edge_style::normal;
                for (int i = 0; i < args_count; i++)
                {
                    dbg_output arg(env);
                    ref->get_arg(i)->accept(&arg);
                    link(pred_id, arg.id, style);
                    pred_id = arg.id;
                    style = edge_style::node_to_next_one;
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
            print_binary(ref, L"addition", L"+");
        }

        void dbg_output::visit(subtraction *ref)
        {
            print_binary(ref, L"subtraction", L"-");
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
            edge_style style = edge_style::normal;
            for (int i = 0, cnt = ref->get_count(); i < cnt; i++)
            {
                variable_info info = ref->get_variable(i);
                dbg_output var(env);
                var.print(ref, L"variable", info.name);
                link(pred_id, var.id, style);
                if (info.init_val)
                {
                    dbg_output init(env);
                    info.init_val->accept(&init);
                    var.link_child(init, L"init");
                }
                pred_id = var.id;
                style = edge_style::node_to_next_one;
            }
        }

        void dbg_output::visit(assignment *ref)
        {
            print_binary(ref, L"assignment", L"=");
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
            print(ref, ref->guarded() ? L"guarded property" : L"property", ref->get_name());
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
            print_binary(ref, L"is equal to", L"==");
        }

        void dbg_output::visit(is_not_equal_to *ref)
        {
            print_binary(ref, L"is not equal to", L"!=");
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
                edge_style style = edge_style::normal;
                for (int i = 0; i < args_count; i++)
                {
                    dbg_output arg(env);
                    ref->get_arg(i)->accept(&arg);
                    link(pred_id, arg.id, style);
                    pred_id = arg.id;
                    style = edge_style::node_to_next_one;
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
                edge_style style = edge_style::normal;
                for (int i = 0; i < objects_count; i++)
                {
                    dbg_output obj(env);
                    ref->get_object(i)->accept(&obj);
                    link(pred_id, obj.id, style);
                    pred_id = obj.id;
                    style = edge_style::node_to_next_one;
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
                edge_style style = edge_style::normal;
                for (int i = 0; i < code_size; i++)
                {
                    dbg_output stmt(env);
                    ref->get_stmt(i)->accept(&stmt);
                    link(pred_id, stmt.id, style);
                    pred_id = stmt.id;
                    style = edge_style::node_to_next_one;
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
            std::wstring text = lib::escape_html_entities(lib::escape_special_chars(tmp, 1));
            print(ref, L"char", text);
        }

        void dbg_output::visit(statement_for *ref)
        {
            print(ref, L"for");
            auto stmt_init = ref->get_stmt_init();
            if (stmt_init)
            {
                dbg_output out_stmt_init(env);
                stmt_init->accept(&out_stmt_init);
                link_child(out_stmt_init, L"init");
            }
            auto condition = ref->get_condition();
            if (condition)
            {
                dbg_output out_condition(env);
                condition->accept(&out_condition);
                link_child(out_condition, L"condition");
            }
            auto increment = ref->get_increment();
            if (increment)
            {
                dbg_output out_increment(env);
                increment->accept(&out_increment);
                link_child(out_increment, L"increment");
            }
            dbg_output out_body(env);
            ref->get_body()->accept(&out_body);
            link_child(out_body, L"body");
        }

        void dbg_output::visit(is_less_than *ref)
        {
            print_binary(ref, L"is less than", L"&lt;");
        }

        void dbg_output::visit(statement_empty *ref)
        {
            print(ref, L"do nothing");
        }

        void dbg_output::visit(operator_new *ref)
        {
            print(ref, L"new");
            dbg_output child(env);
            ref->get_proto()->accept(&child);
            link_child(child, L"proto");
            int args_count = ref->get_args_count();
            if (args_count > 0)
            {
                dbg_output args(env);
                args.print(ref, L"args");
                link_child(args);
                int pred_id = args.id;
                edge_style style = edge_style::normal;
                for (int i = 0; i < args_count; i++)
                {
                    dbg_output arg(env);
                    ref->get_arg(i)->accept(&arg);
                    link(pred_id, arg.id, style);
                    pred_id = arg.id;
                    style = edge_style::node_to_next_one;
                }
            }
        }

        void dbg_output::visit(prefix_increment *ref)
        {
            print(ref, L"prefix increment", L"++");
            dbg_output right(env);
            ref->get_right()->accept(&right);
            link_child(right);
        }

        void dbg_output::visit(statement_lock *ref)
        {
            print(ref, L"lock");
            dbg_output out_stmt(env);
            ref->get_statement()->accept(&out_stmt);
            link_child(out_stmt);
        }

        void dbg_output::visit(index_access *ref)
        {
            print(ref, L"access by index");
            dbg_output child(env);
            ref->get_object()->accept(&child);
            link_child(child, L"object");
            int args_count = ref->get_args_count();
            if (args_count > 0)
            {
                dbg_output args(env);
                args.print(ref, L"args");
                link_child(args);
                int pred_id = args.id;
                edge_style style = edge_style::normal;
                for (int i = 0; i < args_count; i++)
                {
                    dbg_output arg(env);
                    ref->get_arg(i)->accept(&arg);
                    link(pred_id, arg.id, style);
                    pred_id = arg.id;
                    style = edge_style::node_to_next_one;
                }
            }
        }

        void dbg_output::visit(statement_for_in *ref)
        {
            std::wstringstream wss;
            if (ref->is_declared())
                wss << L"var ";
            wss << ref->get_name() << L" in";
            print(ref, L"for", wss.str());
            dbg_output out_body(env);
            ref->get_body()->accept(&out_body);
            link_child(out_body, L"body");
        }

        void dbg_output::visit(statement_do_while *ref)
        {
            print(ref, L"do while");
            dbg_output condition(env);
            ref->get_expression()->accept(&condition);
            link_child(condition, L"condition");
            dbg_output stmt(env);
            ref->get_statement()->accept(&stmt);
            link_child(stmt, L"statement");
        }

        void dbg_output::visit(statement_switch *ref)
        {
            print(ref, L"switch");
            int pred_id = id;
            dbg_output out_expr(env);
            ref->get_expression()->accept(&out_expr);
            link_child(out_expr, L"expression");
            edge_style style = edge_style::normal;
            for (int i = 0, in = ref->get_count(); i < in; i++)
            {
                auto block = ref->get_block(i);
                dbg_output out_block(env);
                out_block.print(ref, L"case");
                link(pred_id, out_block.id, style);
                dbg_output out_block_expr(env);
                block->get_expression()->accept(&out_block_expr);
                link(out_block.id, out_block_expr.id, edge_style::normal, L"expression");
                int pred_id_2 = out_block.id;
                edge_style style_2 = edge_style::normal;
                for (int j = 0, jn = block->get_code_size(); j < jn; j++)
                {
                    dbg_output out_stmt(env);
                    block->get_statement(j)->accept(&out_stmt);
                    link(pred_id_2, out_stmt.id, style_2);
                    pred_id_2 = out_stmt.id;
                    style_2 = edge_style::node_to_next_one;
                }
                pred_id = out_block.id;
                style = edge_style::node_to_next_one;
            }
            if (ref->has_default_block())
            {
                auto block = ref->get_default_block();
                dbg_output out_block(env);
                out_block.print(ref, L"default");
                link(pred_id, out_block.id, style);
                int pred_id_2 = out_block.id;
                edge_style style_2 = edge_style::normal;
                for (int j = 0, jn = block->get_code_size(); j < jn; j++)
                {
                    dbg_output out_stmt(env);
                    block->get_statement(j)->accept(&out_stmt);
                    link(pred_id_2, out_stmt.id, style_2);
                    pred_id_2 = out_stmt.id;
                    style_2 = edge_style::node_to_next_one;
                }
            }
        }

        void dbg_output::visit(suffix_increment *ref)
        {
            print(ref, L"suffix increment", L"++");
            dbg_output left(env);
            ref->get_left()->accept(&left);
            link_child(left);
        }

        void dbg_output::visit(prefix_decrement *ref)
        {
            print(ref, L"prefix_decrement", L"--");
            dbg_output right(env);
            ref->get_right()->accept(&right);
            link_child(right);
        }

        void dbg_output::visit(suffix_decrement *ref)
        {
            print(ref, L"suffix decrement", L"--");
            dbg_output left(env);
            ref->get_left()->accept(&left);
            link_child(left);
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
            print(ref, L"unary_plus", L"+");
            dbg_output right(env);
            ref->get_right()->accept(&right);
            link_child(right);
        }

        void dbg_output::visit(logical_not *ref)
        {
            print(ref, L"logical not", L"!");
            dbg_output right(env);
            ref->get_right()->accept(&right);
            link_child(right);
        }

        void dbg_output::visit(bitwise_not *ref)
        {
            print(ref, L"bitwise_not", L"~");
            dbg_output right(env);
            ref->get_right()->accept(&right);
            link_child(right);
        }

        void dbg_output::visit(operator_bool *ref)
        {
            print(ref, L"operator bool", L"!!");
            dbg_output right(env);
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
            print_binary(ref, L"is greater than or equal", L"&gt;=");
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

        void dbg_output::visit(ternary *ref)
        {
            print(ref, L"ternary");
            dbg_output condition(env);
            ref->get_condition()->accept(&condition);
            link_child(condition, L"condition");
            dbg_output out_true(env);
            ref->get_expr_true()->accept(&out_true);
            link_child(out_true, L"expr if true");
            dbg_output out_false(env);
            ref->get_expr_false()->accept(&out_false);
            link_child(out_false, L"expr if false");
        }

        void dbg_output::visit(statement_break *ref)
        {
            print(ref, L"break");
        }

        void dbg_output::visit(statement_continue *ref)
        {
            print(ref, L"continue");
        }

        void dbg_output::visit(statement_debug *ref)
        {
            print(ref, L"debug");
        }

        void dbg_output::visit(protection *ref)
        {
            print(ref, L"protection", L"#");
            dbg_output right(env);
            ref->get_right()->accept(&right);
            link_child(right);
        }
    };
};