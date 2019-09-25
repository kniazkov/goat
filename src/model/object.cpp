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
#include "thread.h"
#include "object_string.h"
#include "object_function_built_in.h"
#include "object_exception.h"
#include "resource/strings.h"
#include "lib/assert.h"
#include <sstream>

namespace g0at
{
    namespace model
    {
        object::object(object_pool *pool)
            : marked(false)
        {
#ifdef MODEL_DEBUG
            id = pool->get_next_id();
#endif
            pool->add(this);
            proto = pool->get_generic_proto_instance();
            topology = nullptr;
        }

        object::object(object_pool *pool, object *proto)
            : marked(false)
        {
#ifdef MODEL_DEBUG
            id = pool->get_next_id();
#endif
            pool->add(this);
            this->proto = proto;
            topology = nullptr;
        }

        object::object(object_pool *pool, object *proto_0, object *proto_1)
            : marked(false)
        {
            assert(proto_0 != nullptr);
            assert(proto_1 != nullptr);
#ifdef MODEL_DEBUG
            id = pool->get_next_id();
#endif
            pool->add(this);

            proto = nullptr;
            topology = new topology_descriptor();
            topology->proto.init(2);
            topology->proto[0] = proto_0;
            topology->proto[1] = proto_1;
            topology->build();
        }

        object::~object()
        {
        }

        void object::kill(object_pool *pool)
        {
            pool->population.remove(this);
            delete this;
        }

        void object::trace()
        {
        }

        object_type object::get_type() const
        {
            return object_type::generic;
        }

        object_string *object::to_object_string()
        {
            return nullptr;
        }

        object_function *object::to_object_function()
        {
            return nullptr;
        }

        object_thread *object::to_object_thread()
        {
            return nullptr;
        }

        object_integer *object::to_object_integer()
        {
            return nullptr;
        }

        object_real *object::to_object_real()
        {
            return nullptr;
        }

        object_void *object::to_object_void()
        {
            return nullptr;
        }

        object_undefined *object::to_object_undefined()
        {
            return nullptr;
        }

        object_null *object::to_object_null()
        {
            return nullptr;
        }

        object_boolean *object::to_object_boolean()
        {
            return nullptr;
        }

        object_array *object::to_object_array()
        {
            return nullptr;
        }

        object_exception *object::to_object_exception()
        {
            return nullptr;
        }

        object_char *object::to_object_char()
        {
            return nullptr;
        }

        object_runner *object::to_object_runner()
        {
            return nullptr;
        }

        object_uid *object::to_object_uid()
        {
            return nullptr;
        }

        object_string_builder *object::to_object_string_builder()
        {
            return nullptr;
        }

        object_byte_array *object::to_object_byte_array()
        {
            return nullptr;
        }

        bool object::less(const object *obj) const
        {
            return this < obj;
        }
    
        std::wstring object::to_string() const
        {
            std::wstringstream stream;
            stream << '{';

            int k = 0;
            for (auto pair : objects)
            {
                if (k)
                    stream << ',';
                k++;
                stream << pair.first->to_string_notation() << ':' << pair.second.to_string_notation();
            }

            stream << '}';
            return stream.str();
        }

        std::wstring object::to_string_notation() const
        {
            return to_string();
        }

        void object::copy_objects_to(object *dst)
        {
            for (auto pair : objects)
            {
                dst->add_object(pair.first, pair.second);
            }
        }

        void object::copy_proto_to(object *dst)
        {
            dst->proto = proto;
            dst->topology = topology;
        }

        void object::tsort(tsort_data &data)
        {
            if (data.processed.find(this) != data.processed.end())
                return;
            
            if (proto)
            {
                proto->tsort(data);
            }
            else if (topology)
            {
                for (int i = 0, size = topology->proto.size(); i < size; i++)
                    topology->proto[i]->tsort(data);
            }

            data.processed.insert(this);
            data.stack.push(this);
        }

        bool object::instance_of(object *base)
        {
            if (base == this)
                return true;

            if (proto)
            {
                return proto->instance_of(base);
            }
            else if (topology)
            {
                for (int i = 0, size = topology->flat.size(); i < size; i++)
                {
                    if (base == topology->flat[i])
                        return true;
                }
            }
            return false;
        }

        void object::flat(object *dst)
        {
            if (proto)
            {
                proto->flat(dst);
            }
            else if (topology)
            {
                for (int i = topology->flat.size() - 1; i > -1; i--)
                {
                    topology->flat[i]->copy_objects_to(dst);
                }
            }
            copy_objects_to(dst);
        }

        std::vector<object*> object::get_keys()
        {
            std::vector<object*> keys;
            for (auto pair : objects)
            {
                keys.push_back(pair.first);
            }
            return keys;
        }

        void object::add_object(object *key, variable &value)
        {
            assert(key != nullptr);
            objects[key] = value;
        }

        void object::add_object(object *key, object *value)
        {
            assert(key != nullptr);
            assert(value != nullptr);

            variable var;
            var.set_object(value);
            objects[key] = var;
        }

        variable *object::find_object(object *key)
        {
            assert(key != nullptr);

            auto iter = objects.find(key);
            if (iter != objects.end())
            {
                return &iter->second;
            }
            
            if (proto)
            {
                return proto->find_object(key);
            }
            else if (topology)
            {
                for (int i = 0, size = topology->flat.size(); i < size; i++)
                {
                    object *proto = topology->flat[i];
                    auto iter = proto->objects.find(key);
                    if (iter != proto->objects.end())
                    {
                        return &iter->second;
                    }
                }
            }
            return nullptr;
        }

        variable *object::find_own_object(object *key)
        {
            assert(key != nullptr);

            auto iter = objects.find(key);
            if (iter != objects.end())
            {
                return &iter->second;
            }
            
            return nullptr;
        }

        void object::find_own_and_call_if_exists(thread *thr, int arg_count, object_string *key, call_mode mode)
        {
            variable *var = find_own_object(key);
            if (var)
            {
                model::object *obj = var->get_object();
                if(obj)
                {
                    object_function *func = obj->to_object_function();
                    if (func)
                    {
                        func->call(thr, arg_count, mode);
                    }
                }
            }
        }

        bool object::find_and_vcall(thread *thr, int arg_count, std::wstring name)
        {
            object_function *func = nullptr;
            object_string *key = thr->pool->get_static_string(name);
            variable *var = find_object(key);
            if (var)
            {
                model::object *obj = var->get_object();
                if(obj)
                    func = obj->to_object_function();
            }
            if (!func)
                return false;

            // call
            func->call(thr, arg_count, call_mode::as_method);
            return true;
        }

        void object::for_each_proto(std::function<void(object*)> callback)
        {
            if (proto)
            {
                callback(proto);
                proto->for_each_proto(callback);
            }
            else if (topology)
            {
                for (int i = 0, size = topology->flat.size(); i < size; i++)
                {
                    callback(topology->flat[i]);
                }
            }
        }

        bool object::get_integer(int64_t *pval)
        {
            return false;
        }

        bool object::get_real(double *pval)
        {
            return false;
        }

        bool object::get_boolean(bool *pval)
        {
            return false;
        }

        bool object::get_char(wchar_t *pval)
        {
            return false;
        }

        void object::op_new(thread *thr, int arg_count)
        {
            model::object *new_object = new model::object(thr->pool, this);

            model::variable var;
            var.set_object(new_object);
            thr->push(var);

            model::object_string *key = thr->pool->get_static_string(resource::str_init);
            find_own_and_call_if_exists(thr, arg_count, key, model::call_mode::as_constructor);
            for_each_proto([thr, key](model::object *proto_proto_object)
            {
                proto_proto_object->find_own_and_call_if_exists(thr, 0, key, model::call_mode::as_constructor);
            });
        }

        void object::op_add(thread *thr)
        {
            if (!find_and_vcall(thr, 1, resource::str_oper_plus))
                thr->raise_exception(new object_exception_operator_not_found(thr->pool, resource::str_oper_plus));
        }

        void object::op_sub(thread *thr)
        {
            if (!find_and_vcall(thr, 1, resource::str_oper_minus))
                thr->raise_exception(new object_exception_operator_not_found(thr->pool, resource::str_oper_minus));
        }

        void object::op_pos(thread *thr)
        {
            if (!find_and_vcall(thr, 0, resource::str_oper_plus))
                thr->raise_exception(new object_exception_operator_not_found(thr->pool, resource::str_oper_plus));
        }

        void object::op_neg(thread *thr)
        {
            if (!find_and_vcall(thr, 0, resource::str_oper_minus))
                thr->raise_exception(new object_exception_operator_not_found(thr->pool, resource::str_oper_minus));
        }

        void object::op_inc(thread *thr)
        {
            if (!find_and_vcall(thr, 0, resource::str_oper_plus_plus))
                thr->raise_exception(new object_exception_operator_not_found(thr->pool, resource::str_oper_plus_plus));
         }

        void object::op_dec(thread *thr)
        {
            if (!find_and_vcall(thr, 0, resource::str_oper_minus_minus))
                thr->raise_exception(new object_exception_operator_not_found(thr->pool, resource::str_oper_minus_minus));
        }

        void object::op_not(thread *thr)
        {
            if (!find_and_vcall(thr, 0, resource::str_oper_exclamation))
                thr->raise_exception(new object_exception_operator_not_found(thr->pool, resource::str_oper_exclamation));
        }

        void object::op_bool(thread *thr)
        {
            if (!find_and_vcall(thr, 0, resource::str_oper_double_exclamation))
                thr->raise_exception(new object_exception_operator_not_found(thr->pool, resource::str_oper_double_exclamation));
        }

        void object::op_inv(thread *thr)
        {
            if (!find_and_vcall(thr, 0, resource::str_oper_tilde))
                thr->raise_exception(new object_exception_operator_not_found(thr->pool, resource::str_oper_tilde));
        }

        void object::op_mul(thread *thr)
        {
            if (!find_and_vcall(thr, 1, resource::str_oper_asterisk))
                thr->raise_exception(new object_exception_operator_not_found(thr->pool, resource::str_oper_asterisk));
        }

        void object::op_exp(thread *thr)
        {
            if (!find_and_vcall(thr, 1, resource::str_oper_double_asterisk))
                thr->raise_exception(new object_exception_operator_not_found(thr->pool, resource::str_oper_double_asterisk));
        }

        void object::op_div(thread *thr)
        {
            if (!find_and_vcall(thr, 1, resource::str_oper_slash))
                thr->raise_exception(new object_exception_operator_not_found(thr->pool, resource::str_oper_slash));
        }

        void object::op_mod(thread *thr)
        {
            if (!find_and_vcall(thr, 1, resource::str_oper_percent))
                thr->raise_exception(new object_exception_operator_not_found(thr->pool, resource::str_oper_percent));
        }

        void object::op_eq(thread *thr)
        {
            thr->pop();
            object *right = thr->pop().get_object();
            variable result;
            result.set_boolean(this == right);
            thr->push(result);
        }

        void object::op_neq(thread *thr)
        {
            thr->pop();
            object *right = thr->pop().get_object();
            variable result;
            result.set_boolean(this != right);
            thr->push(result);
        }

        void object::op_shl(thread *thr)
        {
            if (!find_and_vcall(thr, 1, resource::str_oper_double_less))
                thr->raise_exception(new object_exception_operator_not_found(thr->pool, resource::str_oper_double_less));
        }

        void object::op_shr(thread *thr)
        {
            if (!find_and_vcall(thr, 1, resource::str_oper_double_greater))
                thr->raise_exception(new object_exception_operator_not_found(thr->pool, resource::str_oper_double_greater));
        }

        void object::op_shrz(thread *thr)
        {
            if (!find_and_vcall(thr, 1, resource::str_oper_triple_greater))
                thr->raise_exception(new object_exception_operator_not_found(thr->pool, resource::str_oper_triple_greater));
        }

        void object::op_less(thread *thr)
        {
            if (!find_and_vcall(thr, 1, resource::str_oper_less))
                thr->raise_exception(new object_exception_operator_not_found(thr->pool, resource::str_oper_less));
        }

        void object::op_leq(thread *thr)
        {
            if (!find_and_vcall(thr, 1, resource::str_oper_less_equal))
                thr->raise_exception(new object_exception_operator_not_found(thr->pool, resource::str_oper_less_equal));
        }

        void object::op_great(thread *thr)
        {
            if (!find_and_vcall(thr, 1, resource::str_oper_greater))
                thr->raise_exception(new object_exception_operator_not_found(thr->pool, resource::str_oper_greater));
        }

        void object::op_greq(thread *thr)
        {
            if (!find_and_vcall(thr, 1, resource::str_oper_greater_equal))
                thr->raise_exception(new object_exception_operator_not_found(thr->pool, resource::str_oper_greater_equal));
        }

        void object::op_inherit(thread *thr)
        {
            thr->pop();
            object *right = thr->peek().to_object(thr->pool);
            right->proto = this;
            right->topology.reset();
        }

        void object::op_and(thread *thr)
        {
            if (!find_and_vcall(thr, 1, resource::str_oper_double_ampersand))
                thr->raise_exception(new object_exception_operator_not_found(thr->pool, resource::str_oper_double_ampersand));
        }

        void object::op_or(thread *thr)
        {
            if (!find_and_vcall(thr, 1, resource::str_oper_double_vertical_bar))
                thr->raise_exception(new object_exception_operator_not_found(thr->pool, resource::str_oper_double_vertical_bar));
        }

        void object::op_bitand(thread *thr)
        {
            if (!find_and_vcall(thr, 1, resource::str_oper_ampersand))
                thr->raise_exception(new object_exception_operator_not_found(thr->pool, resource::str_oper_ampersand));
        }

        void object::op_bitor(thread *thr)
        {
            if (!find_and_vcall(thr, 1, resource::str_oper_vertical_bar))
                thr->raise_exception(new object_exception_operator_not_found(thr->pool, resource::str_oper_vertical_bar));
        }

        void object::op_xor(thread *thr)
        {
            if (!find_and_vcall(thr, 1, resource::str_oper_caret))
                thr->raise_exception(new object_exception_operator_not_found(thr->pool, resource::str_oper_caret));
        }

        void object::m_clone(thread *thr, int arg_count)
        {
            // base object just returns the object itself, so, primitives are not cloneable
            variable tmp = thr->pop();
            thr->pop(arg_count);
            thr->push(tmp);
        }

        void object::m_instance_of(thread *thr, int arg_count)
        {
            // find and call own 'instanceof()' method
            find_and_vcall(thr, arg_count, resource::str_instanceof);
        }

        void object::m_flat(thread *thr, int arg_count)
        {
            // find and call own 'flat()' method
            find_and_vcall(thr, arg_count, resource::str_flat);
        }

        void object::m_get(thread *thr, int arg_count)
        {
            // find and call own 'get()' method
            find_and_vcall(thr, arg_count, resource::str_get);
        }

        void object::m_set(thread *thr, int arg_count)
        {
            // find and call own 'set()' method
            find_and_vcall(thr, arg_count, resource::str_set);
        }

        void object::m_iterator(thread *thr, int arg_count)
        {
            // find and call own 'iterator()' method
            find_and_vcall(thr, arg_count, resource::str_iterator);
        }

        void object::m_next(thread *thr, int arg_count)
        {
            // find and call own 'next()' method
            find_and_vcall(thr, arg_count, resource::str_next);
        }

        void object::m_valid(thread *thr, int arg_count)
        {
            // find and call own 'valid()' method
            find_and_vcall(thr, arg_count, resource::str_valid);
        }
    };
};
