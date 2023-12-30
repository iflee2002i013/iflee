#include <opencv2/core.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>
#include <fstream>


	using namespace cv;
	using namespace std;
	
//定义将数据写入文本文档中的函数；
void Writedata(Mat src)
{
	ofstream datafile;
	datafile.open("datafile.txt",ofstream::app);
	datafile << src;
	
	datafile.close();

}


int main(int argc,char* argv[])
{

	Mat src= imread("test2.jpg");//Mat是一个矩阵，所有图像都会是这样一个矩阵
	if(!src.data)
	{
		cout << "fail to load imagine"<<endl;
		return -1;
	}
	
	namedWindow("原图像",WINDOW_FREERATIO);
	imshow("原图像",src);//imshow 第一个参数表示弹出窗口的名称，src表示调用哪个图像矩阵
	
	int RowNums = src.rows;
	int ColNums = src.cols;
	unsigned int averageColor = 0;
	unsigned int addColor = 0;
	unsigned int Imagedata = 0;
	int i,j;
	long int c;
	long int index = 0;
	cout << ColNums << endl;
	cout << RowNums<< endl;
	cout << endl;
	
	for ( i = 0; i < RowNums; ++i)
	{
		for(j = 0; j < ColNums; ++j)
		{
			
			index++;	//像素索引，每替换一个像素后+1
			//cout << "=============第"<<i<<"*"<<j<<"=============";
			//cout << endl;

			int16_t RedData = src.data[index*3+0] << 2;
			int16_t GreenData = src.data[index*3+1] << 3;
			int16_t BlueData = src.data[index*3+2] << 1;
			//将每个像素的RGB值相加,存到addColor
			addColor = RedData + GreenData + BlueData;
			addColor >>= 4;
			
			src.data[index*3+0] =
			src.data[index*3+1] =
			src.data[index*3+2] = addColor;
			
		}
	}
	
	namedWindow("转灰度后图像",WINDOW_FREERATIO);
	imshow("转灰度后图像",src);//imshow 第一个参数表示弹出窗口的名称，src表示调用哪个图像矩阵
	
	//Writedata(src);//调用将数据写入文本文档中的函数
	
	
	waitKey(0);
	while(1);
	return 0;
}


