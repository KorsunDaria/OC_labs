#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int global_var = 100;  // Глобальная переменная

int main() {
  int local_var = 10;  // Локальная переменная
  pid_t pid;

  printf("--- До fork() ---\n");
  printf("Global: addr=%p, val=%d\n", (void*)&global_var, global_var);
  printf("Local:  addr=%p, val=%d\n", (void*)&local_var, local_var);
  printf("PID: %d\n\n", getpid());

  pid = fork();

  if (pid < 0) {
    perror("fork failed");
    exit(1);
  } else if (pid == 0) {  // Дочерний процесс
    printf("--- Дочерний процесс ---\n");
    printf("PID: %d, PPID: %d\n", getpid(), getppid());
    printf("Адреса те же: Global=%p, Local=%p\n", (void*)&global_var,
           (void*)&local_var);
    printf(
        "Значения родительских переменных в дочернем процессе : Global=%d, "
        "Local=%d\n",
        global_var, local_var);
    global_var = 200;
    local_var = 20;
    printf(" ");
    printf("Измененные значения в child: Global=%d, Local=%d\n", global_var,
           local_var);

    exit(5);
  } else {
    sleep(1);
    printf("\n--- Родительский процесс ---\n");
    printf("Значения в parent: Global=%d, Local=%d\n", global_var, local_var);
    sleep(40);

    int status;
    waitpid(pid, &status, 0);  // Ожидание завершения

    if (WIFEXITED(status)) {
      printf("\nДочерний процесс завершен. Код: %d\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
      printf("\nДочерний процесс убит сигналом: %d\n", WTERMSIG(status));
    }
  }

  return 0;
}