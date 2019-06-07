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
#include "lib/assert.h"
#include <sstream>

namespace g0at
{
    namespace model
    {
        /* 
            Base object
        */

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

        void object::find_and_vcall(thread *thr, int arg_count, std::wstring name)
        {
            model::object_function *func = nullptr;
            model::object_string *key = thr->pool->get_static_string(name);
            model::variable *var = thr->peek().to_object(thr->pool)->find_object(key);
            if (var)
            {
                model::object *obj = var->get_object();
                if(obj)
                    func = obj->to_object_function();
            }
            assert(func != nullptr); // TODO: exception if is not a function

            // call
            func->call(thr, arg_count, call_mode::as_method);
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

        void object::op_add(thread *thr)
        {
            assert(false); // not implemented
        }

        void object::op_sub(thread *thr)
        {
            assert(false); // not implemented
        }

        void object::op_neg(thread *thr)
        {
            assert(false); // not implemented
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

        void object::op_less(thread *thr)
        {
            assert(false); // not implemented
        }

        void object::op_inherit(thread *thr)
        {
            thr->pop();
            object *right = thr->peek().to_object(thr->pool);
            right->proto = this;
            right->topology.reset();
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
            // find and call own 'instanceOf()' method
            find_and_vcall(thr, arg_count, L"instanceOf");
        }

        void object::m_flat(thread *thr, int arg_count)
        {
            // find and call own 'flat()' method
            find_and_vcall(thr, arg_count, L"flat");
        }

        /*
            Topology
        */
        void topology_descriptor::build()
        {
            object::tsort_data data;
            int i;
            for (i = proto.size() - 1; i > -1; i--)
            {
                proto[i]->tsort(data);
            }
            int size = (int)data.stack.size();
            flat.init(size);
            for (i = 0; i < size; i++)
            {
                flat[i] = data.stack.top();
                data.stack.pop();
            }
        }

        /* 
            Generic object
        */
        generic_object::generic_object(object_pool *pool)
            : object(pool)
        {
        }

        void generic_object::kill(object_pool *pool)
        {
            if (pool->generic_objects.destroy_or_cache(this, pool))
            {
                objects.clear();
                proto = nullptr;
                topology.reset();
            }
        }

        void generic_object::reinit(object_pool *pool)
        {
            assert(!proto && !topology);
            proto = pool->get_generic_proto_instance();
        }

        void generic_object::m_clone(thread *thr, int arg_count)
        {
            // find and call own 'clone()' method
            find_and_vcall(thr, arg_count, L"clone");
        }

        /* 
            Generic proto
        */
        class generic_clone : public object_function_built_in
        {
        public:
            generic_clone(object_pool *_pool)
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
                thr->pop(arg_count);
                object *this_ptr = thr->pop().get_object();
                assert(this_ptr != nullptr);

                object *clone = thr->pool->create_generic_object();
                this_ptr->copy_objects_to(clone);
                this_ptr->copy_proto_to(clone);

                variable tmp;
                tmp.set_object(clone);
                thr->push(tmp);
            }
        };

        class generic_instance_of : public object_function_built_in
        {
        public:
            generic_instance_of(object_pool *_pool)
                : object_function_built_in(_pool)
            {
            }
            
            void call(thread *thr, int arg_count, call_mode mode) override
            {
                if (arg_count < 1)
                {
                    thr->raise_exception(thr->pool->get_exception_illegal_argument_instance());
                    return;
                }
                if (mode != call_mode::as_method)
                {
                    thr->raise_exception(thr->pool->get_exception_illegal_context_instance());
                    return;
                }
                object *this_ptr = thr->pop().get_object();
                assert(this_ptr != nullptr);
                object *base = thr->peek().to_object(thr->pool);
                thr->pop(arg_count);
                variable tmp;
                tmp.set_boolean(this_ptr->instance_of(base));
                thr->push(tmp);
            }
        };

        class generic_flat : public object_function_built_in
        {
        public:
            generic_flat(object_pool *_pool)
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
                thr->pop(arg_count);
                object *this_ptr = thr->pop().get_object();
                assert(this_ptr != nullptr);

                object *flat = thr->pool->create_generic_object();
                this_ptr->flat(flat);

                variable tmp;
                tmp.set_object(flat);
                thr->push(tmp);
            }
        };

        generic_proto::generic_proto(object_pool *pool)
            : object(pool, nullptr)
        {
        }

        void generic_proto::init(object_pool *pool)
        {
            add_object(pool->get_static_string(L"clone"), new generic_clone(pool));
            add_object(pool->get_static_string(L"instanceOf"), new generic_instance_of(pool));
            add_object(pool->get_static_string(L"flat"), new generic_flat(pool));
        }

        /*
            Base handler
        */
        handler::~handler()
        {
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
            assert(false);
        }

        void handler::op_sub(variable *var, thread *thr)
        {
            assert(false);
        }

        void handler::op_neg(variable *var, thread *thr)
        {
            assert(false);
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

        /* 
            Generic handler
        */

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
        };

        handler *handler::get_instance_generic()
        {
            return generic_handler::get_instance();
        }
    };
};
