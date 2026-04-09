#include <dlfcn.h>
#include <stdio.h>

void hello_from_static_lib();
void hello_from_dynamic_lib();

int main() {
  hello_from_static_lib();
  hello_from_dynamic_lib();
  return 0;
}
//readelf --syms ./hello_with_symtab