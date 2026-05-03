#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

int main() {
  size_t pg_sz = sysconf(_SC_PAGESIZE);
  void *sh_buf =
      mmap(0, pg_sz, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  unsigned int *buf = (unsigned int *)sh_buf, n = pg_sz / sizeof(int);

  if (fork() == 0) {  
    unsigned int exp = 0;
    while (1) {
      for (size_t i = 0; i < n; i++) {
        if (buf[i] != exp) {
          printf("[Reader] Сбой! Ждали %u, получили %u\n", exp, buf[i]);
          exp = buf[i]; 
        }
        exp = (exp == UINT_MAX) ? 0 : exp + 1;
      }
    }
  } else {  
    unsigned int val = 0;
    while (1) {
      for (size_t i = 0; i < n; i++)
        buf[i] = (val == UINT_MAX) ? (val = 0) : val++;
    }
  }
  return 0;
}