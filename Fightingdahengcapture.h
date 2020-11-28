#ifndef FIGHTINGDAHENGCAPTURE_H
#define FIGHTINGDAHENGCAPTURE_H
//#include "FightingCapture.h"
#include <opencv2/opencv.hpp>
//#include "CircularQueue.h"

//#include "Include/Daheng_include/GxIAPI.h"
//#include "Include/Daheng_include/DxImageProc.h"

class FightingDahengCapture : public FightingCapture
{
public:
    FightingDahengCapture()
    {
        hDevice = NULL;
    }

    ~FightingDahengCapture()
    {
        //发送停采命令
        GXSendCommand(hDevice, GX_COMMAND_ACQUISITION_STOP);
        //注销采集回调
        GXUnregisterCaptureCallback(hDevice);
        GXCloseDevice(hDevice);
        GXCloseLib();
    }



    /*void GX_STDC OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM* pFrame)
    {
        if (pFrame->status == GX_FRAME_STATUS_SUCCESS)
        {
            VxInt32 DxStatus = DxRaw8toRGB24Ex((void*)(pFrame->pImgBuf), pFrameBuffer, 1280, 1024,cvtype, nBayerType, bFlip, emChannelOrder);
            cv::Mat matImage(cv::Size(pFrame->nWidth, pFrame->nHeight), CV_8UC3,pFrameBuffer);
            circular_queue.push(matImage.clone());
        }
        return;
    }*/

    bool init()
    {
        GX_STATUS status;
        //在起始位置调用GXInitLib()进行初始化，申请资源
        status = GXInitLib();
        if (status != GX_STATUS_SUCCESS)
        {
            printf("Camera init fail");
            return 0;
        }

        uint32_t nDeviceNum = 0;
        status = GXUpdateDeviceList(&nDeviceNum, 1000);
        if (status != GX_STATUS_SUCCESS || nDeviceNum <= 0)
        {
            printf("Camera enumrate fail");
            return 0;
        }

        GX_OPEN_PARAM stOpenParam;

        //打开枚举列表中的第一台设备。
        //假设枚举到了3台可用设备，那么用户可设置stOpenParam参数的pszContent字段为1、2、3
        stOpenParam.accessMode = GX_ACCESS_EXCLUSIVE;
        stOpenParam.openMode = GX_OPEN_INDEX;
        stOpenParam.pszContent = "1";

        status = GXOpenDevice(&stOpenParam, &hDevice);

        if (status != GX_STATUS_SUCCESS)
        {
            printf("Camera open fail");
            return 0;
        }

        //设置参数

        //关闭自动白平衡
        GXSetEnum(hDevice, GX_ENUM_BALANCE_WHITE_AUTO, GX_BALANCE_WHITE_AUTO_OFF);
        //设置白平衡系数
        GXSetFloat(hDevice, GX_FLOAT_BALANCE_RATIO, 1.6211);

        //读取当前值
        int64_t nValueW = 0;
        int64_t  nValueH = 0;
        GXGetInt(hDevice, GX_INT_WIDTH, &nValueW);
        GXGetInt(hDevice, GX_INT_WIDTH, &nValueH);

        //设置采集buffer个数
        status = GXSetAcqusitionBufferNumber(hDevice, 10);

        //声明图像格式转换句柄
        //DX_IMAGE_FORMAT_CONVERT_HANDLE handle;
        //创建图像格式转换句柄
        //DxImageFormatConvertCreate(&handle);
        //GX_PIXEL_FORMAT_ENTRY emPixelFormat = GX_PIXEL_FORMAT_BGR8;
        //设置图像输出格式
        //DxImageFormatConvertSetOutputPixelFormat(handle, emPixelFormat);

        //注册图像处理回调函数
        status = GXRegisterCaptureCallback(hDevice, this, OnFrameCallbackFun);

        //发送开采命令
        status = GXSendCommand(hDevice, GX_COMMAND_ACQUISITION_START);
        //---------------------
        //
        //在这个区间图像会通过OnFrameCallbackFun接口返给用户
        //
        //---------------------
        printf("camera init success\n");
        return true;
    }

    bool read(cv::Mat& image)
    {
        double start = static_cast<double>(cv::getTickCount());
        while (circular_queue.empty()) {
            double time = ((double)cv::getTickCount() - start) / cv::getTickFrequency();
            if (time > 0.5)
                return false;
        }
        return circular_queue.pop(image);
    }

private:

    GX_DEV_HANDLE hDevice;
    static BYTE* pFrameBuffer;

    static DX_BAYER_CONVERT_TYPE cvtype; //选择插值算法
    static DX_PIXEL_COLOR_FILTER nBayerType;  //选择图像Bayer格式
    static bool bFlip;
    static DX_RGB_CHANNEL_ORDER emChannelOrder;

    static CircularQueue<cv::Mat, 10> circular_queue;
};

#endif // FIGHTINGDAHENGCAPTURE_H
