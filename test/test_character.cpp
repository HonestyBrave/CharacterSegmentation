#include <iostream>
#include <io.h>
#include <direct.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include "imgProcess.h"

using namespace cv;
using namespace std;


int main()
{
	string fileDir = "D:/self_study/exampleForOpenCV/pictureSource/sourcePicture/source_";
	for (int fileNum = 1; fileNum <= 100; fileNum++)
	{
		string filePath = "D:/self_study/exampleForOpenCV/pictureSource/sourcePicture/source" + to_string(fileNum) + ".bmp";
		Mat src_img = imread(filePath, 1);

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
		const int blankSize = 900; //太小会越界
		int firstBlankCol[blankSize], secondBlankCol[blankSize];
		memset(firstBlankCol, 0, sizeof(firstBlankCol));
		memset(secondBlankCol, 0, sizeof(secondBlankCol));

		imgProcess.findBlankCol(gray_img, midNumRow, firstBlankCol, secondBlankCol);
	
		// 分割第一行的数字
		const int edgeSize = 60; //太小会越界
		int firstRowRealEdge[edgeSize], secondRowRealEdge[edgeSize]; // 30 只是估计，因为一个数字有两个边界
		//int* firstRowRealEdge = (int*)malloc(sizeof(int) * 30);
		memset(firstRowRealEdge, 0, sizeof(firstRowRealEdge));
		memset(secondRowRealEdge, 0, sizeof(secondRowRealEdge));
		imgProcess.spliteCharacter(firstBlankCol, firstRowRealEdge, blankSize);
		imgProcess.spliteCharacter(secondBlankCol, secondRowRealEdge, blankSize);
		static int fileName = 1;
		ostringstream dirName;
		dirName << fileName;
		string fileDirPath = fileDir + dirName.str();
		dirName.str("");
		if (_access(fileDirPath.c_str(), 0) == -1)
		{
			_mkdir(fileDirPath.c_str());
		}
		imgProcess.saveSpliteCharacter(gray_img, edgeSize, midNumRow, firstRowRealEdge, secondRowRealEdge, fileDirPath);
		fileName++;
	}
	return 0;
}