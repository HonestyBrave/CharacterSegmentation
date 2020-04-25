#include "imgProcess.h"
#include <opencv2/ml.hpp>
#include <io.h>

using namespace ml;

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
	//ʹ�����ع�ϵ�ز�������ͼ����Сʱ�򣬸÷������Ա��Ⲩ�Ƴ��� 
	// ����ע�������� https://www.cnblogs.com/chenzhefan/p/7629441.html
	resize(img, resize_mat, Size(resizeWidth, resizeHeight), INTER_AREA);
	std::ostringstream numStr;
	numStr << picNUm;
	std::string fileAbsPath = savePath + "/" + numStr.str() + ".bmp";
	imwrite(fileAbsPath.c_str(), resize_mat);
	picNUm++;
}

void IMGPROCESS::train()
{
	const string file_form = "*.bmp";
	const string file_dir = "D:/self_study/exampleForOpenCV/pictureSource/train_data/";
	const int classNum = 12; // ѵ��������
	const int perClassNum = 50; // ÿ������������
	const int img_width = 30;
	const int img_height = 90;
	static float training_data[classNum*perClassNum][img_width*img_height] = { {0} };
	static float labels[classNum*perClassNum][classNum] = { {0} };
	char temp[256];
	string file_name, file_path;

	for (int i = 0; i < classNum; i++)
	{
		int j = 0;
		if (i == 3 || i == 6 || i == 9) continue;
		if (i <= 9)
		{
			sprintf_s(temp, "%d", i);
		}
		else
		{
			string strTemp = "C";
			if (i == 10) sprintf_s(temp, "%c", 67); //C
			else if (i == 11) sprintf_s(temp, "%c", 76); //L
			else if (i == 12) sprintf_s(temp, "%c", 77); //M
			else if (i == 13) sprintf_s(temp, "%c", 75); //K
		}
		file_path = file_dir + temp + "/" + to_string(j) + ".bmp";
		cout << "�ļ���" << endl;

		do
		{
			j++;
			Mat src_img = imread(file_path, 0);
			if (!src_img.data)
			{
				cout << file_path << " empty" << endl;
			}
			cout << "�ļ���1" << file_dir + temp + "/" + to_string(j) + ".bmp" << endl;
			Mat result;
			for (int k = 0; k < img_width*img_height; k++)
			{
				training_data[i*perClassNum + (j - 1)][k] = (float)src_img.data[k];
			}
		} while (j < perClassNum);
	}

	Mat trainingDataMat(classNum*perClassNum, img_width*img_height, CV_32FC1, training_data);
	Mat labelsMat(classNum*perClassNum, classNum, CV_32FC1, labels);

	for (int i = 0; i < classNum; i++)
	{
		for (int j = 0; j < perClassNum; j++)
		{
			labels[i*perClassNum + j][i] = 1;
		}
	}

	Ptr<ml::ANN_MLP> bp = ml::ANN_MLP::create();
	Mat layerSizes = (Mat_<int>(1, 5) << resizeWidth * resizeHeight, 128, 128, 128, classNum);
	bp->setLayerSizes(layerSizes);
	bp->setTrainMethod(ml::ANN_MLP::BACKPROP, 0.001, 0.1);
	bp->setActivationFunction(ml::ANN_MLP::SIGMOID_SYM, 1.0, 1.0);
	bp->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER | TermCriteria::EPS, 10000, 0.0001));
	Ptr<TrainData> trainData = TrainData::create(trainingDataMat, ml::ROW_SAMPLE, labelsMat);
	bool trained = bp->train(trainData);
	bp->save("D:/self_study/exampleForOpenCV/MPLModel.xml");
	cout << "training finish...bpModel.xml saved!" << endl;

}

void IMGPROCESS::predict()
{
	Ptr<ml::ANN_MLP> bp = ml::ANN_MLP::load("D:/self_study/exampleForOpenCV/MPLModel.xml");

	const int img_width = 30;
	const int img_height = 90;

	Mat test, dst;
	test = imread("D:/self_study/exampleForOpenCV/pictureSource/sourcePicture/9.bmp", 0);
	if (test.empty())
	{
		cout << "test empty()" << endl;
	}

	Mat_<float> testMat(1, img_width*img_height);
	for (int i = 0; i < img_width*img_height; i++)
	{
		testMat.at<float>(0, i) = (float)test.at<uchar>(i / 30, i % 30);
	}

	bp->predict(testMat, dst);
	cout<< "testMat: \n" << testMat << "\n" << endl;
	cout << "dst: \n" << dst << "\n" << endl;

	double maxVal = 0;
	Point maxLoc;
	minMaxLoc(dst, NULL, &maxVal, NULL, &maxLoc);
	char temp[256];
	if (maxLoc.x <= 9)
	{
		sprintf_s(temp, "%d", maxLoc.x);
	}
	else
	{
		if (maxLoc.x == 10) sprintf_s(temp, "%c", 67);
		else if (maxLoc.x == 11) sprintf_s(temp, "%c", 76);
	}

	cout << "���Խ����" << temp << " ���Ŷȣ�" << maxVal * 100 << "%" << endl;

	
}