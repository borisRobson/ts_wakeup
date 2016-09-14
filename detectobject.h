#ifndef DETECTOBJECT_H
#define DETECTOBJECT_H

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include <vector>
#include <QtCore>
#include <QDebug>
#include <stdio.h>

using namespace cv;
using namespace std;

class detectobject
{
public:
    detectobject();
    ~detectobject();
    Mat findFace(Mat& image);
    void detectlargestobject(Mat& image, CascadeClassifier &cascade, vector<Rect> &objects);
};

#endif // DETECTOBJECT_H
