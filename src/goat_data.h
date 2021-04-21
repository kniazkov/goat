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
#include <stdbool.h>
#include <memory.h>
#include <string.h>

typedef void * (*goat_allocator)(size_t size, void *mem_info);

typedef struct 
{
    goat_allocator allocator;
    void *mem_info;
} goat_ext_environment;

static __inline void * goat_alloc(goat_ext_environment* env, size_t size)
{
    return env->allocator(size, env->mem_info);
}

typedef enum
{
    goat_type_unknown = 0,
    goat_type_null,
    goat_type_boolean,
    goat_type_integer,
    goat_type_real,
    goat_type_char,
    goat_type_string,
    goat_type_array,
    goat_type_object
} goat_type;

typedef struct
{
    goat_type type;
} goat_value;

typedef goat_value * (*goat_ext_function)(goat_ext_environment* env, int argc, goat_value **argv);

typedef struct
{
    goat_value base;
    bool value;
} goat_boolean;

typedef struct
{
    goat_value base;
    int64_t value;
} goat_integer;

typedef struct
{
    goat_value base;
    double value;
} goat_real;

typedef struct
{
    goat_value base;
    wchar_t value;
} goat_char;

typedef struct
{
    goat_value base;
    wchar_t *value;
    size_t value_length;
} goat_string;

typedef struct goat_array_item goat_array_item;

struct goat_array_item
{
    goat_array_item *next;
    goat_value *data;
};

typedef struct
{
    goat_value base;
    goat_array_item *first;
    goat_array_item *last;
    int size;
} goat_array;

typedef struct goat_object_record goat_object_record;

struct goat_object_record
{
    goat_object_record *next;
    wchar_t *key;
    size_t key_length;
    goat_value *data;
};

typedef struct
{
    goat_value base;
    goat_object_record *first;
    goat_object_record *last;
} goat_object;

static __inline goat_value * create_goat_unknown_value(goat_ext_environment *env)
{
    goat_value *obj = (goat_value*)goat_alloc(env, sizeof(goat_value));
    obj->type = goat_type_unknown;
    return obj;
}

static __inline goat_value * create_goat_null(goat_ext_environment *env)
{
    goat_value *obj = (goat_value*)goat_alloc(env, sizeof(goat_value));
    obj->type = goat_type_null;
    return obj;
}

static __inline goat_value * create_goat_boolean(goat_ext_environment *env, bool value)
{
    goat_boolean *obj = (goat_boolean*)goat_alloc(env, sizeof(goat_boolean));
    obj->base.type = goat_type_boolean;
    obj->value = value;
    return (goat_value*)obj;
}

static __inline goat_value * create_goat_integer(goat_ext_environment *env, int64_t value)
{
    goat_integer *obj = (goat_integer*)goat_alloc(env, sizeof(goat_integer));
    obj->base.type = goat_type_integer;
    obj->value = value;
    return (goat_value*)obj;
}

static __inline goat_value * create_goat_real(goat_ext_environment *env, double value)
{
    goat_real *obj = (goat_real*)goat_alloc(env, sizeof(goat_real));
    obj->base.type = goat_type_real;
    obj->value = value;
    return (goat_value*)obj;
}

static __inline goat_value * create_goat_char(goat_ext_environment *env, wchar_t value)
{
    goat_char *obj = (goat_char*)goat_alloc(env, sizeof(goat_char));
    obj->base.type = goat_type_char;
    obj->value = value;
    return (goat_value*)obj;
}

static __inline goat_value * create_goat_string_ext(goat_ext_environment *env, const wchar_t *value, size_t value_length)
{
    goat_string *obj = (goat_string*)goat_alloc(env, sizeof(goat_string));
    obj->base.type = goat_type_string;
    obj->value = (wchar_t*)goat_alloc(env, sizeof(wchar_t) * (value_length + 1));
    memcpy(obj->value, value, sizeof(wchar_t) * value_length);
    obj->value[value_length] = L'\0';
    obj->value_length = value_length;
    return (goat_value*)obj;
}

static __inline goat_value * create_goat_string(goat_ext_environment *env, const wchar_t *value)
{
    return create_goat_string_ext(env, value, value ? wcslen(value) : 0);
}

static __inline goat_array * create_goat_array(goat_ext_environment *env)
{
    goat_array *obj = (goat_array*)goat_alloc(env, sizeof(goat_array));
    obj->base.type = goat_type_array;
    obj->first = NULL;
    obj->last = NULL;
    obj->size = 0;
    return obj;
}

static __inline void goat_array_push_back(goat_ext_environment *env, goat_array* obj, goat_value *value)
{
    goat_array_item *item = (goat_array_item*)goat_alloc(env, sizeof(goat_array_item));
    item->data = value;
    item->next = NULL;
    if (obj->last)
        obj->last->next = item;
    else
        obj->first = item;
    obj->last = item;
    obj->size++;
}

static __inline goat_object * create_goat_object(goat_ext_environment *env)
{
    goat_object *obj = (goat_object*)goat_alloc(env, sizeof(goat_object));
    obj->base.type = goat_type_object;
    obj->first = NULL;
    obj->last = NULL;
    return obj;
}

static __inline void goat_object_add_record_ext(goat_ext_environment *env, goat_object* obj, 
    const wchar_t *key, size_t key_length, goat_value *value)
{
    goat_object_record *rec = (goat_object_record*)goat_alloc(env, sizeof(goat_object_record));
    rec->key = (wchar_t*)goat_alloc(env, sizeof(wchar_t) * (key_length + 1));
    memcpy(rec->key, key, sizeof(wchar_t) * key_length);
    rec->key[key_length] = L'\0';
    rec->key_length = key_length;
    rec->data = value;
    rec->next = NULL;
    if (obj->last)
        obj->last->next = rec;
    else
        obj->first = rec;
    obj->last = rec;
}

static __inline void goat_object_add_record(goat_ext_environment *env, goat_object* obj,
    const wchar_t *key, goat_value *value)
{
    goat_object_add_record_ext(env, obj, key, key ? wcslen(key) : 0, value);
}

static __inline bool is_goat_number(goat_value *obj)
{
    return obj->type == goat_type_integer || obj->type == goat_type_real;
}

static __inline int64_t goat_value_to_int64(goat_value *obj)
{
    if (obj->type == goat_type_integer)
        return ((goat_integer*)obj)->value;
    if (obj->type == goat_type_real)
        return (int64_t)((goat_real*)obj)->value;
    return 0;
}

static __inline double goat_value_to_double(goat_value *obj)
{
    if (obj->type == goat_type_integer)
        return (double)((goat_integer*)obj)->value;
    if (obj->type == goat_type_real)
        return ((goat_real*)obj)->value;
    return 0;
}
