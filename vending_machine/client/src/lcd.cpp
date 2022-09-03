#include "lcd.h"


LcdDevice* LcdDevice::p_lcd = NULL;

LcdDevice::LcdDevice(){}
LcdDevice::~LcdDevice(){}

LcdDevice* LcdDevice::getLcdDevice()
{
    if(p_lcd == NULL){
        p_lcd = new LcdDevice;
    }
    return p_lcd;
}
void LcdDevice::delLcdDevice()
{
    if(p_lcd != NULL)
    {
        delete p_lcd;
    }
    p_lcd = NULL;
}

void LcdDevice::lcd_init()
{
    //1、打开液晶屏文件
    lcdFd = open(LCD_PATH,O_RDWR);
    if(lcdFd == -1)
    {
        perror("open lcd error");
        return ;
    }
	//内存映射
	lcd_map = (int*)mmap(NULL, LCD_FRAME_SIZE*LCD_BLOCK, 
    PROT_READ|PROT_WRITE, MAP_SHARED, lcdFd, 0);
	if(lcd_map == MAP_FAILED)
    {
		perror("mmap lcd error");
		return ;
	}
}

void LcdDevice::lcd_free()
{
    close(lcdFd);
	munmap(lcd_map,LCD_FRAME_SIZE*LCD_BLOCK);
}


//定义一个接口，实现在指定位置(x,y) 显示 指定大小的 (width,height)颜色数据(color)
void LcdDevice::lcd_show_rectangle(int color, int x, int y, int width, int height)
{
	for(int j=0; j<height; j++) //你要显示的颜色行数
	{
		for(int i=0; i<width; i++)//每一行显示的像素点
		{
			lcd_map[y*800+x+j*800+i] = color;
		}
	}
}


//在任意的位置(posX,posY)显示原图，原图多大就显示多大
int LcdDevice::lcd_showBmp_anypos(const char* filePath, int posX, int posY)
{
	//2、打开图片文件
    int bmpFd = file_open(filePath);
    
	//根据这54个字节，获取图片的宽度 高度  深度  大小
	//先获取文件信息头 14个字节 BITMAPFILEHEADER bitmap file header
	BITMAPFILEHEADER fileHeader;
	read(bmpFd,&fileHeader,14);
	//再读取位图信息头 40个字节  --图片的宽度 高度  深度  大小
	BITMAPINFOHEADER infoHeader;
	read(bmpFd,&infoHeader,40);
	
	if(infoHeader.biBitCount != 24)
	{
		printf("该图片深度不是24bitCount...\n");
		close(bmpFd);
		return -1;
	}

	//获取到图片的宽度 和 高度
	int W = infoHeader.biWidth;
	int H = infoHeader.biHeight;
	if((W > 800) || (H > 480))
	{
		printf("该图片超过屏幕显示的大小\n");
		close(bmpFd);
		return -1;
	}
	//准备buf储存图片的像素点信息
	char bmpbuf[W*H*3];
	int  tmpbuf[W*H];
	int  lastbuf[W*H];
	
	//保存每一行的无效字节
	int invalid_byte = (4-(W*3)%4)%4;
	
	//按行读取bmp图片的信息，跳过无效字节
	for (int i = 0; i<H; i++)
	{
		read(bmpFd, &bmpbuf[i*W*3], 3*W);
		lseek(bmpFd, invalid_byte, SEEK_CUR);
	}

	//将BGR转换成ARGB
	for (int i = 0; i < W*H; i++)
	{
		tmpbuf[i] = 0x00<<24 | bmpbuf[3*i+2]<<16 | bmpbuf[3*i+1]<<8 | bmpbuf[3*i];
	}
	//将图片倒过来
	for(int j=0; j<H; j++)
	{
		for(int i=0; i<W; i++)
		{
			lastbuf[j*W+i] = tmpbuf[(H-1-j)*W+i];
		}

	}

	//显示 也就是将缩小后的数据 拷贝 到 映射的内存空间中
	for(int j=0; j<H; j++)
	{
		for(int i=0; i<W; i++)
		{
			lcd_map[(posY+j)*800 + posX+i] = lastbuf[j*W + i];
		}
	}

    //5、关闭图片文件
    close(bmpFd);
}


//在任意的位置(posX,posY)显示缩小n倍的图片
int LcdDevice::lcd_showBmp_narrow(const char* filePath, int posX, int posY, int N)
{
	//打开图片文件
    int bmpFd = file_open(filePath);
    
	//根据这54个字节，获取图片的宽度 高度  深度  大小
	//先获取文件信息头 14个字节 BITMAPFILEHEADER bitmap file header
	BITMAPFILEHEADER fileHeader;
	read(bmpFd,&fileHeader,14);
	//再读取位图信息头 40个字节  --图片的宽度 高度  深度  大小
	BITMAPINFOHEADER infoHeader;
	read(bmpFd,&infoHeader,40);
	
	
	if(infoHeader.biBitCount != 24)
	{
		printf("该图片深度不是24bitCount...\n");
		close(bmpFd);
		return -1;
	}
	//获取到图片的宽度 和 高度
	int W = infoHeader.biWidth;
	int H = infoHeader.biHeight;
	if((W/N > 800) || (H/N > 480))
	{
		printf("该图片超过屏幕显示的大小\n");
		close(bmpFd);
		return -1;
	}
	//准备buf储存图片的像素点信息
	char* bmpbuf = (char*)malloc(W*H*3);
	int*  tmpbuf = (int*)malloc(W*H*sizeof(int));
	int*  lastbuf = (int*)malloc(W*H*sizeof(int));
	
	//保存每一行的无效字节
	int invalid_byte = (4-(W*3)%4)%4;
	
	//按行读取bmp图片的信息，跳过无效字节
	for (int i = 0; i<H; i++)
	{
		read(bmpFd, &bmpbuf[i*W*3], 3*W);
		lseek(bmpFd, invalid_byte, SEEK_CUR);
	}

	//将BGR转换成ARGB
	for (int i = 0; i < W*H; i++)
	{
		tmpbuf[i] = 0x00<<24 | bmpbuf[3*i+2]<<16 | bmpbuf[3*i+1]<<8 | bmpbuf[3*i];
	}
	//将图片倒过来
	for(int j=0; j<H; j++)
	{
		for(int i=0; i<W; i++)
		{
			lastbuf[j*W+i] = tmpbuf[(H-1-j)*W+i];
		}

	}

	//显示 也就是将缩小后的数据 拷贝 到 映射的内存空间中
	for(int j=0; j<H/N; j++)
	{
		for(int i=0; i<W/N; i++)
		{
			lcd_map[(posY+j)*800 + posX+i] = lastbuf[j*W*N + i*N];
		}
	}

    //释放空间
	free(bmpbuf);
	bmpbuf = NULL;
	free(tmpbuf);
	tmpbuf = NULL;
	free(lastbuf);
	lastbuf = NULL;
	//5、关闭图片文件
    close(bmpFd);

}
