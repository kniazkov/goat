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
#include "compiler/ast/bracket.h"
#include "compiler/ast/brackets_pair.h"
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
            data->functions.add(root.get());
            parser_data_filler data_filler(data);
            parse_brackets_and_fill_data(scan, root.cast<ast::token_with_list>(), &data_filler, L'\0');
        }

        void parser::parse()
        {
            assert(data != nullptr);
            lib::pointer<grammar> gr = grammar_factory(data).create_grammar();
            gr->apply();
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
                    }
                }
                else
                {
                    tok_list->add(tok);
                    tok->accept(data_filler);
                }
            }
        }
    };
};