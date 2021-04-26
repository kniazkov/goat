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

#include "object_char.h"
#include "object_string.h"
#include "object_function_built_in.h"
#include "object_exception.h"
#include "lib/functional.h"
#include "thread.h"
#include "lib/assert.h"
#include "lib/utils.h"
#include "resource/strings.h"
#include <sstream>

namespace g0at
{
    namespace model
    {
        static bool convert_to_char(variable &var, variable *result)
        {
            wchar_t char_value;
            if (var.get_char(&char_value))
            {
                result->set_char(char_value);
                return true;
            }
            int64_t int_value;
            if (var.get_integer(&int_value))
            {
                if (int_value >= 0 && int_value <= WCHAR_MAX)
                {
                    result->set_char((wchar_t)int_value);
                    return true;
                }
                else
                {
                    return false;
                }
            }
            object *obj = var.get_object();
            if (obj)
            {
                object_string *obj_string = obj->to_object_string();
                if (obj_string)
                {
                    std::wstring string_value = obj_string->get_data();
                    if (string_value.size() == 1)
                    {
                        result->set_char(string_value[0]);
                        return true;
                    }
                }
            }
            return false;
        }

        object_char::object_char(object_pool *pool, wchar_t value)
            : variable_wrapper(pool, pool->get_char_proto_instance())
        {
            var.set_char(value);
        }

        void object_char::reinit(wchar_t value)
        {
            var.set_char(value);
        }

#ifndef GC_DEBUG
        void object_char::kill(object_pool *pool)
        {
            if (pool->chars.destroy_or_cache(this, pool))
            {
                objects.clear();
                immutable = false;
            }
        }
#endif

        object_type object_char::get_type() const
        {
            return object_type::charact;
        }

        object_char *object_char::to_object_char()
        {
            return this;
        }

        bool object_char::less(const object *obj) const
        {
            assert(obj->get_type() == object_type::charact);
            const object_char *obj_char = static_cast<const object_char*>(obj);
            return var.data.c < obj_char->var.data.c;
        }

        void object_char::m_iterator(thread *thr, int arg_count)
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

        object_char_proto::object_char_proto(object_pool *pool)
            : object(pool)
        {
        }

        class object_char_valueof : public object_function_built_in
        {
        public:
            object_char_valueof(object_pool *_pool)
                : object_function_built_in(_pool)
            {
            }

            void call(thread *thr, int arg_count, call_mode mode) override
            {
                if (arg_count > 0)
                {
                    if (mode == call_mode::as_method)
                        thr->pop();
                    variable arg = thr->peek();
                    thr->pop(arg_count);
                    variable result;
                    if (!convert_to_char(arg, &result))
                    {
                        result.set_object(thr->pool->get_null_instance());
                    }
                    thr->push(result);
                    return;
                }
                thr->raise_exception(new object_exception_illegal_argument(thr->pool));
            }
        };

        void object_char_proto::init(object_pool *pool)
        {
            add_object(pool->get_static_string(resource::str_valueof), new object_char_valueof(pool));
            add_object(pool->get_static_string(resource::str_oper_plus_plus), pool->get_wrap_inc_instance());
            add_object(pool->get_static_string(resource::str_oper_minus_minus), pool->get_wrap_dec_instance());
            add_object(pool->get_static_string(resource::str_oper_exclamation), pool->get_wrap_not_instance());
            add_object(pool->get_static_string(resource::str_oper_double_exclamation), pool->get_wrap_bool_instance());
            add_object(pool->get_static_string(resource::str_oper_less), pool->get_wrap_less_instance());
            add_object(pool->get_static_string(resource::str_oper_less_equal), pool->get_wrap_leq_instance());
            add_object(pool->get_static_string(resource::str_oper_greater), pool->get_wrap_great_instance());
            add_object(pool->get_static_string(resource::str_oper_greater_equal), pool->get_wrap_greq_instance());
            add_object(pool->get_static_string(resource::str_oper_double_ampersand), pool->get_wrap_and_instance());
            add_object(pool->get_static_string(resource::str_oper_double_vertical_bar), pool->get_wrap_or_instance());
            lock();
        }

        void object_char_proto::op_new(thread *thr, int arg_count)
        {
            variable result;
            if (arg_count > 0)
            {
                variable arg = thr->peek();
                thr->pop(arg_count);
                if (!convert_to_char(arg, &result))
                {
                    result.set_object(thr->pool->get_null_instance());
                }
            }
            else
            {
                result.set_integer(0);
            }
            thr->push(result);
        }

        /*
            Primitive handler
        */

        class char_handler : public handler
        {
        public:
            static handler *get_instance()
            {
                static char_handler instance;
                return &instance;
            }

            std::wstring to_string(const variable *var) const override
            {
                wchar_t tmp[] = { var->data.c, 0 };
                return std::wstring(tmp);
            }

            std::wstring to_string_notation(const variable *var) const override
            {
                std::wstringstream wss;
                wss << L'\'' << lib::escape_special_chars(&var->data.c, 1) << L'\'';
                return wss.str();
            }

            object *to_object(variable *var, object_pool *pool) override
            {
                object *obj = pool->create_object_char(var->data.c);
                var->set_object(obj);
                return obj;
            }

            bool get_char(variable *var, wchar_t *pval) override
            {
                *pval = var->data.c;
                return true;
            }

            goat_value * get_value(variable *var, const goat_allocator *allocator) override
            {
                return create_goat_char(allocator, var->data.c);
            }

            void op_inc(variable *var, thread *thr)  override
            {
                unary_operation<lib::func::inc>(var, thr);
            }

            void op_dec(variable *var, thread *thr)  override
            {
                unary_operation<lib::func::dec>(var, thr);
            }

            void op_not(variable *var, thread *thr)  override
            {
                thr->pop();
                variable result;
                result.set_boolean(false);
                thr->push(result);
            }

            void op_bool(variable *var, thread *thr)  override
            {
                thr->pop();
                variable result;
                result.set_boolean(true);
                thr->push(result);
            }

            void op_eq(variable *var, thread *thr)  override
            {
                binary_logical_operation<lib::func::equals, false>(var, thr);
            }

            void op_neq(variable *var, thread *thr)  override
            {
                binary_logical_operation<lib::func::not_equal, true>(var, thr);
            }

            void op_less(variable *var, thread *thr)  override
            {
                binary_logical_operation<lib::func::less, false>(var, thr);
            }

            void op_leq(variable *var, thread *thr)  override
            {
                binary_logical_operation<lib::func::less_or_equal, false>(var, thr);
            }

            void op_great(variable *var, thread *thr)  override
            {
                binary_logical_operation<lib::func::greater, false>(var, thr);
            }

            void op_greq(variable *var, thread *thr)  override
            {
                binary_logical_operation<lib::func::greater_or_equal, false>(var, thr);
            }

            void op_and(variable *var, thread *thr)  override
            {
                thr->pop();
            }

            void op_or(variable *var, thread *thr)  override
            {
                thr->pop();
                thr->pop();
                thr->push(*var);
            }

            void m_instance_of(variable *var, thread *thr, int arg_count) override
            {
                thr->pop();
                object *base = thr->peek().get_object();
                thr->pop(arg_count);
                variable result;
                if (base)
                    result.set_boolean(base == thr->pool->get_char_proto_instance() || base == thr->pool->get_generic_proto_instance());
                else
                    result.set_boolean(false);
                thr->push(result);
            }

        protected:
            char_handler()
            {
            }

            template <template<typename R, typename A> class F> void unary_operation(variable *var, thread *thr)
            {
                thr->pop();
                variable result;
                result.set_char(F<wchar_t, wchar_t>::calculate(var->data.c));
                thr->push(result);
            }

            template <template<typename R, typename X, typename Y> class F, bool Def> void binary_logical_operation(variable *var, thread *thr)
            {
                thr->pop();
                variable right = thr->pop();
                wchar_t right_value;
                bool right_is_char = right.get_char(&right_value);
                variable result;
                if(right_is_char)
                {
                    result.set_boolean(F<bool, wchar_t, wchar_t>::calculate(var->data.c, right_value));
                }
                else
                {
                    result.set_boolean(Def);
                }
                thr->push(result);
            }
        };

        handler *handler::get_instance_char()
        {
            return char_handler::get_instance();
        }
    };
};
