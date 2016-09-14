#include "background_detect.h"

Ptr<BackgroundSubtractor> pMOG2;
Mat fgMask;
Mat ref_image;
Mat comp_image;
int imgcount;
Mat ref_hsv, comp_hsv;
MatND ref_hist, comp_hist;

background_detect::background_detect()
{
    imgcount = 0;
}

double background_detect::compareImages(Mat ref, Mat comp)
{
    ref_image = ref;
    comp_image = comp;

    //convert to hsv
    if(ref_hsv.empty()){
        cvtColor(ref, ref_hsv, CV_BGR2HSV);
    }
    cvtColor(comp,comp_hsv, CV_BGR2HSV);

    //use 50 bins for hue and 60 for saturation
    int h_bins = 50;
    int s_bins = 60;
    int histSize[] = {h_bins,s_bins};

    //hue varies from 0 - 179, saturation from 0 - 255
    float h_ranges[] = {0,180};
    float s_ranges[] = {0,255};

    const float* ranges[] = {h_ranges, s_ranges};

    int channels[] = {0,1};

    //calculate histograms
    if(ref_hist.empty()){
        calcHist(&ref_hsv, 1, channels, Mat(), ref_hist,2, histSize, ranges, true, false);
        normalize(ref_hist, ref_hist, 0, 1, NORM_MINMAX, -1, Mat());
    }

    calcHist(&comp_hsv, 1, channels, Mat(), comp_hist,2, histSize, ranges, true, false);
    normalize(comp_hist, comp_hist, 0, 1, NORM_MINMAX, -1, Mat());

    double comparison = compareHist(ref_hist, comp_hist, 0);

    return comparison;
}

void background_detect::drawThresh()
{
    imgcount++;
    //create the subtractor
    pMOG2 = new BackgroundSubtractorMOG2();

    //convert to greyscale
    Mat ref_grey, comp_grey;
    cvtColor(ref_image, ref_grey, CV_BGR2GRAY);
    cvtColor(comp_image, comp_grey, CV_BGR2GRAY);

    //load in background image
    pMOG2->operator ()(ref_image,fgMask);
    pMOG2->operator ()(comp_image, fgMask);

    vector<int>compression_params;
    compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(9);

    char* file = new char[1024];
#ifdef IMX6
    sprintf(file, "/nvdata/tftpboot/cap%d.png", imgcount);
#else
    sprintf(file, "cap%d.png", imgcount);
#endif
    imwrite(file, fgMask, compression_params);
    delete [] file;


}
