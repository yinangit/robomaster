#include <iostream>
#include<opencv2/opencv.hpp>
#include<opencv2/imgproc/types_c.h>
#include<math.h>
#include<opencv2/highgui.hpp>
#include<LightDescriptor.h>
#include<armor_descriptor.h>
#include<armor_param.h>
using namespace cv;//注意:装甲板范围设置上限不可太小，下限不可太大！！！！！！
using namespace std;//注意:abs绝对值函数的使用，条件多一点，Size2f两参数的顺序！！！！！！

extern bool armor_recognotion(Mat &src,RotatedRect record[10], Point2i *point,RotatedRect &armor)
{
    int track=0;
    for(int i=0;record[i].size.width;i++)//record[i]不可转化成为bool值
    {
        for(int j=i+1;record[j].size.width;j++)
        if (abs(record[i].size.width / record[j].size.width-1)<0.4&&//作差与0比或作商与1比
            abs(record[i].size.height / record[j].size.height-1)<0.4&&
            abs(record[i].angle-record[j].angle)<5&&
            abs(record[i].angle)<30&&abs(record[i].angle)<30&&
            record[i].angle*record[i].angle>0&&
            abs((record[i].center.y- record[j].center.y))<5&&//不用斜率;削减识别的装甲板的总数
            sqrt(pow(record[i].center.y- record[j].center.y,2)+//扩大范围：削减识别的装甲板的总数
                 pow(record[i].center.x- record[j].center.x,2))/record[i].size.width<5.5&&
            sqrt(pow(record[i].center.y- record[j].center.y,2)+//2到4.5倍的灯条的宽度
                 pow(record[i].center.x- record[j].center.x,2))/record[i].size.width>3)
        {
            if(track<10)
            {
                point[track].x=i;
                point[track].y=j;
                track++;
                continue;
            }
            else break;
        }
    }//上限10个等条，筛选出匹配的几组
    for(int i=1;i<10&&point[i].x;i++)
       if(abs(record[point[0].x].angle- record[point[0].y].angle )>
          abs(record[point[i].x].angle - record[point[i].y].angle)||
          //abs(record[int(point[0].x)].size.width/record[int(point[0].y)].size.width-1)>宽度相差不大
          //abs(record[int(point[i].x)].size.width/record[int(point[i].y)].size.width-1)&&使其权重为0
          abs(record[point[0].x].size.height-record[point[0].y].size.height)>
          abs(record[point[i].x].size.height-record[point[i].y].size.height)||
          sqrt(pow(record[point[0].x].center.y- record[point[0].y].center.y,2)+
          pow(record[point[0].x].center.x- record[point[0].y].center.x,2))>
          sqrt(pow(record[point[i].x].center.y- record[point[i].y].center.y,2)+
          pow(record[point[i].x].center.x- record[point[i].y].center.x,2)))
          point[0]=point[i];//筛选出一组，存在point[0]里面

    if(track)
    {
    //画出装甲板
    float pixel_width=sqrt(pow(record[int(point[0].x)].center.y- record[int(point[0].y)].center.y,2)+pow(record[int(point[0].x)].center.x- record[int(point[0].y)].center.x,2));
    pixel_width -=record[int(point[0].x)].size.width/2+record[int(point[0].y)].size.width/2;
    armor=RotatedRect(Point2f((record[int(point[0].x)].center.x+record[int(point[0].y)].center.x)/2,record[int(point[0].x)].center.y),
                Size2f(pixel_width,record[int(point[0].x)].size.height),
                record[int(point[0].x)].angle);
    Point2f vertex[4];
    armor.points(vertex);
    for (int i = 0; i < 4; i++)
    {
        line(src, vertex[i], vertex[(i + 1) % 4], Scalar(0, 255, 0),2,CV_AA);
    }
    /*
    //vector<Point2f> point;无法转换？？定义同时需要初始化大小？？
    Point2f point[4];
    armor.points(point);
    rectangle(src,point[1],point[3],Scalar(0,255,0));这样画的矩形没有line效果好
    */
    return true;
    }
    else return false;
}
