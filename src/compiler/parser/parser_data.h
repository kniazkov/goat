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

#include "compiler/ast/token_2nd_list.h"
#include "compiler/ast/function.h"
#include "compiler/ast/function_call.h"
#include "compiler/ast/token_object.h"
#include "compiler/ast/method_call.h"
#include "compiler/ast/token_array.h"
#include "compiler/ast/statement_block.h"
#include <vector>

namespace g0at
{
    namespace parser
    {
        class parser_data
        {
        public:
            ast::token_2nd_list identifiers;
            ast::token_2nd_list expressions;
            ast::token_2nd_list opers_plus_minus;
            ast::token_2nd_list opers_assign;
            ast::token_2nd_list var_keywords;
            ast::token_2nd_list function_keywords;
            ast::token_2nd_list return_keywords;
            ast::token_2nd_list pairs_of_curly_brackets;
            ast::token_2nd_list pairs_of_square_brackets;
            ast::token_2nd_list dots;
            ast::token_2nd_list opers_equals_and_not;
            ast::token_2nd_list while_keywords;

            std::vector<ast::function*> functions;
            std::vector<ast::function_call*> function_calls;
            std::vector<ast::method_call*> method_calls;
            std::vector<ast::token_object*> objects;
            std::vector<ast::token_array*> arrays;
            std::vector<ast::statement_block*> blocks;
        };
    };
};
