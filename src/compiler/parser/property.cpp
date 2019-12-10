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
#include "compiler/ast/expression.h"
#include "compiler/ast/dot.h"
#include "compiler/ast/question_with_dot.h"
#include "compiler/ast/identifier.h"
#include "compiler/ast/property.h"
#include "compiler/ast/index_access.h"
#include "common_exceptions.h"
#include "global/global.h"
#include "lib/assert.h"

namespace g0at
{
    namespace parser
    {

        class property : public pattern
        {
        public:
            property(parser_data *_data)
                : pattern(&_data->dots, _data)
            {
            }

        protected:
            bool check(ast::token *tok) override
            {
                bool guard = false;
                ast::dot *dot = tok->to_dot();
                if (!dot)
                {
                    ast::question_with_dot *qdot = tok->to_question_with_dot();
                    assert(qdot != nullptr);
                    guard = true;
                }

                if (!tok->prev)
                    throw expected_an_expression_before_dot(tok->get_fragment().begin);

                lib::pointer<ast::expression> left = tok->prev->to_expression();
                if (!left)
                    throw expected_an_expression_before_dot(tok->get_fragment().begin);

                if (!tok->next)
                    throw expected_an_identifier_after_dot(tok->get_fragment().end);

                ast::index_access *index_access = tok->next->to_index_access();
                if (index_access)
                {
                    /*
                               identifier  
                                   |
                        left.index_access[index]
                    */
                    ast::identifier *ident = index_access->get_expression()->to_identifier();
                    if (!ident)
                        throw expected_an_identifier_after_dot(tok->get_fragment().end);
                    lib::pointer<ast::expression> prop = new ast::property(left, ident, guard);
                    left->remove_2nd();
                    left->remove();
                    tok->remove_2nd();
                    tok->remove();
                    index_access->set_expression(prop);
                }
                else
                {
                    ast::identifier *right = tok->next->to_identifier();
                    if (!right)
                        throw expected_an_identifier_after_dot(tok->get_fragment().end);

                    lib::pointer<ast::token> prop = new ast::property(left, right, guard);
                    left->replace(tok->next.get(), prop);
                    data->expressions.add(prop.get());
                }
                return false;
            }
        };

        lib::pointer<pattern> grammar_factory::create_pattern_property()
        {
            return new property(data);
        }
    };
};
