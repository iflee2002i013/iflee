#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>

	using namespace cv;
	using namespace std;
	
int main()
{
	Mat src = imread("xiaoyuan.png",IMREAD_GRAYSCALE);
	namedWindow("opencv灰度转换",WINDOW_FREERATIO);
	imshow("opencv灰度转换",src);
	waitKey(0);
	destroyAllWindows();
	return 0;
}
