/*
 * gcc -c ext_lib.c
 * gcc ext_lib.o --shared -o ext_lib.dll
 */

#include <stdio.h>
#include "../src/goat_data.h"

goat_value * do_something(goat_ext_environment *env, int argc, goat_value **argv)
{
    goat_array *a = (goat_array*)argv[0];
    printf("it works: %d\n", a->size);
    goat_array_item *i = a->first;
    int64_t n = 0;
    while(i)
    {
        if (i->data->type == goat_type_integer)
        {
            n += ((goat_integer*)(i->data))->value;
        }
        i = i->next;
    }
    goat_array *r = create_goat_array(env);
    goat_array_push_back(env, r, create_goat_integer(env, n));
    goat_array_push_back(env, r, argv[0]);
    return &r->base;
}
