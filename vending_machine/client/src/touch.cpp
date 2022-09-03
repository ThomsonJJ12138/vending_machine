#include "touch.h"






TOUCH_STATE TouchScreen::get_touch_info(const int &ts_fd)
{
    int ts_x,ts_y;
    struct input_event event;
    memset(&event,0,sizeof(event));

    while(1)
    {   
        read(ts_fd, &event, sizeof(event));
        if (event.type == EV_ABS && event.code == ABS_X)
        {
            ts_x = event.value;
        }  
        if (event.type == EV_ABS && event.code == ABS_Y)
        {
            ts_y = event.value;
        }
        if(event.type == EV_KEY && event.code == BTN_TOUCH && event.value == 0)
        {		
            //printf("弹起:(%d,%d)\n",touch.ts_x,touch.ts_y);
            touch.ts_x = ts_x * 800 *1.0 / 1024 ;
            touch.ts_y = ts_y * 480 *1.0 / 600 ;
            return STATE_SINGLE_CLICK;
        }
    }

}


TOUCH& TouchScreen::getTouchState()
{
    return touch
}

void* touch_scan(void* arg)
{
    //打开触摸屏 
	int ts_fd = open(PATH_TOUCH, O_RDONLY);
	if(ts_fd == -1)
	{
		perror("open touch");
		return NULL;
	}
    TOUCH &rtouch = getTouchState();
    rtouch.state = STATE_NONE;

    while(1)
    {
        rtouch.state = get_touch_info(ts_fd);
    }

}

void TouchScreen::touch_pthread_init()
{
    int ret;
    ret = pthread_create(&tid, NULL,touch_scan,NULL);
    if(ret < 0) 
    {
        perror("pthread create error");
        return ;
    }
}  

void TouchScreen::responseOver()
{
    touch.state = STATE_NONE;
    touch.ts_x = 0;
    touch.ts_y = 0;
}

void TouchScreen::get_MaState(TOUCH &tc)
{
    memcpy(&tc, &touch, sizeof(touch));
}


