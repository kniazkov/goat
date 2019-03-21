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

#include "pattern_right_to_left.h"
#include "grammar_factory.h"
#include "common_exceptions.h"
#include "compiler/ast/keyword_if.h"
#include "compiler/ast/keyword_else.h"
#include "compiler/ast/brackets_pair.h"
#include "compiler/ast/expression.h"
#include "compiler/ast/statement.h"
#include "compiler/ast/statement_if.h"
#include <assert.h>

namespace g0at
{
    namespace parser
    {
        class statement_if : public pattern_right_to_left
        {
        public:
            statement_if(parser_data *_data)
                : pattern_right_to_left(&_data->if_keywords, _data)
            {
            }

        protected:
            bool check(ast::token *tok) override
            {
                ast::keyword_if *kw = tok->to_keyword_if();
                assert(kw != nullptr);

                if (!kw->next)
                    throw expected_a_conditional_expression(kw->get_position());
                
                ast::brackets_pair *condition = kw->next->to_brackets_pair();
                if (!condition || condition->get_symbol() != L'(')
                    throw expected_a_conditional_expression(kw->get_position());

                auto list = condition->get_raw_list();
                if (!list->has_only_one_item())
                    throw expected_an_expression(condition->get_position());

                ast::expression *expr = list->first->to_expression();
                if (!expr)
                    throw expected_an_expression(list->first->get_position());

                if (!condition->next)
                    throw expected_a_statement(condition->get_position());

                ast::statement *stmt_if = condition->next->to_statement();
                if (!stmt_if)
                    throw expected_a_statement(condition->next->get_position());

                ast::statement *stmt_else = nullptr;

                if (stmt_if->next)
                {
                    ast::keyword_else *kw_else = stmt_if->next->to_keyword_else();
                    if (kw_else)
                    {
                        if (!kw_else->next)
                            throw expected_a_statement(kw_else->get_position());
                        
                        stmt_else = kw_else->next->to_statement();
                        if (!stmt_else)
                            throw expected_a_statement(kw_else->next->get_position());
                    }
                }

                if (stmt_else)
                {
                    lib::pointer<ast::token> result = new ast::statement_if(kw, expr, stmt_if, stmt_else);
                    kw->replace(stmt_else, result);
                }
                else
                {
                    lib::pointer<ast::token> result = new ast::statement_if(kw, expr, stmt_if);
                    kw->replace(stmt_if, result);
                }
                return false;
            }
        };

        lib::pointer<pattern> grammar_factory::create_pattern_statement_if()
        {
            return new statement_if(data);
        }
    };
};
