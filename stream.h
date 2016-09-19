#ifndef STREAM_H
#define STREAM_H

#include <QObject>
#include <QDebug>
#include <QTime>
#include <QTimer>
#include <QFile>
#include <vector>
#include <QThread>
#include <string>
#include <QtNetwork>

#include "gst/gst.h"
#include "gst/app/gstappsink.h"
#include "gst/app/gstappsrc.h"
#include "glib-2.0/glib.h"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

class stream : public QObject
{
    Q_OBJECT
public:
    stream();
    bool buildpipeline();
    void startstream();
    void sendPanelMessage();
public slots:
    void exit();



};


class Task: public QObject{
    Q_OBJECT
public:
    Task(QObject* parent=0) : QObject(parent){}
private:

public slots:
    void run();
signals:
    void finished();
};

class imgThread : public QThread
{
public:
    imgThread();
    void run();
    double compareImages(Mat& ref, Mat& comp);

};

#endif // STREAM_H
