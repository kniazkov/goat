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

#include "generator.h"
#include "compiler/pt/function.h"
#include "compiler/pt/static_string.h"
#include "compiler/pt/function_call.h"
#include "compiler/pt/statement_expression.h"
#include "compiler/pt/addition.h"
#include "compiler/pt/integer.h"
#include "compiler/pt/subtraction.h"
#include "code/load_string.h"
#include "code/load_var.h"
#include "code/call.h"
#include "code/pop.h"
#include "code/end.h"
#include "code/add.h"
#include "code/load_integer.h"
#include "code/sub.h"
#include <assert.h>

namespace g0at
{
    namespace codegen
    {
        generator::generator()
        {
            code = new code::code();
        }

        lib::pointer<code::code> generator::generate(lib::pointer<pt::function> node_root)
        {
            generator gen;
            node_root->accept(&gen);
            gen.code->set_identifiers_list(gen.name_cache.get_vector());
            return gen.code;
        }

        void generator::visit(pt::function *ref)
        {
            for (int i = 0, cnt = ref->get_code_size(); i < cnt; i++)
            {
                ref->get_stmt(i)->accept(this);
            }

            if (ref->is_root_function())
            {
                code->add_instruction(new code::end);
            }
        }

        void generator::visit(pt::static_string *ref)
        {
            code->add_instruction(new code::load_string(ref->get_text()));
        }

        void generator::visit(pt::function_call *ref)
        {
            int args_count = ref->get_args_count();
            assert(args_count < UINT16_MAX);
            for (int i = args_count - 1; i > -1; i--)
            {
                ref->get_arg(i)->accept(this);
            }
            int id = name_cache.get_id(ref->get_name());
            code->add_instruction(new code::load_var(id));
            code->add_instruction(new code::call);
            code->add_instruction(new code::pop(static_cast<uint16_t>(args_count)));
        }

        void generator::visit(pt::statement_expression *ref)
        {
            ref->get_expression()->accept(this);
        }

        void generator::visit(pt::addition *ref)
        {
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
            code->add_instruction(new code::add());
        }

        void generator::visit(pt::integer *ref)
        {
            code->add_instruction(new code::load_integer(ref->get_value()));
        }

        void generator::visit(pt::subtraction *ref)
        {
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
            code->add_instruction(new code::sub());
        }
    };
};