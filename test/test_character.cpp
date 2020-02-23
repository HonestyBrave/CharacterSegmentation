#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include "imgProcess.h"

using namespace cv;
using namespace std;


int main()
{
	Mat src_img = imread("D:/self_study/exampleForOpenCV/pictureSource/source1.bmp", 1);
	if (src_img.empty()) return -1;

	IMGPROCESS imgProcess;

	Mat gray_img;
	cvtColor(src_img, gray_img, COLOR_BGR2GRAY);
	threshold(gray_img, gray_img, 80, 255, THRESH_BINARY_INV);

	Mat element = getStructuringElement(MORPH_RECT, Size(5, 5)); 
	//第一个参数MORPH_RECT表示矩形的卷积核，当然还可以选择椭圆形的、交叉型的
	morphologyEx(gray_img, gray_img, MORPH_OPEN, element);

	threshold(gray_img, gray_img, 80, 255, THRESH_BINARY_INV);

	// 70 只是确定一个首行就是有字的，可以设置其它，只要包含字
	int midNumRow = imgProcess.findRowMiddle(gray_img, 70);

	// 找到第一行的空白列，第二行的空白列
	int firstBlankCol[500], secondBlankCol[500];
	memset(firstBlankCol, 0, sizeof(firstBlankCol));
	memset(secondBlankCol, 0, sizeof(secondBlankCol));

	imgProcess.findBlankCol(gray_img, midNumRow, firstBlankCol, secondBlankCol);
	
	// 分割第一行的数字
	int firstRowRealEdge[30], secondRowRealEdge[30]; // 30 只是估计，因为一个数字有两个边界
	memset(firstRowRealEdge, 0, sizeof(firstRowRealEdge));
	memset(secondRowRealEdge, 0, sizeof(secondRowRealEdge));
	int count = 0;
	for (int i = 0; i < sizeof(firstBlankCol)/sizeof(int) -1; i++)
	{
		if (firstBlankCol[i] + 1 != firstBlankCol[i + 1])
		{
			firstRowRealEdge[count] = firstBlankCol[i];
			++count;
			firstRowRealEdge[count] = firstBlankCol[i+1];
			++count;
		}
	}

	return 0;
}