#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    printf("PID: %d\n", getpid());
    sleep(1);
    execv(argv[0],argv);

    printf("hello");
    return 0;
}

//watch -d -n1 'cat /proc/34052/maps'
