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
        if (is_goat_number(i->data))
        {
            n += goat_value_to_int64(i->data);
        }
        i = i->next;
    }
    if (argc > 1)
    {
        if (argv[1]->type == goat_type_thread)
        {
            goat_thread *thr = (goat_thread*)argv[1];
            run_goat_thread(env, thr, 0, NULL);
        }
    }
    goat_array *r = create_goat_array(env);
    goat_array_push_back(env, r, create_goat_integer(env, n));
    goat_array_push_back(env, r, argv[0]);
    return &r->base;
}
