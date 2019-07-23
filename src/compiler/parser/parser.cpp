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
#include "lib/utils.h"
#include "compiler/source/source_file.h"
#include "compiler/ast/bracket.h"
#include "compiler/ast/brackets_pair.h"
#include "compiler/ast/expression.h"
#include "compiler/ast/statement_expression.h"
#include "compiler/ast/token_object.h"
#include "compiler/ast/token_array.h"
#include "compiler/ast/colon.h"
#include "compiler/ast/comma.h"
#include "compiler/ast/identifier.h"
#include "compiler/ast/static_string.h"
#include "compiler/ast/dbg_output.h"
#include "global/global.h"
#include "lib/assert.h"
#include <iostream>
#include <memory>

namespace g0at
{
    namespace parser
    {
        class key_must_be_separated_by_a_colon : public compilation_error
        {
        public:
            key_must_be_separated_by_a_colon(lib::pointer<position> pos)
                : compilation_error(pos, global::resource->key_must_be_separated_by_a_colon())
            {
            }
        };

        class pairs_must_be_separated_by_commas : public compilation_error
        {
        public:
            pairs_must_be_separated_by_commas(lib::pointer<position> pos)
                : compilation_error(pos, global::resource->pairs_must_be_separated_by_commas())
            {
            }
        };

        class objects_must_be_separated_by_commas : public compilation_error
        {
        public:
            objects_must_be_separated_by_commas(lib::pointer<position> pos)
                : compilation_error(pos, global::resource->objects_must_be_separated_by_commas())
            {
            }
        };

        class expected_case_or_default_keyword : public compilation_error
        {
        public:
            expected_case_or_default_keyword(lib::pointer<position> pos)
                : compilation_error(pos, global::resource->expected_case_or_default_keyword())
            {
            }
        };

        class can_have_only_one_default_block : public compilation_error
        {
        public:
            can_have_only_one_default_block(lib::pointer<position> pos)
                : compilation_error(pos, global::resource->can_have_only_one_default_block())
            {
            }
        };

        class expected_a_file_name : public compilation_error
        {
        public:
            expected_a_file_name(lib::pointer<position> pos)
                : compilation_error(pos, global::resource->expected_a_file_name())
            {
            }
        };

        class wrong_file_name : public compilation_error
        {
        public:
            wrong_file_name(lib::pointer<position> pos, std::wstring file_name)
                : compilation_error(pos, global::resource->wrong_file_name(file_name))
            {
            }
        };

        class file_not_found : public compilation_error
        {
        public:
            file_not_found(lib::pointer<position> pos, const char *file_name)
                : compilation_error(pos, global::resource->file_not_found(file_name))
            {
            }
        };

        parser::parser()
            : root(nullptr), data(nullptr)
        {
        }

        parser::~parser()
        {
            delete data;
        }

        lib::pointer<ast::root> parser::parse(scanner *scan, bool debug, const char *prog_name, std::vector<std::string> &lib_path)
        {
            parser pobj;
            pobj.create_root(scan, lib_path);
            try
            {
                pobj.parse();
            }
            catch(...)
            {
                if (debug)
                    lib::dump_file(prog_name, "tokens.txt", ast::dbg_output::to_string(pobj.get_root().get()));
                throw;
            }
            return pobj.get_root();
        }

        void parser::create_root(scanner *scan, std::vector<std::string> &lib_path)
        {
            root = new ast::root();
            delete data;
            data = new parser_data();
            data->functions.push_back(root.get());
            parser_data_filler data_filler(data);
            std::set<std::string> imported;
            parse_brackets_and_fill_data(scan, root.cast<ast::token_with_list>(), &data_filler, L'\0', imported, lib_path);
        }

        void parser::parse()
        {
            assert(data != nullptr);

            lib::pointer<grammar> gr = grammar_factory(data).create_grammar();
            gr->apply();

            for (auto op_new : data->operators_new)
            {
                parse_operator_new_args(op_new);
            }

            for (auto vcall : data->method_calls)
            {
                parse_method_call_args(vcall);
            }

            for (auto fcall : data->function_calls)
            {
                parse_function_call_args(fcall);
            }

            for (auto ia : data->index_access)
            {
                parse_index_access_args(ia);
            }

            for (auto func : data->functions)
            {
                parse_function_body(func);
                parse_function_args(func);
            }

            for (auto block : data->blocks)
            {
                parse_statement_block_body(block);
            }

            for (auto obj : data->objects)
            {
                parse_object_body(obj);
            }

            for (auto arr : data->arrays)
            {
                parse_array_body(arr);
            }

            for (auto stmt : data->switches)
            {
                parse_switch_body(stmt);
            }
        }

        void parser::parse_brackets_and_fill_data(scanner *scan, lib::pointer<ast::token_with_list> dst,
            parser_data_filler *data_filler, wchar_t open_bracket, std::set<std::string> &imported,
            std::vector<std::string> &lib_path)
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
                        std::set<std::string> imported2;
                        parse_brackets_and_fill_data(scan, bracket_expr.cast<ast::token_with_list>(), data_filler,
                            bracket->get_symbol(), imported2, lib_path);
                        bracket_expr->accept(data_filler);
                    }
                }
                else if (tok->to_keyword_import())
                {
                    auto tok_file_name = scan->get_token();
                    if (!tok_file_name)
                        throw expected_a_file_name(tok->get_position());
                    auto tok_file_name_string = tok_file_name->to_static_string();
                    if (!tok_file_name_string)
                        throw expected_a_file_name(tok->get_position());

                    auto semicolon = scan->get_token();
                    if (!semicolon || !semicolon->to_semicolon())
                        throw the_next_token_must_be_a_semicolon(tok_file_name->get_position());

                    std::wstring file_name = tok_file_name_string->get_text();
                    if (file_name.length() == 0)
                        throw wrong_file_name(tok_file_name->get_position(), file_name);

                    bool is_ascii;
                    std::string file_name_ascii = lib::wstring_to_ascii_string(file_name, &is_ascii);
                    if (!is_ascii)
                        throw wrong_file_name(tok_file_name->get_position(), file_name);

                    bool found = false;
                    std::string full_path;
                    for(size_t i = 0, size = lib_path.size(); i < size; i++)
                    {
                        full_path = lib::normalize_file_path(lib_path[i] + '/' + file_name_ascii);
                        if (lib::file_exists(full_path.c_str()))
                        {
                            found = true;
                            break;
                        }
                    }
                    if (!found)
                        throw file_not_found(tok_file_name->get_position(), file_name_ascii.c_str());

                    if (imported.find(full_path) == imported.end())
                    {
                        imported.insert(full_path);
                        source_file src2(full_path);
                        scanner scan2(&src2);
                        parse_brackets_and_fill_data(&scan2, dst, data_filler, L'\0', imported, lib_path);
                    }
                }
                else
                {
                    tok_list->add(tok);
                    tok->accept(data_filler);
                }
            }
        }

        void parser::parse_block_body(ast::token_list *src, ast::token_list *dst)
        {
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

        void parser::parse_function_body(ast::function *func)
        {
            auto src = func->get_raw_list();
            auto dst = func->get_body();
            parse_block_body(src, dst);
        }

        void parser::parse_statement_block_body(ast::statement_block *block)
        {
            auto src = block->get_raw_list();
            auto dst = block->get_body();
            parse_block_body(src, dst);
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

        void parser::parse_function_and_method_call_args(ast::token_list *src, ast::token_list *dst)
        {
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

        void parser::parse_operator_new_args(ast::operator_new *op_new)
        {
            auto src = op_new->get_raw_list();
            auto dst = op_new->get_args_list();
            parse_function_and_method_call_args(src, dst);
        }

        void parser::parse_function_call_args(ast::function_call *fcall)
        {
            auto src = fcall->get_raw_list();
            auto dst = fcall->get_args_list();
            parse_function_and_method_call_args(src, dst);
        }

        void parser::parse_index_access_args(ast::index_access *ia)
        {
            auto src = ia->get_raw_list();
            auto dst = ia->get_args_list();
            parse_function_and_method_call_args(src, dst);
        }

        void parser::parse_method_call_args(ast::method_call *vcall)
        {
            auto src = vcall->get_raw_list();
            auto dst = vcall->get_args_list();
            parse_function_and_method_call_args(src, dst);
        }

        void parser::parse_object_body(ast::token_object *obj)
        {
            auto src = obj->get_raw_list();
            auto tok = src->first;
            while(tok)
            {
                ast::expression *key = tok->to_expression();
                if (!key)
                    throw expected_an_expression(tok->get_position());

                if (!key->next)
                    throw the_next_token_must_be_a_colon(key->get_position());

                ast::colon *colon = key->next->to_colon();
                if (!colon)
                    throw key_must_be_separated_by_a_colon(key->next->get_position());

                if (!colon->next)
                    throw expected_an_expression(colon->get_position());

                ast::expression *value = colon->next->to_expression();
                if (!value)
                    throw expected_an_expression(colon->next->get_position());

                if (value->next)
                {
                    ast::comma *comma = value->next->to_comma();
                    if (!comma)
                        throw pairs_must_be_separated_by_commas(value->next->get_position());

                    tok = comma->next;
                }
                else
                {
                    tok = nullptr;
                }

                ast::identifier *ident = key->to_identifier();
                if (ident)
                {
                    obj->add_item(new ast::static_string(ident), value);
                }
                else
                {
                    obj->add_item(key, value);
                }
            }
            src->clear();
        }

        void parser::parse_array_body(ast::token_array *arr)
        {
            auto src = arr->get_raw_list();
            auto dst = arr->get_object_list();
            auto tok = src->first;
            bool even = false;
            while(tok)
            {
                auto next = tok->next;
                if (even)
                {
                    if (!tok->to_comma())
                        throw objects_must_be_separated_by_commas(tok->get_position());
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

        void parser::parse_switch_body(ast::statement_switch *stmt)
        {
            auto raw = stmt->get_raw_list();
            auto tok = raw->first;
            bool has_default = false;
            if (!tok)
                return;
            if (!tok->to_keyword_case() && !tok->to_keyword_default())
                throw expected_case_or_default_keyword(tok->get_position());
            ast::token_list *list = nullptr;
            while(tok)
            {
                auto next = tok->next;
                if (tok->to_keyword_default())
                {
                    if (has_default)
                        throw can_have_only_one_default_block(tok->get_position());
                    has_default = true;
                    if (!tok->next)
                        throw the_next_token_must_be_a_colon(tok->get_position());
                    if (!tok->next->to_colon())
                        throw the_next_token_must_be_a_colon(tok->next->get_position());
                    next = tok->next->next;
                    tok->next->remove();
                    tok->remove();
                    list = stmt->get_default_block();
                }
                else if (tok->to_keyword_case())
                {
                    if (!tok->next)
                        throw expected_an_expression(tok->get_position());
                    lib::pointer<ast::expression> expr = tok->next->to_expression();
                    if (!expr)
                        throw expected_an_expression(tok->next->get_position());
                    lib::pointer<ast::case_block> block = new ast::case_block(expr);
                    stmt->add_block(block);
                    if (!expr->next)
                        throw the_next_token_must_be_a_colon(tok->get_position());
                    if (!expr->next->to_colon())
                        throw the_next_token_must_be_a_colon(tok->next->get_position());\
                    next = expr->next->next;
                    expr->next->remove();
                    expr->remove();
                    tok->remove();
                    list = block->get_body();
                }
                else
                {
                    if (!tok->to_statement())
                        throw expected_a_statement(tok->get_position());
                    list->add(tok);
                }
                tok = next;
            }
            assert(raw->is_empty());
        }
    };
};