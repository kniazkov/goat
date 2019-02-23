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

#include "parser.h"
#include "grammar_factory.h"
#include "common_exceptions.h"
#include "compiler/ast/bracket.h"
#include "compiler/ast/brackets_pair.h"
#include "compiler/ast/expression.h"
#include "compiler/ast/statement_expression.h"
#include "compiler/ast/dbg_output.h"
#include "global/global.h"
#include <assert.h>
#include <iostream>

namespace g0at
{
    namespace parser
    {
        parser::parser()
            : root(nullptr), data(nullptr)
        {
        }

        parser::~parser()
        {
            delete data;
        }

        lib::pointer<ast::root> parser::parse(scanner *scan, bool debug)
        {
            parser pobj;
            pobj.create_root(scan);
            try
            {
                pobj.parse();
            }
            catch(...)
            {
                if (debug)
                    std::cout << global::char_encoder->encode(g0at::ast::dbg_output::to_string(pobj.get_root())) << std::endl;
                throw;
            }
            return pobj.get_root();
        }

        void parser::create_root(scanner *scan)
        {
            root = new ast::root();
            delete data;
            data = new parser_data();
            data->functions.push_back(root.get());
            parser_data_filler data_filler(data);
            parse_brackets_and_fill_data(scan, root.cast<ast::token_with_list>(), &data_filler, L'\0');
        }

        void parser::parse()
        {
            assert(data != nullptr);

            lib::pointer<grammar> gr = grammar_factory(data).create_grammar();
            gr->apply();

            for (auto fcall : data->function_calls)
            {
                parse_function_call_args(fcall);
            }

            for (auto func : data->functions)
            {
                parse_function_body(func);
                parse_function_args(func);
            }
        }

        void parser::parse_brackets_and_fill_data(scanner *scan, lib::pointer<ast::token_with_list> dst,
            parser_data_filler *data_filler, wchar_t open_bracket)
        {
            auto *tok_list = dst->get_raw_list();
            while(true)
            {
                auto tok = scan->get_token();
                if (!tok)
                {
                    assert(open_bracket == L'\0'); // TODO: exception
                    return;
                }

                ast::bracket *bracket = tok->to_bracket();
                if (bracket)
                {
                    if (bracket->is_closed())
                    {
                        assert(bracket->get_inverse_symbol() == open_bracket); // TODO: exception
                        return;
                    }
                    else
                    {
                        lib::pointer<ast::brackets_pair> bracket_expr = new ast::brackets_pair(bracket);
                        tok_list->add(bracket_expr.cast<ast::token>());
                        parse_brackets_and_fill_data(scan, bracket_expr.cast<ast::token_with_list>(), data_filler, bracket->get_symbol());
                        bracket_expr->accept(data_filler);
                    }
                }
                else
                {
                    tok_list->add(tok);
                    tok->accept(data_filler);
                }
            }
        }

        void parser::parse_function_body(ast::function *func)
        {
            auto src = func->get_raw_list();
            auto dst = func->get_body();
            auto tok = src->first;
            while(tok)
            {
                if (!tok->to_statement())
                {
                    if (!tok->next)
                    {
                        ast::expression *expr = tok->to_expression();
                        if (expr)
                        {
                            // last expression w/o semicolon
                            lib::pointer<ast::token> st_expr  = new ast::statement_expression(expr);
                            expr->replace(st_expr);
                            break;
                        }
                    }
                    throw unable_to_parse_token_sequence(tok->get_position());
                }
                else
                {
                    tok = tok->next;
                }
            }
            dst->swap(src);
        }


        void parser::parse_function_args(ast::function *func)
        {
            auto src = func->get_raw_args_list();
            auto dst = func->get_args_list();
            auto tok = src->first;
            bool even = false;
            while(tok)
            {
                auto next = tok->next;
                if (even)
                {
                    if (!tok->to_comma())
                        throw function_arguments_must_be_separated_by_commas(tok->get_position());
                    tok->remove();
                }
                else
                {
                    if (!tok->to_identifier())
                        throw expected_an_identifier(tok->get_position());
                    dst->add(tok);
                }
                tok = next;
                even = !even;
            }
            assert(src->is_empty());
        }

        void parser::parse_function_call_args(ast::function_call *fcall)
        {
            auto src = fcall->get_raw_list();
            auto dst = fcall->get_args_list();
            auto tok = src->first;
            bool even = false;
            while(tok)
            {
                auto next = tok->next;
                if (even)
                {
                    if (!tok->to_comma())
                        throw function_arguments_must_be_separated_by_commas(tok->get_position());
                    tok->remove();
                }
                else
                {
                    if (!tok->to_expression())
                        throw expected_an_expression(tok->get_position());
                    dst->add(tok);
                }
                tok = next;
                even = !even;
            }
            assert(src->is_empty());
        }
    };
};