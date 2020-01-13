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
#include "compiler/ast/keyword_do.h"
#include "compiler/ast/keyword_while.h"
#include "compiler/ast/brackets_pair.h"
#include "compiler/ast/expression.h"
#include "compiler/ast/statement.h"
#include "compiler/ast/statement_do_while.h"
#include "lib/assert.h"

namespace g0at
{
    namespace parser
    {
        class expected_a_while_keyword : public compilation_error
        {
        public:
            expected_a_while_keyword(lib::pointer<position> pos)
                : compilation_error(pos, global::resource->expected_a_while_keyword())
            {
            }
        };

        class statement_do_while : public pattern
        {
        public:
            statement_do_while(parser_data *_data)
                : pattern(&_data->do_keywords, _data)
            {
            }

        protected:
            bool check(ast::token *tok) override
            {
                ast::keyword_do *kw = tok->to_keyword_do();
                assert(kw != nullptr);

                if (!kw->next)
                    throw expected_a_statement(kw->get_fragment().end);

                ast::statement *stmt = kw->next->to_statement();
                if (!stmt)
                    throw expected_a_statement(kw->next->get_fragment().begin);

                if (!stmt->next)
                    throw expected_a_while_keyword(stmt->get_fragment().end);
                
                ast::keyword_while *kw_while = stmt->next->to_keyword_while();
                if (!kw_while)
                    throw expected_a_while_keyword(stmt->next->get_fragment().begin);

                if (!kw_while->next)
                    throw expected_a_conditional_expression(kw->get_fragment().end);
                
                ast::brackets_pair *condition = kw_while->next->to_brackets_pair();
                if (!condition || condition->get_symbol() != L'(')
                    throw expected_a_conditional_expression(kw->get_fragment().end);

                auto list = condition->get_raw_list();
                if (!list->has_only_one_item())
                    throw expected_an_expression(condition->get_fragment().begin);

                ast::expression *expr = list->first->to_expression();
                if (!expr)
                    throw expected_an_expression(list->first->get_fragment().begin);

                lib::pointer<ast::token> result = new ast::statement_do_while(kw, expr, stmt);
                kw->replace(condition, result);

                return false;
            }
        };

        lib::pointer<pattern> grammar_factory::create_pattern_statement_do_while()
        {
            return new statement_do_while(data);
        }
    };
};
