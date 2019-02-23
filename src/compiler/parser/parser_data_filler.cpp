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

#include "parser_data_filler.h"
#include "compiler/ast/identifier.h"
#include "compiler/ast/plus.h"
#include "compiler/ast/minus.h"
#include "compiler/ast/static_string.h"
#include "compiler/ast/integer.h"
#include "compiler/ast/keyword_var.h"
#include "compiler/ast/keyword_function.h"
#include "compiler/ast/assign.h"
#include "compiler/ast/keyword_return.h"
#include "compiler/ast/brackets_pair.h"

namespace g0at
{
    namespace parser
    {
        parser_data_filler::parser_data_filler(parser_data *_data)
            : data(_data)
        {
        }

        void parser_data_filler::visit(ast::identifier *ref)
        {
            data->identifiers.add(ref);
        }

        void parser_data_filler::visit(ast::plus *ref)
        {
            data->opers_plus_minus.add(ref);
        }

        void parser_data_filler::visit(ast::minus *ref)
        {
            data->opers_plus_minus.add(ref);
        }

        void parser_data_filler::visit(ast::static_string *ref)
        {
            data->expressions.add(ref);
        }

        void parser_data_filler::visit(ast::integer *ref)
        {
            data->expressions.add(ref);
        }

        void parser_data_filler::visit(ast::keyword_var *ref)
        {
            data->var_keywords.add(ref);
        }

        void parser_data_filler::visit(ast::assign *ref)
        {
            data->opers_assign.add(ref);
        }

        void parser_data_filler::visit(ast::keyword_function *ref)
        {
            data->function_keywords.add(ref);
        }

        void parser_data_filler::visit(ast::keyword_return *ref)
        {
            data->return_keywords.add(ref);
        }

        void parser_data_filler::visit(ast::brackets_pair *ref)
        {
            if (ref->get_symbol() == L'{')
                data->pairs_of_curly_brackets.add(ref);
        }
    };
};