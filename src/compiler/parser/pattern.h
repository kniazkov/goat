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

#pragma once

#include "lib/ref_counter.h"
#include "compiler/ast/token_2nd_list.h"
#include "parser_data.h"

namespace g0at
{
    namespace parser
    {
        class pattern : public lib::ref_counter
        {
        public:
            pattern(ast::token_2nd_list *_list, parser_data *_data);
            virtual ~pattern();
            virtual bool pass();
        
        protected:
            /**
             * @brief Pattern matching check
             * 
             * Applies a pattern to a sequence of tokens.
             * Optionally, replaces a sequence of tokens with a non-terminal token.
             * 
             * @param tok A start token (not necessarily the first)
             * @return true if the second tokens list was changed as a result of applying the template,
             *   otherwise false
             */
            virtual bool check(ast::token *tok) = 0;
            
            /**
             * @brief The second token list which contains start tokens
             */
            ast::token_2nd_list *list;
            /**
             * @brief Parser data
             */
            parser_data *data;
        };
    };
};
