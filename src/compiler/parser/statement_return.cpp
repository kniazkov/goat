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
#include "compiler/ast/keyword_return.h"
#include "compiler/ast/expression.h"
#include "compiler/ast/semicolon.h"
#include "compiler/ast/statement_return.h"
#include "lib/assert.h"

namespace g0at
{
    namespace parser
    {
        class statement_return : public pattern
        {
        public:
            statement_return(parser_data *_data)
                : pattern(&_data->return_keywords, _data)
            {
            }

        protected:
            bool check(ast::token *tok) override
            {
                ast::keyword_return *kw = tok->to_keyword_return();
                assert(kw != nullptr);

                if (!kw->next)
                {
                    lib::pointer<ast::token> stmt = new ast::statement_return(kw, nullptr);
                    kw->replace(stmt);
                    return false;
                }
                
                ast::semicolon *semicolon = kw->next->to_semicolon();
                if (semicolon)
                {
                    lib::pointer<ast::token> stmt = new ast::statement_return(kw, nullptr);
                    kw->replace(semicolon, stmt);
                }
                else
                {
                    ast::expression *expr = kw->next->to_expression();
                    if (!expr)
                        throw expected_an_expression(kw->next->get_fragment().begin);

                    lib::pointer<ast::token> stmt = new ast::statement_return(kw, expr);

                    if (!expr->next)
                    {
                        kw->replace(expr, stmt);
                    }
                    else
                    {
                        semicolon = expr->next->to_semicolon();
                        if (!semicolon)
                            throw the_next_token_must_be_a_semicolon(expr->next->get_fragment().begin);
                        kw->replace(semicolon, stmt);
                    }
                }

                return false;
            }
        };

        lib::pointer<pattern> grammar_factory::create_pattern_statement_return()
        {
            return new statement_return(data);
        }
    };
};
