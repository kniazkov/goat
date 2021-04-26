/*
 * gcc -c ext_lib.c
 * gcc ext_lib.o --shared -o ext_lib.dll
 */

#include <stdio.h>
#include "../src/goat_data.h"

goat_value * do_something(const goat_shell *shell, const goat_allocator *allocator, int arg_count, goat_value **arg_list)
{
    goat_array *a = (goat_array*)arg_list[0];
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
    if (arg_count > 1)
    {
        if (arg_list[1]->type == goat_type_thread)
        {
            goat_allocator *ta = create_goat_allocator(shell->thread_runner);
            goat_value *list[] = {
                create_goat_integer(ta, 13),
                create_goat_raw_data(ta, (void*)"goat", "test")
            };
            goat_thread *thr = (goat_thread*)arg_list[1];

            run_goat_thread(shell->thread_runner, thr, ta, 2, list);
        }
    }
    goat_array *r = create_goat_array(allocator);
    goat_array_push_back(allocator, r, create_goat_integer(allocator, n));
    goat_array_push_back(allocator, r, arg_list[0]);
    return &r->base;
}
