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

namespace g0at
{
    namespace model
    {
        class hw_port
        {
        public:
            virtual ~hw_port() { }
            virtual variable get(object_pool *pool) = 0;
            virtual bool set(variable value) = 0;
        };

        class object_port : public object
        {
        public:
            object_port(object_pool *pool);
            ~object_port();
            object_port * to_object_port() override;

            void m_get(thread *thr, int arg_count) override;
            void m_set(thread *thr, int arg_count) override;
            void m_iterator(thread *thr, int arg_count) override;
            void m_clone(thread *thr, int arg_count) override;
        
        private:
            std::map<object*, hw_port*> ports;
        };
    };
};
