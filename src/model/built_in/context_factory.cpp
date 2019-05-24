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

#include "context_factory.h"
#include "model/object_string.h"
#include <cmath>

namespace g0at
{
    namespace model
    {
        namespace built_in
        {
            context_factory::context_factory(object_pool *_pool)
                : pool(_pool)
            {
            }

            context *context_factory::create_context()
            {
                context *ctx = pool->create_context();

                ctx->add_object(pool->get_static_string(L"Object"), pool->get_generic_proto_instance());
                ctx->add_object(pool->get_static_string(L"Void"), pool->get_void_instance());
                ctx->add_object(pool->get_static_string(L"Undefined"), pool->get_undefined_instance());
                ctx->add_object(pool->get_static_string(L"Null"), pool->get_null_instance());
                ctx->add_object(pool->get_static_string(L"Char"), pool->get_char_proto_instance());
                ctx->add_object(pool->get_static_string(L"String"), pool->get_string_proto_instance());
                ctx->add_object(pool->get_static_string(L"Number"), pool->get_number_proto_instance());
                ctx->add_object(pool->get_static_string(L"Integer"), pool->get_integer_proto_instance());
                ctx->add_object(pool->get_static_string(L"Function"), pool->get_function_proto_instance());
                ctx->add_object(pool->get_static_string(L"Boolean"), pool->get_boolean_proto_instance());
                ctx->add_object(pool->get_static_string(L"Real"), pool->get_real_proto_instance());
                ctx->add_object(pool->get_static_string(L"Array"), pool->get_array_proto_instance());
                ctx->add_object(pool->get_static_string(L"Exception"), pool->get_exception_proto_instance());

                ctx->add_object(pool->get_static_string(L"print"), create_function_print());
                ctx->add_object(pool->get_static_string(L"println"), create_function_println());
                ctx->add_object(pool->get_static_string(L"exit"), create_function_exit());
                ctx->add_object(pool->get_static_string(L"abs"), create_function_abs());
                ctx->add_object(pool->get_static_string(L"sqrt"), create_function_sqrt());
                ctx->add_object(pool->get_static_string(L"cbrt"), create_function_cbrt());
                ctx->add_object(pool->get_static_string(L"sin"), create_function_sin());
                ctx->add_object(pool->get_static_string(L"atan2"), create_function_atan2());
                ctx->add_object(pool->get_static_string(L"clock"), create_function_clock());

                model::variable tmp;
                tmp.set_real(std::atan((double)1.0) * 4);
                ctx->add_object(pool->get_static_string(L"PI"), tmp);

                return pool->create_context(ctx);
            }
        };
    };
};
