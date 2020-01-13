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

#include "pattern_right_to_left.h"
#include "grammar_factory.h"
#include "common_exceptions.h"
#include "compiler/ast/keyword_lock.h"
#include "compiler/ast/statement.h"
#include "compiler/ast/statement_lock.h"
#include "lib/assert.h"

namespace g0at
{
    namespace parser
    {
        class statement_lock : public pattern_right_to_left
        {
        public:
            statement_lock(parser_data *_data)
                : pattern_right_to_left(&_data->lock_keywords, _data)
            {
            }

        protected:
            bool check(ast::token *tok) override
            {
                ast::keyword_lock *kw = tok->to_keyword_lock();
                assert(kw != nullptr);

                if (!kw->next)
                    throw expected_a_statement(kw->get_fragment().end);

                ast::statement *stmt = kw->next->to_statement();
                if (!stmt)
                    throw expected_a_statement(kw->next->get_fragment().begin);

                lib::pointer<ast::token> result = new ast::statement_lock(kw, stmt);
                kw->replace(stmt, result);

                return false;
            }
        };

        lib::pointer<pattern> grammar_factory::create_pattern_statement_lock()
        {
            return new statement_lock(data);
        }
    };
};
