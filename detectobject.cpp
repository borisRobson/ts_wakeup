#include "detectobject.h"

using namespace cv;
using namespace std;

CascadeClassifier faceCascade;
QDomDocument doc;

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
    doc = loadsettings("/nvdata/tftpboot/settings.xml");
}

Mat detectobject::findFace(Mat &image)
{    
    Mat grey;
    int width = image.cols;
    if(width == 320){
        grey = Mat(Size(320,240), CV_8UC1);
    }else{
        grey = Mat(Size(640,480), CV_8UC1);
    }
    //convert image to greyscale

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
        grey.release();
        return face;
    }else{
        grey.release();
        //return empty Mat
        qDebug() << " no face found";
        return Mat();
    }
}

void detectobject::detectlargestobject(Mat &image, CascadeClassifier &cascade, vector<Rect> &objects)
{ 
    //set flags for single object detection
    int flags = CASCADE_FIND_BIGGEST_OBJECT;

    /*
        set detection parameters:
        -minimum object size
        -image scaling
        -detection threshold - (1 - 6) - low is less strict
    */

    QDomElement root = doc.firstChildElement();
    QString str = getvalues(root, "param",  "Name", "minNeighbours");
    int minNeighbours = str.toInt();

    str = getvalues(root, "param",  "Name", "scaleFactor");
    float searchScaleFactor = ((float)(str.toInt())/1000);

    str = getvalues(root, "param", "Name", "minFeature");
    int minFeature = str.toInt();
    Size minFeatureSize = Size(minFeature, minFeature);

    Mat dst;
    image.copyTo(dst);

    //opencv obj detect function
    cascade.detectMultiScale(dst,objects,searchScaleFactor,minNeighbours, flags, minFeatureSize);

    dst.release();

    return;
}

QDomDocument detectobject::loadsettings(QString filename)
{
    /*
     *Try to load /nvdata file, this could be lost if system
     * has rebooted. This is for test only,
     *once values are decided xml files can be kept in
     *permanent storage
     */
    QDomDocument document;
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "Could not open file,using default";
        goto usedefault;
    }
    else{
        if(!document.setContent(&file)){
            qDebug() << "Could not open file";
        }
        file.close();
    }
    return document;

usedefault:
    QFile defFile("./default.xml");
    if(!defFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "Could not open default";
        goto usedefault;
    }
    else{
        if(!document.setContent(&defFile)){
            qDebug() << "Could not open file";
        }
        defFile.close();
    }
    return document;

}


QString detectobject::getvalues(QDomElement root, QString tagname, QString attribute, QString valname)
{
    QString val;
    QDomNodeList items = root.elementsByTagName(tagname);

    for(int i = 0; i < items.count(); i++){
        QDomNode itemnode = items.at(i);

        //convert to element
        if(itemnode.isElement()){
            QDomElement itemel = itemnode.toElement();
            if(itemel.attribute(attribute) == valname){
                val = itemel.text();
            }
        }
    }
    return val;
}







