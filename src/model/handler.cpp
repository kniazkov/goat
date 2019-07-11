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

        void handler::op_add(variable *var, thread *thr)
        {
            thr->raise_exception(new object_exception_operator_not_found(thr->pool, resource::str_oper_plus));
        }

        void handler::op_sub(variable *var, thread *thr)
        {
            assert(false);
        }

        void handler::op_neg(variable *var, thread *thr)
        {
            assert(false);
        }

        void handler::op_inc(variable *var, thread *thr)
        {
            thr->raise_exception(new object_exception_operator_not_found(thr->pool, resource::str_oper_plus_plus));
        }

        void handler::op_eq(variable *var, thread *thr)
        {
            assert(false);
        }

        void handler::op_neq(variable *var, thread *thr)
        {
            assert(false);
        }

        void handler::op_less(variable *var, thread *thr)
        {
            assert(false);
        }

        void handler::op_inherit(variable *var, thread *thr)
        {
            assert(false);
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
