/*
 * gcc -c ext_lib.c
 * gcc ext_lib.o --shared -o ext_lib.dll
 */

int do_something(int x)
{
    return x * x + 1;
}
