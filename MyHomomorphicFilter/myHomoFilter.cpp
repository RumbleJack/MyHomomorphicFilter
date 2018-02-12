#include"myHomoFilter.h"
#include<iostream>
using namespace std;

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
using namespace cv;


//�Բ�ɫ��Ҷ�ͼ�����̬ͬ�˲�
bool myHomoFilter(Mat inImage, Mat& outImage)
{
	if (inImage.channels() >= 3)	//�����ɫRGBͼ��
	{
		Mat YCrCbImage;
		vector<Mat> channels;
		cvtColor(inImage, YCrCbImage, COLOR_BGR2YCrCb);
		split(YCrCbImage, channels);
		myHomoFilterForSingleChannel(channels[0], channels[0]);
		merge(channels, YCrCbImage);
		cvtColor(YCrCbImage, outImage, COLOR_YCrCb2BGR);
	}
	else							//����ͨ��ͼ��
	{
		myHomoFilterForSingleChannel(inImage, outImage);
	}
	return true;

}

//�Ե�ͨ��ͼ��̬ͬ�˲�
bool myHomoFilterForSingleChannel(Mat inImage, Mat& outImage)
{
	Mat tmpImg = inImage.clone();

	// ȡ�����Խ����շ����ͷ�������ɳ˻��������
	inImage.convertTo(tmpImg, CV_32FC1);
	log(tmpImg + 0.00001, tmpImg);		//��ֹ����Ϊ0���ʼ�һ����С�ĸ�����

	// DFT��ͼ��ת����Ƶ����
	Mat complexI;
	Mat planes[] = { Mat_<float>(tmpImg), Mat::zeros(tmpImg.size(), CV_32F) };
	merge(planes, 2, complexI);
	dft(complexI, complexI);	//������Ӧ���Ǹ�����˫ͨ����
	split(complexI, planes); // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))

	// ����butterWorth�˲���
	Mat butterWorth = getButterWorth(inImage.rows,inImage.cols,80,0.4,1.5,0.5);
	
	// Ƶ�����
	imshow("butterWorth", butterWorth/2);
	multiply(planes[0], butterWorth, planes[0]);
	multiply(planes[1], butterWorth, planes[1]);
	merge(planes, 2, complexI);

	// IDFT���任��ʱ��
	idft(complexI, tmpImg, cv::DFT_SCALE | DFT_REAL_OUTPUT);
	
	//ȡָ��
	exp(tmpImg, tmpImg);
	tmpImg.convertTo(outImage, inImage.type());

	return true;
}

Mat getButterWorth(int rows, int cols, float a, float n, float d, float e)
{
	//Mat filter = Mat::zeros(LuminaceChannel.rows, LuminaceChannel.cols, CV_32FC1);
	Mat butterWorth = Mat(cv::Size(cols, rows), CV_32FC1, cv::Scalar(0));

	//����butterworth�˲�����Ƶ���ʾ
	//Mat butterworthFilterImage = dftImage.clone();
	for (int i = 0; i <= rows / 2; i++)
	{
		for (int j = 0; j <= cols / 2; j++)
		{
			double q = i*i + j*j;
			double fLow = 1.0 / (1 + pow(q / a, n));
			double fHigh = 1 - fLow;
			double fBoost = d*fHigh + e;

			//ֱ������Ϊ1
			if (i == 0 && j == 0)
				fBoost = 1;
			butterWorth.at<float>(i, j) = fBoost;
			//������һ��������
			butterWorth.at<float>(rows - 1 - i, j) = butterWorth.at<float>(i, j);
			butterWorth.at<float>(i, cols - 1 - j) = butterWorth.at<float>(i, j);
			butterWorth.at<float>(rows - 1 - i, cols - 1 - j) = butterWorth.at<float>(i, j);
		}
	}
	return butterWorth;
}
