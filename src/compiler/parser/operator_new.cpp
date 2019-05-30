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
                
                return false;
            }
        };

        lib::pointer<pattern> grammar_factory::create_pattern_operator_new()
        {
            return new operator_new(data);
        }
    };
};
