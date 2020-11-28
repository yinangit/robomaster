#include <iostream>
#include<opencv2/opencv.hpp>
#include<opencv2/imgproc/types_c.h>
#include<math.h>
#include<opencv2/highgui.hpp>
#include<LightDescriptor.h>
#include<armor_descriptor.h>
#include<armor_param.h>
//#include<Fightingdahengcapture.h>
using namespace cv;
using namespace std;//下一步：加载识别的库？？红蓝不同？？？条件编译？？？

int main()
{
    Mat pretreatment(Mat src);//需要声明，不用文件包含？？
    void adjustRec(RotatedRect& rec, const int mode);
    bool getlight(Mat &src,Mat&src1,RotatedRect *record);
    bool armor_recognotion(Mat &src,RotatedRect record[5],Point2i *point,RotatedRect &armor);
    void calculate(RotatedRect armor,float &distance,float *angle);
    //测试蓝车视频路径/home/yinan/objects(opencv)/testvideo/步兵素材蓝车旋转-ev-0.MOV（预处理不同）
    VideoCapture cap("/home/yinan/objects(opencv)/testvideo/步兵素材红车旋转-ev-0.MOV");//error??
    //VideoCapture cap;
    //cap.open(1);
    /*FightingCapture* cap;
    cap = new FightingDahengCapture();*/
    if (!cap.isOpened()) return -1;
    Mat src,dst1;
    //src.create(cap.grab().size(),cap.grab().type());
    while (cap.grab())//下一帧是否为空
    {
    cap >> src;
    dst1=pretreatment(src);//预处理
    RotatedRect record[10];//=RotatedRect{Point(320, 240), Size(200, 200), 30}
    if(!getlight(dst1,src,record))
        putText(src, "No light!", Point(50, 100),CV_FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 255));
    else putText(src, "Getlight!", Point(50, 100),CV_FONT_HERSHEY_PLAIN, 2, Scalar(255, 0, 0));
    /*try//异常处理
    {
        if(!getlight(dst1,src,record)) throw int(1);
        else putText(src, "Shoot!", Point(50, 150),CV_FONT_HERSHEY_PLAIN, 2, Scalar(255, 0, 0));
    }
    catch(int)
    {
        putText(src, "No shooting!", Point(50, 150),CV_FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 255));
        continue;
    }
    catch(...)
    {
        continue;
    }*/
    Point2i point[10];//用于存储匹配的灯条(不用Point2f,因为他的作用是储存整数，否则)   
    RotatedRect armor;

    if(armor_recognotion(src,record,point,armor))
    {
        float distance=0;
        float angle[3]={0};
        calculate(armor,distance,angle);
        putText(src, "success!", Point(50, 150),CV_FONT_HERSHEY_PLAIN, 2, Scalar(255, 0, 0));       
        /*char text0[20] = {};//putText为输出文本
        memset(text0, 0, 20*sizeof(char));
        sprintf(text0, "%.2f",distance);
        putText(src, text0, Point(300, 200),CV_FONT_HERSHEY_PLAIN, 2, Scalar(255, 0, 0));*/
        putText(src, "Distance:  "+to_string(distance)+" meter", Point(50, 200),CV_FONT_HERSHEY_PLAIN, 2, Scalar(255, 0, 0));
        putText(src, "Angle_x :  "+to_string(angle[0])+" degree", Point(50, 250),CV_FONT_HERSHEY_PLAIN, 2, Scalar(255, 0, 0));
        putText(src, "Angle_y :  "+to_string(angle[1])+" degree", Point(50, 300),CV_FONT_HERSHEY_PLAIN, 2, Scalar(255, 0, 0));
        putText(src, "Angle_z :  "+to_string(angle[2])+" degree", Point(50, 350),CV_FONT_HERSHEY_PLAIN, 2, Scalar(255, 0, 0));
        imshow("Fighting_vision", src);
        waitKey(500);
    }
    else
    {
        putText(src, "defeat!", Point(50, 150),CV_FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 255));
        imshow("Fighting_vision", src);
    }

    char c =waitKey(20);//可实现控制帧速率
    if (c == 52) break;//右边键盘4为数字，左一横排为字符？waitKey(1000)
    }
    return 0;
}
