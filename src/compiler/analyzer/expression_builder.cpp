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

#include "expression_builder.h"
#include "../ast/static_string.h"
#include "../pt/static_string.h"
#include "../ast/function_call.h"
#include "../pt/function_call.h"
#include <assert.h>

namespace g0at
{
    namespace analyzer
    {
        void expression_builder::visit(ast::static_string *ref)
        {
            expr = new pt::static_string(ref->get_position(), ref->get_text());
        }

        void expression_builder::visit(ast::function_call *ref)
        {
            lib::pointer<pt::function_call> fcall = 
                new pt::function_call(ref->get_position(), ref->get_name());
            auto args = ref->get_raw_list();
            auto tok_arg = args->first;
            while(tok_arg)
            {
                expression_builder visitor;
                tok_arg->accept(&visitor);
                assert(visitor.has_expr()); // TODO: exception ?
                fcall->add_arg(visitor.get_expr());
                tok_arg = tok_arg->next;
            }
            expr = fcall.cast<pt::expression>();
        }
    };
};