#define _GNU_SOURCE  // 1. Сначала это!
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>   // 2. Потом это

int main() {
    int my_pipe[2]; // Объявляем массив
    
    if (pipe(my_pipe) == -1) {
        perror("pipe");
        return 1;
    }

    // Получаем размер буфера
    int capacity = fcntl(my_pipe[0], F_GETPIPE_SZ);
    
    if (capacity == -1) {
        perror("fcntl");
    } else {
        printf("Текущая емкость пайпа: %d байт\n", capacity);
    
    }

    return 0;
}