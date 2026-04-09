#include <stdio.h>

void hello_from_dynamic_lib() {
    printf("Hello dynamic_lib\n");
}
// gcc -c -fPIC dynamic_lib.c -o dynamic_lib.o
// gcc -shared dynamic_lib.o -o dynamic_lib.so