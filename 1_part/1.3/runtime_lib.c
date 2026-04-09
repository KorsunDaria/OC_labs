#include <stdio.h>

void hello_from_dyn_runtime_lib() { printf("Hello runtime_lib\n"); }
//gcc -shared -fPIC runtime_lib.c -o runtime_lib.so