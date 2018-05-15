led_test.c
#include <stdio.h>  
#include <fcntl.h>  
#include <unistd.h>  
#include <sys/stat.h>
#include <sys/types.h>
#include <memory.h>
#define FILE 20560
int main(void)
{
    int fd;
    int fd_file;
    char udata[26];  //定义用户缓冲区
    static int size;
    memset(udata,0,sizeof(udata));
    //打开设备
    //open->....->调用led_open
    fd = open("/dev/myled", O_RDONLY|O_NDELAY);
    fd_file = open("file.txt",O_RDWR|O_CREAT,0777);

    if (fd < 0) {
        printf("打开设备失败!\n");
        return -1;
    }

    while(1)
    { 
    	read(fd, udata, sizeof(udata));
	size = lseek(fd_file,0,SEEK_CUR);
    	//printf("从驱动读取的数据为:udata = %s\n", udata);
	if((FILE-size) > sizeof(udata))
	{
		lseek(fd_file,size,SEEK_SET);
    		write(fd_file,udata,sizeof(udata));
	}
	else
	{
		write(fd_file,udata,(FILE-size));
		lseek(fd_file,0,SEEK_SET);
		write(fd_file,udata,(sizeof(udata)-FILE-size));
	}
    }
    //关闭设备
    //close->...->调用led_close
    close(fd);
    return 0;
}

