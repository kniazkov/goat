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

#include "variable_wrapper.h"

namespace g0at
{
    namespace model
    {
        class object_real : public variable_wrapper
        {
        friend class object_pool;
        protected:
            object_real(object_pool *pool, double value);
            void reinit(double value);

        public:
#ifndef GC_DEBUG
            void kill(object_pool *pool) override;
#endif            
            object_type get_type() const override;
            object_real *to_object_real() override;
            bool less(const object *obj) const override;

            void m_iterator(thread *thr, int arg_count) override;

            double get_value() { return var.data.r; }
        };

        class object_real_proto : public object
        {
        friend class object_pool;
        protected:
            object_real_proto(object_pool *pool);
            void init(object_pool *pool);
        public:
            void op_new(thread *thr, int arg_count) override;
        };
    };
};