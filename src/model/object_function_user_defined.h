/*

Copyright (C) 2017-2021 Ivan Kniazkov

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

#include "object_function.h"
#include "code/iid_t.h"
#include <vector>

namespace g0at
{
    namespace model
    {
        class object_function_user_defined : public object_function
        {
        public:
            object_function_user_defined(object_pool *pool, code::iid_t _first_iid, context *_proto_ctx);
            object_function_user_defined * to_object_function_user_defined() override;
            void call(thread *thr, int arg_count, call_mode mode) override;
            void trace() override;
            void trace_parallel(object_pool *pool) override;
            goat_value * get_value(const goat_allocator *allocator) override;

            code::iid_t get_first_iid() { return first_iid; }
            void add_arg_name(object *arg_name) { arg_names.push_back(arg_name); }
            int get_arg_names_count() { return (int)arg_names.size(); }
            object * get_arg_name(int index) { return arg_names.at(index); }
            context *get_proto_ctx() { return proto_ctx; }

        protected:
            code::iid_t first_iid;
            context *proto_ctx;
            std::vector<object *> arg_names;
        };
    };
};