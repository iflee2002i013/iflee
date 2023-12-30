#include <opencv2/core.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>

	using namespace cv;
	using namespace std;

int main(int argc,char* argv[])
{

	Mat src= imread("Default.jpg");//Mat是一个矩阵，所有图像都会是这样一个矩阵
	if(!src.data)
	{
		cout << "fail to load imagine"<<endl;
		return -1;
	}
	
	namedWindow("test opencv setup");
	imshow("test opencv setup",src);//imshow 第一个参数表示弹出窗口的名称，src表示调用哪个图像矩阵
	cv::waitKey(0);//waitKey=0表示执行到这一句话就会阻塞程序运行，若为1则表示停顿1ms
	
	return 0;
}
