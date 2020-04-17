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

const string saveFileDir = "D:/self_study/exampleForOpenCV/pictureSource/sourcePicture/source_";
const string sourceDir = "D:/self_study/exampleForOpenCV/pictureSource/sourcePicture/source";


int main()
{
	for (int fileNum = 1; fileNum <= 100; fileNum++)
	{
		string sourceFilePath = sourceDir + to_string(fileNum) + ".bmp";
		Mat src_img = imread(sourceFilePath, 1);

		if (src_img.empty()) return -1;

		IMGPROCESS imgProcess;

		// ��ֵ�˲�
		Mat blur_mat;
		blur(src_img, blur_mat, Size(5, 5));

		Mat gray_img;
		cvtColor(src_img, gray_img, COLOR_BGR2GRAY);
		threshold(gray_img, gray_img, 100, 255, THRESH_BINARY_INV);

		Mat element = getStructuringElement(MORPH_RECT, Size(5, 5)); 
		//��һ������MORPH_RECT��ʾ���εľ���ˣ���Ȼ������ѡ����Բ�εġ������͵�
		morphologyEx(gray_img, gray_img, MORPH_OPEN, element);

		threshold(gray_img, gray_img, 0, 255, THRESH_BINARY_INV); //������ת

		// 70 ֻ��ȷ��һ�����о������ֵģ���������������ֻҪ������
		int midNumRow = imgProcess.findRowMiddle(gray_img, 70);

		// �ҵ���һ�еĿհ��У��ڶ��еĿհ���
		const int blankSize = 900; //̫С��Խ��
		int firstBlankCol[blankSize], secondBlankCol[blankSize];
		memset(firstBlankCol, 0, sizeof(firstBlankCol));
		memset(secondBlankCol, 0, sizeof(secondBlankCol));

		imgProcess.findBlankCol(gray_img, midNumRow, firstBlankCol, secondBlankCol);
	
		// �ָ��һ�е�����
		const int edgeSize = 60; //̫С��Խ��
		int firstRowRealEdge[edgeSize], secondRowRealEdge[edgeSize]; // 30 ֻ�ǹ��ƣ���Ϊһ�������������߽�
		//int* firstRowRealEdge = (int*)malloc(sizeof(int) * 30);
		memset(firstRowRealEdge, 0, sizeof(firstRowRealEdge));
		memset(secondRowRealEdge, 0, sizeof(secondRowRealEdge));
		imgProcess.spliteCharacter(firstBlankCol, firstRowRealEdge, blankSize);
		imgProcess.spliteCharacter(secondBlankCol, secondRowRealEdge, blankSize);
		static int fileName = 1;
		ostringstream dirName;
		dirName << fileName;
		string fileDirPath = saveFileDir + dirName.str();
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