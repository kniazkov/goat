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
#include "compiler/ast/question_mark.h"
#include "compiler/ast/colon.h"
#include "compiler/ast/ternary.h"
#include "lib/assert.h"

namespace g0at
{
    namespace parser
    {
        class ternary : public pattern_right_to_left
        {
        public:
            ternary(parser_data *_data)
                : pattern_right_to_left(&_data->question_marks, _data)
            {
            }

        protected:
            bool check(ast::token *tok) override
            {
                ast::question_mark *quest = tok->to_question_mark();
                assert(quest != nullptr);

                if (!quest->prev)
                    throw expected_a_conditional_expression(quest->get_position());

                ast::expression *condition = quest->prev->to_expression();
                if (!condition)
                    throw expected_a_conditional_expression(quest->get_position());

                if (!quest->next)
                    throw expected_an_expression(quest->get_position());
 
                ast::expression *expr_true = quest->next->to_expression();
                if (!expr_true)
                    throw expected_an_expression(quest->get_position());

                if (!expr_true->next)
                    throw the_next_token_must_be_a_colon(expr_true->get_position());

                ast::colon *colon = expr_true->next->to_colon();
                if (!colon)
                    throw the_next_token_must_be_a_colon(expr_true->get_position());

                if (!colon->next)
                    throw expected_an_expression(colon->get_position());

                ast::expression *expr_false = colon->next->to_expression();
                if (!expr_false)
                    throw expected_an_expression(colon->next->get_position());
                
                lib::pointer<ast::token> result = new ast::ternary(condition, expr_true, expr_false);
                condition->replace(expr_false, result);
                data->expressions.add(result.get());

                return false;
            }
        };

        lib::pointer<pattern> grammar_factory::create_pattern_ternary()
        {
            return new ternary(data);
        }
    };
};
