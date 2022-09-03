#ifndef TOUCH_H
#define TOUCH_H

#ifdef     __cplusplus
    extern "C"{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <pthread.h>

#ifdef     __cplusplus
    }
#endif


#define PATH_TOUCH "/dev/input/event0"


typedef enum
{
    STATE_NONE = 100,				/* 无操作 */
    STATE_PRESS,					/* 按下*/
    ISDITHERING,					/* 是否抖动 */
    STATE_PRESS_SURE,				/*确认按下*/
    STATE_RELEASE,					/* 释放 */
    STATE_RELEASE_SURE,				/*确认释放*/

    STATE_SINGLE_CLICK,				/* 单击 */
    STATE_DOUBLE_CLICK,				/* 双击 */
    STATE_LONG_CLICK,				/* 长按 */
    STATE_SLIDING,
    SLID_LEFT,
    SLID_RIGHT,
    SLID_UP,
    SLID_DOWN,				 	    /* 滑屏 */
    STATE_DRAG					    /* 拖动 */
}TOUCH_STATE;

typedef struct
{
    bool response;				    /* 是否有被响应 */
    int x1,y1;				        /* 上一轮的坐标，用来判断双击 */
    int x2,y2;				        /* 最后一次的坐标 */
    int ts_x,ts_y;					/* 第一次的坐标 */
    float time_pre;					/* 上一轮的时间 */
    float time_last;				/* 最后一次的时间 */
    float time;						/* 第一次的时间 */
    TOUCH_STATE	state;			    /* 状态机的状态 */
    
}TOUCH;

class TouchScreen
{
public:
    TOUCH_STATE get_touch_info(const int &ts_fd);
    void touch_pthread_init();
    void responseOver();
    void get_MaState(TOUCH &tc);
    TOUCH& getTouchState();
private:
    pthread_t tid;
    TOUCH touch;
};




#endif