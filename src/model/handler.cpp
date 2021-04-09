/*

Copyright (C) 2017-2021 Ivan Kniazkov

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

#include "object.h"
#include "object_pool.h"
#include "object_exception.h"
#include "thread.h"
#include "lib/assert.h"
#include "resource/strings.h"

namespace g0at
{
    namespace model
    {
        handler::~handler()
        {
        }

        variable handler::deref(variable *var)
        {
            return *var;
        }

        object* handler::get_object(variable *var)
        {
            return nullptr;
        }

        bool handler::get_integer(variable *var, int64_t *pval)
        {
            return false;
        }

        bool handler::get_real(variable *var, double *pval)
        {
            return false;
        }

        bool handler::get_boolean(variable *var, bool *pval)
        {
            return false;
        }

        bool handler::get_char(variable *var, wchar_t *pval)
        {
            return false;
        }

        goat_value * handler::get_value(variable *var, goat_ext_environment *env)
        {
            return create_goat_unknown_value(env);
        }
            
        void handler::op_add(variable *var, thread *thr)
        {
            thr->raise_exception(new object_exception_operator_not_found(thr->pool, resource::str_oper_plus));
        }

        void handler::op_sub(variable *var, thread *thr)
        {
            thr->raise_exception(new object_exception_operator_not_found(thr->pool, resource::str_oper_minus));
        }

        void handler::op_pos(variable *var, thread *thr)
        {
            thr->raise_exception(new object_exception_operator_not_found(thr->pool, resource::str_oper_plus));
        }

        void handler::op_neg(variable *var, thread *thr)
        {
            thr->raise_exception(new object_exception_operator_not_found(thr->pool, resource::str_oper_minus));
        }

        void handler::op_inc(variable *var, thread *thr)
        {
            thr->raise_exception(new object_exception_operator_not_found(thr->pool, resource::str_oper_plus_plus));
        }

        void handler::op_dec(variable *var, thread *thr)
        {
            thr->raise_exception(new object_exception_operator_not_found(thr->pool, resource::str_oper_minus_minus));
        }

        void handler::op_not(variable *var, thread *thr)
        {
            thr->raise_exception(new object_exception_operator_not_found(thr->pool, resource::str_oper_exclamation));
        }

        void handler::op_bool(variable *var, thread *thr)
        {
            thr->raise_exception(new object_exception_operator_not_found(thr->pool, resource::str_oper_double_exclamation));
        }

        void handler::op_inv(variable *var, thread *thr)
        {
            thr->raise_exception(new object_exception_operator_not_found(thr->pool, resource::str_oper_tilde));
        }

        void handler::op_mul(variable *var, thread *thr)
        {
            thr->raise_exception(new object_exception_operator_not_found(thr->pool, resource::str_oper_asterisk));
        }

        void handler::op_exp(variable *var, thread *thr)
        {
            thr->raise_exception(new object_exception_operator_not_found(thr->pool, resource::str_oper_double_asterisk));
        }

        void handler::op_div(variable *var, thread *thr)
        {
            thr->raise_exception(new object_exception_operator_not_found(thr->pool, resource::str_oper_slash));
        }

        void handler::op_mod(variable *var, thread *thr)
        {
            thr->raise_exception(new object_exception_operator_not_found(thr->pool, resource::str_oper_percent));
        }

        void handler::op_eq(variable *var, thread *thr)
        {
            thr->raise_exception(new object_exception_operator_not_found(thr->pool, resource::str_oper_equals));
        }

        void handler::op_neq(variable *var, thread *thr)
        {
            thr->raise_exception(new object_exception_operator_not_found(thr->pool, resource::str_oper_not_equal));
       }

        void handler::op_shl(variable *var, thread *thr)
        {
            thr->raise_exception(new object_exception_operator_not_found(thr->pool, resource::str_oper_double_less));
        }

        void handler::op_shr(variable *var, thread *thr)
        {
            thr->raise_exception(new object_exception_operator_not_found(thr->pool, resource::str_oper_double_greater));
        }

        void handler::op_shrz(variable *var, thread *thr)
        {
            thr->raise_exception(new object_exception_operator_not_found(thr->pool, resource::str_oper_triple_greater));
        }

        void handler::op_less(variable *var, thread *thr)
        {
            thr->raise_exception(new object_exception_operator_not_found(thr->pool, resource::str_oper_less));
        }

        void handler::op_leq(variable *var, thread *thr)
        {
            thr->raise_exception(new object_exception_operator_not_found(thr->pool, resource::str_oper_less_equal));
        }

        void handler::op_great(variable *var, thread *thr)
        {
            thr->raise_exception(new object_exception_operator_not_found(thr->pool, resource::str_oper_greater));
        }

        void handler::op_greq(variable *var, thread *thr)
        {
            thr->raise_exception(new object_exception_operator_not_found(thr->pool, resource::str_oper_greater_equal));
        }

        void handler::op_inherit(variable *var, thread *thr)
        {
            thr->raise_exception(new object_exception_operator_not_found(thr->pool, resource::str_oper_inherit));
        }

        void handler::op_and(variable *var, thread *thr)
        {
            thr->raise_exception(new object_exception_operator_not_found(thr->pool, resource::str_oper_double_ampersand));
        }

        void handler::op_or(variable *var, thread *thr)
        {
            thr->raise_exception(new object_exception_operator_not_found(thr->pool, resource::str_oper_double_vertical_bar));
        }

        void handler::op_bitand(variable *var, thread *thr)
        {
            thr->raise_exception(new object_exception_operator_not_found(thr->pool, resource::str_oper_ampersand));
        }

        void handler::op_bitor(variable *var, thread *thr)
        {
            thr->raise_exception(new object_exception_operator_not_found(thr->pool, resource::str_oper_vertical_bar));
        }

        void handler::op_xor(variable *var, thread *thr)
        {
            thr->raise_exception(new object_exception_operator_not_found(thr->pool, resource::str_oper_caret));
        }

        void handler::op_protect(variable *var, thread *thr)
        {
            thr->raise_exception(new object_exception_operator_not_found(thr->pool, resource::str_oper_protect));
        }

        void handler::m_clone(variable *var, thread *thr, int arg_count)
        {
            // base handler just returns the object itself, so, primitives are not cloneable
            variable tmp = thr->pop();
            thr->pop(arg_count);
            thr->push(tmp);
        }

        void handler::m_instance_of(variable *var, thread *thr, int arg_count)
        {
            thr->pop();
            thr->pop(arg_count);
            variable tmp;
            tmp.set_boolean(false);
            thr->push(tmp);
        }

        void handler::m_flat(variable *var, thread *thr, int arg_count)
        {
            object *this_ptr = thr->pop().to_object(thr->pool);
            thr->pop(arg_count);

            object *flat = thr->pool->create_generic_object();
            this_ptr->flat(flat);

            variable tmp;
            tmp.set_object(flat);
            thr->push(tmp);
        }

        void handler::m_iterator(variable *var, thread *thr, int arg_count)
        {
            // all primitives returns 'empty' iterator
            thr->pop();
            thr->pop(arg_count);
            variable tmp;
            tmp.set_object(thr->pool->get_iterator_proto_instance());
            thr->push(tmp);
        }

        void handler::m_next(variable *var, thread *thr, int arg_count)
        {
            thr->pop();
            thr->pop(arg_count);
            thr->push_undefined();
        }

        void handler::m_valid(variable *var, thread *thr, int arg_count)
        {
            thr->pop();
            thr->pop(arg_count);
            thr->push_undefined();
        }
    };
};
