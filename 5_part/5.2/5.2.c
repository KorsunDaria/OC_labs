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
    else if (pid == 0) {
        printf("Ребенок (PID: %d) завершается и становится ЗОМБИ.\n", getpid());
        exit(0); 
    } 
    else {

        printf("Родитель (PID: %d) засыпает на 60 секунд!\n", getpid());

        
        sleep(60); 
        
    }

    return 0;
}