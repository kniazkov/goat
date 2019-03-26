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

#include "object.h"

namespace g0at
{
    namespace model
    {
        enum class context_value_type
        {
            none,
            ret_address,
            catch_address
        };

        class context : public object
        {
        friend class object_pool;
        protected:
            context(object_pool *pool);
            context(object_pool *pool, context *proto);
            context(object_pool *pool, context *proto, context *parent);
            context(object_pool *pool, object *this_ptr, context *proto, context *parent);
            void reinit(object_pool *pool);
            void reinit(context *proto);
            void reinit(context *proto, context *parent);
            void reinit(object *this_ptr, context *proto, context *parent);
        public:
            void kill(object_pool *pool) override;
            void trace() override;

            context *prev;
            object *this_ptr;
            int value;
            context_value_type value_type;
            variable *ret;
            int stack_size;
        };
    };
};