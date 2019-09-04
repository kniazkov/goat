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
#include "compiler/ast/property.h"
#include "compiler/ast/identifier.h"
#include "compiler/ast/brackets_pair.h"
#include "compiler/ast/method_call.h"
#include "lib/assert.h"


namespace g0at
{
    namespace parser
    {
        class method_call : public pattern
        {
        public:
            method_call(parser_data *_data)
                : pattern(&_data->expressions, _data)
            {
            }

        protected:
            bool check(ast::token *tok) override
            {
                ast::expression *left = tok->to_expression();
                assert(left != nullptr);

                ast::property *prop = left->to_property();
                if (!prop || !prop->next)
                    return false;
                
                ast::brackets_pair *args = prop->next->to_brackets_pair();
                if (!args || args->get_symbol() != '(')
                    return false;

                lib::pointer<ast::method_call> vcall  = new ast::method_call(prop->get_left(), prop->get_name(), args);
                left->replace(args, vcall.cast<ast::token>());
                data->expressions.add(vcall.get());
                data->method_calls.push_back(vcall.get());
                return true;
            }
        };

        lib::pointer<pattern> grammar_factory::create_pattern_method_call()
        {
            return new method_call(data);
        }
    };
};
