#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>

using namespace cv;
using namespace std;

int findRowMiddle(Mat img, int rowBegin)
{
	// edgeRowNum �洢��һ���հ��У��Ϳհ��к��һ���ǿհ���
	int edgeRowNum[2] = { 0,0 };
	int elementNum;
	bool firstWhite = false, firstBlack = false;
	int midNumRow = 0;

	for (int i = 70; i < img.rows; i++)
	{
		long totalSize = 0;
		for (int j = 0; j < img.cols; j++)
		{
			totalSize += int(img.at<unsigned char>(i, j));
		}


		if (totalSize == (img.step.buf[0] * 255) && !firstWhite)
		{
			edgeRowNum[0] = i;
			firstWhite = true;
		}
		if (totalSize != (img.step.buf[0] * 255) && firstWhite)
		{
			edgeRowNum[1] = i;
			firstBlack = true;
			break;
		}
	}

	if (firstWhite && firstBlack)
	{
		midNumRow = (edgeRowNum[0] + edgeRowNum[1]) / 2;
	}

	return midNumRow;
}

int main()
{
	Mat src_img = imread("D:/self_study/exampleForOpenCV/pictureSource/source1.bmp", 1);
	if (src_img.empty()) return -1;

	Mat gray_img;
	cvtColor(src_img, gray_img, COLOR_BGR2GRAY);
	threshold(gray_img, gray_img, 80, 255, THRESH_BINARY_INV);

	Mat element = getStructuringElement(MORPH_RECT, Size(5, 5)); 
	//��һ������MORPH_RECT��ʾ���εľ���ˣ���Ȼ������ѡ����Բ�εġ������͵�
	morphologyEx(gray_img, gray_img, MORPH_OPEN, element);

	threshold(gray_img, gray_img, 80, 255, THRESH_BINARY_INV);

	// 70 ֻ��ȷ��һ�����о������ֵģ���������������ֻҪ������
	int midNumRow = findRowMiddle(gray_img, 70);

	return 0;
}