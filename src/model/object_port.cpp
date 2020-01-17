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

#include "object_port.h"
#include "object_string.h"
#include "object_function_built_in.h"
#include "object_exception.h"
#include "thread.h"
#include "lib/assert.h"
#include "resource/strings.h"

namespace g0at
{
    namespace model
    {
        /*
            Ports
        */
        class port_null : public hw_port
        {
        public:
            static hw_port *get_instance()
            {
                static port_null instance;
                return &instance;
            }

            variable get(object_pool *pool) override
            {
                variable result;
                result.set_object(pool->get_null_instance());
                return result;
            }

            bool set(variable value) override
            {
                return true;
            }

        private:
            port_null()
            {
            }
        };

        /*
            Object
        */
        object_port::object_port(object_pool *pool)
            : object(pool)
        {
            // creating hardware dependent ports 
            ports[pool->get_static_string(resource::str_null)] = port_null::get_instance(); 
        }

        object_port::~object_port()
        {
        }

        object_port *object_port::to_object_port()
        {
            return this;
        }

        void object_port::m_get(thread *thr, int arg_count)
        {
            if (arg_count < 1)
            {
                thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                return;
            }
            thr->pop();
            object *key = thr->peek(0).to_object(thr->pool);
            thr->pop(arg_count);
            auto pair = ports.find(key);
            if (pair == ports.end())
            {
                thr->push_undefined();
                return;
            }
            thr->push(pair->second->get(thr->pool));
        }

        void object_port::m_set(thread *thr, int arg_count)
        {
            if (arg_count < 2)
            {
                thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                return;
            }
            thr->pop();
            object *key = thr->peek(0).to_object(thr->pool);
            variable value = thr->peek(arg_count - 1);
            thr->pop(arg_count);
            auto pair = ports.find(key);
            if (pair == ports.end())
            {
                thr->push_undefined();
                return;
            }
            if (!pair->second->set(value))
            {
                thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                return;
            }
            thr->push(value);
            return;
        }

        void object_port::m_clone(thread *thr, int arg_count)
        {
            thr->pop();
            thr->pop(arg_count);
            variable result;
            result.set_object(this);
            thr->push(result);
        }

        class object_port_iterator : public object
        {
        public:
            object_port_iterator(object_pool *pool, std::map<object*, hw_port*> &ports)
                : object(pool, pool->get_iterator_proto_instance())
            {
                pair = ports.begin();
                lastPair = ports.end();
            }

            void m_valid(thread *thr, int arg_count) override
            {
                thr->pop();
                thr->pop(arg_count);
                variable tmp;
                tmp.set_boolean(pair != lastPair);
                thr->push(tmp);
            }

            void m_next(thread *thr, int arg_count) override
            {
                thr->pop();
                thr->pop(arg_count);
                if (pair != lastPair)
                {
                    variable tmp;
                    tmp.set_object(pair->first);
                    thr->push(tmp);
                    pair++;
                }
                else
                    thr->push_undefined();
            }

        protected:
            std::map<object*, hw_port*>::iterator pair,
                lastPair;
        };

        void object_port::m_iterator(thread *thr, int arg_count)
        {
            thr->pop();
            thr->pop(arg_count);
            variable tmp;
            tmp.set_object(new object_port_iterator(thr->pool, ports));
            thr->push(tmp);
        }
    };
};

