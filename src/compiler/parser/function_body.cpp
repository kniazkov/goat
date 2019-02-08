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
#include "compiler/ast/function.h"
#include "compiler/ast/expression.h"
#include "compiler/ast/statement_expression.h"
#include <assert.h>

namespace g0at
{
    namespace parser
    {
        class function_body : public pattern
        {
        public:
            function_body(parser_data *_data)
                : pattern(&_data->functions, _data)
            {
            }

        protected:
            int check(ast::token *tok) override
            {
                ast::function *tok_func = tok->to_function();
                assert(tok_func != nullptr);
                
                auto list = tok_func->get_raw_list();
                auto line = list->first;
                while(line)
                {
                    if (!line->to_statement())
                    {
                        if (!line->next)
                        {
                            ast::expression *expr = line->to_expression();
                            if (expr)
                            {
                                // last expression w/o semicolon
                                lib::pointer<ast::token> st_expr  = new ast::statement_expression(expr);
                                expr->replace(st_expr);
                                return 0;
                            }
                        }
                        throw unable_to_parse_token_sequence(line->get_position());
                    }
                    else
                    {
                        line = line->next;
                    }
                }

                return 0;
            }
        };

        lib::pointer<pattern> grammar_factory::create_pattern_function_body()
        {
            return new function_body(data);
        }
    };
};
