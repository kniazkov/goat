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

#include "pattern_right_to_left.h"
#include "grammar_factory.h"
#include "compiler/ast/token_operator.h"
#include "compiler/ast/expression.h"
#include "compiler/ast/unary_prefix.h"
#include "lib/assert.h"

namespace g0at
{
    namespace parser
    {
        class unary_prefix : public pattern_right_to_left
        {
        public:
            unary_prefix(ast::token_2nd_list *_list, parser_data *_data)
                : pattern_right_to_left(_list, _data)
            {
            }

        protected:
            bool check(ast::token *tok) override
            {
                ast::token_operator *oper = tok->to_token_operator();
                assert(oper != nullptr);
                
                do
                {
                    if (!oper->prev)
                        break;

                    if (oper->prev->to_token_operator())
                        break;

                    if (oper->prev->to_keyword())
                        break;

                    if (oper->prev->to_comma())
                        break;

                    if (oper->prev->to_semicolon())
                        break;

                    if (oper->prev->to_statement())
                        break;

                    return false;
                } while(false);

                if (!oper->next)
                    return false;

                ast::expression *right = oper->next->to_expression();
                if (!right)
                    return false;

                lib::pointer<ast::token> un_op = oper->create_unary_prefix_operation(right);
                oper->replace(right, un_op);
                data->expressions.add(un_op.get());
                return false;
            }
        };

        lib::pointer<pattern> grammar_factory::create_pattern_unary_prefix(ast::token_2nd_list *_list)
        {
            return new unary_prefix(_list, data);
        }
    };
};
