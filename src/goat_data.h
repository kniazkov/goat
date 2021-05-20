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
#include <wchar.h>
#include <stddef.h>

typedef struct goat_value goat_value;
typedef struct goat_shell goat_shell;
typedef struct goat_allocator goat_allocator;
typedef struct goat_function_caller goat_function_caller;
typedef struct goat_thread_runner goat_thread_runner;

typedef goat_value * (*goat_ext_function)(const goat_shell* shell, const goat_allocator *allocator, int argc, goat_value **argv);
typedef void * (*goat_function_alloc)(void *memory_map, size_t size);
typedef goat_value * (*goat_function_call_function)(void *function_ptr, void *function_caller_data,
    const goat_allocator *allocator, int argc, goat_value **argv);
typedef bool (*goat_function_run_thread)(void *thread_ptr, void *thread_runner_data,
    const goat_allocator *allocator, int argc, goat_value **argv);
typedef goat_allocator * (*goat_function_create_allocator)(void *thread_runner_data);

struct goat_allocator
{
    goat_function_alloc alloc;
    void *memory_map;
};

struct goat_function_caller
{
    goat_function_call_function call_function;
    void *data;
};

struct goat_thread_runner
{
    goat_function_create_allocator create_allocator;
    goat_function_run_thread run_thread;
    void *data;
};

struct goat_shell
{
    const goat_function_caller *function_caller;
    const goat_thread_runner *thread_runner;
};

static __inline void * goat_alloc(const goat_allocator* allocator, size_t size)
{
    return allocator->alloc(allocator->memory_map, size);
}

typedef enum
{
    goat_type_invalid = 0,
    goat_type_unknown,
    goat_type_null,
    goat_type_boolean,
    goat_type_integer,
    goat_type_real,
    goat_type_char,
    goat_type_string,
    goat_type_array,
    goat_type_object,
    goat_type_byte_array,
    goat_type_function,
    goat_type_thread,
    goat_type_raw_data
} goat_type;

struct goat_value
{
    goat_type type;
};

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
    const wchar_t *value;
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
    const wchar_t *key;
    size_t key_length;
    goat_value *value;
};

typedef struct
{
    goat_value base;
    goat_object_record *first;
    goat_object_record *last;
} goat_object;

typedef struct
{
    goat_value base;
    const uint8_t *data;
    size_t length;
} goat_byte_array;

typedef struct
{
    goat_value base;
    void *ir_ptr;
} goat_function;

typedef struct
{
    goat_value base;
    void *ir_ptr;
} goat_thread;

typedef struct
{
    goat_value base;
    void *raw_data;
    const char *descriptor;
} goat_raw_data;

static __inline goat_value * create_goat_unknown_value(const goat_allocator *allocator)
{
    goat_value *obj = (goat_value*)goat_alloc(allocator, sizeof(goat_value));
    obj->type = goat_type_unknown;
    return obj;
}

static __inline goat_value * create_goat_null(const goat_allocator *allocator)
{
    goat_value *obj = (goat_value*)goat_alloc(allocator, sizeof(goat_value));
    obj->type = goat_type_null;
    return obj;
}

static __inline goat_value * create_goat_boolean(const goat_allocator *allocator, bool value)
{
    goat_boolean *obj = (goat_boolean*)goat_alloc(allocator, sizeof(goat_boolean));
    obj->base.type = goat_type_boolean;
    obj->value = value;
    return (goat_value*)obj;
}

static __inline goat_value * create_goat_integer(const goat_allocator *allocator, int64_t value)
{
    goat_integer *obj = (goat_integer*)goat_alloc(allocator, sizeof(goat_integer));
    obj->base.type = goat_type_integer;
    obj->value = value;
    return (goat_value*)obj;
}

static __inline goat_value * create_goat_real(const goat_allocator *allocator, double value)
{
    goat_real *obj = (goat_real*)goat_alloc(allocator, sizeof(goat_real));
    obj->base.type = goat_type_real;
    obj->value = value;
    return (goat_value*)obj;
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

static __inline goat_value * create_goat_char(const goat_allocator *allocator, wchar_t value)
{
    goat_char *obj = (goat_char*)goat_alloc(allocator, sizeof(goat_char));
    obj->base.type = goat_type_char;
    obj->value = value;
    return (goat_value*)obj;
}

static __inline goat_value * create_goat_string_from_c_string_ext(const goat_allocator *allocator, const char *value, size_t value_length)
{
    goat_string *obj = (goat_string*)goat_alloc(allocator, sizeof(goat_string));
    size_t i;
    obj->base.type = goat_type_string;
    wchar_t *buff = (wchar_t*)goat_alloc(allocator, sizeof(wchar_t) * (value_length + 1));
    for(i = 0; i < value_length; i++)
        buff[i] = (wchar_t)value[i];
    buff[value_length] = L'\0';
    obj->value = buff;
    obj->value_length = value_length;
    return (goat_value*)obj;
}

static __inline goat_value * create_goat_string_from_c_string(const goat_allocator *allocator, const char *value)
{
    return create_goat_string_from_c_string_ext(allocator, value, value ? strlen(value) : 0);
}

static __inline goat_value * create_goat_string_ext(const goat_allocator *allocator, const wchar_t *value, size_t value_length)
{
    goat_string *obj = (goat_string*)goat_alloc(allocator, sizeof(goat_string));
    obj->base.type = goat_type_string;
    wchar_t *buff = (wchar_t*)goat_alloc(allocator, sizeof(wchar_t) * (value_length + 1));
    memcpy(buff, value, sizeof(wchar_t) * value_length);
    buff[value_length] = L'\0';
    obj->value = buff;
    obj->value_length = value_length;
    return (goat_value*)obj;
}

static __inline goat_value * create_goat_string(const goat_allocator *allocator, const wchar_t *value)
{
    return create_goat_string_ext(allocator, value, value ? wcslen(value) : 0);
}

static __inline goat_array * create_goat_array(const goat_allocator *allocator)
{
    goat_array *obj = (goat_array*)goat_alloc(allocator, sizeof(goat_array));
    obj->base.type = goat_type_array;
    obj->first = NULL;
    obj->last = NULL;
    obj->size = 0;
    return obj;
}

static __inline void goat_array_push_back(const goat_allocator *allocator, goat_array* obj, goat_value *value)
{
    goat_array_item *item = (goat_array_item*)goat_alloc(allocator, sizeof(goat_array_item));
    item->data = value;
    item->next = NULL;
    if (obj->last)
        obj->last->next = item;
    else
        obj->first = item;
    obj->last = item;
    obj->size++;
}

static __inline goat_object * create_goat_object(const goat_allocator *allocator)
{
    goat_object *obj = (goat_object*)goat_alloc(allocator, sizeof(goat_object));
    obj->base.type = goat_type_object;
    obj->first = NULL;
    obj->last = NULL;
    return obj;
}

static __inline void goat_object_add_record_ext(const goat_allocator *allocator, goat_object* obj, 
    bool copy_key, const wchar_t *key, size_t key_length, goat_value *value)
{
    goat_object_record *rec = (goat_object_record*)goat_alloc(allocator, sizeof(goat_object_record));
    if (copy_key)
    {
        wchar_t *copy = (wchar_t*)goat_alloc(allocator, sizeof(wchar_t) * (key_length + 1));
        memcpy(copy, key, sizeof(wchar_t) * key_length);
        copy[key_length] = L'\0';
        rec->key = copy;
    }
    else
    {
        rec->key = key;
    }
    rec->key_length = key_length;
    rec->value = value;
    rec->next = NULL;
    if (obj->last)
        obj->last->next = rec;
    else
        obj->first = rec;
    obj->last = rec;
}

static __inline void goat_object_add_record(const goat_allocator *allocator, goat_object* obj,
    const wchar_t *key, goat_value *value)
{
    goat_object_add_record_ext(allocator, obj, true, key, key ? wcslen(key) : 0, value);
}

static __inline goat_value * create_goat_byte_array(const goat_allocator *allocator, bool copy_data, const uint8_t *data, size_t length)
{
    goat_byte_array *obj = (goat_byte_array*)goat_alloc(allocator, sizeof(goat_byte_array));
    obj->base.type = goat_type_byte_array;
    if (copy_data)
    {
        uint8_t *copy = (uint8_t*)goat_alloc(allocator, length);
        memcpy(copy, data, length);
        obj->data = copy;
    }
    else
    {
        obj->data = data;
    }
    obj->length = length;
    return (goat_value*)obj;
}

static __inline goat_value * create_goat_function(const goat_allocator *allocator, void *ir_ptr)
{
    goat_function *obj = (goat_function*)goat_alloc(allocator, sizeof(goat_function));
    obj->base.type = goat_type_function;
    obj->ir_ptr = ir_ptr;
    return (goat_value*)obj;
}

static __inline goat_value * call_goat_function(const goat_function_caller* caller, goat_function *func,
    const goat_allocator *allocator, int argc, goat_value **argv)
{
    return caller->call_function(func->ir_ptr, caller->data, allocator, argc, argv);
}

static __inline goat_value * create_goat_thread(const goat_allocator *allocator, void *ir_ptr)
{
    goat_thread *obj = (goat_thread*)goat_alloc(allocator, sizeof(goat_thread));
    obj->base.type = goat_type_thread;
    obj->ir_ptr = ir_ptr;
    return (goat_value*)obj;
}

static __inline bool run_goat_thread(const goat_thread_runner* runner, goat_thread *thread,
    const goat_allocator *allocator, int argc, goat_value **argv)
{
    return runner->run_thread(thread->ir_ptr, runner->data, allocator, argc, argv);
}

static __inline goat_allocator * create_goat_allocator(const goat_thread_runner* runner)
{
    return runner->create_allocator(runner->data);
}

static __inline goat_value * create_goat_raw_data(const goat_allocator *allocator, void *raw_data,
    const char *descriptor)
{
    goat_raw_data *obj = (goat_raw_data*)goat_alloc(allocator, sizeof(goat_raw_data));
    obj->base.type = goat_type_raw_data;
    obj->raw_data = raw_data;
    obj->descriptor = descriptor;
    return (goat_value*)obj;
}
