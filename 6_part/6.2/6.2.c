#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void run_child() {
  unsigned int received;
  while (scanf("%u", &received) != EOF) {
    printf("[R] Получено число: %u\n", received);
  }
  printf("[R] Поток завершен.\n");
}

void spawn_and_redirect() {
  int pipefd[2];
  if (pipe(pipefd) == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  pid_t pid = fork();

  if (pid == -1) {
    perror("fork");
    exit(EXIT_FAILURE);
  }

  if (pid == 0) {   
    close(pipefd[1]);  

    dup2(pipefd[0], STDIN_FILENO);
    close(pipefd[0]);

    run_child();
    exit(EXIT_SUCCESS);
  } else {             
    close(pipefd[0]);  

    dup2(pipefd[1], STDOUT_FILENO);
    close(pipefd[1]);

    unsigned int numbers[] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    for (int i = 0; i < 10; i++) {
      printf("%u\n", numbers[i]);
      fflush(stdout);  
      sleep(1);       
    }


    close(STDOUT_FILENO);
    wait(NULL); 
  }
}

int main() {
  spawn_and_redirect();
  return 0;
}