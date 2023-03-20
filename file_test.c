#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int main(int argc,char **argv)
{   
    if(argc <2)
    {
        printf("Usage:%s -w <string>\n",argv[0]);
        printf("Usage:%s -r \n",argv[0]);

        return -1;
    }
    
    int fd;
    char buf[1024];
    fd = open("/dev/filectl",O_RDWR);
    if(fd < 0)
    {
        printf("can not open /dev/filectl\n");
        return -1;
    }
    printf("open /dev/filectl success \n");
    if(strcmp(argv[1],"-w")==0 && argc==3)
    {
        int err = write(fd, argv[2], strlen(argv[2]));
        if (err < 0 ) {
            printf("write /dev/filectl error \n");
            return -1;
        }
        printf("write /dev/filectl success \n");
    }else
    {
        int err = read(fd,buf,1024);
        if (err < 0 ) {
            printf("read /dev/filectl error \n");
            return -1;
        }
        printf("read /dev/filectl success \n");
        buf[1023] = '\n';

        printf("APP get data %s\n",buf);
    }

    close(fd);
    
    return  0;
}