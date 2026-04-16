#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>



int main() {
    int pagemap_fd = open("/proc/self/pagemap", O_RDONLY);
    FILE *maps_file = fopen("/proc/self/maps", "r");
    
    int page_size = sysconf(_SC_PAGESIZE);
    printf("PID: %d\n\n", getpid());
    
    char line[256];
    int count = 0;
    
    while (fgets(line, sizeof(line), maps_file) && count < 10) {
        unsigned long start, end;
        sscanf(line, "%lx-%lx", &start, &end);
        
        off_t offset = (start / page_size) * sizeof(uint64_t);
        lseek(pagemap_fd, offset, SEEK_SET);
        
        for (unsigned long addr = start; addr < end && count < 10; addr += page_size) {
            uint64_t entry;
            if (read(pagemap_fd, &entry, sizeof(entry)) != sizeof(entry))
                break;
            
            int present = (entry >> 63) & 1;
            uint64_t pfn = entry & ((1ULL << 55) - 1);
            unsigned long phys = present ? pfn * page_size : 0;
            
            char flags[4] = "---";
            if (present) flags[0] = 'P';
            if ((entry >> 61) & 1) flags[1] = 'F';
            else if (present) flags[1] = 'A';
            if ((entry >> 62) & 1) flags[2] = 'S';
            
            printf("0x%016lx, 0x%016lx, %s\n", addr, phys, flags);
            count++;
        }
    }
    
    printf("\n(First %d pages)\n", count);
    
    fclose(maps_file);
    close(pagemap_fd);
    return 0;
}