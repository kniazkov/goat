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
#include "common_exceptions.h"
#include "grammar_factory.h"
#include "compiler/ast/token_operator.h"
#include "compiler/ast/left_expression.h"
#include "compiler/ast/binary.h"
#include "compiler/common/compilation_error.h"
#include "global/global.h"
#include "lib/assert.h"

namespace g0at
{
    namespace parser
    {
        class invald_lvalue_expression : public compilation_error
        {
        public:
            invald_lvalue_expression(lib::pointer<position> pos)
                : compilation_error(pos, global::resource->invald_lvalue_expression())
            {
            }
        };

        class assignment : public pattern_right_to_left
        {
        public:
            assignment(parser_data *_data)
                : pattern_right_to_left(&_data->opers_assign, _data)
            {
            }

        protected:
            bool check(ast::token *tok) override
            {
                ast::token_operator *oper = tok->to_token_operator();
                assert(oper != nullptr);
                
                if (!oper->prev)
                    throw invald_lvalue_expression(oper->get_position());

                ast::left_expression *left = oper->prev->to_left_expression();
                if (!left)
                    throw invald_lvalue_expression(oper->get_position());

                if (!oper->next)
                    throw expected_an_expression_after_operator(oper->get_position());

                ast::expression *right = oper->next->to_expression();
                if (!right)
                    throw expected_an_expression_after_operator(oper->get_position());

                lib::pointer<ast::token> bin_op = oper->create_binary_operation(left, right);
                left->replace(right, bin_op);
                data->expressions.add(bin_op.get());
                return false;
            }
        };

        lib::pointer<pattern> grammar_factory::create_pattern_assignment()
        {
            return new assignment(data);
        }
    };
};
