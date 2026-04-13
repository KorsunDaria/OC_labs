#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(1);
    } 
    else if (pid > 0) {

        printf("Родитель (PID: %d) создал ребенка завершается!\n", getpid());
        exit(0); 
    } 
    else {

        printf("Ребенок (PID: %d). Мой изначальный родитель (PPID): %d\n", getpid(), getppid());
        

        sleep(2);
        
        printf("\nРебенок (PID: %d) проснулся.\n", getpid());
        printf("Родитель мертв. Мой НОВЫЙ усыновитель (PPID): %d\n", getppid());
        
       
        sleep(30); 
    }

    return 0;
}