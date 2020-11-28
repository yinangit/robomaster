#include <iostream>
#include<opencv2/opencv.hpp>
#include<opencv2/imgproc/types_c.h>
#include<math.h>
#include<opencv2/highgui.hpp>
#include<LightDescriptor.h>
using namespace cv;
using namespace std;

extern vector<RotatedRect> light_match(Mat &image)
{
    Rect  external_rect;
    float area;
    Mat temp_image = image.clone();
    vector<vector<Point>>  contour;
    findContours(temp_image, contour, RETR_EXTERNAL, CHAIN_APPROX_NONE);
    vector<RotatedRect>  ellipsee(contour.size());
    vector<RotatedRect>  ellipsee1, ellipsee2, ellipsee3, ellipsee4, finally;
    int count1 = 0;
    for (int i0 = 0; i0 < contour.size(); i0++)
    {
        if (contour[i0].size()>10)
        {
            ellipsee[i0] = fitEllipse(contour[i0]);
            external_rect = ellipsee[i0].boundingRect();
            area = external_rect.area();//面积
            if (area>30 && external_rect.height>0 && external_rect.width>0 && ((double)external_rect.height / (double)external_rect.width)<6.7 && (ellipsee[i0].angle<rotate_angle || ellipsee[i0].angle>(180 - rotate_angle)))
            {
                ellipsee1.push_back(ellipsee[i0]);
                count1++;
            }
        }
    }

    int i2, j2;
    int flag = 1;
    RotatedRect temp;
    for (i2 = 1; i2 < ellipsee1.size() && flag == 1; i2++)
    {
        flag = 0;
        for (j2 = 0; j2 < ellipsee1.size() - i2; j2++)
        {
            if (ellipsee1[j2].size.height < ellipsee1[j2 + 1].size.height)
            {
                flag = 1;
                temp = ellipsee1[j2];
                ellipsee1[j2] = ellipsee1[j2 + 1];
                ellipsee1[j2 + 1] = temp;
            }
        }
    }

    int count2 = 0;
    int horizontal_angle_real;
    for (int i1 = 0; i1 < count1; i1++)
    {
        for (int j1 = i1 + 1; j1 < count1; j1++)
        {
            horizontal_angle_real = hor_angle(ellipsee1[i1].center, ellipsee1[j1].center);
            if (horizontal_angle_real < horizontal_angle && ((ellipsee1[i1].center.x - ellipsee1[j1].center.x)<780 || (ellipsee1[i1].center.x - ellipsee1[j1].center.x)>184) && (ellipsee1[i1].center.y - ellipsee1[j1].center.y)<50)
            {
                ellipsee2.push_back(ellipsee1[i1]);
                ellipsee2.push_back(ellipsee1[j1]);
                count2++;
                if (ellipsee2.size() == 2)
                {
                    break;
                }
            }
        }
        if (ellipsee2.size() == 2)
        {
            break;
        }
    }
    finally = ellipsee2;

    return finally;

}
