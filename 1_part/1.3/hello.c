#include <dlfcn.h>
#include <stdio.h>

int main() {
  void* handle = dlopen("./runtime_lib.so", 1);
  void (*func)() = dlsym(handle, "hello_from_dyn_runtime_lib");

  func();
  dlclose(handle);

  return 0;
}
//gcc hello.c -o hello_runtime
//gcc hello.c -o hello_runtime -ldl
//gcc -fPIC -shared runtime_lib.c -o runtime_lib.so