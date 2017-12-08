#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>
#include <fstream>
#include <map>
#include <list>
#include <ctype.h>
#include <thread>        
#include <mutex>          

using namespace cv;
using namespace std;

#define MAXTIME 300

Mat frame;
mutex mtxFrame;
mutex mtxProbe;
map<int, Rect> probe;
map<int, list<Vec4f> >probeValue;
Point lastPoint;
int indFrame = 0;
double tpsFrame = 0;
char mode = 's';
vector<Scalar> color = { Scalar(255, 0, 0), Scalar(0, 255, 0), Scalar(0, 0, 255), Scalar(255, 255, 0), Scalar(255, 255, 255) };


Mat getButterWorth( int rows,int cols, float a, float n, float d, float e)
{
	//Mat filter = Mat::zeros(LuminaceChannel.rows, LuminaceChannel.cols, CV_32FC1);
	Mat butterWorth = Mat(cv::Size( rows,cols), CV_32FC1, cv::Scalar(0));

	//生成butterworth滤波器的频域表示
	//Mat butterworthFilterImage = dftImage.clone();
	for (int i = 0; i <= rows / 2; i++)
	{
		for (int j = 0; j <= cols / 2; j++)
		{
			double q = i*i + j*j;
			double fLow = 1.0 / (1 + pow(q / a, n));
			double fHigh = 1 - fLow;
			double fBoost = d*fHigh + e;

			//直流分量为1
			if (i == 0 && j == 0)
				fBoost = 1;
			butterWorth.at<float>(i, j) = fBoost;
			//第三、一、四象限
			butterWorth.at<float>(rows - 1 - i, j) = butterWorth.at<float>(i, j);
			butterWorth.at<float>(i, cols - 1 - j) = butterWorth.at<float>(i, j);
			butterWorth.at<float>(rows - 1 - i, cols - 1 - j) = butterWorth.at<float>(i, j);
		}
	}
	return butterWorth;
}

int main(int ac, char** av)
{
	Mat m = imread("../data/test.jpg", IMREAD_COLOR);

	double minVal, maxVal;
	float gainMax = 2, gainMin = 1;
	int begSlope = 2 * m.cols / 8;
	int endSlope = 3 * m.cols / 8;
	imshow("original", m);
	waitKey(0);
	Mat mYuv, LuminaceChannel;

	//判断颜色通道数
	if (m.channels() >= 3)
		cvtColor(m, mYuv, COLOR_BGR2YCrCb);
	else
		m.copyTo(mYuv);

	//分开颜色通道
	vector<Mat> plan;
	split(mYuv, plan);

	//取最大最小值
	for (int i = 0; i < 3; i++)
	{
		minMaxLoc(plan[i], &minVal, &maxVal);
		cout << minVal << "\t" << maxVal << endl;
	}

	//转化位深防止0
	plan[0].convertTo(LuminaceChannel, CV_32FC1);
	log(LuminaceChannel+0.00001, LuminaceChannel);

	Mat dftImage;
	dft(LuminaceChannel, dftImage, cv::DFT_COMPLEX_OUTPUT);
	
	double a = 80; //Butterworth equations for homomorphic fltering of images Computers in Biology and Medicine 28 (1998) 169±181
	double d = 1.5, e = 0.5;
	double n = 4;
	char key = 'r';
	while (key != 27)
	{
		bool newFilter = true;
		key = waitKey();
		switch (key){
		case 'd':
			d -= 0.1;
			if (d == 0)
				d = 0.1;
			break;
		case 'D':
			d += 0.1;
			if (d == 0)
				d = 0.1;
			break;
		case 'e':
			e -= 0.1;
			if (e == 0)
				e = -0.1;
			break;
		case 'E':
			e += 0.1;
			if (e == 0)
				e = 0.1;
			break;
		case 'a':
			a -= 0.02;
			if (a == 0)
				a = -0.02;
			break;
		case 'A':
			a += 0.02;
			if (a == 0)
				a = +0.02;
			break;
		case 'n':
			n -= 0.1;
			if (n == 0)
				n = -0.1;
			break;
		case 'N':
			n += 0.1;
			if (n == 0)
				n = 0.1;
			break;
		default:
			newFilter = false;
			break;
		}
		if (newFilter)
		{
			cout << "d = " << d << "\e = " << e << "\t a=" << a << "\t n=" << n << endl;

			//生成butterworth滤波器的频域表示
			int middleWidth = LuminaceChannel.cols / 2, middleHeight = LuminaceChannel.rows / 2 ;
			Mat filter = Mat::zeros(LuminaceChannel.rows, LuminaceChannel.cols, CV_32FC1);
			Mat butterworthFilterImage =  dftImage.clone();
			for (int i = 0; i <= middleHeight; i++)
			{
				for (int j = 0; j <= middleWidth; j++)
				{
					double r =  i*i 
						+ j*j;

					double fLow = 1.0  / (1 + pow( r / a, n));
					double fHigh = 1 - fLow;
					double fBoost = d*fHigh + e;
					if (i == 0 && j == 0)
						fBoost = 1;

					filter.at<float>(i, j) = fBoost;
					//第三象限
					filter.at<float>(LuminaceChannel.rows -1 - i, j) = filter.at<float>(i, j);
					//第一象限
					filter.at<float>(i, LuminaceChannel.cols-1 - j) = filter.at<float>(i, j);
					//第四象限
					filter.at<float>(LuminaceChannel.rows -1 - i, LuminaceChannel.cols -1 - j) = filter.at<float>(i, j);		
				}
			}
			filter = getButterWorth(LuminaceChannel.rows, LuminaceChannel.cols, 80, 4, 1.5, 0.5);
			//频域相乘
			for (int i = 0; i <LuminaceChannel.rows; i++)
				for (int j = 0; j < LuminaceChannel.cols; j++)
				{
					Vec2f v = dftImage.at<cv::Vec2f>(i, j);
					complex<float> z(v[0], v[1]);
					z = z * filter.at<float>(i, j);
					v[0] = z.real(); v[1] = z.imag();
					butterworthFilterImage.at<Vec2f>(i, j) = v;
				}
			Mat mfHomomorphic;
			idft(butterworthFilterImage, mfHomomorphic, cv::DFT_SCALE | DFT_REAL_OUTPUT);
			exp(mfHomomorphic, mfHomomorphic);

			Mat result;

			mfHomomorphic.convertTo(mfHomomorphic, CV_8UC1);
			
			plan[0] = mfHomomorphic;
			if (m.channels() >= 3)
				merge(plan, result);
			else
				result = mfHomomorphic;

			cvtColor(result, mfHomomorphic, COLOR_YCrCb2BGR);
			imshow("result", mfHomomorphic);
			waitKey(0);
			imwrite("result.jpg", mfHomomorphic);
		}
	}
	return 0;
}
