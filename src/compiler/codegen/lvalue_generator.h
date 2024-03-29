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

#pragma once

#include "compiler/pt/node_visitor.h"
#include "code/code.h"
#include "model/name_cache.h"
#include "lib/pointer.h"
#include "lib/ref_counter.h"

namespace g0at
{
    namespace codegen
    {
        class generator;

        class lvalue_generator : public pt::node_visitor, public lib::ref_counter
        {
        public:
            lvalue_generator(lib::pointer<code::code> _code, model::name_cache *_name_cache, generator *_rgen);
            void visit(pt::variable *ref) override;
            void visit(pt::property *ref) override;
            void visit(pt::index_access *ref) override;

            void visit(pt::function *ref) override;
            void visit(pt::static_string *ref) override;
            void visit(pt::function_call *ref) override;
            void visit(pt::statement_expression *ref) override;
            void visit(pt::addition *ref) override;
            void visit(pt::integer *ref) override;
            void visit(pt::subtraction *ref) override;
            void visit(pt::negation *ref) override;
            void visit(pt::value_void *ref) override;
            void visit(pt::value_undefined *ref) override;
            void visit(pt::value_null *ref) override;
            void visit(pt::declare_variable *ref) override;
            void visit(pt::assignment *ref) override;
            void visit(pt::real *ref) override;
            void visit(pt::declare_function *ref) override;
            void visit(pt::statement_return *ref) override;
            void visit(pt::node_object *ref) override;
            void visit(pt::value_true *ref) override;
            void visit(pt::value_false *ref) override;
            void visit(pt::is_equal_to *ref) override;
            void visit(pt::is_not_equal_to *ref) override;
            void visit(pt::statement_while *ref) override;
            void visit(pt::method_call *ref) override;
            void visit(pt::this_ptr *ref) override;
            void visit(pt::node_array *ref) override;
            void visit(pt::statement_block *ref) override;
            void visit(pt::statement_if *ref) override;
            void visit(pt::statement_throw *ref) override;
            void visit(pt::statement_try *ref) override;
            void visit(pt::inheritance *ref) override;
            void visit(pt::character *ref) override;
            void visit(pt::statement_for *ref) override;
            void visit(pt::is_less_than *ref) override;
            void visit(pt::statement_empty *ref) override;
            void visit(pt::operator_new *ref) override;
            void visit(pt::prefix_increment *ref) override;
            void visit(pt::statement_lock *ref) override;
            void visit(pt::statement_for_in *ref) override;
            void visit(pt::statement_do_while *ref) override;
            void visit(pt::statement_break *ref) override;
            void visit(pt::statement_continue *ref) override;
            void visit(pt::statement_switch *ref) override;
            void visit(pt::suffix_increment *ref) override;
            void visit(pt::prefix_decrement *ref) override;
            void visit(pt::suffix_decrement *ref) override;
            void visit(pt::multiplication *ref) override;
            void visit(pt::exponentiation *ref) override;
            void visit(pt::division *ref) override;
            void visit(pt::remainder *ref) override;
            void visit(pt::unary_plus *ref) override;
            void visit(pt::logical_not *ref) override;
            void visit(pt::bitwise_not *ref) override;
            void visit(pt::operator_bool *ref) override;
            void visit(pt::left_shift *ref) override;
            void visit(pt::signed_right_shift *ref) override;
            void visit(pt::zero_fill_right_shift *ref) override;
            void visit(pt::is_less_than_or_equal_to *ref) override;
            void visit(pt::is_greater_than *ref) override;
            void visit(pt::is_greater_than_or_equal_to *ref) override;
            void visit(pt::bitwise_and *ref) override;
            void visit(pt::bitwise_or *ref) override;
            void visit(pt::bitwise_xor *ref) override;
            void visit(pt::logical_and *ref) override;
            void visit(pt::logical_or *ref) override;
            void visit(pt::assignment_by_sum *ref) override;
            void visit(pt::assignment_by_difference *ref) override;
            void visit(pt::assignment_by_product *ref) override;
            void visit(pt::assignment_by_quotient *ref) override;
            void visit(pt::assignment_by_remainder *ref) override;
            void visit(pt::assignment_by_left_shift *ref) override;
            void visit(pt::assignment_by_signed_right_shift *ref) override;
            void visit(pt::assignment_by_zero_fill_right_shift *ref) override;
            void visit(pt::assignment_by_bitwise_and *ref) override;
            void visit(pt::assignment_by_bitwise_or *ref) override;
            void visit(pt::assignment_by_bitwise_xor *ref) override;
            void visit(pt::ternary *ref) override;
            void visit(pt::statement_debug *ref) override;
            void visit(pt::protection *ref) override;
        protected:
            lib::pointer<code::code> code; 
            model::name_cache *name_cache;
            generator *rgen;
        };
    };
};
