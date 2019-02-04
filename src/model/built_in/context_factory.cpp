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

namespace g0at
{
    namespace model
    {
        namespace built_in
        {
            context_factory::context_factory(object_list *_list, object_cache *_cache)
                : list(_list), cache(_cache)
            {
            }

            context *context_factory::create_context()
            {
                context *ctx = new context(list);

                ctx->add_object(cache->get_object(L"Void"), list->get_void_instance());
                ctx->add_object(cache->get_object(L"String"), list->get_string_proto_instance());
                ctx->add_object(cache->get_object(L"Integer"), list->get_integer_proto_instance());
                ctx->add_object(cache->get_object(L"Function"), list->get_function_proto_instance());

                ctx->add_object(cache->get_object(L"print"), create_function_print());
                ctx->add_object(cache->get_object(L"println"), create_function_println());
                ctx->add_object(cache->get_object(L"exit"), create_function_exit());
                return ctx;
            }
        };
    };
};
