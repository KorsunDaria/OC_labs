#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>


// hexdump -C heap.bin

#define HEAP_SIZE (1024)

typedef struct block {
    size_t size;
    int free;
    struct block* next;
} block_t;

block_t* heap = NULL;

void init_heap_file(const char* filename) {
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0666);

    if(fd < 0) { perror("open"); return; }

    if(ftruncate(fd, HEAP_SIZE) != 0) { perror("ftruncate"); return; }

    heap = mmap(NULL, HEAP_SIZE,
                PROT_READ | PROT_WRITE,
                MAP_SHARED, 
                fd, 0);

    if(heap == MAP_FAILED) { perror("mmap"); return; }

    heap->size = HEAP_SIZE - sizeof(block_t);
    heap->free = 1;
    heap->next = NULL;

    close(fd);
}

void print_heap() {
    block_t* cur = heap;
    printf("Heap blocks:\n");
    while(cur) {
        printf("  block %p size=%zu free=%d\n", cur, cur->size, cur->free);
        cur = cur->next;
    }
    printf("\n");
}


void wait_for_enter() {
    printf("Press [Enter] to continue...\n");
    while(getchar() != '\n');
}

void* my_malloc(size_t size) {
    block_t* cur = heap;
    while(cur) {
        if(cur->free && cur->size >= size) {
            if(cur->size > size + sizeof(block_t) + 16) {
                block_t* new_block = (block_t*)((char*)cur + sizeof(block_t) + size);
                new_block->size = cur->size - size - sizeof(block_t);
                new_block->free = 1;
                new_block->next = cur->next;
                cur->next = new_block;
                cur->size = size;
            }
            cur->free = 0;
            return (char*)cur + sizeof(block_t);
        }
        cur = cur->next;
    }
    return NULL;
}

void my_free(void* ptr) {
    if(!ptr) return;
    block_t* block = (block_t*)((char*)ptr - sizeof(block_t));
    block->free = 1;
}

int main() {
    init_heap_file("heap.bin");

    printf("Initial heap:\n");
    print_heap();
    wait_for_enter();

    void* a = my_malloc(100);
    printf("After malloc 100 bytes (a=%p):\n", a);
    print_heap();
    wait_for_enter();

    void* b = my_malloc(200);
    printf("After malloc 200 bytes (b=%p):\n", b);
    print_heap();
    wait_for_enter();

    my_free(a);
    printf("After free(a):\n");
    print_heap();
    wait_for_enter();

    void* c = my_malloc(50);
    printf("After malloc 50 bytes (c=%p):\n", c);
    print_heap();
    wait_for_enter();

    void* d = my_malloc(50000);
    printf("After malloc 50000 bytes (d=%p):\n", d);
    print_heap();
    wait_for_enter();

    return 0;
}
