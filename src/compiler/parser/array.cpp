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
#include "compiler/ast/brackets_pair.h"
#include "compiler/ast/token_object.h"
#include "lib/assert.h"

namespace g0at
{
    namespace parser
    {
        class array : public pattern
        {
        public:
            array(parser_data *_data)
                : pattern(&_data->pairs_of_square_brackets, _data)
            {
            }

        protected:
            bool check(ast::token *tok) override
            {
                ast::brackets_pair *body = tok->to_brackets_pair();
                assert (body != nullptr && body->get_symbol() == L'[');

                lib::pointer<ast::token_array> obj  = new ast::token_array(body);
                body->replace(obj.cast<ast::token>());
                data->expressions.add(obj.get());
                data->arrays.push_back(obj.get());
                return false;
            }
        };

        lib::pointer<pattern> grammar_factory::create_pattern_array()
        {
            return new array(data);
        }
    };
};
