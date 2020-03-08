#ifndef IMG_PROCESS_H_
#define IMG_PROCESS_H_
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>

using namespace cv;
using namespace std;

class _declspec(dllexport) IMGPROCESS
{
public:
	IMGPROCESS();
	~IMGPROCESS();
	int findRowMiddle(Mat img, int rowBegin);
	void findBlankCol(Mat img, int MidRowNum, int* firstColArray, int* secondColArray);
	void spliteCharacter(int* characterBuff, int* firstRowRealEdge, int blankSize);
	void saveSpliteCharacter(Mat gray_img, int edgeSize, int midNumRow, int* firstRowRealEdge, int* secondRowRealEdge, string fileDirPath);

private:
	int midRow;
	string filePathdir;
};	


#endif // !IMG_PROCESS_H_
