#include <iostream>
#include "myHomoFilter.h"
using namespace std;

int main()
{
	//读入图像
	Mat inImage = imread("lena.jpg",IMREAD_COLOR);

	//同态滤波
	Mat outImage;
	myHomoFilter(inImage, outImage);
	
	//显示对比结果
	imshow("SrcImg", inImage);
	imshow("DstImg", outImage);
	waitKey(0);

	//输出图像
	imwrite("homeFilterResult.jpg", outImage);

	return 0;
}