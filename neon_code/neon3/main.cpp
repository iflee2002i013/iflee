#include <iostream>
#include <arm_neon.h>
#include <chrono>
#include <opencv2/core.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
using namespace std;
using namespace cv;
auto start = chrono::steady_clock::now();
auto end  = chrono::steady_clock::now();
	

int test(int argc,char* argv[])
{

	Mat src= imread(argv[1]);//Mat是一个矩阵，所有图像都会是这样一个矩阵
	if(!src.data)
	{
		cout << "fail to load imagine"<<endl;
		return -1;
	}
	
	//namedWindow("原图像",WINDOW_FREERATIO);
	//imshow("原图像",src);//imshow 第一个参数表示弹出窗口的名称，src表示调用哪个图像矩阵
	
	int RowNums = src.rows;
	int ColNums = src.cols*3;
	
	//cout << RowNums << endl;
	//cout << ColNums << endl;
	//cout << sizeof(*src.data) << endl;
	long int c;
	long int index = 0;
	start = chrono::steady_clock::now();
	for ( int i = 0; i < RowNums; ++i)
	{
		int j;
		//cout << i << endl;
		for(j = 0;j < ColNums-48; j+=48)
		{	
			uint8x16x3_t reg = vld3q_u8(src.data+i*ColNums+j);
			uint16x8_t Redl = vmovl_u8(*reinterpret_cast<uint8x8_t *>(&reg.val[0]));
			uint16x8_t Redh = vmovl_high_u8(reg.val[0]);
			uint16x8_t Greenl = vmovl_u8(*reinterpret_cast<uint8x8_t *>(&reg.val[1]));
			uint16x8_t Greenh = vmovl_high_u8(reg.val[1]);
			uint16x8_t Bluel = vmovl_u8(*reinterpret_cast<uint8x8_t *>(&reg.val[2]));
			uint16x8_t Blueh = vmovl_high_u8(reg.val[2]);
			
			Redl = vqshlq_n_u16(Redl,2);
			Redh = vqshlq_n_u16(Redh,2);
			Greenl = vqshlq_n_u16(Greenl,3);
			Greenh = vqshlq_n_u16(Greenh,3);
			Bluel = vqshlq_n_u16(Bluel,1);
			Blueh = vqshlq_n_u16(Blueh,1);
			
			uint16x8_t addColorl = vqaddq_u16(vqaddq_u16(Redl,Greenl),Bluel);
			uint16x8_t addColorh = vqaddq_u16(vqaddq_u16(Redh,Greenh),Blueh);
			
			addColorl = vshrq_n_u16(addColorl,4);
			addColorh = vshrq_n_u16(addColorh,4);
			
			uint8x16x3_t result;
			result.val[0] = vqmovn_high_u16(vqmovn_u16(addColorl),addColorh);
			result.val[1] = result.val[0];
			result.val[2] = result.val[1];
			vst3q_u8(src.data+i*ColNums+j,result);
			
			
			
		}
		for(; j < ColNums; j+=3)
		{
			unsigned int addColor = 0;

			int16_t RedData = src.data[i*ColNums+j] << 2;
			int16_t GreenData = src.data[i*ColNums+j+1] << 3;
			int16_t BlueData = src.data[i*ColNums+j+2] << 1;
			
			//将每个像素的RGB值相加,存到addColor
			addColor = RedData + GreenData + BlueData;
			addColor >>= 4;
			
			src.data[i*ColNums+j+0] =
			src.data[i*ColNums+j+1] =
			src.data[i*ColNums+j+2] = addColor;
		
		}
	}
	::end  = chrono::steady_clock::now();	
	//namedWindow("转灰度后图像",WINDOW_FREERATIO);
	//imshow("转灰度后图像",src);//imshow 第一个参数表示弹出窗口的名称，src表示调用哪个图像矩阵
	
	//waitKey(0);
	return 0;
}


int test2(int argc,char* argv[])
{

	Mat src= imread(argv[1]);//Mat是一个矩阵，所有图像都会是这样一个矩阵
	if(!src.data)
	{
		cout << "fail to load imagine"<<endl;
		return -1;
	}
	
	
	int RowNums = src.rows;
	int ColNums = src.cols;
	unsigned int averageColor = 0;
	unsigned int addColor = 0;
	unsigned int Imagedata = 0;
	int i,j;
	long int c;
	long int index = 0;
	start = chrono::steady_clock::now();
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
	::end  = chrono::steady_clock::now();
	return 0;
}

int main(int argc,char* argv[])
{
	test(argc,argv);
	cout << (::end-start).count()/1000000000. <<"s"<< endl;
	
	test2(argc,argv);
	cout << (::end-start).count()/1000000000. <<"s"<< endl;
	while(1);	
}


















