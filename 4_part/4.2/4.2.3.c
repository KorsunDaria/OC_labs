#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>


void f(int d){
    char a[4096];                         
    printf("stack depth=%d addr=%p\n", d, a);
    usleep(300000);                     
    if(d >= 70) return;                    
    f(d+1);                                
}

int main(){


    printf("PID=%d\n", getpid());


    sleep(10); // watch cat /proc/PID/maps


    //f(1);
    sleep(5); 

    // void* p[10];
    // for(int i=0;i<50;i++){
    //     p[i] = malloc(1024*1024);          
    //     printf("malloc %p\n", p[i]);
    //     sleep(1);                          
    // }
    // for(int i=0;i<50;i++)
    //     free(p[i]);

    // printf("heap freed\n");
    // sleep(5);

    size_t ps = getpagesize();
    size_t sz = ps * 10;

    char* r = mmap(NULL, sz,
                   PROT_READ | PROT_WRITE,
                   MAP_PRIVATE | MAP_ANONYMOUS,
                   -1, 0);

    printf("mmap addr=%p\n", r);
    sleep(5); 


    // sleep(10); 
    // mprotect(r, sz, PROT_NONE);       
    // printf("try read...\n");
    // char c = r[0];                     
    // printf("%c\n", c);
    // exit(0);


   
    // mprotect(r, sz, PROT_READ);        
    // printf("try write...\n");
    // r[0] = 'A';                        
    // exit(0);
   

    munmap(r + 4*ps, 3*ps);                
    printf("partial munmap done\n");

    sleep(10); 

    munmap(r, 4*ps);
    munmap(r+ 7*ps, 3*ps);

    return 0;
}
