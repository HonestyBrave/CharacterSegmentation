#include"imgProcess.h"


using namespace cv;
using namespace std;

IMGPROCESS::IMGPROCESS()
{
	filePathdir = "D:/self_study/exampleForOpenCV/pictureSource/characterPicture/";
}

IMGPROCESS::~IMGPROCESS()
{
}

// �ҵ���һ�к͵ڶ���֮��Ŀհ��У����ڷָ�����
int IMGPROCESS::findRowMiddle(Mat img, int rowBegin)
{
	// edgeRowNum �洢��һ���հ��У��Ϳհ��к��һ���ǿհ���
	int edgeRowNum[2] = { 0,0 };
	int elementNum;
	bool firstWhite = false, firstBlack = false;
	int midNumRow = 0;
	int maxDiff = 0;
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
		}

		if (firstWhite && firstBlack)
		{
			firstWhite = false;
			firstBlack = false;
			int diff = edgeRowNum[1] - edgeRowNum[0];
			if (diff > maxDiff)
			{
				midNumRow = (edgeRowNum[0] + edgeRowNum[1]) / 2;
				maxDiff = diff;
			}
		}
	}
	midRow = midNumRow;
	return midNumRow;
}

// Ѱ�ҵ�һ�еڶ��е�ÿ���հ���
void IMGPROCESS::findBlankCol(Mat img, int MidRowNum, int* firstColArray, int* secondColArray)
{
	int colNum = 0;
	for (int i = 0; i < img.cols; i++)
	{
		int flagBlankCol = 0;
		for (int j = 0; j < MidRowNum; j++)
		{
			int elementNum = img.at<unsigned char>(j, i);
			if (elementNum == 0) flagBlankCol++;
		}
		if (flagBlankCol < 6) // ��С�ĵ��������һ�� 6 �� 
		{
			firstColArray[colNum] = i;
			colNum++;
		}
	
	}

	colNum = 0;

	for (int i = 0; i < img.cols; i++)
	{
		int flagBlankCol = 0;
		for (int j = MidRowNum; j < img.rows; j++)
		{
			int elementNum = img.at<unsigned char>(j, i);
			if (elementNum == 0) flagBlankCol++;
		}
		if (flagBlankCol <= 10) secondColArray[colNum] = i, colNum++;
	}
}

// ��ȡ��һ�еڶ��е�ÿһ���ַ�������Ϊ".bmp"�ļ�
void IMGPROCESS::spliteCharacter(int* firstBlankCol, int* firstRowRealEdge, int blankSize)
{
	int count = 0;
	for (int i = 0; i < blankSize - 1; i++)
	{
		if (firstBlankCol[i] + 1 < firstBlankCol[i + 1])
		{
			firstRowRealEdge[count] = firstBlankCol[i];
			std::cout << firstRowRealEdge[count] << " ";
			++count;
			firstRowRealEdge[count] = firstBlankCol[i + 1];
			std::cout << firstRowRealEdge[count] << " ";
			++count;
		}
	}
}


void IMGPROCESS::saveSpliteCharacter(Mat gray_img, int edgeSize, int midNumRow, int* firstRowRealEdge, int* secondRowRealEdge, string fileDirPath)
{
	Mat firstCharacterMat;
	static int characterNum = 0;
	for (int i = 0; i < edgeSize - 1 && (firstRowRealEdge[i] < firstRowRealEdge[i + 1]); i += 2)
	{
		firstCharacterMat = gray_img(Range(0, midNumRow), Range(firstRowRealEdge[i], firstRowRealEdge[i + 1]));
		//resize(firstCharacterMat, firstCharacterMat, Size(53, 138));
		std::ostringstream t;
		t << characterNum;
		std::string filePath = fileDirPath + "/"+ t.str() + ".bmp";
		imwrite(filePath.c_str(), firstCharacterMat);
		characterNum++;
	}

	Mat secondCharacterMat;
	for (int i = 0; i < edgeSize - 1 && (secondRowRealEdge[i] < secondRowRealEdge[i + 1]); i += 2)
	{
		secondCharacterMat = gray_img(Range(midNumRow, gray_img.rows), Range(secondRowRealEdge[i], secondRowRealEdge[i + 1]));
		//resize(secondCharacterMat, secondCharacterMat, Size(53, 138));
		std::ostringstream t;
		t << characterNum;
		std::string filePath = fileDirPath + "/" + t.str() + ".bmp";
		imwrite(filePath.c_str(), secondCharacterMat);
		characterNum++;
	}
}

