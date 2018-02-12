#pragma once
#include "opencv2/highgui/highgui.hpp"
using namespace cv;

// ̬ͬ�˲�
bool myHomoFilter(cv::Mat inImage, cv::Mat& outImage);
//�Ե�ͨ��ͼ��̬ͬ�˲�
bool myHomoFilterForSingleChannel(Mat inImage, Mat& outImage);
//����butterWorth�˲���
Mat getButterWorth(int width, int height, float a = 80, float n = 4, float d = 1.5, float e = 0.5);
