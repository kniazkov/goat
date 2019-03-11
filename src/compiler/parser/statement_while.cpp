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
#include "compiler/ast/keyword_while.h"
#include "compiler/ast/brackets_pair.h"
#include "compiler/ast/expression.h"
#include "compiler/ast/statement.h"
#include "compiler/ast/statement_while.h"
#include <assert.h>

namespace g0at
{
    namespace parser
    {
        class statement_while : public pattern
        {
        public:
            statement_while(parser_data *_data)
                : pattern(&_data->while_keywords, _data)
            {
            }

        protected:
            bool check(ast::token *tok) override
            {
                ast::keyword_while *kw = tok->to_keyword_while();
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

                ast::statement *stmt = condition->next->to_statement();
                if (!stmt)
                    throw expected_a_statement(condition->next->get_position());
                
                lib::pointer<ast::token> result = new ast::statement_while(kw, expr, stmt);
                kw->replace(stmt, result);

                return false;
            }
        };

        lib::pointer<pattern> grammar_factory::create_pattern_statement_while()
        {
            return new statement_while(data);
        }
    };
};
