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
	//��һ������MORPH_RECT��ʾ���εľ���ˣ���Ȼ������ѡ����Բ�εġ������͵�
	morphologyEx(gray_img, gray_img, MORPH_OPEN, element);

	threshold(gray_img, gray_img, 80, 255, THRESH_BINARY_INV);

	// 70 ֻ��ȷ��һ�����о������ֵģ���������������ֻҪ������
	int midNumRow = imgProcess.findRowMiddle(gray_img, 70);

	// �ҵ���һ�еĿհ��У��ڶ��еĿհ���
	const int blankSize = 500;
	int firstBlankCol[blankSize], secondBlankCol[blankSize];
	memset(firstBlankCol, 0, sizeof(firstBlankCol));
	memset(secondBlankCol, 0, sizeof(secondBlankCol));

	imgProcess.findBlankCol(gray_img, midNumRow, firstBlankCol, secondBlankCol);
	
	// �ָ��һ�е�����
	const int edgeSize = 30;
	int firstRowRealEdge[edgeSize], secondRowRealEdge[edgeSize]; // 30 ֻ�ǹ��ƣ���Ϊһ�������������߽�
	//int* firstRowRealEdge = (int*)malloc(sizeof(int) * 30);
	memset(firstRowRealEdge, 0, sizeof(firstRowRealEdge));
	memset(secondRowRealEdge, 0, sizeof(secondRowRealEdge));
	imgProcess.spliteCharacter(firstBlankCol, firstRowRealEdge, blankSize);
	imgProcess.spliteCharacter(secondBlankCol, secondRowRealEdge, blankSize);

	imgProcess.saveSpliteCharacter(gray_img, edgeSize, midNumRow, firstRowRealEdge, secondRowRealEdge);
	
	return 0;
}