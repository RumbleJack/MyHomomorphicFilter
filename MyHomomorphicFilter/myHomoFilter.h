#pragma once
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
using namespace cv;
using namespace std;

#define MAXTIME 300


bool myHomoFilter(Mat inImage, Mat& outImage);