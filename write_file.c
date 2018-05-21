#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <memory.h>
#include <fcntl.h>

#define FILESIZE 966
#define BUFSIZE  100
#define DEVSIZE  27
#define SYSIZE   26

int main()
{
	int fd,fd_move,fd_dev;	
	int buf_size;	
	int move = 0;	
	int j,z;	
	static int len = 0;
	int sy_size = 0; 
	int sy_space;
	int flag;
	
	char sy_buf[SYSIZE];
	char buf[BUFSIZE];
	char dev_data[DEVSIZE];

	memset(sy_buf,0,sizeof(sy_buf));
	memset(buf,0,sizeof(buf));
	memset(dev_data,0,sizeof(dev_data));

	fd = open("write_file.txt",O_RDWR|O_CREAT,0777);
		
	fd_move = open("move.txt",O_RDWR|O_CREAT,0777);	
		
	fd_dev = open("/dev/myled",O_RDONLY|O_NDELAY);
	if(fd_dev == -1)
	{
		printf("open dev failure!\n");
			
	}	

	while(1)
	{				
		//判断缓冲区大小
		while(strlen(buf) < BUFSIZE)
		{
			flag = 0;
			
			read(fd_dev,dev_data,sizeof(dev_data));
			
			//剩余空间能否放下读取的数组
			if((sizeof(buf)-strlen(buf))>=sizeof(dev_data))
			{
				sprintf(buf,"%s%s",buf,dev_data);	
				memset(dev_data,0,sizeof(dev_data));

				if(sizeof(buf)==strlen(buf))
				{
					flag=1;
				}
			}
			else
			{
				sy_space = sizeof(buf)-strlen(buf);
	
				for(j=0;j<sy_space;j++)
				{
					sy_buf[j] = dev_data[j];
				}
	
				sprintf(buf,"%s%s",buf,sy_buf);
				memset(sy_buf,0,sizeof(sy_buf));

				for(z=0;j<26;j++,z++)
				{
					sy_buf[z] = dev_data[j];
					
				}

				memset(dev_data,0,sizeof(dev_data));
				flag = 1;				
			}
		}
		
		if(flag == 1)
		{
			flag = 0;

			lseek(fd_move,0,SEEK_SET);
			read(fd_move,&move,sizeof(move));	

			if(move < FILESIZE)
			{
				len = move;			
			}
			else
			{
				len =0;
			}
			
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
				len = 0;
				lseek(fd,len,SEEK_SET);
				write(fd,&(buf[FILESIZE-len]),BUFSIZE-(FILESIZE-len));
				memset(buf,0,sizeof(buf));
				len = BUFSIZE - (FILESIZE-len);

				lseek(fd_move,0,SEEK_SET);
				write(fd_move,&len,sizeof(len));
			}	
			
			if(strlen(sy_buf)>0)
			{
				sprintf(buf,"%s%s",buf,sy_buf);
				//printf("%s\n",sy_buf);
				memset(sy_buf,0,sizeof(sy_buf));
			}
			else
			{
				memset(sy_buf,0,sizeof(sy_buf));
			}
						
		}		
	}
	
	close(fd);
	close(fd_dev);
	close(fd_move);
}
