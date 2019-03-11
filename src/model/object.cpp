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
            proto.push_back(pool->get_generic_proto_instance());
        }

        object::object(object_pool *pool, object *proto)
            : marked(false)
        {
#ifdef MODEL_DEBUG
            id = pool->get_next_id();
#endif
            pool->add(this);
            if (proto)
            {
                this->proto.push_back(proto);
            }
        }

        object::object(object_pool *pool, object *proto_1, object *proto_2)
            : marked(false)
        {
            assert(proto_1 != nullptr);
            assert(proto_2 != nullptr);
#ifdef MODEL_DEBUG
            id = pool->get_next_id();
#endif
            pool->add(this);

            this->proto.push_back(proto_1);
            this->proto.push_back(proto_2);
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
            
            variable *var = nullptr;
            for (object *pt : proto)
            {
                var = pt->find_object(key);
                if (var != nullptr)
                    break;
            }
            return var;
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
                proto.clear();
            }
        }

        void generic_object::reinit(object_pool *pool)
        {
            assert(proto.empty());
            proto.push_back(pool->get_generic_proto_instance());
        }

        /* 
            Generic proto
        */
        generic_proto::generic_proto(object_pool *pool)
            : object(pool, nullptr)
        {
        }

        /*
            Base handler
        */
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

        /* 
            Generic handler
        */

        handler::~handler()
        {
        }

        handler *handler::get_instance_generic()
        {
            return generic_handler::get_instance();
        }

        generic_handler::generic_handler()
        {
        }

        handler *generic_handler::get_instance()
        {
            static generic_handler instance;
            return &instance;
        }

        std::wstring generic_handler::to_string(const variable *var) const
        {
            return var->data.obj->to_string();
        }

        std::wstring generic_handler::to_string_notation(const variable *var) const
        {
            return var->data.obj->to_string_notation();
        }

        object *generic_handler::to_object(variable *var, object_pool *pool)
        {
            return var->data.obj;
        }

        object *generic_handler::get_object(variable *var)
        {
            return var->data.obj;
        }

        bool generic_handler::get_integer(variable *var, int64_t *pval)
        {
            return var->data.obj->get_integer(pval);
        }

        bool generic_handler::get_real(variable *var, double *pval)
        {
            return var->data.obj->get_real(pval);
        }

        bool generic_handler::get_boolean(variable *var, bool *pval)
        {
            return var->data.obj->get_boolean(pval);
        }

        void generic_handler::op_add(variable *var, thread *thr)
        {
            var->data.obj->op_add(thr);
        }

        void generic_handler::op_sub(variable *var, thread *thr)
        {
            var->data.obj->op_sub(thr);
        }

        void generic_handler::op_neg(variable *var, thread *thr)
        {
            var->data.obj->op_neg(thr);
        }

        void generic_handler::op_eq(variable *var, thread *thr)
        {
            var->data.obj->op_eq(thr);
        }

        void generic_handler::op_neq(variable *var, thread *thr)
        {
            var->data.obj->op_neq(thr);
        }
    };
};
