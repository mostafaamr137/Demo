#include <jni.h>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <iostream>

using namespace cv;
using namespace std;
extern "C" {
JNIEXPORT jstring

JNICALL
Java_com_example_mostafa_demo_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
jstring
Java_com_example_mostafa_demo_MainActivity_salt(JNIEnv *env, jobject instance,
                                                jlong matAddrGray) {
    Mat &img=*(Mat*)matAddrGray;
    Mat img_threshold;
    Mat img_gray;
    Mat img_roi;
    Rect roi(50, 50, 500, 500);
    img_roi = img(roi);
    rectangle(img, roi, Scalar(255), 1, 8, 0);
    cvtColor(img_roi, img_gray, CV_RGB2GRAY);

    GaussianBlur(img_gray, img_gray, Size(19, 19), 0.0, 0);
    threshold(img_gray, img_threshold, 0, 255, THRESH_BINARY_INV + THRESH_OTSU);
    vector<vector<Point>> Contours;
    vector<Vec4i> hier;
    findContours(img_threshold, Contours, hier, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE, Point(0, 0));

    //finding the contour of largest area and storing its index
    int lrgctridx = 0;
    int maxarea = 0;
    for (int i = 0; i < Contours.size(); i++)
    {
        double a = contourArea(Contours[i]);
        if (a> maxarea)
        {
            maxarea = a;
            lrgctridx = i;
        }
    }
    //convex hulls
    vector<vector<Point> >hull(Contours.size());
    vector<vector<int> > hullsI(Contours.size());
    vector<vector<Vec4i>> defects(Contours.size());
    for (int i = 0; i < Contours.size(); i++)
    {
        convexHull(Contours[i], hull[i], false);
        convexHull(Contours[i], hullsI[i], false);
        if (hullsI[i].size() > 3)
        {
            convexityDefects(Contours[i], hullsI[i], defects[i]);
        }
    }
    //REQUIRED contour is detected,then convex hell is found and also convexity defects are found and stored in defects
    int count = 0;
    if (maxarea>100) {
        drawContours(img_roi, hull, lrgctridx, Scalar(255, 0, 255), 3, 8, vector<Vec4i>(), 0, Point());

        /// Draw convexityDefects
        for (int j = 0; j<defects[lrgctridx].size(); ++j)
        {
            const Vec4i& v = defects[lrgctridx][j];
            float depth = v[3] / 256;
            if (depth > 10) //  filter defects by depth
            {
                int startidx = v[0]; Point ptStart(Contours[lrgctridx][startidx]);
                int endidx = v[1]; Point ptEnd(Contours[lrgctridx][endidx]);
                int faridx = v[2]; Point ptFar(Contours[lrgctridx][faridx]);
                count++;
                line(img_roi, ptStart, ptEnd, Scalar(0, 255, 0), 1);
                line(img_roi, ptStart, ptFar, Scalar(0, 255, 0), 1);
                line(img_roi, ptEnd, ptFar, Scalar(0, 255, 0), 1);
                circle(img_roi, ptFar, 4, Scalar(0, 255, 0), 2);
            }
        }
    }
    char  a[40];
    if(count-1==0)
        strcpy(a, "0");
    else if (count-1 == 1)
        strcpy(a, "1");
    else if (count-1 == 2)
        strcpy(a, "2 ");
    else if (count-1 == 3)
        strcpy(a, "3");
    else if (count-1 == 4)
        strcpy(a, "4");
    else if (count-1 == 5)
        strcpy(a, "5");
    else
        strcpy(a, "Welcome !!");
    putText(img, a, Point(70, 70), CV_FONT_HERSHEY_SIMPLEX, 3, Scalar(255, 0, 0), 2, 8, false);
    string hello = to_string(count-1);
    return env->NewStringUTF(hello.c_str());
    }
}

