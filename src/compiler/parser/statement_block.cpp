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

#include "pattern.h"
#include "grammar_factory.h"
#include "common_exceptions.h"
#include "compiler/ast/brackets_pair.h"
#include "compiler/ast/statement_block.h"
#include "lib/assert.h"

namespace g0at
{
    namespace parser
    {
        class statement_block : public pattern
        {
        public:
            statement_block(parser_data *_data)
                : pattern(&_data->pairs_of_curly_brackets, _data)
            {
            }

        protected:
            bool check(ast::token *tok) override
            {
                ast::brackets_pair *body = tok->to_brackets_pair();
                assert (body != nullptr && body->get_symbol() == L'{');

                do
                {
                    if (!body->prev)
                        break;

                    ast::brackets_pair *prev_br_pair = body->prev->to_brackets_pair();
                    if (prev_br_pair)
                    {
                        wchar_t prev_br_pair_symbol = prev_br_pair->get_symbol();
                        if (prev_br_pair_symbol == '{')
                            goto okay;

                        if (prev_br_pair_symbol == '(')
                        {
                            // while(){} ?..
                            if (prev_br_pair->prev)
                            {
                                if (prev_br_pair->prev->to_keyword_while())
                                    goto okay;
                                if (prev_br_pair->prev->to_keyword_if())
                                    goto okay;
                                if (prev_br_pair->prev->to_keyword_catch())
                                    goto okay;
                                if (prev_br_pair->prev->to_keyword_for())
                                    goto okay;
                                if (prev_br_pair->prev->to_keyword_switch())
                                    return false;
                            }
                        }
                        goto error;
                    }
                    if (body->prev->to_keyword_else())
                        goto okay;                
                    if (body->prev->to_keyword_try())
                        goto okay;                
                    if (body->prev->to_keyword_catch())
                        goto okay;                
                    if (body->prev->to_keyword_finally())
                        goto okay;                
                    if (body->prev->to_keyword_lock())
                        goto okay;                
                    if (body->prev->to_keyword_do())
                        goto okay;
                    if (body->prev->to_colon()) 
                    {
                        // case expression : { } ?
                        ast::token *left = body->prev->prev;
                        while(left)
                        {
                            if (left->to_keyword_case())
                                goto okay;
                            if (left->to_comma())
                                return false;
                            if (left->to_colon())
                                return false;
                            left = left->prev;
                        };
                        return false;
                    }
                } while(false);

                return false;

            error:
                throw unable_to_parse_token_sequence(body->get_fragment().begin);

            okay:
                lib::pointer<ast::statement_block> obj  = new ast::statement_block(body);
                body->replace(obj.cast<ast::token>());
                data->blocks.push_back(obj.get());
                return false;
            }
        };

        lib::pointer<pattern> grammar_factory::create_pattern_statement_block()
        {
            return new statement_block(data);
        }
    };
};
