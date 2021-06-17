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

#include "model/context.h"
#include "model/object_cache.h"

namespace g0at
{
    namespace model
    {
        namespace built_in
        {
            class context_factory
            {
            public:
                context_factory(object_pool *_pool);
                context *create_context(std::vector<char*> *args, std::vector<std::string> *lib_path);

            protected:
                object *create_function_print();
                object *create_function_println();
                object *create_function_getc();
                object *create_function_gets();
                object *create_function_exit();
                object *create_function_abs();
                object *create_function_sqrt();
                object *create_function_cbrt();
                object *create_function_round();
                object *create_function_sin();
                object *create_function_cos();
                object *create_function_tan();
                object *create_function_asin();
                object *create_function_acos();
                object *create_function_atan();
                object *create_function_atan2();
                object *create_function_log();
                object *create_function_log2();
                object *create_function_log10();
                object *create_function_exp();
                object *create_function_clock();
                object *create_function_range();
                object *create_function_uid();
                object *create_function_open();
                object *create_function_dlopen(std::vector<std::string> *lib_path);
                object *create_function_exec();

                object_pool *pool;
            };
        };
    };
};
