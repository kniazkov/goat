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
#include "compiler/ast/keyword_switch.h"
#include "compiler/ast/brackets_pair.h"
#include "compiler/ast/expression.h"
#include "compiler/ast/statement_switch.h"
#include "lib/assert.h"

namespace g0at
{
    namespace parser
    {
        class expected_a_switch_body : public compilation_error
        {
        public:
            expected_a_switch_body(lib::pointer<position> pos)
                : compilation_error(pos, global::resource->expected_a_switch_body())
            {
            }
        };

        class statement_switch : public pattern
        {
        public:
            statement_switch(parser_data *_data)
                : pattern(&_data->switch_keywords, _data)
            {
            }

        protected:
            bool check(ast::token *tok) override
            {
                ast::keyword_switch *kw = tok->to_keyword_switch();
                assert(kw != nullptr);

                if (!kw->next)
                    throw expected_an_expression(kw->get_fragment().end);
                
                ast::brackets_pair *expr_token = kw->next->to_brackets_pair();
                if (!expr_token || expr_token->get_symbol() != L'(')
                    throw expected_an_expression(kw->get_fragment().end);

                auto list = expr_token->get_raw_list();
                if (!list->has_only_one_item())
                    throw expected_an_expression(expr_token->get_fragment().begin);

                ast::expression *expr = list->first->to_expression();
                if (!expr)
                    throw expected_an_expression(list->first->get_fragment().begin);

                if (!expr_token->next)
                    throw expected_a_switch_body(expr_token->get_fragment().end);

                ast::brackets_pair *body = expr_token->next->to_brackets_pair();
                if (!body || body->get_symbol() != L'{')
                    throw expected_a_switch_body(expr_token->next->get_fragment().begin);

                lib::pointer<ast::statement_switch> result = new ast::statement_switch(kw, expr, body);
                kw->replace(body, result.cast<ast::token>());
                data->switches.push_back(result.get());
                return false;
            }
        };

        lib::pointer<pattern> grammar_factory::create_pattern_statement_switch()
        {
            return new statement_switch(data);
        }
    };
};
