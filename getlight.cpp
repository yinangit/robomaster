#include <iostream>
#include<opencv2/opencv.hpp>
#include<opencv2/imgproc/types_c.h>
#include<math.h>
#include<opencv2/highgui.hpp>
#include<LightDescriptor.h>
using namespace cv;
using namespace std;//53，16行！！！

enum
{
    WIDTH_GREATER_THAN_HEIGHT,
    ANGLE_TO_UP
};

extern bool getlight(Mat &src,Mat&src1,RotatedRect *record)//参数！！
{
    void adjustRec(RotatedRect& rec, const int mode);

    int detection=0;

    vector<vector<Point>> lightContours;//找轮廓
    findContours(src, lightContours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);//发现轮廓,但会在图像上改变
    for(const auto& contour : lightContours)//for的语法糖，保证可以从开始循环遍历到结束，不必考虑起始与终止
        {
          //detection=0;不可每次初始化为0，只要有一个灯条即可
          float lightContourArea = contourArea (contour);//得到面积
          //cout<<contour.size()<<endl<<lightContourArea<<endl;
          if(contour.size() <= 15||lightContourArea < 25) continue;//面积太小的不要
          RotatedRect lightRec = fitEllipse(contour); //椭圆拟合区域得到外接矩形
          adjustRec(lightRec, ANGLE_TO_UP); //矫正灯条
          //cout<<lightRec.size.width / lightRec.size.height<<endl;
          //cout<<lightContourArea / lightRec.size.area()<<endl<<endl;
          if (lightRec.size.width / lightRec.size.height >1.5||lightContourArea / lightRec.size.area() >1)
                continue; //宽高比、凸度筛选灯条
          if(abs(lightRec.angle)>60) continue;
          lightRec.size.width *= 1; //对灯条范围适当扩大
          lightRec.size.height *= 1;
          detection+=1;
          record[detection-1]=lightRec;

          //因为颜色通道相减后己方灯条直接过滤，不需要判断颜色了,可以直接将灯条保存
          //rectangle(src, lightRec, Scalar(255, 0, 0),1, LINE_8,0);仅可画出rect类矩形

          //将矩形画出来（方法一二）
          /*Point2f* vertices = new Point2f[4];
          lightRec.points(vertices);
          vector<Point> contour1;
          for (int i = 0; i < 4; i++)
          {
              contour1.push_back(vertices[i]);
          }
          vector<vector<Point>> contours;
          contours.push_back(contour1);
          drawContours(src1, contours, 0, Scalar(255, 0, 0), 1);*/

          Point2f vertex[4];//通道分开后，无法显示直线颜色？？
          lightRec.points(vertex);
          for (int i = 0; i < 4; i++)
          {
              line(src1, vertex[i], vertex[(i + 1) % 4], Scalar(255, 0, 0),2,CV_AA);
          }

         }
    if(detection>0) return true;
    else return false;
}
