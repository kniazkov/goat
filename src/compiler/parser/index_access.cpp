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
#include "compiler/ast/brackets_pair.h"
#include "compiler/ast/index_access.h"
#include "lib/assert.h"


namespace g0at
{
    namespace parser
    {
        class index_access : public pattern
        {
        public:
            index_access(parser_data *_data)
                : pattern(&_data->expressions, _data)
            {
            }

        protected:
            bool check(ast::token *tok) override
            {
                ast::expression *object = tok->to_expression();
                assert(object != nullptr);
                
                if (!object->next)
                    return false;

                ast::brackets_pair *args = object->next->to_brackets_pair();
                if (args == nullptr || args->get_symbol() != '[')
                    return false;

                lib::pointer<ast::index_access> expr  = new ast::index_access(object, args);
                object->replace(args, expr.cast<ast::token>());
                data->expressions.add(expr.get());
                data->index_access.push_back(expr.get());
                return false;
            }
        };

        lib::pointer<pattern> grammar_factory::create_pattern_index_access()
        {
            return new index_access(data);
        }
    };
};
