#ifndef LCD_H
#define LCD_H

#include <iostream>

using namespace std;

#ifdef     __cplusplus
    extern "C"{
#endif

#include "file.h"

#ifdef     __cplusplus
    }
#endif




#define  DWORD  unsigned int
#define  LONG   unsigned int
#define  WORD   unsigned short
#define  UINT   unsigned short

#define GREEN			0x0000ff00 //绿色
#define BLACK   		0x00000000 //黑色
#define YELLOW			0x00ffff00 //黄色
#define RED				0x00ff0000 //红色
#define WHITE			0x00ffffff //白色
#define BLUE			0x00ff8080 //蓝色
#define PINK			0x00ff00ff //粉色
#define DARK_READ		0x00800000 //暗红
#define ORANGE			0x00ff8040 //橘黄
#define BROWN			0x00804000 //棕色
#define LIGHT_GREEN		0x0080ff80 //浅绿
#define LIGHT_BLUE		0x0000ffff //浅蓝
#define PURPLE			0x008000ff //紫色
#define GREY			0x00C8C8C8 //灰色

#define  COLOR_NUMBER     14 //颜色的数量


#define LCD_PATH        "/dev/fb0"              /* lcd设备文件路径 */

#define LCD_HEIGHT      480
#define LCD_WIDE        800
#define LCD_SIZE        (LCD_HEIGHT*LCD_WIDE)   /* 像素大小 */
#define LCD_FRAME_SIZE   (LCD_SIZE*4)           /* 800*480*4 byte */

#define LCD_BLOCK       3


#if 0
#define LCD_SET_BASE(fb,n)    ((fb) + LCD_SIZE*((n)%LCD_BLOCK))

#define LCD_SET_DISPLAY(lcdFd,vinfo,x,y)    vinfo->xoffset = x; \
                                            vinfo->yoffset = y; \
                                            ioctl((lcdFd), FBIOPAN_DISPLAY, (vinfo))

#define LCD_CLEAN()     for(int i=0; i<LCD_SIZE; i++) \
                            *(lcd_map+i) = (WHITE);

#endif

/*文件信息头 14字节*/
typedef struct tagBITMAPFILEHEADER
{
	UINT bfType;//2
	DWORD bfSize;//4
	UINT bfReserved1;//2
	UINT bfReserved2;//2
	DWORD bfOffBits;//4
}BITMAPFILEHEADER;

/*位图信息头 40个字节 */
typedef struct tagBITMAPINFOHEADER
{
	DWORD biSize; //4
	LONG biWidth;//图片的宽度
	LONG biHeight;//图片的高度
	WORD biPlanes; //2
	WORD biBitCount;//图片的深度 //2
	DWORD biCompression; //4
	DWORD biSizeImage; //图片的大小 //4
	LONG biXPelsPerMeter;
	LONG biYPelsPerMeter;
	DWORD biClrUsed; //4
	DWORD biClrImportant;  //4
}BITMAPINFOHEADER;

//液晶屏类
class LcdDevice:public File
{
    //把该类的构造函数放在私有权限下，也就是不让该类进行 外部的实例化对象
    LcdDevice();
public:       
    ~LcdDevice();
    static LcdDevice* getLcdDevice();
    static void delLcdDevice();
    void lcd_init();
    void lcd_free();
    void lcd_show_rectangle(int color, int x, int y, int width, int height);
    int lcd_showBmp_anypos(const char* filePath, int posX, int posY);
    int lcd_showBmp_narrow(const char* filePath, int posX, int posY, int N);
    
        
private:
    //定义一个静态 该类的 指针成员
    static LcdDevice* p_lcd;
    int lcdFd;
    int* lcd_map;
};


#endif