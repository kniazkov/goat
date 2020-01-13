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
#include "compiler/ast/identifier.h"
#include "compiler/ast/keyword_in.h"
#include "compiler/ast/keyword_var.h"
#include "compiler/ast/variable_in.h"
#include "lib/assert.h"

namespace g0at
{
    namespace parser
    {
        class expected_a_var_keyword : public compilation_error
        {
        public:
            expected_a_var_keyword(lib::pointer<position> pos)
                : compilation_error(pos, global::resource->expected_a_var_keyword())
            {
            }
        };

        class variable_in : public pattern
        {
        public:
            variable_in(parser_data *_data)
                : pattern(&_data->in_keywords, _data)
            {
            }

        protected:
            bool check(ast::token *tok) override
            {
                ast::keyword_in *kw_in = tok->to_keyword_in();
                assert(kw_in != nullptr);
                
                if (!kw_in->prev)
                    throw expected_an_identifier(kw_in->get_fragment().begin);

                ast::identifier *ident = kw_in->prev->to_identifier();
                if (!ident)
                    throw expected_an_identifier(kw_in->get_fragment().begin);

                if (ident->prev)
                {
                    if (ident->prev->to_keyword_var())
                    {
                        lib::pointer<ast::token> result = new ast::variable_in(ident, true);
                        ident->prev->replace(kw_in, result);
                    }
                    else
                    {
                        throw expected_a_var_keyword(ident->get_fragment().begin);
                    }
                }
                else
                {
                    lib::pointer<ast::token> result = new ast::variable_in(ident, false);
                    ident->replace(kw_in, result);
                }
                
                return false;
            }
        };

        lib::pointer<pattern> grammar_factory::create_pattern_variable_in()
        {
            return new variable_in(data);
        }
    };
};
