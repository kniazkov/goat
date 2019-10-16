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
#include "compiler/pt/character.h"
#include "compiler/pt/statement_for.h"
#include "compiler/pt/is_less_than.h"
#include "compiler/pt/statement_empty.h"
#include "compiler/pt/operator_new.h"
#include "compiler/pt/prefix_increment.h"
#include "compiler/pt/statement_lock.h"
#include "compiler/pt/index_access.h"
#include "compiler/pt/statement_for_in.h"
#include "compiler/pt/statement_do_while.h"
#include "compiler/pt/statement_break.h"
#include "compiler/pt/statement_continue.h"
#include "compiler/pt/statement_switch.h"
#include "compiler/pt/suffix_increment.h"
#include "compiler/pt/prefix_decrement.h"
#include "compiler/pt/suffix_decrement.h"
#include "compiler/pt/multiplication.h"
#include "compiler/pt/exponentiation.h"
#include "compiler/pt/division.h"
#include "compiler/pt/remainder.h"
#include "compiler/pt/unary_plus.h"
#include "compiler/pt/bitwise_not.h"
#include "compiler/pt/logical_not.h"
#include "compiler/pt/operator_bool.h"
#include "compiler/pt/left_shift.h"
#include "compiler/pt/signed_right_shift.h"
#include "compiler/pt/zero_fill_right_shift.h"
#include "compiler/pt/is_less_than_or_equal_to.h"
#include "compiler/pt/is_greater_than.h"
#include "compiler/pt/is_greater_than_or_equal_to.h"
#include "compiler/pt/bitwise_and.h"
#include "compiler/pt/bitwise_or.h"
#include "compiler/pt/bitwise_xor.h"
#include "compiler/pt/logical_and.h"
#include "compiler/pt/logical_or.h"
#include "compiler/pt/assignment_by_sum.h"
#include "compiler/pt/assignment_by_difference.h"
#include "compiler/pt/assignment_by_product.h"
#include "compiler/pt/assignment_by_quotient.h"
#include "compiler/pt/assignment_by_remainder.h"
#include "compiler/pt/assignment_by_left_shift.h"
#include "compiler/pt/assignment_by_signed_right_shift.h"
#include "compiler/pt/assignment_by_zero_fill_right_shift.h"
#include "compiler/pt/assignment_by_bitwise_and.h"
#include "compiler/pt/assignment_by_bitwise_or.h"
#include "compiler/pt/assignment_by_bitwise_xor.h"
#include "compiler/pt/ternary.h"
#include "compiler/pt/statement_debug.h"
#include "compiler/pt/protection.h"

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
#include "code/read.h"
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
#include "code/char.h"
#include "code/less.h"
#include "code/nop.h"
#include "code/new.h"
#include "code/inc.h"
#include "code/thread.h"
#include "code/rethrow.h"
#include "code/lock.h"
#include "code/unlock.h"
#include "code/get.h"
#include "code/set.h"
#include "code/iter.h"
#include "code/valid.h"
#include "code/next.h"
#include "code/dup.h"
#include "code/store.h"
#include "code/if.h"
#include "code/sector.h"
#include "code/break.h"
#include "code/cont.h"
#include "code/dec.h"
#include "code/mul.h"
#include "code/exp.h"
#include "code/div.h"
#include "code/mod.h"
#include "code/pos.h"
#include "code/not.h"
#include "code/inv.h"
#include "code/bool.h"
#include "code/shl.h"
#include "code/shr.h"
#include "code/shrz.h"
#include "code/leq.h"
#include "code/great.h"
#include "code/greq.h"
#include "code/and.h"
#include "code/or.h"
#include "code/bitand.h"
#include "code/bitor.h"
#include "code/xor.h"
#include "code/swap.h"
#include "code/frame.h"
#include "code/debug.h"
#include "code/protect.h"

namespace g0at
{
    namespace codegen
    {
        using namespace code;

        generator::generator(model::name_cache *_name_cache, bool _debug)
            : name_cache(_name_cache), debug(_debug)
        {
            code = new code::code();
            lgen = new lvalue_generator(code, name_cache, this);
        }

        lib::pointer<code::code> generator::generate(model::name_cache *name_cache, lib::pointer<pt::function> node_root, bool debug)
        {
            generator gen(name_cache, debug);
            node_root->accept(&gen);
            while(!gen.queue.empty())
            {
                deferred_node def = gen.queue.back();
                gen.queue.pop_back();
                def.iid_ptr.set(gen.code->get_current_iid());
                def.node->accept(&gen);
            }
            gen.code->set_identifiers_list(gen.name_cache->get_vector());
            return gen.code;
        }

        void generator::add_frame(pt::node *node)
        {
            if (debug)
            {
                auto frag = node->get_fragment();
                code->add_instruction(new _frame(frag.begin->get_absolute_position(), frag.end->get_absolute_position()));
            }
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
                code->add_instruction(new _end());
            }
            else
            {
                if ((code_size > 0 && ref->get_stmt(code_size - 1)->to_statement_return() == nullptr) || code_size == 0)
                    code->add_instruction(new _ret());
            }
        }

        void generator::visit(pt::static_string *ref)
        {
            int id = name_cache->get_id(ref->get_text());
            code->add_instruction(new _string(id));
        }

        void generator::visit(pt::variable *ref)
        {
            int id = name_cache->get_id(ref->get_name());
            code->add_instruction(new _load(id));
        }

        void generator::visit(pt::function_call *ref)
        {
            int args_count = ref->get_args_count();
            for (int i = args_count - 1; i > -1; i--)
            {
                ref->get_arg(i)->accept(this);
            }
            ref->get_func_object()->accept(this);
            code->add_instruction(new _call(args_count));
        }

        void generator::visit(pt::statement_expression *ref)
        {
            add_frame(ref);
            ref->get_expression()->accept(this);
            code->add_instruction(new _pop());
        }

        void generator::visit(pt::addition *ref)
        {
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
            code->add_instruction(new _add());
        }

        void generator::visit(pt::integer *ref)
        {
            code->add_instruction(new _integer(ref->get_value()));
        }

        void generator::visit(pt::subtraction *ref)
        {
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
            code->add_instruction(new _sub());
        }

        void generator::visit(pt::negation *ref)
        {
            ref->get_right()->accept(this);
            code->add_instruction(new _neg());
        }

        void generator::visit(pt::value_void *ref)
        {
            code->add_instruction(new _void());
        }

        void generator::visit(pt::value_undefined *ref)
        {
            code->add_instruction(new _undef());
        }

        void generator::visit(pt::value_null *ref)
        {
            code->add_instruction(new _null());
        }

        void generator::visit(pt::declare_variable *ref)
        {
            add_frame(ref);
            for (int i = 0, count = ref->get_count(); i < count; i++)
            {
                pt::variable_info info = ref->get_variable(i);
                if (info.init_val)
                    info.init_val->accept(this);
                else
                    code->add_instruction(new _undef());
                int id = name_cache->get_id(info.name);
                code->add_instruction(new _var(id));
            }
        }

        void generator::visit(pt::assignment *ref)
        {
            ref->get_right()->accept(this);
            ref->get_left()->accept(lgen.get());
        }

        void generator::visit(pt::real *ref)
        {
            code->add_instruction(new _real(ref->get_value()));
        }

        void generator::visit(pt::declare_function *ref)
        {
            pt::function *func = ref->get_func().get();
            
            if (func->get_type() == pt::function_type::function)
            {
                _func *instr = new _func(iid_unknown());
                code->add_instruction(instr);
                deferred_node def;
                def.iid_ptr = instr->get_first_iid_ptr();
                def.node = func;            
                queue.push_front(def);
                for (int i = 0, count = func->get_args_count(); i < count; i++)
                {
                    int id = name_cache->get_id(func->get_arg(i));
                    instr->add_arg_id(id);
                }
            }
            else if (func->get_type() == pt::function_type::thread)
            {
                _thread *instr = new _thread(iid_unknown());
                code->add_instruction(instr);
                deferred_node def;
                def.iid_ptr = instr->get_first_iid_ptr();
                def.node = func;            
                queue.push_front(def);
                for (int i = 0, count = func->get_args_count(); i < count; i++)
                {
                    int id = name_cache->get_id(func->get_arg(i));
                    instr->add_arg_id(id);
                }
            }
        }

        void generator::visit(pt::statement_return *ref)
        {
            add_frame(ref);
            auto expr = ref->get_expression();
            if (expr)
            {
                expr->accept(this);
                code->add_instruction(new _retv());
            }
            else
            {
                code->add_instruction(new _ret());
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
            code->add_instruction(new _object(count));
        }

        void generator::visit(pt::property *ref)
        {
            ref->get_left()->accept(this);
            int id = name_cache->get_id(ref->get_name());
            code->add_instruction(new _read(id));
        }

        void generator::visit(pt::value_true *ref)
        {
            code->add_instruction(new _true());
        }

        void generator::visit(pt::value_false *ref)
        {
            code->add_instruction(new _false());
        }

        void generator::visit(pt::is_equal_to *ref)
        {
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
            code->add_instruction(new _eq());
        }

        void generator::visit(pt::is_not_equal_to *ref)
        {
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
            code->add_instruction(new _neq());
        }

        void generator::visit(pt::statement_while *ref)
        {
            add_frame(ref);
            _sector *cycle = new _sector(iid_unknown(), iid_unknown());
            code->add_instruction(cycle);
            iid_t iid_begin = code->get_current_iid();
            ref->get_expression()->accept(this);
            code->add_instruction(new _bool());
            _ifnot *if_not = new _ifnot(iid_unknown());
            code->add_instruction(if_not);
            cycle->get_begin_ptr().set(code->get_current_iid());
            ref->get_statement()->accept(this);
            code->add_instruction(new _jmp(iid_begin));
            if_not->get_iid_ptr().set(code->get_current_iid());
            code->add_instruction(new _leave());
            cycle->get_end_ptr().set(code->get_current_iid());
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
                code->add_instruction(new _clone(args_count));
            else if (name == L"instanceof")
                code->add_instruction(new _insof(args_count));
            else if (name == L"flat")
                code->add_instruction(new _flat(args_count));
            else if (name == L"get")
                code->add_instruction(new _get(args_count));
            else if (name == L"set")
                code->add_instruction(new _set(args_count));
            else if (name == L"iterator")
                code->add_instruction(new _iter(args_count));
            else if (name == L"valid")
                code->add_instruction(new _valid(args_count));
            else if (name == L"next")
                code->add_instruction(new _next(args_count));
            else
            {
                int id = name_cache->get_id(name);
                code->add_instruction(new _vcall(id, args_count));
            }
        }

        void generator::visit(pt::this_ptr *ref)
        {
            code->add_instruction(new _this());
        }

        void generator::visit(pt::node_array *ref)
        {
            int count = ref->get_objects_count();
            for (int i = count - 1; i > -1; i--)
            {
                auto item = ref->get_object(i);
                item->accept(this);
            }
            code->add_instruction(new _array(count));
        }

        void generator::visit(pt::statement_block *ref)
        {
            int code_size = ref->get_code_size();
            bool has_variables = ref->has_variables();

            if (has_variables)
            {
                code->add_instruction(new _enter());
            }
            for (int i = 0; i < code_size; i++)
            {
                ref->get_stmt(i)->accept(this);
            }
            if (has_variables)
            {
                code->add_instruction(new _leave());
            }
        }

        void generator::visit(pt::statement_if *ref)
        {
            add_frame(ref);
            ref->get_expression()->accept(this);
            code->add_instruction(new _bool());
            _ifnot *ifnot = new _ifnot(iid_unknown());
            code->add_instruction(ifnot);
            ref->get_stmt_if()->accept(this);
            iid_ptr_t iid_ptr_end;
            auto stmt_else = ref->get_stmt_else();
            if (stmt_else)
            {
                _jmp *jmp = new _jmp(iid_unknown());
                iid_ptr_end = jmp->get_iid_ptr();
                code->add_instruction(jmp);
            }
            ifnot->get_iid_ptr().set(code->get_current_iid());
            if (stmt_else)
            {
                stmt_else->accept(this);
                iid_ptr_end.set(code->get_current_iid());
            }
        }

        void generator::visit(pt::statement_throw *ref)
        {
            add_frame(ref);
            auto expr = ref->get_expression();
            if (expr)
            {
                expr->accept(this);
                code->add_instruction(new _throw());
            }
            else
            {
                code->add_instruction(new _rethrow());
            }
        }

        void generator::visit(pt::statement_try *ref)
        {
            auto stmt_catch = ref->get_stmt_catch();
            auto stmt_finally = ref->get_stmt_finally();

            iid_ptr_t iid_finally_ptr;

            if (stmt_finally)
            {
                _finally *instr_finally = new _finally(iid_unknown());
                iid_finally_ptr = instr_finally->get_iid_ptr();
                code->add_instruction(instr_finally);
            }

            if (stmt_catch)
            {
                _try *instr_try = new _try(iid_unknown());
                iid_ptr_t iid_catch_ptr = instr_try->get_iid_ptr();
                code->add_instruction(instr_try);
                ref->get_stmt_try()->accept(this);
                _jmp *jmp_end = new _jmp(iid_unknown());
                iid_ptr_t iid_end_ptr = jmp_end->get_iid_ptr();
                code->add_instruction(jmp_end);
                iid_catch_ptr.set(code->get_current_iid());
                if (ref->has_var())
                {
                    int id = name_cache->get_id(ref->get_var_name());
                    code->add_instruction(new _catch(id));
                }
                stmt_catch->accept(this);
                iid_end_ptr.set(code->get_current_iid());
            }
            else
            {
                code->add_instruction(new _enter());
                ref->get_stmt_try()->accept(this);
            }
            code->add_instruction(new _leave());

            if (stmt_finally)
            {
                iid_finally_ptr.set(code->get_current_iid());
                stmt_finally->accept(this);
                code->add_instruction(new _leave());
            }
        }

        void generator::visit(pt::inheritance *ref)
        {
            ref->get_right()->accept(this);
            code->add_instruction(new _clone(0));
            ref->get_left()->accept(this);
            code->add_instruction(new _inherit());
        }

        void generator::visit(pt::character *ref)
        {
            code->add_instruction(new _char(ref->get_value()));
        }

        void generator::visit(pt::statement_for *ref)
        {
            add_frame(ref);
            _sector *cycle = new _sector(iid_unknown(), iid_unknown());
            code->add_instruction(cycle);
            auto stmt_init = ref->get_stmt_init();
            if (stmt_init)
            {
                stmt_init->accept(this);
            }
            iid_t iid_begin = code->get_current_iid();
            _ifnot *if_not = nullptr;
            auto condition = ref->get_condition();
            if (condition)
            {
                condition->accept(this);
                code->add_instruction(new _bool());
                if_not = new _ifnot(iid_unknown());
                code->add_instruction(if_not);
            }
            ref->get_body()->accept(this);
            cycle->get_begin_ptr().set(code->get_current_iid());
            auto increment = ref->get_increment();
            if (increment)
            {
                increment->accept(this);
            }
            code->add_instruction(new _jmp(iid_begin));
            if (condition)
            {
                if_not->get_iid_ptr().set(code->get_current_iid());
            }
            code->add_instruction(new _leave());
            cycle->get_end_ptr().set(code->get_current_iid());
        }

        void generator::visit(pt::is_less_than *ref)
        {
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
            code->add_instruction(new _less());
        }

        void generator::visit(pt::statement_empty *ref)
        {
#if 0
            code->add_instruction(new _nop());
#endif
        }

        void generator::visit(pt::operator_new *ref)
        {
            int args_count = ref->get_args_count();
            for (int i = args_count - 1; i > -1; i--)
            {
                ref->get_arg(i)->accept(this);
            }
            ref->get_proto()->accept(this);
            code->add_instruction(new _new(args_count));
        }

        void generator::visit(pt::prefix_increment *ref)
        {
            ref->get_right()->accept(this);
            code->add_instruction(new _inc());
            ref->get_right()->accept(lgen.get());
        }

        void generator::visit(pt::statement_lock *ref)
        {
            code->add_instruction(new _lock());
            ref->get_statement()->accept(this);
            code->add_instruction(new _unlock());
        }

        void generator::visit(pt::index_access *ref)
        {
            int args_count = ref->get_args_count();
            for (int i = args_count - 1; i > -1; i--)
            {
                ref->get_arg(i)->accept(this);
            }
            ref->get_object()->accept(this);
            code->add_instruction(new _get(args_count));
        }

        void generator::visit(pt::statement_for_in *ref)
        {
            add_frame(ref);
            _sector *cycle = new _sector(iid_unknown(), iid_unknown());
            code->add_instruction(cycle);
            int name_id = name_cache->get_id(ref->get_name());
            if (ref->is_declared())
            {
                code->add_instruction(new _undef());
                code->add_instruction(new _var(name_id));
            }
            ref->get_expression()->accept(this);
            code->add_instruction(new _iter(0));
            iid_t iid_begin = code->get_current_iid();
            cycle->get_begin_ptr().set(iid_begin);
            code->add_instruction(new _dup());
            code->add_instruction(new _valid(0));
            _ifnot *if_not = new _ifnot(iid_unknown());
            code->add_instruction(if_not);
            code->add_instruction(new _dup());
            code->add_instruction(new _next(0));
            code->add_instruction(new _store(name_id));
            code->add_instruction(new _pop());
            ref->get_body()->accept(this);
            code->add_instruction(new _jmp(iid_begin));
            if_not->get_iid_ptr().set(code->get_current_iid());
            code->add_instruction(new _leave());
            cycle->get_end_ptr().set(code->get_current_iid());
        }

        void generator::visit(pt::statement_do_while *ref)
        {
            _sector *cycle = new _sector(iid_unknown(), iid_unknown());
            code->add_instruction(cycle);
            iid_t iid_begin = code->get_current_iid();
            cycle->get_begin_ptr().set(iid_begin);
            ref->get_statement()->accept(this);
            ref->get_expression()->accept(this);
            code->add_instruction(new _bool());
            code->add_instruction(new _if(iid_begin));
            code->add_instruction(new _leave());
            cycle->get_end_ptr().set(code->get_current_iid());
        }

        void generator::visit(pt::statement_break *ref)
        {
            code->add_instruction(new _break());
        }

        void generator::visit(pt::statement_continue *ref)
        {
            code->add_instruction(new _cont());
        }

        void generator::visit(pt::statement_switch *ref)
        {
            add_frame(ref);
            _sector *sector = new _sector(iid_unknown(), iid_unknown());
            code->add_instruction(sector);
            ref->get_expression()->accept(this);
            int i;
            int blocks_count = ref->get_count();
            iid_ptr_t *pointers = new iid_ptr_t[blocks_count];
            for (i = 0; i < blocks_count; i++)
            {
                auto block = ref->get_block(i);
                code->add_instruction(new _dup());
                block->get_expression()->accept(this);
                code->add_instruction(new _eq);
                code::_if *jmp_if = new _if(iid_unknown());
                pointers[i] = jmp_if->get_iid_ptr();
                code->add_instruction(jmp_if);
            }
            code::_jmp *jmp_if_no_matches = new _jmp(iid_unknown());
            code->add_instruction(jmp_if_no_matches);
            for (int i = 0; i < blocks_count; i++)
            {
                auto block = ref->get_block(i);
                int code_size = block->get_code_size();
                pointers[i].set(code->get_current_iid());
                for (int j = 0; j < code_size; j++)
                {
                    block->get_statement(j)->accept(this);
                }
            }
            jmp_if_no_matches->get_iid_ptr().set(code->get_current_iid());
            if (ref->has_default_block())
            {
                auto block = ref->get_default_block();
                int code_size = block->get_code_size();
                for (int j = 0; j < code_size; j++)
                {
                    block->get_statement(j)->accept(this);
                }
            }
            code->add_instruction(new _leave());
            sector->get_begin_ptr().set(code->get_current_iid());
            sector->get_end_ptr().set(code->get_current_iid());
            delete[] pointers;
        }

        void generator::visit(pt::suffix_increment *ref)
        {
            ref->get_left()->accept(this);
            code->add_instruction(new _dup());
            code->add_instruction(new _inc());
            ref->get_left()->accept(lgen.get());
            code->add_instruction(new _pop());
        }

        void generator::visit(pt::prefix_decrement *ref)
        {
            ref->get_right()->accept(this);
            code->add_instruction(new _dec());
            ref->get_right()->accept(lgen.get());
        }

        void generator::visit(pt::suffix_decrement *ref)
        {
            ref->get_left()->accept(this);
            code->add_instruction(new _dup());
            code->add_instruction(new _dec());
            ref->get_left()->accept(lgen.get());
            code->add_instruction(new _pop());
        }

        void generator::visit(pt::multiplication *ref)
        {
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
            code->add_instruction(new _mul());
        }

        void generator::visit(pt::exponentiation *ref)
        {
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
            code->add_instruction(new _exp());
        }

        void generator::visit(pt::division *ref)
        {
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
            code->add_instruction(new _div());
        }

        void generator::visit(pt::remainder *ref)
        {
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
            code->add_instruction(new _mod());
        }

        void generator::visit(pt::unary_plus *ref)
        {
            ref->get_right()->accept(this);
            code->add_instruction(new _pos());
        }

        void generator::visit(pt::logical_not *ref)
        {
            ref->get_right()->accept(this);
            code->add_instruction(new _not());
        }

        void generator::visit(pt::bitwise_not *ref)
        {
            ref->get_right()->accept(this);
            code->add_instruction(new _inv());
        }

        void generator::visit(pt::operator_bool *ref)
        {
            ref->get_right()->accept(this);
            code->add_instruction(new _bool());
        }

        void generator::visit(pt::left_shift *ref)
        {
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
            code->add_instruction(new _shl());
        }

        void generator::visit(pt::signed_right_shift *ref)
        {
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
            code->add_instruction(new _shr());
        }

        void generator::visit(pt::zero_fill_right_shift *ref)
        {
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
            code->add_instruction(new _shrz());
        }

        void generator::visit(pt::is_less_than_or_equal_to *ref)
        {
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
            code->add_instruction(new _leq());
        }

        void generator::visit(pt::is_greater_than *ref)
        {
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
            code->add_instruction(new _great());
        }

        void generator::visit(pt::is_greater_than_or_equal_to *ref)
        {
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
            code->add_instruction(new _greq());
        }

        void generator::visit(pt::bitwise_and *ref)
        {
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
            code->add_instruction(new _bitand());
        }

        void generator::visit(pt::bitwise_or *ref)
        {
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
            code->add_instruction(new _bitor());
        }

        void generator::visit(pt::bitwise_xor *ref)
        {
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
            code->add_instruction(new _xor());
        }

        void generator::visit(pt::logical_and *ref)
        {
            ref->get_left()->accept(this);
            code->add_instruction(new _dup());
            code->add_instruction(new _bool());
            code::_ifnot *if_false = new _ifnot(iid_unknown());
            code->add_instruction(if_false);
            ref->get_right()->accept(this);
            code->add_instruction(new _swap());
            code->add_instruction(new _and());
            if_false->get_iid_ptr().set(code->get_current_iid());
        }

        void generator::visit(pt::logical_or *ref)
        {
            ref->get_left()->accept(this);
            code->add_instruction(new _dup());
            code->add_instruction(new _bool());
            code::_if *if_true = new code::_if(iid_unknown());
            code->add_instruction(if_true);
            ref->get_right()->accept(this);
            code->add_instruction(new _swap());
            code->add_instruction(new _or());
            if_true->get_iid_ptr().set(code->get_current_iid());
        }

        void generator::visit(pt::assignment_by_sum *ref)
        {
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
            code->add_instruction(new _add());
            ref->get_left()->accept(lgen.get());
        }

        void generator::visit(pt::assignment_by_difference *ref)
        {
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
            code->add_instruction(new _sub());
            ref->get_left()->accept(lgen.get());
        }

        void generator::visit(pt::assignment_by_product *ref)
        {
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
            code->add_instruction(new _mul());
            ref->get_left()->accept(lgen.get());
        }

        void generator::visit(pt::assignment_by_quotient *ref)
        {
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
            code->add_instruction(new _div());
            ref->get_left()->accept(lgen.get());
        }

        void generator::visit(pt::assignment_by_remainder *ref)
        {
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
            code->add_instruction(new _mod());
            ref->get_left()->accept(lgen.get());
        }

        void generator::visit(pt::assignment_by_left_shift *ref)
        {
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
            code->add_instruction(new _shl());
            ref->get_left()->accept(lgen.get());
        }

        void generator::visit(pt::assignment_by_signed_right_shift *ref)
        {
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
            code->add_instruction(new _shr());
            ref->get_left()->accept(lgen.get());
        }

        void generator::visit(pt::assignment_by_zero_fill_right_shift *ref)
        {
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
            code->add_instruction(new _shrz());
            ref->get_left()->accept(lgen.get());
        }

        void generator::visit(pt::assignment_by_bitwise_and *ref)
        {
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
            code->add_instruction(new _bitand());
            ref->get_left()->accept(lgen.get());
        }

        void generator::visit(pt::assignment_by_bitwise_or *ref)
        {
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
            code->add_instruction(new _bitor());
            ref->get_left()->accept(lgen.get());
        }

        void generator::visit(pt::assignment_by_bitwise_xor *ref)
        {
            ref->get_right()->accept(this);
            ref->get_left()->accept(this);
            code->add_instruction(new _xor());
            ref->get_left()->accept(lgen.get());
        }

        void generator::visit(pt::ternary *ref)
        {
            ref->get_condition()->accept(this);
            code->add_instruction(new code::_bool());
            code::_if *if_true = new code::_if(iid_unknown());
            code->add_instruction(if_true);
            ref->get_expr_false()->accept(this);
            code::_jmp *jmp_end = new code::_jmp(iid_unknown());            
            code->add_instruction(jmp_end);
            if_true->get_iid_ptr().set(code->get_current_iid());
            ref->get_expr_true()->accept(this);
            jmp_end->get_iid_ptr().set(code->get_current_iid());
        }

        void generator::visit(pt::statement_debug *ref)
        {
            code->add_instruction(new _debug());
        }

        void generator::visit(pt::protection *ref)
        {
            ref->get_right()->accept(this);
            code->add_instruction(new _clone(0));
            code->add_instruction(new _protect());
        }
    };
};