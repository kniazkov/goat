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
#include "compiler/ast/semicolon.h"
#include "compiler/ast/statement_empty.h"
#include "lib/assert.h"

namespace g0at
{
    namespace parser
    {
        class statement_empty : public pattern
        {
        public:
            statement_empty(parser_data *_data)
                : pattern(&_data->semicolons, _data)
            {
            }

        protected:
            bool check(ast::token *tok) override
            {
                ast::semicolon *scolon = tok->to_semicolon();
                assert(scolon != nullptr);
                
                lib::pointer<ast::token> stmt  = new ast::statement_empty(scolon);
                tok->replace(stmt);
                return false;
            }
        };

        lib::pointer<pattern> grammar_factory::create_pattern_statement_empty()
        {
            return new statement_empty(data);
        }
    };
};
