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

#include "object_char.h"
#include "object_string.h"
#include "object_function_built_in.h"
#include "lib/functional.h"
#include "thread.h"
#include "lib/assert.h"
#include "lib/utils.h"
#include <sstream>

namespace g0at
{
    namespace model
    {
        object_char::object_char(object_pool *pool, wchar_t _value)
            : object(pool, pool->get_char_proto_instance()), value(_value)
        {
        }

        void object_char::reinit(wchar_t _value)
        {
            value = _value;
        }

        void object_char::kill(object_pool *pool)
        {
            if (pool->chars.destroy_or_cache(this, pool))
            {
                objects.clear();
            }
        }

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
            return value < obj_char->value;
        }

        std::wstring object_char::to_string() const
        {
            wchar_t tmp[] = { value, 0 };
            return std::wstring(tmp);
        }

        std::wstring object_char::to_string_notation() const
        {
            std::wstringstream wss;
            wss << L'\'' << lib::escape_special_chars(&value, 1) << L'\'';
            return wss.str();
        }

        bool object_char::get_char(wchar_t *pval)
        {
            *pval = value;
            return true;
        }

        void object_char::op_inc(thread *thr)
        {
            unary_operation<lib::func::inc>(thr);
        }

        void object_char::op_eq(thread *thr)
        {
            binary_logical_operation<lib::func::equals, false>(thr);
        }

        void object_char::op_neq(thread *thr)
        {
            binary_logical_operation<lib::func::not_equal, true>(thr);
        }

        void object_char::op_less(thread *thr)
        {
            binary_logical_operation<lib::func::less, true>(thr);
        }

        void object_char::m_iterator(thread *thr, int arg_count)
        {
            thr->pop();
            thr->pop(arg_count);
            variable tmp;
            tmp.set_object(thr->pool->get_iterator_proto_instance());
            thr->push(tmp);
        }

        template <template<typename R, typename A> class F> void object_char::unary_operation(thread *thr)
        {
            thr->pop();
            variable result;
            result.set_char(F<wchar_t, wchar_t>::calculate(value));
            thr->push(result);
        }

        template <template<typename R, typename X, typename Y> class F, bool Def> void object_char::binary_logical_operation(thread *thr)
        {
            thr->pop();
            variable right = thr->pop();
            wchar_t right_value;
            bool right_is_char = right.get_char(&right_value);
            variable result;
            if(right_is_char)
            {
                result.set_boolean(F<bool, wchar_t, wchar_t>::calculate(value, right_value));
            }
            else
            {
                result.set_boolean(Def);
            }
            thr->push(result);
        }

        /*
            Prototype
        */

        template <template<typename R, typename A> class F> class object_char_unary_operator : public object_function_built_in
        {
        public:
            object_char_unary_operator(object_pool *_pool)
                : object_function_built_in(_pool)
            {
            }
            
            void call(thread *thr, int arg_count, call_mode mode) override
            {
                if (mode != call_mode::as_method)
                {
                    thr->raise_exception(thr->pool->get_exception_illegal_context_instance());
                    return;
                }
                object *this_ptr = thr->pop().get_object();
                assert(this_ptr != nullptr);
                object_char *this_ptr_char = this_ptr->to_object_char();
                if (!this_ptr_char)
                {
                    thr->raise_exception(thr->pool->get_exception_illegal_context_instance());
                    return;
                }
                thr->pop(arg_count);
                variable result;
                result.set_char(F<wchar_t, wchar_t>::calculate(this_ptr_char->get_value()));
                thr->push(result);
            }
        };

        object_char_proto::object_char_proto(object_pool *pool)
            : object(pool)
        {
        }

        void object_char_proto::init(object_pool *pool)
        {
            add_object(pool->get_static_string(L"++"), new object_char_unary_operator<lib::func::inc>(pool));
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
                object *obj = pool->create_object_char(var->data.b);
                var->set_object(obj);
                return obj;
            }

            bool get_char(variable *var, wchar_t *pval)
            {
                *pval = var->data.c;
                return true;
            }

            void op_inc(variable *var, thread *thr)  override
            {
                unary_operation<lib::func::inc>(var, thr);
            }

            void op_eq(variable *var, thread *thr)  override
            {
                binary_logical_operation<lib::func::equals, false>(var, thr);
            }

            void op_neq(variable *var, thread *thr)  override
            {
                binary_logical_operation<lib::func::not_equal, false>(var, thr);
            }

            void op_less(variable *var, thread *thr)  override
            {
                binary_logical_operation<lib::func::less, false>(var, thr);
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
