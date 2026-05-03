#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

void *sh_buf;
size_t pg_sz;
pid_t pid;
volatile sig_atomic_t got_sig = 0, stop = 0, sig_type = 0;

void hdl(int s) {
  if (s == SIGINT || s == SIGTERM) {
    stop = 1;
    sig_type = s;
  } else
    got_sig = 1;
}

void cleanup() { munmap(sh_buf, pg_sz); }

int main() {
  pg_sz = sysconf(_SC_PAGESIZE);
  sh_buf =
      mmap(0, pg_sz, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  unsigned int *buf = (unsigned int *)sh_buf, n = pg_sz / sizeof(int);

  struct sigaction sa = {.sa_handler = hdl};
  sigemptyset(&sa.sa_mask);
  sigaction(SIGUSR1, &sa, NULL);
  sigaction(SIGUSR2, &sa, NULL);
  sigaction(SIGINT, &sa, NULL);
  sigaction(SIGTERM, &sa, NULL);

  sigset_t mask, old;
  sigemptyset(&mask);
  sigaddset(&mask, SIGUSR1);
  sigaddset(&mask, SIGUSR2);
  sigprocmask(SIG_BLOCK, &mask, &old);

  if ((pid = fork()) == 0) {
    unsigned int exp = 0;
    while (!stop) {
      while (!got_sig && !stop) sigsuspend(&old);
      if (stop) break;
      got_sig = 0;
      for (size_t i = 0; i < n; i++) {
        if (buf[i] != exp) {
          printf("[Reader] Сбой: ждали %u, получили %u\n", exp, buf[i]);
          exp = buf[i];
        }
        exp = (exp == UINT_MAX) ? 0 : exp + 1;
      }
      kill(getppid(), SIGUSR2);
    }
    printf("[Reader] %s\n",
           sig_type == SIGINT ? "Получен SIGINT" : "Останов писателя");
    cleanup();
    exit(0);
  }

  unsigned int val = 0;
  while (!stop) {
    for (size_t i = 0; i < n; i++)
      buf[i] = (val == UINT_MAX) ? (val = 0) : val++;

    if (kill(pid, SIGUSR1) == -1) break;
    while (!got_sig && !stop) {
      if (sigsuspend(&old) == -1 && errno == ECHILD) stop = 1;
    }
    got_sig = 0;
  }
  printf("[Writer] %s\n",
         sig_type == SIGINT ? "Получен SIGINT" : "Останов читателя");
  kill(pid, SIGTERM);  // Оповещаем второго, если выходим по SIGINT
  cleanup();
  return 0;
}