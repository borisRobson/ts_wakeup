#include "stream.h"
#include "detectobject.h"

using namespace cv;
using namespace std;

//gstreamer function prototypes
gboolean bus_cb(GstBus *bus, GstMessage *msg, gpointer user_data);
GstFlowReturn new_preroll(GstAppSink *asink, gpointer user_data);
GstFlowReturn new_buffer(GstAppSink *asink, gpointer user_data);

//gst elements
GstElement *input_pipe;
GMainLoop *loop;
GstElement *appsink;

detectobject *detect;
stream *this_stream;

gboolean compareImg;
int fcount;
QUdpSocket *socket;
QDomDocument document;

static int START_THRESH = 3;

stream::stream()
{
    detect = new detectobject();
    this_stream = this;
    socket = new QUdpSocket(this);
    QHostAddress *addr = new QHostAddress("127.0.0.1");
    socket->connectToHost(*addr, 8888);

    QFile file("/nvdata/tftpboot/settings.xml");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "Could not open file";

    }
    else{
        if(!document.setContent(&file)){
            qDebug() << "Could not read file";

        }
        file.close();
    }
}


//gst-launch-0.10 mfw_v4lsrc capture-mode=1 ! videoscale ! video/x-raw-yuv,width=320,height=240 ! ffmpegcolorspace ! video/x-raw-rgb ! ffmpegcolorspace ! appsink

bool stream::buildpipeline()
{
    loop = g_main_loop_new(NULL, FALSE);

    GstElement *src;
    GstElement *scale;
    GstElement *yuvfilter;
    GstElement *yuv_conv;
    GstElement *rgbfilter;
    GstElement *rgb_conv;

    //initialise count to 0
    fcount = 0;
    //ensure initialised to false
    compareImg = false;

    //create elements
    input_pipe = gst_pipeline_new("input-pipe");
#ifdef IMX6
    src = gst_element_factory_make("mfw_v4lsrc", NULL);
#else
    src = gst_element_factory_make("v4l2src", NULL);
#endif
    scale = gst_element_factory_make("videoscale", NULL);
    yuvfilter = gst_element_factory_make("capsfilter", NULL);
    yuv_conv = gst_element_factory_make("ffmpegcolorspace", NULL);
    rgbfilter = gst_element_factory_make("capsfilter", NULL);
    rgb_conv = gst_element_factory_make("ffmpegcolorspace", NULL);
    appsink = gst_element_factory_make("appsink", NULL);

    //confirm elements created
    if(!src || !scale || !yuvfilter || !yuv_conv || !rgbfilter || !rgb_conv || !appsink){
        g_printerr("could not create all input elements\n");
        gst_object_unref(input_pipe);

        return false;
    }

    QDomElement root = document.firstChildElement();
    QString str = detect->getvalues(root, "param",  "Name", "size");

#ifdef IMX6

    if(str == (QString)"large"){
        g_object_set(src, "capture-mode", 0 , NULL);
    }else{
        g_object_set(src, "capture-mode", 1, NULL);
    }

#endif

    if(str == (QString)"large"){
        gst_util_set_object_arg(G_OBJECT(yuvfilter), "caps",
                                "video/x-raw-yuv,width=640,height=480");
    }else{
        gst_util_set_object_arg(G_OBJECT(yuvfilter), "caps",
                                "video/x-raw-yuv,width=320,height=240");
    }

    //set filter caps
    gst_util_set_object_arg(G_OBJECT(rgbfilter), "caps",
                            "video/x-raw-rgb");

    //add elements to pipelines
    gst_bin_add_many(GST_BIN(input_pipe), src, scale, yuvfilter, yuv_conv, rgbfilter, rgb_conv, appsink, NULL);

    //link elements
    if(!gst_element_link_many(src, scale, yuvfilter, yuv_conv, rgbfilter, rgb_conv, appsink, NULL)){
        g_printerr("could not link all input elements\n");
        gst_object_unref(input_pipe);

        return false;
    }

    //set app sink properties
    gst_app_sink_set_emit_signals((GstAppSink*)appsink, true);
    gst_app_sink_set_drop((GstAppSink*)appsink, true);
    gst_app_sink_set_max_buffers((GstAppSink*)appsink, 1);
    GstAppSinkCallbacks callbacks = {NULL, new_preroll, new_buffer, NULL};
    gst_app_sink_set_callbacks((GstAppSink*)appsink, &callbacks, NULL, NULL);

    //link bus callback
    gst_bus_add_watch(GST_ELEMENT_BUS(input_pipe), bus_cb, loop);

    //set to PAUSED so that all elements are prepared for data flow
    gst_element_set_state(input_pipe, GST_STATE_PAUSED);

    return true;
}

void stream::startstream()
{
    //set pipeline to playing
    gst_element_set_state(input_pipe, GST_STATE_PLAYING);
    //enter main loop
    g_main_loop_run(loop);

    //once exited set pipeline to null and unref
    qDebug() << "unreffing";
    gst_element_set_state(input_pipe, GST_STATE_NULL);
    gst_object_unref(input_pipe);
}

gboolean bus_cb(GstBus *bus, GstMessage *msg, gpointer user_data)
{
    Q_UNUSED(bus);
    Q_UNUSED(user_data);

    //parse bus messages
    switch(GST_MESSAGE_TYPE(msg)){
        case GST_MESSAGE_ERROR:{
            //quit on error
            GError *err = NULL;
            gchar *dbg;
            gst_message_parse_error(msg, &err, &dbg);
            gst_object_default_error(msg->src, err, dbg);
            g_clear_error(&err);
            g_free(dbg);
            g_main_loop_quit(loop);
            break;
        }
        case GST_MESSAGE_STATE_CHANGED:{
            GstState old_state, pending_state, new_state;
            gst_message_parse_state_changed(msg, &old_state, &new_state, &pending_state);
            //if message from either pipeline, display state change
            if(GST_OBJECT_NAME(msg->src) == GST_OBJECT_NAME(input_pipe)  ){
                g_print("'%s' state changed from %s to %s. \n", GST_OBJECT_NAME(msg->src), gst_element_state_get_name(old_state), gst_element_state_get_name(new_state)); 
            }
            break;
        }
        default:{
            break;
        }
    }
    return true;
}

GstFlowReturn new_preroll(GstAppSink *asink, gpointer user_data)
{
    Q_UNUSED(asink);
    Q_UNUSED(user_data);
    g_print("got preroll\n");
    compareImg = true;
    return GST_FLOW_OK;
}

GstFlowReturn new_buffer(GstAppSink *asink, gpointer data)
{
    fcount++;
    Q_UNUSED(data);
    GstBuffer *buffer;

    //discard initial buffers
    if(fcount < START_THRESH){
        return GST_FLOW_OK;
    }

    Size img_size;
    QDomElement root = document.firstChildElement();
    QString str = detect->getvalues(root, "param",  "Name", "size");

    if(str == (QString)"large"){
        img_size = Size(640,480);
    }else{
        img_size = Size(320,240);
    }

    if(compareImg == true){
        compareImg = false;
        gst_app_sink_set_emit_signals((GstAppSink*)appsink, false);
        QTime t;
        t.start();
        //grab available buffer
        buffer =  gst_app_sink_pull_buffer(asink);

        //convert GstBuffer to Mat
        Mat frame(img_size, CV_8UC3, (char*)GST_BUFFER_DATA(buffer), Mat::AUTO_STEP);

        Mat frame2;
        //gstreamer buffer is rgb, opencv wants bgr.
        cvtColor(frame, frame2,CV_RGB2BGR);

        Mat face = detect->findFace(frame2);
        if(!face.empty()){
            qDebug() << "***Person Detected***";
            this_stream->sendPanelMessage();
            compareImg = false;
            g_main_loop_quit(loop);
            return GST_FLOW_OK;
        }

        cout << "time elapsed: " << t.elapsed() << " ms" << endl;

        compareImg = true;

        gst_buffer_unref(buffer);

        gst_app_sink_set_emit_signals((GstAppSink*)appsink, true);
    }
    return GST_FLOW_OK;
}

gboolean timeout_cb(gpointer user_data)
{
    Q_UNUSED(user_data);
    g_print("timeout\n");
    compareImg = !compareImg;
    return true;
}

void stream::sendPanelMessage()
{
    qDebug() << __FUNCTION__;
    QByteArray arr = (QByteArray)"camerawakeup";
    socket->write(arr);
}

void stream::exit()
{
    qDebug() << "exit";
    if(g_main_loop_is_running(loop)){
        g_main_loop_quit(loop);
    }
}

