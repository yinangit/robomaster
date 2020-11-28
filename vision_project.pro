TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_LFLAGS+=-no-pie
SOURCES += vision_project.cpp \
    adjustRec.cpp \
    pretreatment.cpp \
    getlight.cpp \
    armor_recognition.cpp \
    calculate.cpp

IINCLUDEPATH += /usr/local/include \
/usr/local/include/opencv \
/usr/local/include/opencv2

CONFIG(debug|release, debug|release):
{
LIBS += /usr/local/lib/libopencv_calib3d.so \
/usr/local/lib/libopencv_core.so \
/usr/local/lib/libopencv_features2d.so \
/usr/local/lib/libopencv_flann.so \
/usr/local/lib/libopencv_highgui.so \
/usr/local/lib/libopencv_imgcodecs.so \
/usr/local/lib/libopencv_imgproc.so \
/usr/local/lib/libopencv_ml.so \
/usr/local/lib/libopencv_objdetect.so \
/usr/local/lib/libopencv_photo.so \
/usr/local/lib/libopencv_shape.so \
/usr/local/lib/libopencv_stitching.so \
/usr/local/lib/libopencv_superres.so \
/usr/local/lib/libopencv_videoio.so \
/usr/local/lib/libopencv_video.so \
/usr/local/lib/libopencv_videostab.so
}

HEADERS += \
    LightDescriptor.h \
    armor_param.h \
    armor_descriptor.h \
    camera_param.h
