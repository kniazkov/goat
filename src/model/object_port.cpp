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

#include "settings.h"
#include "object_port.h"
#include "object_string.h"
#include "object_function_built_in.h"
#include "object_exception.h"
#include "thread.h"
#include "lib/assert.h"
#include "lib/gpio.h"
#include "resource/strings.h"
#include <sstream>

namespace g0at
{
    namespace model
    {
        class object_port : public object
        {
        public:
            object_port(object_pool *pool);
            virtual void read(variable *pvar, object_pool *pool) = 0;
            virtual void write(variable var) = 0;
        };

        class object_port_method : public object_function_built_in
        {
        public:
            object_port_method(object_pool *_pool, object_port *_port)
                : object_function_built_in(_pool), port(_port)
            {
            }

            void call(thread *thr, int arg_count, call_mode mode) override
            {
                thr->pop();
                variable result;
                if (payload(thr, arg_count, &result))
                {
                    thr->pop(arg_count);
                    thr->push(result);
                }
            }

            virtual bool payload(thread *thr, int arg_count, variable *result) = 0;

        protected:
            object_port *port;
        };

        class object_port_stub : public object_port_method
        {
        public:
            object_port_stub(object_pool *_pool, object_port *_port)
                : object_port_method(_pool, _port)
            {
            }

            bool payload(thread *thr, int arg_count, variable *result) override
            {
                thr->raise_exception(new object_exception_illegal_operation(thr->pool));
                return false;
            }
        };

        class object_port_read : public object_port_method
        {
        public:
            object_port_read(object_pool *_pool, object_port *_port)
                : object_port_method(_pool, _port)
            {
            }

            bool payload(thread *thr, int arg_count, variable *result) override
            {
                port->read(result, thr->pool);
                return true;
            }
        };

        class object_port_write : public object_port_method
        {
        public:
            object_port_write(object_pool *_pool, object_port *_port)
                : object_port_method(_pool, _port)
            {
            }

            bool payload(thread *thr, int arg_count, variable *result) override
            {
                if (arg_count < 1)
                {
                    thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                    return false;
                }
                variable value = thr->peek();
                port->write(value);
                *result = value;
                return true;
            }
        };
        
        object_port::object_port(object_pool *pool)
            : object(pool, pool->get_port_proto_instance())
        {
            add_object(pool->get_static_string(resource::str_read), new object_port_read(pool, this));
            add_object(pool->get_static_string(resource::str_write), new object_port_write(pool, this));
            lock();
        }

        class object_port_null : public object_port
        {
        public:
            object_port_null(object_pool *pool)
                : object_port(pool)
            {
            }

            void read(variable *pvar, object_pool *pool) override
            {
                pvar->set_object(pool->get_null_instance());
            }

            void write(variable var) override
            {
                // do nothing
            }
        };

#ifdef GPIO_ENABLE
        class object_port_gpio : public object_port
        {
        public:
            object_port_gpio(object_pool *pool, unsigned int _port_number)
                : object_port(pool), port_number(_port_number)
            {
            }

            void read(variable *pvar, object_pool *pool) override
            {
                pvar->set_boolean(lib::gpio_get_value(port_number));
            }

            void write(variable var) override
            {
                bool bool_value;
                if (var.get_boolean(&bool_value))
                {
                    lib::gpio_set_value(port_number, bool_value);
                }
            }

        protected:
            unsigned int port_number;
        };
#endif

        object_ports::object_ports(object_pool *pool)
            : object(pool)
        {
            add_object(pool->get_static_string(resource::str_null), new object_port_null(pool));
#ifdef GPIO_ENABLE
            lib::gpio_info i = lib::gpio_get_info();
            for (unsigned int k = 0; k < i.count; k++)
            {
                std::wstringstream wss;
                wss << L"gpio" << i.port_numbers[k];
                add_object(pool->create_object_string(wss.str()), new object_port_gpio(pool, i.port_numbers[k]));
            }
#endif
            lock();
        }

        object_port_proto::object_port_proto(object_pool *pool)
            : object(pool)
        {
        }

        void object_port_proto::init(object_pool *pool)
        {
            object *stub = new object_port_stub(pool, nullptr);
            add_object(pool->get_static_string(resource::str_read), stub);
            add_object(pool->get_static_string(resource::str_write), stub);
            lock();
        }

        void object_port_proto::op_new(thread *thr, int arg_count)
        {
            thr->raise_exception(new object_exception_illegal_operation(thr->pool));
        }
    };
};

