#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <memory.h>
#include <fcntl.h>

#define FILESIZE 1025

int main()
{
	//保存每次读写的位置
	int len = 0;

	//能够写入的大小
	int size = 0;

	//驱动
	int fd;

	//存储文件
	int fd_file;

	//文件指针
    	FILE *fp;

	//自定义缓存区
	char buf[256];
	memset(buf,0,sizeof(buf));	

	
	char udata[26];  //定义用户缓冲区
	memset(udata,0,sizeof(udata));
	fd = open("/dev/myled",O_RDONLY|O_NDELAY);
	//读取驱动信息


	//打开设备
    	//open->....->调用led_open
    	fd = open("/dev/myled", O_RDONLY|O_NDELAY);
	
	//打开存储文件偏移位置的文件
	fd_file = open("prevfile.txt",O_RDWR|O_CREAT,0777);

	//打开待写入的文件
	if((fp = fopen("file.txt","wt+")) == NULL)
	{
		perror("file open failure!");
		exit(-1);
	}
	
	/*获取当前指针偏移量
	fseek(fp,move,SEEK_END);
	len = ftell(fp)+1;
	*/

	//读取内核信息到缓存区
	//read(fd,udata,sizeof(udata));
	
	//自定义缓存区
	setvbuf(fp,buf,_IOFBF,sizeof(buf));

	while(1)
	{

		if((sizeof(buf)-strlen(buf))==sizeof(udata))
		{				
			read(fd,udata,sizeof(udata));
			fwrite(udata,sizeof(udata),1,fp);

			if((FILESIZE-len) > sizeof(buf))
			{
				fseek(fp,len,SEEK_SET);	
				fwrite(buf,sizeof(buf),1,fp);					
				len = ftell(fp) + sizeof(buf);
				memset(udata,0,sizeof(buf));
				lseek(fd,0,SEEK_SET);
				write(fd,&len,sizeof(len));
			}
			else if((FILESIZE-len) < sizeof(udata))
			{
				fseek(fp,len,SEEK_SET);
				size = FILESIZE - len;
				fwrite(buf,size,1,fp);
				memset(buf,0,sizeof(buf));
				fseek(fp,0,SEEK_SET);
				len = ftell(fp) + 1;
				lseek(fd,0,SEEK_SET);
				write(fd,&len,sizeof(len));
			}
			else
			{
				fwrite(buf,sizeof(buf),1,fp);
				memset(buf,0,sizeof(buf));
				fseek(fp,0,SEEK_SET);
				len = ftell(fp) + 1;	
				lseek(fd,0,SEEK_SET);
				write(fd,&len,sizeof(len));
			}
			memset(udata,0,sizeof(udata));

		}

		if((sizeof(buf)-strlen(buf))<sizeof(udata))
		{
			//读取内核信息到缓存区
			read(fd,udata,sizeof(udata));
			size = sizeof(buf)-strlen(buf);
			fwrite(udata,size,1,fp);

			if((FILESIZE-len) > sizeof(buf))
			{
				fseek(fp,len,SEEK_SET);	
				fwrite(buf,sizeof(buf),1,fp);					
				len = ftell(fp) + sizeof(buf);
				memset(udata,0,sizeof(buf));
				lseek(fd,0,SEEK_SET);
				write(fd,&len,sizeof(len));
			}
			else if((FILESIZE-len) < sizeof(udata))
			{
				fseek(fp,len,SEEK_SET);
				size = FILESIZE - len;
				fwrite(buf,size,1,fp);
				memset(buf,0,sizeof(buf));
				fseek(fp,0,SEEK_SET);
				len = ftell(fp) + 1;
				lseek(fd,0,SEEK_SET);
				write(fd,&len,sizeof(len));
			}
			else
			{
				fwrite(buf,sizeof(buf),1,fp);
				memset(buf,0,sizeof(buf));
				fseek(fp,0,SEEK_SET);
				len = ftell(fp) + 1;	
				lseek(fd,0,SEEK_SET);
				write(fd,&len,sizeof(len));
			}
			memset(udata,0,sizeof(udata));
		}
	
		if((sizeof(buf)-strlen(buf))>sizeof(udata))
		{
			//读取内核信息到缓存区
			read(fd,udata,sizeof(udata));
			fwrite(udata,sizeof(udata),1,fp);
			
			memset(udata,0,sizeof(udata));
		}

	}	
	
	fclose(fp);
	return 0;
	
}


