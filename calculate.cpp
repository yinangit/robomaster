#include <iostream>
#include<opencv2/opencv.hpp>
#include<opencv2/imgproc/types_c.h>
#include<cmath>
#include<opencv2/highgui.hpp>
#include<LightDescriptor.h>
#include<armor_descriptor.h>
#include<armor_param.h>
#include<camera_param.h>
using namespace cv;
using namespace std;//大装甲板：23*12    小装甲板：12.5*13.5

extern void calculate(RotatedRect armor,float &distance,float *angle)//相似三角形
{    
    //载入内参矩阵与畸变矩阵
    Mat mat1=Mat(3,3,CV_32FC1,cameraMatrix);//一个float数据占4个字节，一个字节8位
    Mat mat2=Mat(1,5,CV_32FC1,distortion);
    //float fx=Mat(cameraMatrix).at<float>(0,0);Mat(float[][])不可
    float fx=cameraMatrix[0][0];
    float fy=cameraMatrix[1][1];
    float cx=cameraMatrix[2][0];
    float cy=cameraMatrix[2][1];
    /*float k1=distortion[0][0];
    float k2=distortion[0][1];
    float p1=distortion[0][2];
    float p2=distortion[0][3];未用的变量是warnings*/
    float record;
    if(armor.size.width/armor.size.width>1.5) record=23*fx/armor.size.width/100;//大的装甲板
    else record=13*fx/armor.size.width/100;//实际宽度比上像素宽度乘以焦距(注意单位)
    if(record)//8m以外相似三角形，以内则用solvepnp      >8
    {
        //相似三角形测距离
        distance=record;
        //单点测距测角度
        vector<Point2f> in;
        vector<Point2f> out;
        in.push_back(armor.center);
        undistortPoints(in,out,mat1,mat2,noArray(),noArray());//对像素点去畸变
        angle[0]=atan2((out.front().x-cx),fx)/CV_PI*180;
        angle[1]=atan2((out.front().y-cy),fy)/CV_PI*180;
        angle[2]=90;
    }
    else
    {
        //世界坐标系下(正方形装甲板)的坐标，单位为mm
        vector<Point3f> obj=vector<Point3f>{ Point3f(-125/2,135/2,0),
                                             Point3f(125/2,135/2,0),
                                             Point3f(-125/2,135/2,0),
                                             Point3f(-125/2,135/2,0)};
        Mat rVec=Mat::zeros(3,1,CV_64FC1);
        Mat tVec=Mat::zeros(3,1,CV_64FC1);
        //像素坐标系下的坐标
        Point2f pnt[4];//不可Point2f pnt[4]，solvePnP要求最少6个点
        armor.points(pnt);//此处不可传vector
        /*for(int i=0;i<4;i++)
        cout<<pnt[i]<<endl;*/
        vector<Point2f> pnts;//vector可自动转换为InputArray,而数组不可。故再用pnts接住
        for(int i=0;i<4;i++)
        {
            pnts.push_back(pnt[i]);
            pnts.push_back(Point2f((pnt[i].x+pnt[i+1].x)/2,(pnt[i].y+pnt[i+1].y)/2));
        }
        //由solvePnP得到相机坐标系下的坐标
        solvePnP(obj,pnts,mat1,mat2,rVec,tVec,false,SOLVEPNP_ITERATIVE);//这里的第二个参数！！！
        distance=tVec.at<float>(0,2)/100;
        angle[0]=atan2(tVec.at<float>(0,0),tVec.at<float>(0,2))/CV_PI*180;
        angle[1]=atan2(tVec.at<float>(0,1),tVec.at<float>(0,2))/CV_PI*180;
        angle[2]=90;
    }
}
