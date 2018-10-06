#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>

int main()
{
    char *byte;
    int fd = open("/dev/mydevice",O_RDWR);
    write(fd,"Hello Sriharsha",1);
    ssize_t size = read(fd,&byte,1);
    printf("Read Byte %s\n",byte);
}