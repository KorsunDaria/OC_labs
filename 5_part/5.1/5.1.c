#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>

int global_var = 100;


uintptr_t get_page(uintptr_t virtual_addr) {
    int fd = open("/proc/self/pagemap", O_RDONLY);
    if (fd < 0) {
        perror("open pagemap (run with sudo if needed)");
        return 0;
    }

    uint64_t value;
    long page_size = sysconf(_SC_PAGESIZE);
    off_t offset = (virtual_addr / page_size) * sizeof(value);

    if (pread(fd, &value, sizeof(value), offset) != sizeof(value)) {
        close(fd);
        return 0;
    }
    close(fd);

    
    if (!(value & (1ULL << 63))) {
        return 0;
    }

    uint64_t pfn = value & ((1ULL << 55) - 1);
    return pfn;
}

void print_ptrs(const char* label, int* g, int* l) {
    printf("--- %s ---\n", label);
    uintptr_t phys_g = get_page((uintptr_t)g);
    uintptr_t phys_l = get_page((uintptr_t)l);

    printf("Virt: G=%p, L=%p\n", (void*)g, (void*)l);
    printf("Phys PFN: G=0x%lx, L=0x%lx\n", phys_g, phys_l);
    printf("Val : G=%d, L=%d\n", *g, *l);
    printf("\n");
}

int main() {
    int local_var = 10;
    pid_t pid;

    print_ptrs("До fork()", &global_var, &local_var);

    pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(1);
    } 
    else if (pid == 0) { 
        printf("--- В дочернем процессе (PID: %d) ---\n", getpid());
        print_ptrs("Сразу после fork (чтение)", &global_var, &local_var);

        printf("Изменяем переменные в дочернем процессе...\n\n");
        global_var = 200;
        local_var = 20;

        print_ptrs("После изменения (запись)", &global_var, &local_var);
        
        exit(5);
    } 
    else { 
        sleep(2); 
        printf("--- В родительском процессе (PID: %d) ---\n", getpid());
        print_ptrs("После того как ребенок изменил свои копии", &global_var, &local_var);
        
        printf("Родитель засыпает (проверьте ps/maps)...\n");
        sleep(30);

        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            printf("\nДочерний процесс завершен. Код: %d\n", WEXITSTATUS(status));
        }
    }

    return 0;
}