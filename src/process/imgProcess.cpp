#include"imgProcess.h"


using namespace cv;
using namespace std;

IMGPROCESS::IMGPROCESS()
{
	resizeWidth = 30; // ���ڷָ���ַ�resize �� width
	resizeHeight = 90; // ���ڷָ���ַ�resize �� height
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

void IMGPROCESS::saveSpliteCharacter(Mat gray_img, int edgeSize, int midNumRow, int* firstColRealEdge, int* secondColRealEdge, string fileDirPath)
{
	Mat firstCharacterMat;
	static int characterNum = 0;
	for (int i = 0; i < edgeSize - 1 && (firstColRealEdge[i] < firstColRealEdge[i + 1]); i += 2)
	{
		if (firstColRealEdge[i + 1] - firstColRealEdge[i] >= 100)
		{
			int rowBegin = 0;
			doubleCharacterSplit(gray_img, rowBegin, midNumRow, firstColRealEdge[i], firstColRealEdge[i + 1], characterNum, fileDirPath);
		}
		else
		{
			firstCharacterMat = gray_img(Range(0, midNumRow), Range(firstColRealEdge[i], firstColRealEdge[i + 1]));
			saveSplitPicture(firstCharacterMat, fileDirPath);
		}
		characterNum++;
	}

	Mat secondCharacterMat;
	for (int i = 0; i < edgeSize - 1 && (secondColRealEdge[i] < secondColRealEdge[i + 1]); i += 2)
	{
		if (secondColRealEdge[i + 1] - secondColRealEdge[i] >= 100)
		{
			int rowBegin = 0;
			doubleCharacterSplit(gray_img, midNumRow, gray_img.rows, secondColRealEdge[i], secondColRealEdge[i + 1], characterNum, fileDirPath);
		}
		else
		{
			secondCharacterMat = gray_img(Range(midNumRow, gray_img.rows), Range(secondColRealEdge[i], secondColRealEdge[i + 1]));
			saveSplitPicture(secondCharacterMat, fileDirPath);
		}
		characterNum++;
	}
}

int IMGPROCESS::doubleCharacterSplit(Mat gray_img, int rowBegin, int rowEnd, int firstColRealEdgeOne, int firstColRealEdgeTwo, int& characterNum, string fileDirPath)
{
	Mat doubleCharacterMat = gray_img(Range(rowBegin, rowEnd), Range(firstColRealEdgeOne, firstColRealEdgeTwo));
	int colNum = firstColRealEdgeTwo - firstColRealEdgeOne + 1;
	//int* perColPixel = new int[firstColRealEdgeTwo +1]; // ��Ϊ��Ҫ�õ��±꣬�±��Ӧ�У����±�ȡ��firstRowRealEdgeTwo
	int* perColPixel = new int[gray_img.cols + 1];
	//int perColPixel[847 + 1];
	memset(perColPixel, 0, sizeof(int)*(gray_img.cols + 1));
	for (int col = firstColRealEdgeOne; col <= firstColRealEdgeTwo; col++)
	{
		long totalPerColPixel = 0;
		for (int row = rowBegin; row < rowEnd; row++)
		{
			if (gray_img.at<unsigned char>(row, col) == 0) 
				totalPerColPixel += 1;
		}
		perColPixel[col] = totalPerColPixel;
	}
	int minColPixel = perColPixel[firstColRealEdgeOne + 30]; //��Ϊ��ͼƬ����ĵط����������٣����������ǰ�󶼴�+10�ͼ�10�п�ʼ
	int midColNum = firstColRealEdgeOne + 30; //��ʼ��
	for (int i = firstColRealEdgeOne + 31; i < firstColRealEdgeTwo - 30; i++)
	{
		if (perColPixel[i] < minColPixel)
		{
			minColPixel = perColPixel[i];
			midColNum = i;
		}
	}

	Mat firstCharacterMat = gray_img(Range(rowBegin, rowEnd), Range(firstColRealEdgeOne, midColNum));
	saveSplitPicture(firstCharacterMat, fileDirPath);

	Mat secondCharacterMat = gray_img(Range(rowBegin, rowEnd), Range(midColNum, firstColRealEdgeTwo));
	Mat element = getStructuringElement(MORPH_RECT, Size(5, 5)); // ��Ϊ�ָ�����һЩ���������ص㣬��Ҫȥ����
	
	Mat tempSecondMat;
	dilate(secondCharacterMat, tempSecondMat, element);
	
	erode(tempSecondMat, tempSecondMat, element);

	//morphologyEx(secondCharacterMat, tempSecondMat, MORPH_CLOSE, element);
	saveSplitPicture(firstCharacterMat, fileDirPath);

	return midColNum;
}

void IMGPROCESS::saveSplitPicture(const Mat& img, const string& savePath)
{
	static int picNUm = 0;
	Mat resize_mat;
	resize(img, resize_mat, Size(resizeWidth, resizeHeight));
	std::ostringstream numStr;
	numStr << picNUm;
	std::string fileAbsPath = savePath + "/" + numStr.str() + ".bmp";
	imwrite(fileAbsPath.c_str(), resize_mat);
	picNUm++;
}