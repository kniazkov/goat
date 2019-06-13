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

#pragma once

#include "object_function_user_defined.h"

namespace g0at
{
    namespace model
    {
        class object_thread : public object_function_user_defined
        {
        public:
            object_thread(object_pool *pool, int _first_iid, context *_proto_ctx);
            object_thread *to_object_thread() override;
            std::wstring to_string() const override;
        };

        class object_thread_proto : public object
        {
        friend class object_pool;
        protected:
            object_thread_proto(object_pool *pool);
            void init(object_pool *pool);
        };
    };
};