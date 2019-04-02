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
#include "lib/assert.h"
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
#include "compiler/pt/statement_return.h"
#include "compiler/pt/node_object.h"
#include "compiler/pt/property.h"
#include "compiler/pt/is_equal_to.h"
#include "compiler/pt/is_not_equal_to.h"
#include "compiler/pt/statement_while.h"
#include "compiler/pt/method_call.h"
#include "compiler/pt/this_ptr.h"
#include "compiler/pt/node_array.h"
#include "compiler/pt/statement_block.h"
#include "compiler/pt/statement_if.h"
#include "compiler/pt/statement_throw.h"
#include "compiler/pt/statement_try.h"
#include "compiler/pt/inheritance.h"
#include "code/string.h"
#include "code/load.h"
#include "code/call.h"
#include "code/pop.h"
#include "code/end.h"
#include "code/add.h"
#include "code/integer.h"
#include "code/sub.h"
#include "code/neg.h"
#include "code/void.h"
#include "code/undef.h"
#include "code/null.h"
#include "code/var.h"
#include "code/real.h"
#include "code/func.h"
#include "code/ret.h"
#include "code/retv.h"
#include "code/object.h"
#include "code/prop.h"
#include "code/true.h"
#include "code/false.h"
#include "code/eq.h"
#include "code/neq.h"
#include "code/ifnot.h"
#include "code/jmp.h"
#include "code/vcall.h"
#include "code/this.h"
#include "code/clone.h"
#include "code/insof.h"
#include "code/array.h"
#include "code/enter.h"
#include "code/leave.h"
#include "code/throw.h"
#include "code/try.h"
#include "code/catch.h"
#include "code/finally.h"
#include "code/inherit.h"
#include "code/flat.h"

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
                *(def.iid_ptr) = gen.code->get_current_iid();
                def.node->accept(&gen);
            }
            gen.code->set_identifiers_list(gen.name_cache.get_vector());
            return gen.code;
        }

        void generator::visit(pt::function *ref)
        {
            int code_size = ref->get_code_size();
            for (int i = 0; i < code_size; i++)
            {
                ref->get_stmt(i)->accept(this);
            }

            if (ref->is_root_function())
            {
                code->add_instruction(new code::_end());
            }
            else
            {
                if ((code_size > 0 && ref->get_stmt(code_size - 1)->to_statement_return() == nullptr) || code_size == 0)
                    code->add_instruction(new code::_ret());
            }
        }

        void generator::visit(pt::static_string *ref)
        {
            int id = name_cache.get_id(ref->get_text());
            code->add_instruction(new code::_string(id));
        }

        void generator::visit(pt::variable *ref)
        {
            int id = name_cache.get_id(ref->get_name());
            code->add_instruction(new code::_load(id));
        }

        void generator::visit(pt::function_call *ref)
        {
            int args_count = ref->get_args_count();
            for (int i = args_count - 1; i > -1; i--)
            {
                ref->get_arg(i)->accept(this);
            }
            ref->get_func_object()->accept(this);
            code->add_instruction(new code::_call(args_count));
        }

        void generator::visit(pt::statement_expression *ref)
        {
            ref->get_expression()->accept(this);
            code->add_instruction(new code::_pop());
        }

        void generator::visit(pt::addition *ref)
        {
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
            code->add_instruction(new code::_add());
        }

        void generator::visit(pt::integer *ref)
        {
            code->add_instruction(new code::_integer(ref->get_value()));
        }

        void generator::visit(pt::subtraction *ref)
        {
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
            code->add_instruction(new code::_sub());
        }

        void generator::visit(pt::negation *ref)
        {
            ref->get_right()->accept(this);
            code->add_instruction(new code::_neg());
        }

        void generator::visit(pt::value_void *ref)
        {
            code->add_instruction(new code::_void());
        }

        void generator::visit(pt::value_undefined *ref)
        {
            code->add_instruction(new code::_undef());
        }

        void generator::visit(pt::value_null *ref)
        {
            code->add_instruction(new code::_null());
        }

        void generator::visit(pt::declare_variable *ref)
        {
            for (int i = 0, count = ref->get_count(); i < count; i++)
            {
                pt::variable_info info = ref->get_variable(i);
                if (info.init_val)
                    info.init_val->accept(this);
                else
                    code->add_instruction(new code::_undef());
                int id = name_cache.get_id(info.name);
                code->add_instruction(new code::_var(id));
            }
        }

        void generator::visit(pt::assignment *ref)
        {
            ref->get_right()->accept(this);
            ref->get_left()->accept(lgen.get());
        }

        void generator::visit(pt::real *ref)
        {
            code->add_instruction(new code::_real(ref->get_value()));
        }

        void generator::visit(pt::declare_function *ref)
        {
            code::_func *instr = new code::_func(-1);
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

        void generator::visit(pt::statement_return *ref)
        {
            auto expr = ref->get_expression();
            if (expr)
            {
                ref->get_expression()->accept(this);
                code->add_instruction(new code::_retv());
            }
            else
            {
                code->add_instruction(new code::_ret());
            }
        }

        void generator::visit(pt::node_object *ref)
        {
            int count = ref->get_items_count();
            for (int i = count - 1; i > -1; i--)
            {
                auto item = ref->get_item(i);
                item.second->accept(this);
                item.first->accept(this);
            }
            code->add_instruction(new code::_object(count));
        }

        void generator::visit(pt::property *ref)
        {
            ref->get_left()->accept(this);
            int id = name_cache.get_id(ref->get_name());
            code->add_instruction(new code::_prop(id));
        }

        void generator::visit(pt::value_true *ref)
        {
            code->add_instruction(new code::_true());
        }

        void generator::visit(pt::value_false *ref)
        {
            code->add_instruction(new code::_false());
        }

        void generator::visit(pt::is_equal_to *ref)
        {
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
            code->add_instruction(new code::_eq());
        }

        void generator::visit(pt::is_not_equal_to *ref)
        {
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
            code->add_instruction(new code::_neq());
        }

        void generator::visit(pt::statement_while *ref)
        {
            int iid_begin = code->get_current_iid();
            ref->get_expression()->accept(this);
            code::_ifnot *if_not = new code::_ifnot(-1);
            code->add_instruction(if_not);
            ref->get_statement()->accept(this);
            code->add_instruction(new code::_jmp(iid_begin));
            *(if_not->get_iid_ptr()) = code->get_current_iid();
        }

        void generator::visit(pt::method_call *ref)
        {
            int args_count = ref->get_args_count();
            for (int i = args_count - 1; i > -1; i--)
            {
                ref->get_arg(i)->accept(this);
            }
            ref->get_left()->accept(this);
            std::wstring name = ref->get_name();
            if (name == L"clone")
                code->add_instruction(new code::_clone(args_count));
            else if (name == L"instanceOf")
                code->add_instruction(new code::_insof(args_count));
            else if (name == L"flat")
                code->add_instruction(new code::_flat(args_count));
            else
            {
                int id = name_cache.get_id(name);
                code->add_instruction(new code::_vcall(id, args_count));
            }
        }

        void generator::visit(pt::this_ptr *ref)
        {
            code->add_instruction(new code::_this());
        }

        void generator::visit(pt::node_array *ref)
        {
            int count = ref->get_objects_count();
            for (int i = count - 1; i > -1; i--)
            {
                auto item = ref->get_object(i);
                item->accept(this);
            }
            code->add_instruction(new code::_array(count));
        }

        void generator::visit(pt::statement_block *ref)
        {
            int code_size = ref->get_code_size();
            bool has_variables = ref->has_variables();

            if (has_variables)
            {
                code->add_instruction(new code::_enter());
            }
            for (int i = 0; i < code_size; i++)
            {
                ref->get_stmt(i)->accept(this);
            }
            if (has_variables)
            {
                code->add_instruction(new code::_leave());
            }
        }

        void generator::visit(pt::statement_if *ref)
        {
            ref->get_expression()->accept(this);
            code::_ifnot *ifnot = new code::_ifnot(-1);
            code->add_instruction(ifnot);
            ref->get_stmt_if()->accept(this);
            int *iid_ptr_end = nullptr;
            auto stmt_else = ref->get_stmt_else();
            if (stmt_else)
            {
                code::_jmp *jmp = new code::_jmp(-1);
                iid_ptr_end = jmp->get_iid_ptr();
                code->add_instruction(jmp);
            }
            *(ifnot->get_iid_ptr()) = code->get_current_iid();
            if (stmt_else)
            {
                stmt_else->accept(this);
                *iid_ptr_end = code->get_current_iid();
            }
        }

        void generator::visit(pt::statement_throw *ref)
        {
            auto expr = ref->get_expression();
            if (expr)
            {
                ref->get_expression()->accept(this);
                code->add_instruction(new code::_throw());
            }
            else
            {
                assert(false && "Not implemented");
            }
        }

        void generator::visit(pt::statement_try *ref)
        {
            auto stmt_catch = ref->get_stmt_catch();
            auto stmt_finally = ref->get_stmt_finally();

            int *iid_end_ptr = nullptr;
            int *iid_finally_ptr = nullptr;

            if (stmt_finally)
            {
                code::_finally *instr_finally = new code::_finally(-1);
                iid_finally_ptr = instr_finally->get_iid_ptr();
                code->add_instruction(instr_finally);
            }

            code::_try *instr_try = new code::_try(-1);
            int *iid_catch_ptr = instr_try->get_iid_ptr();
            code->add_instruction(instr_try);

            ref->get_stmt_try()->accept(this);

            if (stmt_catch)
            {
                code::_jmp *jmp_end = new code::_jmp(-1);
                iid_end_ptr = jmp_end->get_iid_ptr();
                code->add_instruction(jmp_end);

                *iid_catch_ptr = code->get_current_iid();

                if (ref->has_var())
                {
                    int id = name_cache.get_id(ref->get_var_name());
                    code->add_instruction(new code::_catch(id));
                }
                stmt_catch->accept(this);
            }

            if(iid_end_ptr)
                *iid_end_ptr = code->get_current_iid();

            code->add_instruction(new code::_leave());
            code->add_instruction(new code::_pop());

            if (stmt_finally)
            {
                *iid_finally_ptr = code->get_current_iid();
                stmt_finally->accept(this);
                code->add_instruction(new code::_leave());
            }
        }

        void generator::visit(pt::inheritance *ref)
        {
            ref->get_right()->accept(this);
            code->add_instruction(new code::_clone(0));
            ref->get_left()->accept(this);
            code->add_instruction(new code::_inherit());
        }
    };
};