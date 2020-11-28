#include <iostream>
#include<opencv2/opencv.hpp>
#include<opencv2/imgproc/types_c.h>
#include<math.h>
#include<opencv2/highgui.hpp>
using namespace cv;
using namespace std;

enum
{
    WIDTH_GREATER_THAN_HEIGHT,
    ANGLE_TO_UP
};

extern void adjustRec(RotatedRect& rec, const int mode) //矫正灯条函数
{
    float & width = rec.size.width;
    float & height = rec.size.height;
    float & angle = rec.angle;
    if(mode == WIDTH_GREATER_THAN_HEIGHT)
    {
        if(width < height)
        {
            std::swap(width, height);
            angle += 90.0;
        }
    }
    while(angle >= 90.0) angle -= 180.0;
    while(angle < -90.0) angle += 180.0;
    if(mode == ANGLE_TO_UP)
    {
        if(angle >= 45.0)
        {
            std::swap(width, height);
            angle -= 90.0;
        }
        else if(angle < -45.0)
        {
            std::swap(width, height);
            angle += 90.0;
        }
    }
}
