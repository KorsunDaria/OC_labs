#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid_father = fork();

    if (pid_father < 0) {
        perror("fork father failed");
        exit(1);
    }

    if (pid_father > 0) {
        printf("[Дед] PID: %d, мой ребенок (Папа) имеет PID: %d\n", getpid(), pid_father);
        
        printf("[Дед] Сплю 15 секунд... (В это время Папа будет находиться в состоянии 'Zombie').\n");
        sleep(30);

        wait(NULL);
        printf("[Дед] Папа успешно собран. Завершаюсь.\n");
    } 
    else {
        printf("[Папа] PID: %d, мой родитель (Дед): %d\n", getpid(), getppid());
        
        pid_t pid_son = fork();
        
        if (pid_son < 0) {
            perror("fork son failed");
            exit(1);
        }

        if (pid_son > 0) {
           
            printf("[Папа] Создал Сына с PID: %d. Завершаю работу.\n", pid_son);
            exit(0); 
        } 
        else {

            sleep(2);
            printf("[Сын] PID: %d. Мой живой Папа (PPID): %d\n", getpid(), getppid());
            
            
            printf("[Сын] PID: %d. Папа завершился. Мой НОВЫЙ родитель (PPID): %d\n", getpid(), getppid());
            
            sleep(20);
            printf("[Сын] Завершаю работу.\n");
            exit(0);
        }
    }

    return 0;
}