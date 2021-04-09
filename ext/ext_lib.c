/*
 * gcc -c ext_lib.c
 * gcc ext_lib.o --shared -o ext_lib.dll
 */

#include <stdio.h>

void do_something()
{
    printf("it works.\n");
}
