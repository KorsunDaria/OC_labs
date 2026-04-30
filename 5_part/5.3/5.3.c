#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sched.h>
#include <sys/mman.h>
#include <sys/wait.h>

#define STACK_SIZE (64 * 1024)
#define DEPTH      10
#define MSG        "hello world"

void recurse(int n)
{
    char buf[64];
    memcpy(buf, MSG, sizeof(MSG));

    void *ret_addr = __builtin_return_address(0);

    printf("[child] depth=%d  buf@%p  ret@%p\n", n, buf, ret_addr);
    
    if (n < DEPTH)
        recurse(n + 1);
}

int child_fn(void *arg)
{
    printf("[child] pid=%d ppid=%d\n", getpid(), getppid());
    recurse(1);
    return 0;
}

int main(void)
{
    int fd = open("stack.bin", O_RDWR | O_CREAT | O_TRUNC, 0600);
    if (fd < 0) { perror("open"); exit(1); }
    ftruncate(fd, STACK_SIZE);

    void *stack = mmap(NULL, STACK_SIZE, PROT_READ | PROT_WRITE,
                       MAP_SHARED, fd, 0);
    if (stack == MAP_FAILED) { perror("mmap"); exit(1); }
    close(fd);

    void *stacktop = (char *)stack + STACK_SIZE;

    printf("[parent] pid=%d  stack file: stack.bin\n", getpid());

    pid_t pid = clone(child_fn, stacktop, SIGCHLD, NULL);
    if (pid < 0) { perror("clone"); exit(1); }

    waitpid(pid, NULL, 0);

    munmap(stack, STACK_SIZE);

    printf("[parent] done\n");
}