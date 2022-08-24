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

#include "lvalue_generator.h"
#include "generator.h"
#include "compiler/pt/variable.h"
#include "compiler/pt/property.h"
#include "compiler/pt/index_access.h"
#include "code/store.h"
#include "code/write.h"
#include "code/set.h"

namespace g0at
{
    namespace codegen
    {
        lvalue_generator::lvalue_generator(lib::pointer<code::code> _code, model::name_cache *_name_cache, generator *_rgen)
            : code(_code), name_cache(_name_cache), rgen(_rgen)
        {
        }

        void lvalue_generator::visit(pt::variable *ref)
        {
            int id = name_cache->get_id(ref->get_name());
            code->add_instruction(new code::_store(id));
        }

        void lvalue_generator::visit(pt::property *ref)
        {
            ref->get_left()->accept(rgen);
            int id = name_cache->get_id(ref->get_name());
            code->add_instruction(new code::_write(id));
        }

        void lvalue_generator::visit(pt::index_access *ref)
        {
            int args_count = ref->get_args_count();
            for (int i = args_count - 1; i > -1; i--)
            {
                ref->get_arg(i)->accept(rgen);
            }
            ref->get_object()->accept(rgen);
            code->add_instruction(new code::_set(args_count + 1));
        }

        void lvalue_generator::visit(pt::function *ref)
        {
        }

        void lvalue_generator::visit(pt::static_string *ref)
        {
        }

        void lvalue_generator::visit(pt::function_call *ref)
        {
        }

        void lvalue_generator::visit(pt::statement_expression *ref)
        {
        }

        void lvalue_generator::visit(pt::addition *ref)
        {
        }

        void lvalue_generator::visit(pt::integer *ref)
        {
        }

        void lvalue_generator::visit(pt::subtraction *ref)
        {
        }

        void lvalue_generator::visit(pt::negation *ref)
        {
        }

        void lvalue_generator::visit(pt::value_void *ref)
        {
        }

        void lvalue_generator::visit(pt::value_undefined *ref)
        {
        }

        void lvalue_generator::visit(pt::value_null *ref)
        {
        }

        void lvalue_generator::visit(pt::declare_variable *ref)
        {
        }

        void lvalue_generator::visit(pt::assignment *ref)
        {
        }

        void lvalue_generator::visit(pt::real *ref)
        {
        }

        void lvalue_generator::visit(pt::declare_function *ref)
        {
        }

        void lvalue_generator::visit(pt::statement_return *ref)
        {
        }

        void lvalue_generator::visit(pt::node_object *ref)
        {
        }

        void lvalue_generator::visit(pt::value_true *ref)
        {
        }

        void lvalue_generator::visit(pt::value_false *ref)
        {
        }

        void lvalue_generator::visit(pt::is_equal_to *ref)
        {
        }

        void lvalue_generator::visit(pt::is_not_equal_to *ref)
        {
        }

        void lvalue_generator::visit(pt::statement_while *ref)
        {
        }

        void lvalue_generator::visit(pt::method_call *ref)
        {
        }

        void lvalue_generator::visit(pt::this_ptr *ref)
        {
        }

        void lvalue_generator::visit(pt::node_array *ref)
        {
        }

        void lvalue_generator::visit(pt::statement_block *ref)
        {
        }

        void lvalue_generator::visit(pt::statement_if *ref)
        {
        }

        void lvalue_generator::visit(pt::statement_throw *ref)
        {
        }

        void lvalue_generator::visit(pt::statement_try *ref)
        {
        }

        void lvalue_generator::visit(pt::inheritance *ref)
        {
        }

        void lvalue_generator::visit(pt::character *ref)
        {
        }

        void lvalue_generator::visit(pt::statement_for *ref)
        {
        }

        void lvalue_generator::visit(pt::is_less_than *ref)
        {
        }

        void lvalue_generator::visit(pt::statement_empty *ref)
        {
        }

        void lvalue_generator::visit(pt::operator_new *ref)
        {;
        }

        void lvalue_generator::visit(pt::prefix_increment *ref)
        {
        }

        void lvalue_generator::visit(pt::statement_lock *ref)
        {
        }

        void lvalue_generator::visit(pt::statement_for_in *ref)
        {
        }

        void lvalue_generator::visit(pt::statement_do_while *ref)
        {
        }

        void lvalue_generator::visit(pt::statement_break *ref)
        {;
        }

        void lvalue_generator::visit(pt::statement_continue *ref)
        {
        }

        void lvalue_generator::visit(pt::statement_switch *ref)
        {
        }

        void lvalue_generator::visit(pt::suffix_increment *ref)
        {
        }

        void lvalue_generator::visit(pt::prefix_decrement *ref)
        {
        }

        void lvalue_generator::visit(pt::suffix_decrement *ref)
        {
        }

        void lvalue_generator::visit(pt::multiplication *ref)
        {
        }

        void lvalue_generator::visit(pt::exponentiation *ref)
        {
        }

        void lvalue_generator::visit(pt::division *ref)
        {
        }

        void lvalue_generator::visit(pt::remainder *ref)
        {
        }

        void lvalue_generator::visit(pt::unary_plus *ref)
        {
        }

        void lvalue_generator::visit(pt::logical_not *ref)
        {
        }

        void lvalue_generator::visit(pt::bitwise_not *ref)
        {
        }

        void lvalue_generator::visit(pt::operator_bool *ref)
        {
        }

        void lvalue_generator::visit(pt::left_shift *ref)
        {
        }

        void lvalue_generator::visit(pt::signed_right_shift *ref)
        {
        }

        void lvalue_generator::visit(pt::zero_fill_right_shift *ref)
        {
        }

        void lvalue_generator::visit(pt::is_less_than_or_equal_to *ref)
        {
        }

        void lvalue_generator::visit(pt::is_greater_than *ref)
        {
        }

        void lvalue_generator::visit(pt::is_greater_than_or_equal_to *ref)
        {
        }

        void lvalue_generator::visit(pt::bitwise_and *ref)
        {
        }

        void lvalue_generator::visit(pt::bitwise_or *ref)
        {
        }

        void lvalue_generator::visit(pt::bitwise_xor *ref)
        {
        }

        void lvalue_generator::visit(pt::logical_and *ref)
        {
        }

        void lvalue_generator::visit(pt::logical_or *ref)
        {
        }

        void lvalue_generator::visit(pt::assignment_by_sum *ref)
        {
        }

        void lvalue_generator::visit(pt::assignment_by_difference *ref)
        {
        }

        void lvalue_generator::visit(pt::assignment_by_product *ref)
        {
        }

        void lvalue_generator::visit(pt::assignment_by_quotient *ref)
        {
        }

        void lvalue_generator::visit(pt::assignment_by_remainder *ref)
        {
        }

        void lvalue_generator::visit(pt::assignment_by_left_shift *ref)
        {
        }

        void lvalue_generator::visit(pt::assignment_by_signed_right_shift *ref)
        {
        }

        void lvalue_generator::visit(pt::assignment_by_zero_fill_right_shift *ref)
        {
        }

        void lvalue_generator::visit(pt::assignment_by_bitwise_and *ref)
        {
        }

        void lvalue_generator::visit(pt::assignment_by_bitwise_or *ref)
        {
        }

        void lvalue_generator::visit(pt::assignment_by_bitwise_xor *ref)
        {
        }

        void lvalue_generator::visit(pt::ternary *ref)
        {
        }

        void lvalue_generator::visit(pt::statement_debug *ref)
        {
        }

        void lvalue_generator::visit(pt::protection *ref)
        {
        }
    };
};