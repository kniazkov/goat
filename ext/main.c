#include <dlfcn.h>
#include <stdio.h>

int main(void)
{
    void *dll = dlopen("./ext_lib.dll", RTLD_NOW);
    if (dll) {
        int (*do_something)(int) = dlsym(dll, "do_something");
        if (do_something)
            printf("%d\n", do_something(16));
        dlclose(dll);
    }
    else {
        printf("dlopen() error: %s\n", dlerror());
    }
}
