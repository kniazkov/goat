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

#include "object_boolean.h"
#include "object_string.h"
#include "object_function_built_in.h"
#include "object_exception.h"
#include "resource/strings.h"
#include "lib/functional.h"
#include "thread.h"
#include "lib/assert.h"

namespace g0at
{
    namespace model
    {
        object_boolean::object_boolean(object_pool *pool, bool value)
            : variable_wrapper(pool, pool->get_boolean_proto_instance())
        {
            var.set_boolean(value);
        }

        void object_boolean::reinit(bool value)
        {
            var.set_boolean(value);
        }

#ifndef GC_DEBUG
        void object_boolean::kill(object_pool *pool)
        {
            if (pool->booleans.destroy_or_cache(this, pool))
            {
                objects.clear();
                immutable = false;
            }
        }
#endif

        object_type object_boolean::get_type() const
        {
            return object_type::boolean;
        }

        object_boolean *object_boolean::to_object_boolean()
        {
            return this;
        }

        bool object_boolean::less(const object *obj) const
        {
            assert(obj->get_type() == object_type::boolean);
            const object_boolean *obj_bool = static_cast<const object_boolean*>(obj);
            return (var.data.b ? 0 : 1) < (obj_bool->var.data.b ? 1 : 0);
        }

        void object_boolean::m_iterator(thread *thr, int arg_count)
        {
            thr->pop();
            thr->pop(arg_count);
            variable tmp;
            tmp.set_object(thr->pool->get_iterator_proto_instance());
            thr->push(tmp);
        }

        /*
            Prototype
        */

        object_boolean_proto::object_boolean_proto(object_pool *pool)
            : object(pool)
        {
        }

        class object_boolean_valueof : public object_function_built_in
        {
        public:
            object_boolean_valueof(object_pool *_pool)
                : object_function_built_in(_pool)
            {
            }
            
            void call(thread *thr, int arg_count, call_mode mode) override
            {
                if (mode == call_mode::as_method)
                    thr->pop();
                if (arg_count > 1)
                {
                    variable arg = thr->peek();
                    thr->pop(arg_count);
                    thr->push(arg);
                    arg.op_bool(thr);
                }
                else if (arg_count == 1)
                {
                    variable arg = thr->peek();
                    arg.op_bool(thr);
                }
                else
                {
                    thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                }
            }
        };

        void object_boolean_proto::init(object_pool *pool)
        {
            add_object(pool->get_static_string(resource::str_valueof), new object_boolean_valueof(pool));
            add_object(pool->get_static_string(resource::str_oper_exclamation), pool->get_wrap_not_instance());
            add_object(pool->get_static_string(resource::str_oper_double_exclamation), pool->get_wrap_bool_instance());
            add_object(pool->get_static_string(resource::str_oper_double_ampersand), pool->get_wrap_and_instance());
            add_object(pool->get_static_string(resource::str_oper_double_vertical_bar), pool->get_wrap_or_instance());
            lock();
        }

        void object_boolean_proto::op_new(thread *thr, int arg_count)
        {
            if (arg_count > 1)
            {
                variable arg = thr->peek();
                thr->pop(arg_count);
                thr->push(arg);
                arg.op_bool(thr);
            }
            else if (arg_count == 1)
            {
                variable arg = thr->peek();
                arg.op_bool(thr);
            }
            else
            {
                variable result;
                result.set_boolean(false);
                thr->push(result);
            }
        }

        /*
            Primitive handler
        */

        class boolean_handler : public handler
        {
        public:
            static handler *get_instance()
            {
                static boolean_handler instance;
                return &instance;
            }

            std::wstring to_string(const variable *var) const override
            {
                return var->data.b ? resource::str_true : resource::str_false;
            }

            std::wstring to_string_notation(const variable *var) const override
            {
                return var->data.b ? resource::str_true : resource::str_false;
            }

            object *to_object(variable *var, object_pool *pool) override
            {
                object *obj = pool->create_object_boolean(var->data.b);
                var->set_object(obj);
                return obj;
            }

            bool get_boolean(variable *var, bool *pval) override
            {
                *pval = var->data.b;
                return true;
            }

            goat_value * get_value(variable *var, goat_ext_environment *env) override
            {
                return create_goat_boolean(env, var->data.b);
            }
            
            void op_not(variable *var, thread *thr)  override
            {
                unary_operation<lib::func::log_not>(var, thr);
            }

            void op_bool(variable *var, thread *thr)  override
            {
                thr->pop();
                thr->push(*var);
            }

            void op_eq(variable *var, thread *thr)  override
            {
                binary_operation<lib::func::equals, false>(var, thr);
            }

            void op_neq(variable *var, thread *thr)  override
            {
                binary_operation<lib::func::not_equal, true>(var, thr);
            }

            void op_and(variable *var, thread *thr)  override
            {
                thr->pop();
                if (!var->data.b)
                {
                    thr->pop();
                    thr->push(*var);
                }
            }

            void op_or(variable *var, thread *thr)  override
            {
                thr->pop();
                if (var->data.b)
                {
                    thr->pop();
                    thr->push(*var);
                }
            }

            void m_instance_of(variable *var, thread *thr, int arg_count) override
            {
                thr->pop();
                object *base = thr->peek().get_object();
                thr->pop(arg_count);
                variable result;
                if (base)
                    result.set_boolean(base == thr->pool->get_boolean_proto_instance()
                        || base == thr->pool->get_generic_proto_instance());
                else
                    result.set_boolean(false);
                thr->push(result);
            }

        protected:
            boolean_handler()
            {
            }

            template <template<typename R, typename A> class F> void unary_operation(variable *var, thread *thr)
            {
                thr->pop();
                variable result;
                result.set_boolean(F<bool, bool>::calculate(var->data.b));
                thr->push(result);
            }

            template <template<typename R, typename X, typename Y> class F, bool Def> void binary_operation(variable *var, thread *thr)
            {
                thr->pop();
                variable right = thr->pop();
                bool right_value;
                bool right_is_boolean = right.get_boolean(&right_value);
                variable result;
                if (right_is_boolean)
                {
                    result.set_boolean(F<bool, bool, bool>::calculate(var->data.b, right_value));
                }
                else
                {
                    result.set_boolean(Def);
                }
                thr->push(result);
            }
        };

        handler *handler::get_instance_boolean()
        {
            return boolean_handler::get_instance();
        }
    };
};
