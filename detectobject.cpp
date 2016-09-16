#include "detectobject.h"

using namespace cv;
using namespace std;

CascadeClassifier faceCascade;

detectobject::detectobject()
{
    //init cascades
#ifdef IMX6
    const char* faceCascadeFilename = "/paxton_apps/ts_wakeup/cascades/lbpcascade_frontalface.xml";
#else
    const char* faceCascadeFilename = "./cascades/lbpcascade_frontalface.xml";
#endif



    faceCascade.load(faceCascadeFilename);

    if(faceCascade.empty()){
        qDebug() << "error loading cascade files";
        qDebug() << "check filenames and paths";
        return;
    }else{
        qDebug() << "cascades loaded";
    }
}

Mat detectobject::findFace(Mat &image)
{    
    //convert image to greyscale
    //Mat grey(Size(640,480),CV_8UC1);
    Mat grey(Size(320,240),CV_8UC1);
    //detect channels and apply appropriate conversion
    if(image.channels() == 3){
        cvtColor(image,grey,CV_BGR2GRAY);
    }else if(image.channels() == 4){
        cvtColor(image,grey, CV_BGRA2GRAY);
    }else{
        grey = image;
    }

    //equalise hist - smooths differences in lighting
    equalizeHist(grey,grey);

    //find larget object in image : face
    vector<Rect> objects;
    Rect faceRect;

    detectlargestobject(grey, faceCascade, objects);

    if(objects.size() > 0){
        //set largest object
        faceRect = objects.at(0);
    }else{
        //set false rect
        faceRect = Rect(-1,-1,-1,-1);
        return Mat();
    }

    if (faceRect.width > 0){
        //return face
        Mat face = grey(faceRect);
        //int detectWidth = 200;
        //resize(face,face,Size(detectWidth,detectWidth));
        return face;
    }else{
        //return empty Mat
        qDebug() << " no face found";
        return Mat();
    }
}

void detectobject::detectlargestobject(Mat &image, CascadeClassifier &cascade, vector<Rect> &objects)
{
    bool scaled = false;
    //set flags for single object detection
    int flags = CASCADE_FIND_BIGGEST_OBJECT;

    /*
        set detection parameters:
        -min size
        -search detail
        -false detection threshold
    */
    Size minFeatureSize = Size(30,30);
    float searchScaleFactor = 1.2f;
    int minNeighbours = 3;

    Mat dst;
    image.copyTo(dst);

    //resize large image for faster processing time
    if(dst.cols > 400){
        resize(dst, dst, Size(), 0.5, 0.5);
        scaled = true;
    }

    //opencv obj detect function
    cascade.detectMultiScale(dst,objects,searchScaleFactor,minNeighbours, flags, minFeatureSize);

    //resize if scaled
    if(scaled){
        for(uint i = 0; i < objects.size(); i++){
            objects[i].x = cvRound(objects[i].x * 2);
            objects[i].y = cvRound(objects[i].y * 2);
            objects[i].width = cvRound(objects[i].width * 2);
            objects[i].height = cvRound(objects[i].height * 2);
        }
    }
    return;
}







