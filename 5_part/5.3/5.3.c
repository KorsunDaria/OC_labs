#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sched.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define STACK_SIZE  (1024 * 1024)  
#define FILE_NAME   "stack_file.bin"
#define MAX_DEPTH   10


void recursive_hello(int depth)
{
    char message[] = "hello world";   

    printf("[child] depth=%d  message='%s'  &message=%p\n",
           depth, message, (void *)message);

    if (depth < MAX_DEPTH) {
        recursive_hello(depth + 1);
    }
}


int child_entry(void *arg)
{
    (void)arg;
    printf("[child] PID=%d, PPID=%d — started\n", getpid(), getppid());

    recursive_hello(1);

    printf("[child] done, exiting\n");
    return 0;          
}


int main(void)
{

    int fd = open(FILE_NAME, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) { perror("open"); return 1; }

    if (ftruncate(fd, STACK_SIZE) < 0) { perror("ftruncate"); return 1; }

    void *stack_mem = mmap(NULL, STACK_SIZE,
                           PROT_READ | PROT_WRITE,
                           MAP_SHARED,          
                           fd, 0);
    if (stack_mem == MAP_FAILED) { perror("mmap"); return 1; }

    close(fd);  

    void *stack_top = (char *)stack_mem + STACK_SIZE;

    printf("[parent] PID=%d, stack_mem=%p, stack_top=%p\n",
           getpid(), stack_mem, stack_top);


    pid_t child_pid = clone(child_entry,
                            stack_top,
                            SIGCHLD,    
                            NULL);      

    if (child_pid < 0) { perror("clone"); return 1; }

    printf("[parent] cloned child PID=%d\n", child_pid);

    int status;
    waitpid(child_pid, &status, 0);
    printf("[parent] child exited, status=%d\n", WEXITSTATUS(status));

    msync(stack_mem, STACK_SIZE, MS_SYNC);

    munmap(stack_mem, STACK_SIZE);

    printf("[parent] Done. Inspect '%s' for results.\n", FILE_NAME);
    printf("  strings %s | grep 'hello world'\n", FILE_NAME);
    printf("  hexdump -C %s | grep -A1 'hello'\n", FILE_NAME);
    return 0;
}