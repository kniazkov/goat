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
#include "compiler/ast/keyword_function.h"
#include "compiler/ast/keyword_thread.h"
#include "compiler/ast/brackets_pair.h"
#include "compiler/ast/function.h"
#include "compiler/ast/declare_function.h"
#include "lib/assert.h"

namespace g0at
{
    namespace parser
    {
        class function_decl : public pattern
        {
        public:
            function_decl(parser_data *_data)
                : pattern(&_data->function_keywords, _data)
            {
            }

        protected:
            bool check(ast::token *tok) override
            {
                ast::function_type type = ast::function_type::function;

                ast::keyword *kw = tok->to_keyword_function();
                if (!kw)
                {
                    kw = tok->to_keyword_thread();
                    type = ast::function_type::thread;
                }
                assert(kw != nullptr);

                if (!kw->next)
                    throw expected_an_argument_list(kw->get_position());

                ast::brackets_pair *args = kw->next->to_brackets_pair();
                if (!args || args->get_symbol() != L'(')
                    throw expected_an_argument_list(kw->next->get_position());

                if (!args->next)
                    throw expected_a_function_body(args->get_position());

                ast::brackets_pair *body = args->next->to_brackets_pair();
                if (!body || body->get_symbol() != L'{')
                    throw expected_a_function_body(args->next->get_position());

                lib::pointer<ast::function> func  = new ast::function(kw, args, body, type);
                lib::pointer<ast::declare_function> decl_func = new ast::declare_function(func);
                kw->replace(body, decl_func.cast<ast::token>());
                data->expressions.add(decl_func.get());
                data->functions.push_back(func.get());
                return false;
            }
        };

        lib::pointer<pattern> grammar_factory::create_pattern_function_decl()
        {
            return new function_decl(data);
        }
    };
};
