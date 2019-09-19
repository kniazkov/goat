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
#include "compiler/ast/function_call.h"
#include "lib/assert.h"


namespace g0at
{
    namespace parser
    {
        class function_call : public pattern
        {
        public:
            function_call(parser_data *_data)
                : pattern(&_data->expressions, _data)
            {
            }

        protected:
            bool check(ast::token *tok) override
            {
                ast::expression *func_object = tok->to_expression();
                assert(func_object != nullptr);
                
                if (!func_object->next)
                    return false;

                ast::brackets_pair *args = func_object->next->to_brackets_pair();
                if (args == nullptr || args->get_symbol() != '(')
                    return false;

                lib::pointer<ast::function_call> fcall  = new ast::function_call(func_object, args);
                func_object->replace(args, fcall.cast<ast::token>());
                data->expressions.add(fcall.get());
                data->function_calls.push_back(fcall.get());
                return false;
            }
        };

        lib::pointer<pattern> grammar_factory::create_pattern_function_call()
        {
            return new function_call(data);
        }
    };
};
