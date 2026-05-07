#include <limits.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    int fd[2];
    if (pipe(fd) == -1) return 1;

    pid_t pid = fork();
    if (pid == -1) return 1;

    if (pid == 0) {  
        close(fd[1]); 
        unsigned int val, expected = 0;

        while (read(fd[0], &val, sizeof(val)) > 0) {
            if (val != expected) {
                printf("[R] Ошибка: ждали %u, получили %u\n", expected, val);
                expected = val;
            }
            if (val % 50000 == 0) {
                printf("[R] Получено: %u\n", val);
            }
            expected = (expected == UINT_MAX) ? 0 : expected + 1;
        }
        close(fd[0]);
        printf("[R] Читатель закончил.\n");
        return 0;
    } else {  
        close(fd[0]); 
        unsigned int val = 0;
        

        for (int i = 0; i < 300000; i++) {
            if (write(fd[1], &val, sizeof(val)) <= 0) break;
            val = (val == UINT_MAX) ? 0 : val + 1;
        }
        
        close(fd[1]); 
        wait(NULL);   
        printf("[W] Писатель закончил.\n");
    }
    return 0;
}