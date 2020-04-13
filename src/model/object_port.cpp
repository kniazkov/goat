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
#include "lib/assert.h"
#include "lib/utils.h"
#include "lib/gpio.h"
#include "lib/spinlock.h"
#include "resource/strings.h"
#include <sstream>
#include <algorithm>
#include <mutex>
#include <thread>
#include <chrono>

namespace g0at
{
    namespace model
    {
        class object_port : public object
        {
        public:
            object_port(object_pool *pool);
            object_port(object_pool *pool, object *proto);
            virtual void read(variable *pvar, object_pool *pool) = 0;
            virtual void write(variable var) = 0;
            virtual unsigned int bitwidth() = 0;
        };

        struct boolean_port_action
        {
            int64_t timestamp;
            int8_t value;
            int16_t increment;
        };

        class object_boolean_port : public object_port
        {
        public:
            object_boolean_port(object_pool *pool, object_boolean_port **next);
            virtual void write(bool value) = 0;
            virtual void toggle() = 0;
            
            object_boolean_port *next_port;
            lib::spinlock latch;
            int64_t counter;
            int index;
            std::vector<boolean_port_action> actions;
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
        
        class object_port_bitwidth : public object_port_method
        {
        public:
            object_port_bitwidth(object_pool *_pool, object_port *_port)
                : object_port_method(_pool, _port)
            {
            }

            bool payload(thread *thr, int arg_count, variable *result) override
            {
                result->set_integer(port->bitwidth());
                return true;
            }
        };

        class object_boolean_port_method : public object_function_built_in
        {
        public:
            object_boolean_port_method(object_pool *_pool, object_boolean_port *_port)
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
            object_boolean_port *port;
        };

        class object_port_toggle : public object_boolean_port_method
        {
        public:
            object_port_toggle(object_pool *_pool, object_boolean_port *_port)
                : object_boolean_port_method(_pool, _port)
            {
            }

            bool payload(thread *thr, int arg_count, variable *result) override
            {
                port->toggle();
                result->set_object(thr->pool->get_undefined_instance());
                return true;
            }
        };

        class object_port_schedule : public object_boolean_port_method
        {
        public:
            object_port_schedule(object_pool *_pool, object_boolean_port *_port)
                : object_boolean_port_method(_pool, _port)
            {
            }

            bool payload(thread *thr, int arg_count, variable *result) override
            {
                if (arg_count < 2)
                {
                    thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                    return false;
                }
                variable value = thr->peek(0);
                boolean_port_action action;
                if (!value.get_integer(&action.timestamp) || action.timestamp < 0)
                {
                    thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                    return false;
                }
                value = thr->peek(1);
                bool boolean_value;
                if (!value.get_boolean(&boolean_value))
                {
                    thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                    return false;
                }
                action.value = boolean_value ? 1 : -1;
                action.increment = 0;
                if (arg_count > 2)
                {
                    value = thr->peek(2);
                    if (!value.get_short(&action.increment))
                    {
                        thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                        return false;
                    }
                }
                std::lock_guard<lib::spinlock>(port->latch);
                port->actions.push_back(action);
                result->set_object(thr->pool->get_undefined_instance());
                return true;
            }
        };

        class object_port_pulse : public object_boolean_port_method
        {
        public:
            object_port_pulse(object_pool *_pool, object_boolean_port *_port)
                : object_boolean_port_method(_pool, _port)
            {
            }

            bool payload(thread *thr, int arg_count, variable *result) override
            {
                if (arg_count < 3)
                {
                    thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                    return false;
                }
                variable value = thr->peek(0);
                int64_t start;
                if (!value.get_integer(&start) || start < 0)
                {
                    thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                    return false;
                }
                value = thr->peek(1);
                int64_t interval;
                if (!value.get_integer(&interval) || interval <= 0)
                {
                    thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                    return false;
                }
                value = thr->peek(2);
                int64_t count;
                if (!value.get_integer(&count) || count < 0)
                {
                    thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                    return false;
                }
                int16_t increment = 0;
                if (arg_count > 3)
                {
                    value = thr->peek(3);
                    if (!value.get_short(&increment))
                    {
                        thr->raise_exception(new object_exception_illegal_argument(thr->pool));
                        return false;
                    }
                }
                boolean_port_action action;
                action.timestamp = start;
                action.value = 0; // toggle
                action.increment = increment;
                std::lock_guard<lib::spinlock>(port->latch);
                for (int64_t i = 0; i < count; i++)
                {
                    port->actions.push_back(action);
                    action.timestamp += interval;
                }
                result->set_object(thr->pool->get_undefined_instance());
                return true;
            }
        };

        class object_port_count : public object_boolean_port_method
        {
        public:
            object_port_count(object_pool *_pool, object_boolean_port *_port)
                : object_boolean_port_method(_pool, _port)
            {
            }

            bool payload(thread *thr, int arg_count, variable *result) override
            {
                std::lock_guard<lib::spinlock>(port->latch);
                result->set_integer(port->counter);
                return true;
            }
        };
        
        class object_port_reset : public object_boolean_port_method
        {
        public:
            object_port_reset(object_pool *_pool, object_boolean_port *_port)
                : object_boolean_port_method(_pool, _port)
            {
            }

            bool payload(thread *thr, int arg_count, variable *result) override
            {
                std::lock_guard<lib::spinlock>(port->latch);
                port->counter = 0;
                result->set_object(thr->pool->get_undefined_instance());
                return true;
            }
        };

        object_port::object_port(object_pool *pool)
            : object(pool, pool->get_port_proto_instance())
        {
            add_object(pool->get_static_string(resource::str_read), new object_port_read(pool, this));
            add_object(pool->get_static_string(resource::str_write), new object_port_write(pool, this));
            add_object(pool->get_static_string(resource::str_bitwidth), new object_port_bitwidth(pool, this));
            lock();
        }

        object_port::object_port(object_pool *pool, object *proto)
            : object(pool, proto)
        {
            add_object(pool->get_static_string(resource::str_read), new object_port_read(pool, this));
            add_object(pool->get_static_string(resource::str_write), new object_port_write(pool, this));
            add_object(pool->get_static_string(resource::str_bitwidth), new object_port_bitwidth(pool, this));
        }

        object_boolean_port::object_boolean_port(object_pool *pool, object_boolean_port **next)
            : object_port(pool, pool->get_gpio_proto_instance())
        {
            next_port = *next;
            *next = this;

            counter = 0;
            index = 0;

            add_object(pool->get_static_string(resource::str_toggle), new object_port_toggle(pool, this));
            add_object(pool->get_static_string(resource::str_schedule), new object_port_schedule(pool, this));
            add_object(pool->get_static_string(resource::str_pulse), new object_port_pulse(pool, this));
            add_object(pool->get_static_string(resource::str_count), new object_port_count(pool, this));
            add_object(pool->get_static_string(resource::str_reset), new object_port_reset(pool, this));
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

            unsigned int bitwidth() override
            {
                return 0;
            }
        };

#ifdef GPIO_ENABLE
        class object_port_gpio : public object_boolean_port
        {
        public:
            object_port_gpio(object_pool *pool, object_boolean_port **next, unsigned int _port_number)
                : object_boolean_port(pool, next), port_number(_port_number)
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

            void write(bool value) override
            {
                lib::gpio_set_value(port_number, value);
            }

            unsigned int bitwidth() override
            {
                return 1;
            }

            void toggle() override
            {
                lib::gpio_set_value(port_number, !lib::gpio_get_value(port_number));
            }

        protected:
            unsigned int port_number;
        };
#endif

        class object_port_virtual : public object_boolean_port
        {
        public:
            object_port_virtual(object_pool *pool, object_boolean_port **next)
                : object_boolean_port(pool, next), value(false)
            {
            }

            void read(variable *pvar, object_pool *pool) override
            {
                pvar->set_boolean(value);
            }

            void write(variable var) override
            {
                var.get_boolean(&value);
            }

            void write(bool _value) override
            {
                value = _value;
            }

            unsigned int bitwidth() override
            {
                return 1;
            }

            void toggle() override
            {
                value = !value;
            }

        protected:
            bool value;
        };

        class object_ports_init : public object_function_built_in
        {
        public:
            object_ports_init(object_pool *_pool, object_ports *_ports)
                : object_function_built_in(_pool), ports(_ports)
            {
            }
            
            void call(thread *thr, int arg_count, call_mode mode) override
            {
                if (mode == call_mode::as_method)
                    thr->pop();
                thr->pop(arg_count);
                variable result;
#ifdef GPIO_ENABLE
                if (ports->initialized)
                {
                    result.set_boolean(true);
                }
                else if (lib::gpio_init())
                {
                    lib::gpio_info i = lib::gpio_get_info();
                    ports->unlock();
                    for (unsigned int k = 0; k < i.count; k++)
                    {
                        std::wstringstream wss;
                        wss << L"gpio" << i.port_numbers[k];
                        ports->add_object(thr->pool->create_object_string(wss.str()), new object_port_gpio(thr->pool, &ports->list, i.port_numbers[k]));
                    }
                    ports->initialized = true;
                    ports->lock();
                    result.set_boolean(true);
                }
                else
                {
                    result.set_boolean(false);
                }
#else
                result.set_boolean(true);
#endif
                thr->push(result);
            }

        private:
            object_ports *ports;
        };

        class object_ports_run : public object_function_built_in
        {
        public:
            object_ports_run(object_pool *_pool, object_ports *_ports)
                : object_function_built_in(_pool), ports(_ports)
            {
            }
            
            void call(thread *thr, int arg_count, call_mode mode) override
            {
                if (mode == call_mode::as_method)
                    thr->pop();
                thr->pop(arg_count);
                variable result;
                std::atomic_int64_t *pwm_timestamp = &ports->pwm_timestamp;
                int64_t init_time = lib::get_time_ns();
                if (pwm_timestamp->load() <= 0 // thread is done (< 0) or not started yet (0)
                    && init_time != pwm_timestamp->exchange(init_time)) 
                {
                    object_boolean_port *first_port = ports->list;
                    object_boolean_port *port = first_port;
                    std::thread pwm = std::thread([first_port, pwm_timestamp]()
                    {
                        int64_t start_time = lib::get_time_ns();
                        bool flag;
                        do
                        {
                            flag = false;
                            int64_t time = lib::get_time_ns() - start_time;
                            object_boolean_port *port = first_port;
                            while (port)
                            {
                                if (port->index < port->actions.size())
                                {
                                    flag = true;
                                    boolean_port_action &action = port->actions[port->index];
                                    if (action.timestamp <= time)
                                    {
                                        std::lock_guard<lib::spinlock>(port->latch);
                                        if (action.value)
                                            port->write(action.value > 0 ? true : false);
                                        else
                                            port->toggle();
                                        port->counter += action.increment;
                                        port->index++;
                                    }
                                }
                                port = port->next_port;
                            }
                        }
                        while(flag);
                        pwm_timestamp->store(-lib::get_time_ns());
                    });
                    pwm.detach();
                    result.set_boolean(true);
                }
                else
                {
                    result.set_boolean(false);
                }
                thr->push(result);
            }

        private:
            object_ports *ports;
        };

        class object_ports_busy : public object_function_built_in
        {
        public:
            object_ports_busy(object_pool *_pool, object_ports *_ports)
                : object_function_built_in(_pool), ports(_ports)
            {
            }
            
            void call(thread *thr, int arg_count, call_mode mode) override
            {
                if (mode == call_mode::as_method)
                    thr->pop();
                thr->pop(arg_count);
                variable result;
                result.set_boolean(ports->pwm_timestamp.load() > 0);
                thr->push(result);
            }

        private:
            object_ports *ports;
        };

        class object_ports_create : public object_function_built_in
        {
        public:
            object_ports_create(object_pool *_pool, object_ports *_ports)
                : object_function_built_in(_pool), ports(_ports)
            {
            }
            
            void call(thread *thr, int arg_count, call_mode mode) override
            {
                if (mode == call_mode::as_method)
                    thr->pop();
                if (arg_count > 0)
                {
                    object *arg = thr->peek().get_object();
                    if (arg)
                    {
                        object_string *arg_string = arg->to_object_string();
                        if (arg_string)
                        {
                            std::wstring port_name = arg_string->get_data();
                            if (port_name != L"")
                            {
                                ports->unlock();
                                ports->add_object(thr->pool->create_object_string(port_name), new object_port_virtual(thr->pool, &ports->list));
                                ports->lock();
                                thr->pop(arg_count);
                                thr->push_undefined();
                                return;
                            }
                        }
                    }
                }
                thr->raise_exception(new object_exception_illegal_argument(thr->pool));
            }

        private:
            object_ports *ports;
        };

        class object_ports_reset : public object_function_built_in
        {
        public:
            object_ports_reset(object_pool *_pool, object_ports *_ports)
                : object_function_built_in(_pool), ports(_ports)
            {
            }
            
            void call(thread *thr, int arg_count, call_mode mode) override
            {
                if (mode == call_mode::as_method)
                    thr->pop();
                thr->pop(arg_count);
                object_boolean_port *port = ports->list;
                while(port)
                {
                    std::lock_guard<lib::spinlock>(port->latch);
                    port->index = 0;
                    port->actions.clear();
                    port = port->next_port;
                }
                thr->push_undefined();
            }

        private:
            object_ports *ports;
        };

        object_ports::object_ports(object_pool *pool)
            : object(pool)
        {
            initialized = false;
            list = nullptr;
            pwm_timestamp.store(0);
            add_object(pool->get_static_string(resource::str_null), new object_port_null(pool));
            add_object(pool->get_static_string(resource::str_init), new object_ports_init(pool, this));
            add_object(pool->get_static_string(resource::str_run), new object_ports_run(pool, this));
            add_object(pool->get_static_string(resource::str_busy), new object_ports_busy(pool, this));
            add_object(pool->get_static_string(resource::str_create), new object_ports_create(pool, this));
            add_object(pool->get_static_string(resource::str_reset), new object_ports_reset(pool, this));
            lock();
        }

        object_ports::~object_ports()
        {
            // dirty hack
            int64_t pwm_timestamp_value = pwm_timestamp.load();
            while (pwm_timestamp_value > 0) // the pwm thread is running?..
            {
                // wait
                std::this_thread::sleep_for (std::chrono::milliseconds(50));
                pwm_timestamp_value = pwm_timestamp.load();
            }
            if (pwm_timestamp_value < 0) // the pwm thread was runned but now it finished?..
            {
                if (lib::get_time_ns() + pwm_timestamp_value < 500 * 1000000) // the pwm thread is finished but not so long ago?..
                {
                    // wait a little longer
                    std::this_thread::sleep_for (std::chrono::seconds(1));
                }
            }
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
            add_object(pool->get_static_string(resource::str_bitwidth), stub);
            lock();
        }

        void object_port_proto::op_new(thread *thr, int arg_count)
        {
            thr->raise_exception(new object_exception_illegal_operation(thr->pool));
        }

        object_gpio_proto::object_gpio_proto(object_pool *pool)
            : object(pool, pool->get_port_proto_instance())
        {
        }

        void object_gpio_proto::init(object_pool *pool)
        {
            object *stub = new object_port_stub(pool, nullptr);
            add_object(pool->get_static_string(resource::str_toggle), stub);
            add_object(pool->get_static_string(resource::str_schedule), stub);
            add_object(pool->get_static_string(resource::str_count), stub);
            lock();
        }

        void object_gpio_proto::op_new(thread *thr, int arg_count)
        {
            thr->raise_exception(new object_exception_illegal_operation(thr->pool));
        }
    };
};
