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
#include "compiler/ast/operator_new.h"
#include "compiler/ast/index_access.h"
#include "compiler/ast/statement_switch.h"
#include "compiler/ast/parenthesized_expression.h"
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
            ast::token_2nd_list pairs_of_round_brackets;
            ast::token_2nd_list pairs_of_curly_brackets;
            ast::token_2nd_list pairs_of_square_brackets;
            ast::token_2nd_list dots;
            ast::token_2nd_list opers_equals_and_not;
            ast::token_2nd_list while_keywords;
            ast::token_2nd_list if_keywords;
            ast::token_2nd_list throw_keywords;
            ast::token_2nd_list try_keywords;
            ast::token_2nd_list opers_inherit;
            ast::token_2nd_list for_keywords;
            ast::token_2nd_list opers_less_greater;
            ast::token_2nd_list semicolons;
            ast::token_2nd_list new_keywords;
            ast::token_2nd_list opers_incr_decr;
            ast::token_2nd_list lock_keywords;
            ast::token_2nd_list in_keywords;
            ast::token_2nd_list do_keywords;
            ast::token_2nd_list break_keywords;
            ast::token_2nd_list continue_keywords;
            ast::token_2nd_list switch_keywords;
            ast::token_2nd_list opers_mul_div_mod;
            ast::token_2nd_list opers_exp;
            ast::token_2nd_list opers_excl_tildes;
            ast::token_2nd_list opers_shift;
            ast::token_2nd_list opers_ampersand;
            ast::token_2nd_list opers_dbl_ampersand;
            ast::token_2nd_list opers_vertical_bar;
            ast::token_2nd_list opers_dbl_vertical_bar;
            ast::token_2nd_list opers_caret;
            ast::token_2nd_list question_marks;
            ast::token_2nd_list debug_keywords;

            std::vector<ast::function*> functions;
            std::vector<ast::function_call*> function_calls;
            std::vector<ast::parenthesized_expression*> parenthesized;
            std::vector<ast::method_call*> method_calls;
            std::vector<ast::token_object*> objects;
            std::vector<ast::token_array*> arrays;
            std::vector<ast::statement_block*> blocks;
            std::vector<ast::operator_new*> operators_new;
            std::vector<ast::index_access*> index_access;
            std::vector<ast::statement_switch*> switches;
        };
    };
};
