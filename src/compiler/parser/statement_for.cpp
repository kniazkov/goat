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
#include "compiler/ast/keyword_for.h"
#include "compiler/ast/brackets_pair.h"
#include "compiler/ast/expression.h"
#include "compiler/ast/statement.h"
#include "compiler/ast/statement_for.h"
#include "lib/assert.h"

namespace g0at
{
    namespace parser
    {
        class expected_parameters_of_for_statement : public compilation_error
        {
        public:
            expected_parameters_of_for_statement(lib::pointer<position> pos)
                : compilation_error(pos, global::resource->expected_parameters_of_for_statement())
            {
            }
        };

        class statement_for : public pattern_right_to_left
        {
        public:
            statement_for(parser_data *_data)
                : pattern_right_to_left(&_data->if_keywords, _data)
            {
            }

        protected:
            bool check(ast::token *tok) override
            {
                ast::keyword_for *kw = tok->to_keyword_for();
                assert(kw != nullptr);

                if (!kw->next)
                    throw expected_a_conditional_expression(kw->get_position());
                
                return false;
            }
        };

        lib::pointer<pattern> grammar_factory::create_pattern_statement_for()
        {
            return new statement_for(data);
        }
    };
};
