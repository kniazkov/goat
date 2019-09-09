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

#include "pattern.h"
#include "grammar_factory.h"
#include "common_exceptions.h"
#include "compiler/ast/brackets_pair.h"
#include "compiler/ast/parenthesized_expression.h"
#include "lib/assert.h"

namespace g0at
{
    namespace parser
    {
        class parenthesized_expression : public pattern
        {
        public:
            parenthesized_expression(parser_data *_data)
                : pattern(&_data->pairs_of_round_brackets, _data)
            {
            }

        protected:
            bool check(ast::token *tok) override
            {
                ast::brackets_pair *body = tok->to_brackets_pair();
                assert (body != nullptr && body->get_symbol() == L'(');

                if (body->prev)
                {
                    if (body->prev->to_keyword_while())
                        return false;
                    if (body->prev->to_keyword_if())
                        return false;
                    if (body->prev->to_keyword_catch())
                        return false;
                    if (body->prev->to_keyword_for())
                        return false;
                    if (body->prev->to_keyword_switch())
                        return false;
                }

                lib::pointer<ast::parenthesized_expression> obj  = new ast::parenthesized_expression(body);
                body->replace(obj.cast<ast::token>());
                data->parenthesized.push_back(obj.get());
                return false;
            }
        };

        lib::pointer<pattern> grammar_factory::create_pattern_parenthesized_expression()
        {
            return new parenthesized_expression(data);
        }
    };
};
