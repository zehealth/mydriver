#include <stdio.h>  
#include <fcntl.h>  
#include <unistd.h>  
#include <string.h>
#include <errno.h>
#include <linux/unistd.h>
#include <sys/klog.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <memory.h> 
int main()  
{  
     	char buf[2048];   
    	int fd;
	int file = 10256;
	int size;
	int sum;
   	int move;

    	fd = open("klog.txt",O_RDWR|O_CREAT,0777);
    	if(fd>=0)
    	{
    		printf("open file ok!\n");
    	} 

    	while(1)
    	{
		memset(buf,0,sizeof(buf));

		size = lseek(fd,0,SEEK_CUR);
		
    		klogctl(3,buf,2048);//获取内核数据
  		
	//	if(sizeof(buf)==2048)
		{
			lseek(fd,size,SEEK_SET);
		
			if((file-size) > sizeof(buf))
			{	
    				write(fd, buf, sizeof(buf)); //写入文件数据
			}
			else
			{
				write(fd,buf,(file-size));
				lseek(fd,0,SEEK_SET);
				write(fd,buf,(sizeof(buf)-file-size));
		
			}
    		} 
	}

    	close(fd);

    	return 0;  
}  
