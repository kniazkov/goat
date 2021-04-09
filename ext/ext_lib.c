/*
 * gcc -c ext_lib.c
 * gcc ext_lib.o --shared -o ext_lib.dll
 */

#include <stdio.h>
#include "../src/goat_data.h"

goat_value * do_something(goat_ext_environment *env, int argc, goat_value **argv)
{
    printf("it works.\n");
    goat_integer *v = (goat_integer*)argv[0];
    return create_goat_integer(env, -(v->value + 1));
}
