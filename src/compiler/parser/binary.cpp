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
#include "compiler/ast/token_operator.h"
#include "compiler/ast/expression.h"
#include "compiler/ast/binary.h"
#include "lib/assert.h"

namespace g0at
{
    namespace parser
    {
        class binary : public pattern
        {
        public:
            binary(ast::token_2nd_list *_list, parser_data *_data)
                : pattern(_list, _data)
            {
            }

        protected:
            bool check(ast::token *tok) override
            {
                ast::token_operator *oper = tok->to_token_operator();
                assert(oper != nullptr);
                
                if (!oper->prev)
                    return false;

                ast::expression *left = oper->prev->to_expression();
                if (!left)
                    return false;

                if (!oper->next)
                    throw expected_an_expression_after_operator(oper->get_fragment().end);

                ast::expression *right = oper->next->to_expression();
                if (!right)
                    throw expected_an_expression_after_operator(oper->get_fragment().end);

                lib::pointer<ast::token> bin_op = oper->create_binary_operation(left, right);
                left->replace(right, bin_op);
                data->expressions.add(bin_op.get());
                return false;
            }
        };

        lib::pointer<pattern> grammar_factory::create_pattern_binary(ast::token_2nd_list *_list)
        {
            return new binary(_list, data);
        }
    };
};
