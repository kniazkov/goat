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
#include "lib/assert.h"

namespace g0at
{
    namespace model
    {
        class generic_handler : public handler
        {
        protected:
            generic_handler()
            {
            }

        public:
            static handler *get_instance()
            {
                static generic_handler instance;
                return &instance;
            }

            std::wstring to_string(const variable *var) const override
            {
                return var->data.obj->to_string();
            }

            std::wstring to_string_notation(const variable *var) const override
            {
                return var->data.obj->to_string_notation();
            }

            object *to_object(variable *var, object_pool *pool) override
            {
                return var->data.obj;
            }
            
            object *get_object(variable *var) override
            {
                return var->data.obj;
            }

            bool get_integer(variable *var, int64_t *pval) override
            {
                return var->data.obj->get_integer(pval);
            }

            bool get_real(variable *var, double *pval) override
            {
                return var->data.obj->get_real(pval);
            }

            bool get_boolean(variable *var, bool *pval) override
            {
                return var->data.obj->get_boolean(pval);
            }

            bool get_char(variable *var, wchar_t *pval) override
            {
                return var->data.obj->get_char(pval);
            }
            
            void op_add(variable *var, thread *thr)  override
            {
                var->data.obj->op_add(thr);
            }

            void op_sub(variable *var, thread *thr)  override
            {
                var->data.obj->op_sub(thr);
            }

            void op_neg(variable *var, thread *thr)  override
            {
                var->data.obj->op_neg(thr);
            }

            void op_inc(variable *var, thread *thr)  override
            {
                var->data.obj->op_inc(thr);
            }

            void op_dec(variable *var, thread *thr)  override
            {
                var->data.obj->op_dec(thr);
            }

            void op_eq(variable *var, thread *thr) override
            {
                var->data.obj->op_eq(thr);
            }

            void op_neq(variable *var, thread *thr) override
            {
                var->data.obj->op_neq(thr);
            }

            void op_less(variable *var, thread *thr) override
            {
                var->data.obj->op_less(thr);
            }

            void op_inherit(variable *var, thread *thr) override
            {
                var->data.obj->op_inherit(thr);
            }

            void m_clone(variable *var, thread *thr, int arg_count) override
            {
                var->data.obj->m_clone(thr, arg_count);
            }

            void m_instance_of(variable *var, thread *thr, int arg_count) override
            {
                var->data.obj->m_instance_of(thr, arg_count);
            }

            void m_flat(variable *var, thread *thr, int arg_count) override
            {
                var->data.obj->m_flat(thr, arg_count);
            }

            void m_iterator(variable *var, thread *thr, int arg_count) override
            {
                var->data.obj->m_iterator(thr, arg_count);
            }

            void m_next(variable *var, thread *thr, int arg_count) override
            {
                var->data.obj->m_next(thr, arg_count);
            }

            void m_valid(variable *var, thread *thr, int arg_count) override
            {
                var->data.obj->m_valid(thr, arg_count);
            }
        };

        handler *handler::get_instance_generic()
        {
            return generic_handler::get_instance();
        }
    };
};
