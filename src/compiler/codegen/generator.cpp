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
#include "compiler/pt/variable.h"
#include "compiler/pt/function.h"
#include "compiler/pt/static_string.h"
#include "compiler/pt/function_call.h"
#include "compiler/pt/statement_expression.h"
#include "compiler/pt/addition.h"
#include "compiler/pt/integer.h"
#include "compiler/pt/subtraction.h"
#include "compiler/pt/negation.h"
#include "compiler/pt/declare_variable.h"
#include "compiler/pt/assignment.h"
#include "compiler/pt/real.h"
#include "compiler/pt/declare_function.h"
#include "code/load_string.h"
#include "code/load_var.h"
#include "code/call.h"
#include "code/pop.h"
#include "code/end.h"
#include "code/add.h"
#include "code/load_integer.h"
#include "code/sub.h"
#include "code/neg.h"
#include "code/load_void.h"
#include "code/load_undefined.h"
#include "code/load_null.h"
#include "code/decl_var.h"
#include "code/load_real.h"
#include "code/load_func.h"
#include "code/ret.h"
#include <assert.h>

namespace g0at
{
    namespace codegen
    {
        generator::generator()
        {
            code = new code::code();
            lgen = new lvalue_generator(code, &name_cache);
        }

        lib::pointer<code::code> generator::generate(lib::pointer<pt::function> node_root)
        {
            generator gen;
            node_root->accept(&gen);
            while(!gen.queue.empty())
            {
                deferred_node def = gen.queue.back();
                gen.queue.pop_back();
                *(def.iid_ptr) = gen.code->get_code_size();
                def.node->accept(&gen);
            }
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
            else
            {
                code->add_instruction(new code::ret);
            }
        }

        void generator::visit(pt::static_string *ref)
        {
            code->add_instruction(new code::load_string(ref->get_text()));
        }

        void generator::visit(pt::variable *ref)
        {
            int id = name_cache.get_id(ref->get_name());
            code->add_instruction(new code::load_var(id));
        }

        void generator::visit(pt::function_call *ref)
        {
            int args_count = ref->get_args_count();
            for (int i = args_count - 1; i > -1; i--)
            {
                ref->get_arg(i)->accept(this);
            }
            ref->get_func_object()->accept(this);
            code->add_instruction(new code::call(args_count));
        }

        void generator::visit(pt::statement_expression *ref)
        {
            ref->get_expression()->accept(this);
            code->add_instruction(new code::pop());
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

        void generator::visit(pt::negation *ref)
        {
            ref->get_right()->accept(this);
            code->add_instruction(new code::neg());
        }

        void generator::visit(pt::value_void *ref)
        {
            code->add_instruction(new code::load_void());
        }

        void generator::visit(pt::value_undefined *ref)
        {
            code->add_instruction(new code::load_undefined());
        }

        void generator::visit(pt::value_null *ref)
        {
            code->add_instruction(new code::load_null());
        }

        void generator::visit(pt::declare_variable *ref)
        {
            for (int i = 0, count = ref->get_count(); i < count; i++)
            {
                pt::variable_info info = ref->get_variable(i);
                if (info.init_val)
                    info.init_val->accept(this);
                else
                    code->add_instruction(new code::load_undefined());
                int id = name_cache.get_id(info.name);
                code->add_instruction(new code::decl_var(id));
            }
        }

        void generator::visit(pt::assignment *ref)
        {
            ref->get_right()->accept(this);
            ref->get_left()->accept(lgen.get());
        }

        void generator::visit(pt::real *ref)
        {
            code->add_instruction(new code::load_real(ref->get_value()));
        }

        void generator::visit(pt::declare_function *ref)
        {
            code::load_func *instr = new code::load_func(-1);
            code->add_instruction(instr);

            pt::function *func = ref->get_func().get();

            deferred_node def;
            def.iid_ptr = instr->get_first_iid_ptr();
            def.node = func;            
            queue.push_front(def);

            for (int i = 0, count = func->get_args_count(); i < count; i++)
            {
                int id = name_cache.get_id(func->get_arg(i));
                instr->add_arg_id(id);
            }
        }
    };
};