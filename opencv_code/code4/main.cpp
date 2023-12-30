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

	Mat src= imread("xiaoyuan.png");//Mat是一个矩阵，所有图像都会是这样一个矩阵
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
			for (c = index*3+0; c < index*3+3; c++)
			{
				//将每个像素的RGB值相加,存到addColor
				Imagedata = src.data[c];
				//cout << "Imagedata: " << Imagedata << endl;
				addColor = addColor + Imagedata;
				//cout << "addColor: " << addColor << endl;
			}
			//求出RGB的平均值
			averageColor = addColor / 3;
			//cout << averageColor << endl;
			//cout << endl;
			
			for (c = index*3+0; c < index*3+3;c++)
			{
				//将平均值再赋值回RGB中
				//cout << "Now display new Imagedata"<<endl;
				src.data[c] = averageColor;
				//cout << "Imagedata: " << static_cast <int>(src.data[c]) << endl;
				addColor = 0;
				Imagedata = 0;
			}
		}
	}
	
	namedWindow("转灰度后图像",WINDOW_FREERATIO);
	imshow("转灰度后图像",src);//imshow 第一个参数表示弹出窗口的名称，src表示调用哪个图像矩阵
	
	//Writedata(src);//调用将数据写入文本文档中的函数
	
	
	waitKey(0);
	while(1);
	return 0;
}



