#include "file.h"



File::File(){}
File::~File(){}

int File::file_open(const char* filePath)
{
    //文件
    int fd = open(filePath, O_RDWR);
    if(fd == -1)
    {
        perror("file_open error");
        return -1;
    }
    return fd;
}


#if 0
int get_dir_file(const char* dirPath, const char* suffix, char (*bmpName)[256])
{
	DIR* dirFp = opendir(dirPath);
	if(dirFp == NULL)
	{
		perror("opendir error");
		return -1;
	}
	//2、不断地读取目录下的一个个文件的信息
	//注意：readdir 每次调用只能获取目录下 一个目录项的内容（也就是只能获取一个文件的信息）
	int  bmpCount = 0;
	while(1)
	{
		//该结构体指针info 存储了 读取到的这一个文件的信息
		struct dirent* info = readdir(dirFp);
		if(info == NULL){
			printf("read dir end\n");
			break;
		}
		//判断文件类型是 普通文件 ,把后缀为 .bmp的图片文件留下来
		if(info->d_type == DT_REG ){

			if(strcmp(&(info->d_name[strlen(info->d_name)-strlen(suffix)]), suffix) == 0) //后缀是 .bmp
			{
				sprintf(bmpName[bmpCount],"%s%s",dirPath,info->d_name);
                bmpCount++;

			}
		}
	}

	//3、关闭目录文件
	closedir(dirFp);
    return bmpCount; 
}

int getFileSize(FILE* fp)
{
	//1、先获取文件当前的位置
	long int startPos = ftell(fp);
	//2、将文件的光标偏移到末尾
	fseek(fp,0,SEEK_END);
	//3、再获取文件当前的位置 ，也就是获取文件的大小
	int fileSize = ftell(fp);
	//4、恢复原来文件光标的位置
	fseek(fp,startPos,SEEK_SET);
	
	return fileSize;
}
#endif