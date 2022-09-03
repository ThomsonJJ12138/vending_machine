#include <iostream>
#include "lcd.h"


using namespace std;


int main()
{
    
    LcdDevice* lcd = LcdDevice::getLcdDevice();
    lcd->lcd_show_rectangle(PINK,0,0,800,480);
    while(1)
    {

    }
    return 0;
}