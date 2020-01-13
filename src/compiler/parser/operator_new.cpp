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
#include "compiler/ast/brackets_pair.h"
#include "compiler/ast/keyword_new.h"
#include "compiler/ast/expression.h"
#include "compiler/ast/operator_new.h"
#include "lib/assert.h"


namespace g0at
{
    namespace parser
    {
        class operator_new : public pattern
        {
        public:
            operator_new(parser_data *_data)
                : pattern(&_data->new_keywords, _data)
            {
            }

        protected:
            bool check(ast::token *tok) override
            {
                ast::keyword_new *kw = tok->to_keyword_new();
                assert(kw != nullptr);

                if (!kw->next)
                    throw expected_an_expression(kw->get_fragment().end);

                ast::expression *expr = kw->next->to_expression();
                if (!expr)
                    throw expected_an_expression(kw->next->get_fragment().begin);

                if (expr->next)
                {
                    ast::brackets_pair *args = expr->next->to_brackets_pair();
                    if (args)
                    {
                        if (args->get_symbol() != '(')
                            throw expected_an_argument_list(args->get_fragment().begin);

                        lib::pointer<ast::operator_new> op_new_0  = new ast::operator_new(kw, expr, args);
                        kw->replace(args, op_new_0.cast<ast::token>());
                        data->expressions.add(op_new_0.get());
                        data->operators_new.push_back(op_new_0.get());
                        return false;
                    }
                }
                
                lib::pointer<ast::operator_new> op_new_1  = new ast::operator_new(kw, expr);
                kw->replace(expr, op_new_1.cast<ast::token>());
                data->expressions.add(op_new_1.get());
                data->operators_new.push_back(op_new_1.get());
                return false;
            }
        };

        lib::pointer<pattern> grammar_factory::create_pattern_operator_new()
        {
            return new operator_new(data);
        }
    };
};
