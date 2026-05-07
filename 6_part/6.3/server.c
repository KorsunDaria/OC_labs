#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/echo_sock"
#define BUFFER_SIZE 256

int main() {
    int server_fd, client_fd;
    struct sockaddr_un addr;
    char buffer[BUFFER_SIZE];

    if ((server_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket error");
        exit(EXIT_FAILURE);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    // Удаляем старый файл сокета, если он остался
    unlink(SOCKET_PATH);

    // 2. Привязка сокета (bind)
    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("bind error");
        exit(EXIT_FAILURE);
    }


    if (listen(server_fd, 5) == -1) {
        perror("listen error");
        exit(EXIT_FAILURE);
    }

    printf("Сервер запущен. Ожидание клиентов...\n");

    while (1) {
        if ((client_fd = accept(server_fd, NULL, NULL)) == -1) {
            perror("accept error");
            continue;
        }

       
        if (fork() == 0) {
            close(server_fd); 
            int bytes_read;
            while ((bytes_read = read(client_fd, buffer, sizeof(buffer))) > 0) {
                printf("Получено и отправлено назад: %.*s", bytes_read, buffer);
                write(client_fd, buffer, bytes_read);
            }
            close(client_fd);
            exit(0); 
        }
        close(client_fd); 
    }

    return 0;
}