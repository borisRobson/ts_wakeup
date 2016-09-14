#include "stream.h"
#include "background_detect.h"
#include "detectobject.h"

using namespace cv;
using namespace std;

gboolean bus_cb(GstBus *bus, GstMessage *msg, gpointer user_data);
GstFlowReturn new_preroll(GstAppSink *asink, gpointer user_data);
GstFlowReturn new_buffer(GstAppSink *asink, gpointer user_data);
gboolean timeout_cb(gpointer user_data);

GstElement *input_pipe;
GstElement *output_pipe;
GMainLoop *loop;
GstElement *appsink;
GstElement *appsrc;
GstElement *fsink;

background_detect *bg_detect;
detectobject *detect;
imgThread *imgthread;

gboolean compareImg;
GstBuffer *temp;
int fcount, diff_count, face_count;
MatND ref, comp;


stream::stream()
{
    bg_detect = new background_detect();
    detect = new detectobject();
    imgthread = new imgThread();
}


//gst-launch-0.10 mfw_v4lsrc num-buffers=1 ! videoscale ! capsfilter caps=video/x-raw-yuv,width=640,height=480 ! ffmpegcolorspace ! capsfilter caps=video/x-raw-rgb ! ffmpegcolorspace ! vpuenc codec=12 ! filesink location=/nvdata/tftpboot/cap.jpg

bool stream::buildpipeline()
{
    loop = g_main_loop_new(NULL, FALSE);

    GstElement *src;
    GstElement *scale;
    GstElement *yuvfilter;
    GstElement *yuv_conv;
    GstElement *rgbfilter;
    GstElement *rgb_conv;

    GstElement *enc;

    fcount = diff_count = face_count = 0;

    compareImg = false;

    //create elements
    input_pipe = gst_pipeline_new("input-pipe");
    output_pipe = gst_pipeline_new("output-pipe");

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

    appsrc = gst_element_factory_make("appsrc", NULL);
#ifdef IMX6
    enc = gst_element_factory_make("vpuenc", NULL);
#else
    enc = gst_element_factory_make("jpegenc", NULL);
#endif
    fsink = gst_element_factory_make("fakesink", NULL);

    //confirm elements created
    if(!src || !scale || !yuvfilter || !yuv_conv || !rgbfilter || !rgb_conv || !appsink){
        g_printerr("could not create all input elements\n");
        gst_object_unref(input_pipe);
        gst_object_unref(output_pipe);
        return false;
    }

    if(!appsrc || !enc || !fsink){
        g_printerr("could not create all output elements\n");
        gst_object_unref(input_pipe);
        gst_object_unref(output_pipe);
        return false;
    }
#ifdef IMX6
    g_object_set(src, "capture-mode", 1, NULL);
#endif

    //set filter caps
    //gst_util_set_object_arg(G_OBJECT(yuvfilter), "caps",
    //                        "video/x-raw-yuv,width=640,height=480");
    gst_util_set_object_arg(G_OBJECT(yuvfilter), "caps",
                            "video/x-raw-yuv,width=320,height=240");
    gst_util_set_object_arg(G_OBJECT(rgbfilter), "caps",
                            "video/x-raw-rgb");
   // g_object_set(fsink, "location", "cap%d.jpg", NULL);

    //add elements to pipelines
    gst_bin_add_many(GST_BIN(input_pipe), src, scale, yuvfilter, yuv_conv, rgbfilter, rgb_conv, appsink, NULL);
    gst_bin_add_many(GST_BIN(output_pipe), appsrc, enc, fsink, NULL);

    //link elements
    if(!gst_element_link_many(src, scale, yuvfilter, yuv_conv, rgbfilter, rgb_conv, appsink, NULL)){
        g_printerr("could not link all input elements\n");
        gst_object_unref(input_pipe);
        gst_object_unref(output_pipe);
        return false;
    }

    if(!gst_element_link_many(appsrc, enc, fsink, NULL)){
        g_printerr("could not link all output elements\n");
        gst_object_unref(input_pipe);
        gst_object_unref(output_pipe);
        return false;
    }

    //set app sink properties
    gst_app_sink_set_emit_signals((GstAppSink*)appsink, true);
    gst_app_sink_set_drop((GstAppSink*)appsink, true);
    gst_app_sink_set_max_buffers((GstAppSink*)appsink, 1);
    GstAppSinkCallbacks callbacks = {NULL, new_preroll, new_buffer, NULL};
    gst_app_sink_set_callbacks((GstAppSink*)appsink, &callbacks, NULL, NULL);

    //connect the appsrc buffer signal,
    //set to false initially to prevent it requesting data before we are ready
    gst_app_src_set_emit_signals ((GstAppSrc*)appsrc, false);
   // g_signal_connect(appsrc, "need-data", G_CALLBACK(cb_need_data), NULL);

    //link bus callback
    gst_bus_add_watch(GST_ELEMENT_BUS(input_pipe), bus_cb, loop);
    gst_bus_add_watch(GST_ELEMENT_BUS(output_pipe), bus_cb, loop);

    //set to PAUSED so that all elements are prepared for data flow
    gst_element_set_state(input_pipe, GST_STATE_PAUSED);
    //gst_element_set_state(output_pipe, GST_STATE_PAUSED);

    return true;
}

void stream::startstream()
{
    gst_element_set_state(input_pipe, GST_STATE_PLAYING);

    imgthread->run();
    g_main_loop_run(loop);
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
            if(GST_OBJECT_NAME(msg->src) == GST_OBJECT_NAME(input_pipe) ||GST_OBJECT_NAME(msg->src) == GST_OBJECT_NAME(output_pipe) ){
                g_print("'%s' state changed from %s to %s. \n", GST_OBJECT_NAME(msg->src), gst_element_state_get_name(old_state), gst_element_state_get_name(new_state));
                //if input pipeline is now 'PLAYING' set warped pipeline to 'PLAYING'
                if(GST_OBJECT_NAME(msg->src) == GST_OBJECT_NAME(input_pipe) && new_state == GST_STATE_PLAYING){
                    //gst_element_set_state(input_pipe, GST_STATE_PLAYING);
                }
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
    //g_timeout_add(250, timeout_cb, loop);
    compareImg = false;
    return GST_FLOW_OK;
}

GstFlowReturn new_buffer(GstAppSink *asink, gpointer data)
{
    fcount++;
    Q_UNUSED(data);
    //GstBufferCopyFlags copyflags = GST_BUFFER_COPY_ALL;
    GstBuffer *buffer;

    //discard initial buffers
    if(fcount < 10){
        return GST_FLOW_OK;
    }

    //set emit signals to false whilst processing
    //gst_app_sink_set_emit_signals((GstAppSink*)appsink, false);

    vector<int>compression_params;
    compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(9);


    if(fcount == 10){
        //grab available buffer
        buffer =  gst_app_sink_pull_buffer(asink);

        //convert GstBuffer to Mat
        //Mat frame(Size(640,480), CV_8UC3, (char*)GST_BUFFER_DATA(buffer), Mat::AUTO_STEP);
        Mat frame(Size(320,240), CV_8UC3, (char*)GST_BUFFER_DATA(buffer), Mat::AUTO_STEP);

        Mat frame2;
        //gstreamer buffer is rgb, opencv wants bgr.
        cvtColor(frame, frame2,CV_RGB2BGR);

        char* file = new char[1024];
    #ifdef IMX6
        sprintf(file, "/nvdata/tftpboot/ref%d.png", fcount);
    #else
        sprintf(file, "ref%d.png", fcount);
    #endif
        imwrite(file, frame2, compression_params);
        delete [] file;

        ref = frame2;

        gst_buffer_unref(buffer);

        compareImg = true;

        return GST_FLOW_OK;

    }

    else if(compareImg == true){
        qDebug() << "comparing ";
        compareImg = false;
        gst_app_sink_set_emit_signals((GstAppSink*)appsink, false);
        QTime t;
        t.start();
        //grab available buffer
        buffer =  gst_app_sink_pull_buffer(asink);

        //convert GstBuffer to Mat
        //Mat frame(Size(640,480), CV_8UC3, (char*)GST_BUFFER_DATA(buffer), Mat::AUTO_STEP);
        Mat frame(Size(320,240), CV_8UC3, (char*)GST_BUFFER_DATA(buffer), Mat::AUTO_STEP);

        Mat frame2;
        //gstreamer buffer is rgb, opencv wants bgr.
        cvtColor(frame, frame2,CV_RGB2BGR);

        double comparison = imgthread->compareImages(ref,frame2);

        int time = t.elapsed();
        cout << "imgthread time: " << time << endl;

        if(comparison < 1.0f){
            Mat face = detect->findFace(frame2);
            if(!face.empty()){
                face_count++;

                char* file = new char[1024];
#ifdef IMX6
                sprintf(file, "/nvdata/tftpboot/face%d.png", face_count);
#else
                sprintf(file, "face%d.png", face_count);
#endif
                imwrite(file, face, compression_params);
                delete [] file;
                qDebug() << "***Person Detected***";
            }
        }

        cout << "detection time: " << t.elapsed() - time <<  " ms" << endl;

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


imgThread::imgThread()
{

}

void imgThread::run()
{
    qDebug() << "thread running";

}

double imgThread::compareImages(Mat ref, Mat comp)
{
    double comparison =  bg_detect->compareImages(ref, comp);
    return comparison;
}
