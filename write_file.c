#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <memory.h>
#include <fcntl.h>

#define FILESIZE 1048574 

int main()
{
	int fd,fd_move,fd_dev;	
	int buf_size;	
	int move = 0;	
	int j;	
	static int len = 0;
	int sy_size = 0; 
	int sy_space;
	int flag;
	
	char sy_buf[26];
	char i = '0';
	fd = open("write_file.txt",O_RDWR|O_CREAT,0777);
		
	fd_move = open("move.txt",O_RDWR|O_CREAT,0777);	
		
	fd_dev = open("/dev/myled",O_RDONLY|O_NDELAY);
	if(fd_dev == -1)
	{
		printf("open dev failure!\n");
			
	}
	
	char buf[100];	
	char dev_data[27];
	
	memset(buf,0,sizeof(buf));	
	memset(dev_data,0,sizeof(dev_data));
	memset(sy_buf,0,sizeof(sy_buf));
	
	read(fd_move,&move,sizeof(move));
	

	while(1)
	{
		read(fd_dev,dev_data,sizeof(dev_data));
		printf("%s\n",dev_data);	
		//判断缓冲区大小
		if(strlen(buf)<100)
		{
			if(i>'9')
			{
				i = '0';

			}

			flag = 0;
			//剩余空间能否放下读取的数组
			if((sizeof(buf)-strlen(buf))>=sizeof(dev_data))
			{
				sprintf(buf,"%s%c%s",buf,i,dev_data);	
				i++;
				memset(dev_data,0,sizeof(dev_data));
				if(sizeof(buf)==strlen(buf))
				{
					flag=1;
				}
			}
			else
			{
				{
					sy_space = sizeof(buf)-strlen(buf);
					
					sprintf(buf,"%s%c%s",buf,i,sy_buf);
					i++;
					memset(dev_data,0,sizeof(dev_data));
					flag = 1;
				}
				
			}
		}
		
		if(flag =1)
		{
			if(move>0&&move<FILESIZE)
			{
				len = move;			
			}
			
			if(len>=FILESIZE)
			{
				len =0;
			}
			else
			{
				if((FILESIZE-len) >= sizeof(buf))
				{
					lseek(fd,len,SEEK_SET);
					write(fd,buf,sizeof(buf));
					memset(buf,0,sizeof(buf));
					len += sizeof(buf);
					
					lseek(fd_move,0,SEEK_SET);
					write(fd_move,&len,sizeof(len));
				}
				else
				{
					lseek(fd,len,SEEK_SET);
					write(fd,buf,(FILESIZE-len));
					memset(buf,0,sizeof(buf));
					len =0; 
					
					lseek(fd_move,0,SEEK_SET);
					write(fd_move,&len,sizeof(len));
				}
				
			}
					
		}		
	}
	
	close(fd);
	close(fd_dev);
	close(fd_move);
}
