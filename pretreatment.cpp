#include <iostream>
#include<opencv2/opencv.hpp>
#include<opencv2/imgproc/types_c.h>
#include<math.h>
#include<opencv2/highgui.hpp>
#include<LightDescriptor.h>
using namespace cv;
using namespace std;

extern Mat pretreatment(Mat src)//返回Mat对象的引用？？
{
    Mat gray_src,dst,element;
    vector<Mat> channels;
    split(src,channels);//分离色彩通道
    gray_src=channels[2]-channels[0]; //识别红色物体可以直接用r通道-b通道。在低曝光下只有灯条是有颜色的，两通道相减,剩下的灰度图只留下了红色灯条
    threshold(gray_src,dst,100,255,THRESH_BINARY);//阈值处理得到二值图
    element = getStructuringElement(MORPH_ELLIPSE, Size(3, 3)); //膨胀处理，使得图像中的轮廓更加明显
    dilate(dst, dst, element);

    /*Mat gray_src,dst1,dst2,element,dst;//红车上面效果更好，蓝车这个效果更好（颜色差异？？？蓝车的cvtColor不抛错？？？cvtColor参数的选择？）
    cvtColor(src,gray_src,CV_BGR2GRAY);
    threshold(gray_src,dst1,100,255,THRESH_BINARY);

    vector<Mat> channels;
    split(src,channels);
    subtract(channels[0],channels[2],dst2);//测试蓝车时用channels[0]-channels[2]
    threshold(dst2,dst2,110,255,THRESH_BINARY);
    element = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
    dilate(dst2, dst2, element);
    dst=dst1&dst2;
    dilate(dst, dst, element);*/

    return dst;
}
