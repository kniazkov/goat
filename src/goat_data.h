/*

Copyright (C) 2021 Ivan Kniazkov

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

#include <stdint.h>

typedef void * (*goat_allocator)(size_t size, void *mem_info);

typedef struct 
{
    goat_allocator allocator;
    void *mem_info;
} goat_native_environment;

typedef enum
{
    goat_type_unknown = 0,
    goat_type_integer
} goat_type;

typedef struct
{
    goat_type type;
} goat_value;

typedef struct
{
    goat_type type;
    int64_t value;
} goat_integer;

static __inline goat_value * create_goat_unknown_value(goat_native_environment *env)
{
    goat_value *obj = (goat_value*)env->allocator(sizeof(goat_value), env->mem_info);
    obj->type = goat_type_unknown;
    return obj;
}

static __inline goat_value * create_goat_integer(goat_native_environment *env, int64_t value)
{
    goat_integer *obj = (goat_integer*)env->allocator(sizeof(goat_integer), env->mem_info);
    obj->type = goat_type_integer;
    obj->value = value;
    return (goat_value*)obj;
}
