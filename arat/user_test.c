#include <stdio.h>
#include<sys/types.h>
#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/sysinfo.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/ioctl.h>
#include<fcntl.h>
#define __USE_GNU
#include<sched.h>
#include<ctype.h>
#include<string.h>
#include<pthread.h>
#define THREAD_MAX_NUM 100
int num=0; 
struct useraddr_info {
        int useraddr;
};
#define CHAR_IOCTL_BASE 'W'
#define CHAR_SEND_PARAM         _IOWR(CHAR_IOCTL_BASE, 0, struct useraddr_info)
int triger_timer(int cpuid) {
        int fd, cmd;
        fd = open("/dev/aratchar",O_RDONLY);
        if(fd == -1) {
                printf("Open error !!!\n");
                return -1;
        }
        printf("CPU#:%d, open /dev/aratchar successfully\n", cpuid);
        struct useraddr_info userinfo;
        userinfo.useraddr = 0x400000;
        cmd = CHAR_SEND_PARAM;
        if (ioctl(fd, cmd, &userinfo) < 0)
        {
                printf("Call cmd CHAR_SEND_PARAM fail\n");
                return -1;
        }
        close(fd);
        return 0;
}
void* threadFun(void* arg)
{
         cpu_set_t mask;
         cpu_set_t get;
         int *a = (int *)arg;
         CPU_ZERO(&mask);
         CPU_SET(*a,&mask);
         if (sched_setaffinity(0, sizeof(mask), &mask) == -1)
         {
                   printf("warning: could not set CPU affinity, continuing...\n");
         }
         {
                   CPU_ZERO(&get);
                   if (sched_getaffinity(0, sizeof(get), &get) == -1)
                   {
                            printf("warning: cound not get thread affinity, continuing...\n");
                   }
                   int i;
                   for (i = 0; i < num; i++)
                   {
                            if (CPU_ISSET(i, &get))
                            {
                                     printf("this thread %d is running processor : %d\n", i,i);
                                        triger_timer(i);
                            }
                   }
         }
         return NULL;
}
int main(int argc, char* argv[])
{
         num = sysconf(_SC_NPROCESSORS_CONF);
         pthread_t thread[THREAD_MAX_NUM];
         printf("system has %i processor(s). \n", num);
         int tid[THREAD_MAX_NUM];
         int i;
         for(i=0;i<num;i++)
         {
                   tid[i] = i;
                   pthread_create(&thread[i],NULL,threadFun,(void*)&tid[i]);
         }
         for(i=0; i< num; i++)
         {
                   pthread_join(thread[i],NULL);
         }
         return 0;
}
