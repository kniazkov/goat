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

#pragma once

#include "object.h"
#include <atomic>

namespace g0at
{
    namespace model
    {
        class object_boolean_port;

        class object_ports : public object
        {
        friend class object_ports_init;
        friend class object_ports_run;
        friend class object_ports_busy;
        public:
            object_ports(object_pool *pool);
            ~object_ports();

        private:
            bool initialized;
            object_boolean_port *list;
            std::atomic_int64_t pwm_timestamp;
        };

        class object_port_proto : public object
        {
        friend class object_pool;
        protected:
            object_port_proto(object_pool *pool);
            void init(object_pool *pool);
        public:
            void op_new(thread *thr, int arg_count) override;
        };

        class object_gpio_proto : public object
        {
        friend class object_pool;
        protected:
            object_gpio_proto(object_pool *pool);
            void init(object_pool *pool);
        public:
            void op_new(thread *thr, int arg_count) override;
        };
    };
};
