#include <opencv2/core.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>

	using namespace cv;
	using namespace std;

int main(int argc,char* argv[])
{

	Mat src= imread("xiaoyuan.png");//Mat是一个矩阵，所有图像都会是这样一个矩阵
	if(!src.data)
	{
		cout << "fail to load imagine"<<endl;
		return -1;
	}
	namedWindow("原图像",WINDOW_FREERATIO);
	imshow("原图像",src);//imshow 第一个参数表示弹出窗口的名称，src表示调用哪个图像矩阵
	
	//使用线性灰度算法，g(x,y)=k*f(x,y)+d;
	double k = 0.1 ;
	double b =  50 ;
	cout << "请输入k和b的值: ";
	cin >>k>>b;
	
	int RowsNum = src.rows;
	int ColsNum = src.cols;
	Mat dstImg(src.size(), src.type());

	//遍历图像像素，对每个像素进行相应的线性变换
	for (int i = 0; i< RowsNum; i++)
	{
		for(int j = 0; j <ColsNum; j++)
		{
			for(int c = 0; c<3; c++)
			{
				//使用At操作符，防止越界
				dstImg.at<Vec3b>(i,j)[c] = saturate_cast<uchar>
				(k*(src.at<Vec3b>(i,j)[c]) + b);
			}
		}
	}
	namedWindow("线性灰度变换后的图像",WINDOW_FREERATIO);
	imshow("线性灰度变换后的图像",dstImg);
	cv::waitKey(0);//waitKey=0表示执行到这一句话就会阻塞程序运行，若为1则表示停顿1ms
	
	return 0;
}

