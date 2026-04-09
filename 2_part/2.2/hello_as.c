#include <unistd.h>

int main() {
  const char msg[] = "Hello world\n";

  __asm__ volatile(
      "mov $1, %%rax\n"
      "mov $1, %%rdi\n"
      "lea %0, %%rsi\n"
      "mov $12, %%rdx\n"
      "syscall\n"
      :
      : "m"(msg)
      : "%rax", "%rdi", "%rsi", "%rdx");

  return 0;
}