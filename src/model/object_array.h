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

#include "object_array_like.h"
#include <vector>

namespace g0at
{
    namespace model
    {
        class object_array : public object_array_like
        {
        friend class object_pool;
        protected:
            object_array(object_pool *pool);
            void reinit();

        public:
#ifndef GC_DEBUG
            void kill(object_pool *pool) override;
#endif            
            object_array *to_object_array() override;
            void trace() override;
            void trace_parallel(object_pool *pool) override;

            void op_inherit(thread *thr) override;
            void op_add(thread *thr) override;
            void m_set(thread *thr, int arg_count) override;
            void m_clone(thread *thr, int arg_count) override;

            void add_item(variable item) { vector.push_back(item); }
            int get_length() const override;
            variable get_item(int idx) const override;
            variable get_item(int idx) override;

        protected:
            std::vector<variable> vector;
        };

        class object_array_proto : public object
        {
        friend class object_pool;
        protected:
            object_array_proto(object_pool *pool);
            void init(object_pool *pool);
        public:
            void op_new(thread *thr, int arg_count) override;
        };
    };
};
