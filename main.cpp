#include <QCoreApplication>
#include "stream.h"


stream *strm;


int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    //parent task to app for cleanup
    Task* task = new Task(&app);

    QObject::connect(task, SIGNAL(finished()), &app, SLOT(quit()));

    //build and run the pipeline
     strm = new stream();

     QObject::connect(&app, SIGNAL(aboutToQuit()), strm, SLOT(exit()));

    //init gstreamer
    if(!gst_is_initialized())
        gst_init(NULL,NULL);

    //run program
    QTimer::singleShot(10,task,SLOT(run()));

    return app.exec();
}

void Task::run()
{
    bool pipeBuilt;

    qDebug() << __FUNCTION__;


    //create and link pipeline elements
    pipeBuilt = strm->buildpipeline();

    //if both successful; run main loop
    if(pipeBuilt)
        strm->startstream();

    //quit when done
    QCoreApplication::instance()->quit();

    return;
}
