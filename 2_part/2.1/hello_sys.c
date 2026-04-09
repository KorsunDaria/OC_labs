#include <unistd.h>
#include <sys/syscall.h>

int main() {
    syscall(SYS_write, 1, "Hello world\n", 12);
    return 0;
}

//strace ./hello_syscall