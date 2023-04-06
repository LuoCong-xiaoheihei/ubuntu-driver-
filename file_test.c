#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define FILECTL_READ       _IOR('c', 0, unsigned int)   //对文件进行读取
#define FILECTL_WRITE      _IOW('c', 0, unsigned int)  //对文件进行写入

int main(int argc,char **argv)
{   
	int temp = 0;
	char buffer[1024];  // 存放内核拷贝的数据
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
        printf("APP set  data %s\n",argv[2]);
    }else if (strcmp(argv[1],"-r")==0) {
        int err = read(fd,buf,1024);
        if (err < 0 ) {
            printf("read /dev/filectl error \n");
            return -1;
        }
        printf("read /dev/filectl success \n");
        printf("APP get  data %s\n", buf);
        buf[1023] = '\n';
    } else if (strcmp(argv[1] , "ior") == 0 ) {
        int rc = ioctl(fd, FILECTL_READ, &buffer); // 将内核传递的数据读取出来
        if ( rc < 0 ) {
            printf("ioctl read  /dev/filectl error \n");
            return -1;
        }
        printf("ioctl read  /dev/filectl sucess \n");
        printf("APP get  data %s\n", buffer);
        buf[1023] = '\n';
    } else if ( strcmp(argv[1], "iow") == 0 ) {
        int rc = ioctl(fd, FILECTL_WRITE, argv[2]); // 写入数据
        if ( rc < 0 ) {
            printf("ioctl write /dev/filectl error \n");
            return -1;
        }
        printf("ioctl read  /dev/filectl sucess \n");
        printf("APP set data %s\n",argv[2]);
    } else {
        printf("please input right argv[1], 'ior' or 'iow'");
        printf("It will return error\n");
        return -1;
    }
    close(fd);

    return  0;
}
