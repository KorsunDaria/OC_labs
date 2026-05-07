#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/echo_sock"
#define BUFFER_SIZE 256

int main() {
    int fd;
    struct sockaddr_un addr;
    char buffer[BUFFER_SIZE];

    if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket error");
        exit(EXIT_FAILURE);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    if (connect(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("connect error");
        exit(EXIT_FAILURE);
    }

    printf("Введите сообщение (для выхода Ctrl+C):\n");
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        write(fd, buffer, strlen(buffer));
        int n = read(fd, buffer, sizeof(buffer));
        printf("Эхо от сервера: %.*s", n, buffer);
    }

    close(fd);
    return 0;
}