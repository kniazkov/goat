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
#include "compiler/ast/expression.h"
#include "compiler/ast/semicolon.h"
#include "compiler/ast/statement_expression.h"
#include <assert.h>

namespace g0at
{
    namespace parser
    {
        class statement_expression : public pattern
        {
        public:
            statement_expression(parser_data *_data)
                : pattern(&_data->expressions, _data)
            {
            }

        protected:
            int check(ast::token *tok) override
            {
                ast::expression *expr = tok->to_expression();
                assert(expr != nullptr);
                
                if (expr->next)
                {
                    ast::semicolon *scolon = expr->next->to_semicolon();
                    if (scolon == nullptr)
                        return 0;

                    lib::pointer<ast::token> st_expr  = new ast::statement_expression(expr);
                    tok->remove_2nd();
                    expr->replace(scolon, st_expr);
                }
                return 0;
            }
        };

        lib::pointer<pattern> grammar_factory::create_pattern_statement_expression()
        {
            return new statement_expression(data);
        }
    };
};
