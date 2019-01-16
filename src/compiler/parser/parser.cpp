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
#include "../ast/bracket.h"
#include "../ast/brackets_pair.h"
#include <assert.h>

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

        void parser::create_root(scanner *scan)
        {
            root = std::make_shared<ast::root>();
            delete data;
            data = new parser_data();
            parser_data_filler data_filler(data);
            parse_brackets_and_fill_data(scan, root, &data_filler, L'\0');
        }

        void parser::parse_brackets_and_fill_data(scanner *scan, std::shared_ptr<ast::token_with_list> dst,
            parser_data_filler *data_filler, wchar_t open_bracket)
        {
            auto *tok_list = dst->get_list();
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
                        auto bracket_expr = std::make_shared<ast::brackets_pair>(bracket);
                        tok_list->add(bracket_expr);
                        parse_brackets_and_fill_data(scan, bracket_expr, data_filler, bracket->get_symbol());
                    }
                }
                else
                {
                    tok_list->add(tok);
                }
            }
        }
    };
};