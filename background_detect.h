#ifndef BACKGROUND_DETECT_H
#define BACKGROUND_DETECT_H

#include <QObject>
#include <QtCore>
#include <QDebug>
#include <QTime>
#include <QTimer>
#include <QFile>
#include <vector>
#include <string>


#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

class background_detect
{
public:
    background_detect();
    double compareImages(Mat ref, Mat comp);
    void drawThresh();

};

#endif // BACKGROUND_DETECT_H
