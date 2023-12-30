#include <iostream>
#include <arm_neon.h>
#include <chrono>
#include <opencv2/core.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#define LOOP10 for(int loop = 0; loop < 1000; ++loop)

using namespace std;
using namespace cv;
auto start = chrono::steady_clock::now();
auto end  = chrono::steady_clock::now();

int test(int argc,char* argv[])//自行完成的算法
{

	Mat src= imread(argv[1]);//Mat是一个矩阵，所有图像都会是这样一个矩阵
	if(!src.data)
	{
		cout << "fail to load imagine"<<endl;
		return -1;
	}
	
	
	int RowNums = src.rows;
	int ColNums = src.cols;
	unsigned int addColor = 0;
	
	int i,j;
	long int c;
	long int index = 0;
	start = chrono::steady_clock::now();
	LOOP10
	for ( i = 0,index = 0; i < RowNums; ++i)
	{ 
		for(j = 0; j < ColNums; ++j)
		{
			int16_t RedData = src.data[index*3+0] << 2;
			int16_t GreenData = src.data[index*3+1] << 3;
			int16_t BlueData = src.data[index*3+2] << 1;
			//将每个像素的RGB值相加,存到addColor
			addColor = RedData + GreenData + BlueData;
			addColor >>= 4;
			
			src.data[index*3+0] =
			src.data[index*3+1] =
			src.data[index*3+2] = addColor;
			
			index++;
		}
	}
	::end  = chrono::steady_clock::now();
	return 0;
}

int test1(int argc,char* argv[])//使用neon自行优化灰度转换解决方案
{

	Mat src= imread(argv[1]);
	if(!src.data)
	{
		cout << "fail to load imagine"<<endl;
		return -1;
	}
	
	//namedWindow("原图像",WINDOW_FREERATIO);
	//imshow("原图像",src);//imshow 第一个参数表示弹出窗口的名称，src表示调用哪个图像矩阵
	
	int RowNums = src.rows;
	int ColNums = src.cols*3;//一个像素3个通道因此要*3
	
	
	//cout << RowNums << endl;
	//cout << ColNums << endl;
	//cout << sizeof(*src.data) << endl;

	start = chrono::steady_clock::now();
	LOOP10
	for ( int i = 0; i < RowNums; ++i)
	{
		int j;
		//cout << i << endl;
		for(j = 0;j < ColNums-48; j+=48)
		{	
			
			uint8x16x3_t reg = vld3q_u8(src.data+i*ColNums+j);//将第一个像素的24位每8位交错装载到三个寄存器中
			//0.00135384s
			
			//将寄存器中的8位数据扩展为16位数据
			uint16x8_t Redl = vmovl_u8(*reinterpret_cast<uint8x8_t *>(&reg.val[0]));
			uint16x8_t Redh = vmovl_high_u8(reg.val[0]);
			uint16x8_t Greenl = vmovl_u8(*reinterpret_cast<uint8x8_t *>(&reg.val[1]));
			uint16x8_t Greenh = vmovl_high_u8(reg.val[1]);
			uint16x8_t Bluel = vmovl_u8(*reinterpret_cast<uint8x8_t *>(&reg.val[2]));
			uint16x8_t Blueh = vmovl_high_u8(reg.val[2]);
			//0.00283856s
			
			//对数据的高低八位分别做移位处理，完成灰度权重转换
			Redl = vqshlq_n_u16(Redl,2);
			Redh = vqshlq_n_u16(Redh,2);
			Greenl = vqshlq_n_u16(Greenl,3);
			Greenh = vqshlq_n_u16(Greenh,3);
			Bluel = vqshlq_n_u16(Bluel,1);
			Blueh = vqshlq_n_u16(Blueh,1);
			//0.0036334s
			
			
			//求和
			uint16x8_t addColorl = vqaddq_u16(vqaddq_u16(Redl,Greenl),Bluel);
			uint16x8_t addColorh = vqaddq_u16(vqaddq_u16(Redh,Greenh),Blueh);
			//0.00380634s

			
			//对数据右移4位得出最终的灰度值
			addColorl = vshrq_n_u16(addColorl,4);
			addColorh = vshrq_n_u16(addColorh,4);
			//0.00530014s
			
			
			//将灰度值传递回src中
			uint8x16x3_t result;
			result.val[0] = vqmovn_high_u16(vqmovn_u16(addColorl),addColorh);
			result.val[1] = result.val[0];
			result.val[2] = result.val[1];
			vst3q_u8(src.data+i*ColNums+j,result);
			//0.00752409
		}
		
	}
	::end  = chrono::steady_clock::now();	
	//namedWindow("转灰度后图像",WINDOW_FREERATIO);
	//imshow("转灰度后图像",src);//imshow 第一个参数表示弹出窗口的名称，src表示调用哪个图像矩阵
	
	//waitKey(0);
	return 0;
}

int test2(int argc,char* argv[])//最原始的转灰度算法
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
	LOOP10
	for ( i = 0,index=0; i < RowNums; ++i)
	{
		for(j = 0; j < ColNums; ++j)
		{
			uint8_t RedData = src.data[index*3+0];
			uint8_t GreenData = src.data[index*3+1];
			uint8_t BlueData = src.data[index*3+2];
			
			int r_ratio = 77;
			int g_ratio = 151;
			int b_ratio = 28;
			int temp = RedData*r_ratio + GreenData*g_ratio + BlueData*b_ratio;
			unsigned char gray = temp/256;
			
				//像素索引，每替换一个像素后+1
			//cout << "=============第"<<i<<"*"<<j<<"=============";
			//cout << endl;
			src.data[index*3+0] =
			src.data[index*3+1] =
			src.data[index*3+2] = gray;
			
			index++;
		}
	}
	::end  = chrono::steady_clock::now();
	//namedWindow("转灰度后图像",WINDOW_FREERATIO);
	//imshow("转灰度后图像",src);//imshow 第一个参数表示弹出窗口的名称，src表示调用哪个图像矩阵
	waitKey(0);
	return 0;
}
int test3(int argc,char* argv[])////使用更小的数据类型完成的灰度转换算法
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
	LOOP10
	for ( i = 0,index = 0; i < RowNums; ++i)
	{
		for(j = 0; j < ColNums; ++j)
		{
			uint8_t RedData = src.data[index*3+0];
			uint8_t GreenData = src.data[index*3+1];
			uint8_t BlueData = src.data[index*3+2];
			
			int r_ratio = 77;
			int g_ratio = 151;
			int b_ratio = 28;
			int temp = RedData*r_ratio;
			temp += GreenData*g_ratio;
			temp += (BlueData*b_ratio);
			
				
			//cout << "=============第"<<i<<"*"<<j<<"=============";
			//cout << endl;
			src.data[index*3+0] =
			src.data[index*3+1] =
			src.data[index*3+2] = (temp>>8);
			
			index++;//像素索引，每替换一个像素后+1
		}
	}
	::end  = chrono::steady_clock::now();
	//namedWindow("转灰度后图像",WINDOW_FREERATIO);
	//imshow("转灰度后图像",src);//imshow 第一个参数表示弹出窗口的名称，src表示调用哪个图像矩阵
	//waitKey(0);
	return 0;
}


int test4(int argc,char* argv[])//使用刘文志的方法
{

	Mat src= imread(argv[1]);//Mat是一个矩阵，所有图像都会是这样一个矩阵
	if(!src.data)
	{
		cout << "fail to load imagine"<<endl;
		return -1;
	}
	
	
	int RowNums = src.rows;
	int ColNums = src.cols*3;
	unsigned int averageColor = 0;
	unsigned int addColor = 0;
	unsigned int Imagedata = 0;
	int i,j;
	
	
	
	uint8x8_t r_ratio = vdup_n_u8(77);
	uint8x8_t g_ratio = vdup_n_u8(151);
	uint8x8_t b_ratio = vdup_n_u8(28);
	
	
	start = chrono::steady_clock::now();//设置计时器开始计时
	LOOP10
	for ( i = 0; i < RowNums; ++i)
	{
		for(j = 0; j < ColNums-24; j+=24)
		{
			uint8x8x3_t rgb = vld3_u8(src.data+i*ColNums+j);
			uint8x8_t RedData = rgb.val[0];
			uint8x8_t GreenData = rgb.val[1];
			uint8x8_t BlueData = rgb.val[2];
			
			uint16x8_t y = vmull_u8(RedData,r_ratio);
			y = vmlal_u8(y,GreenData,g_ratio);
			y = vmlal_u8(y,BlueData,b_ratio);
			uint8x8_t ret = vshrn_n_u16(y,8);
			
			uint8x8x3_t result;
			result.val[0] = ret;
			result.val[1] = result.val[0];
			result.val[2] = result.val[1];
			
			vst3_u8(src.data+i*ColNums+j,result);
			
		}
	}
	::end  = chrono::steady_clock::now();//设置计时器停止计时
	
	//namedWindow("转灰度后图像",WINDOW_FREERATIO);
	//imshow("转灰度后图像",src);//imshow 第一个参数表示弹出窗口的名称，src表示调用哪个图像矩阵
	//waitKey(0);
	return 0;
}
/*
int test5(int argc,char* argv[])//使用刘文志的方法
{

	Mat src= imread(argv[1]);//Mat是一个矩阵，所有图像都会是这样一个矩阵
	if(!src.data)
	{
		cout << "fail to load imagine"<<endl;
		return -1;
	}
	
	
	int RowNums = src.rows;
	int ColNums = src.cols*3;
	unsigned int averageColor = 0;
	unsigned int addColor = 0;
	unsigned int Imagedata = 0;
	int i,j;
	
	
	
	uint8x16_t r_ratio = vdup_n_u8(77);
	uint8x16_t g_ratio = vdup_n_u8(151);
	uint8x16_t b_ratio = vdup_n_u8(28);
	
	
	start = chrono::steady_clock::now();//设置计时器开始计时
	for ( i = 0; i < RowNums; ++i)
	{
		for(j = 0; j < ColNums-24; j+=24)
		{
			uint8x16x3_t rgb = vld3q_u8(src.data+i*ColNums+j);
			uint8x16_t RedData = rgb.val[0];
			uint8x16_t GreenData = rgb.val[1];
			uint8x16_t BlueData = rgb.val[2];
			
			uint16x8_t y = vmullq_u8(RedData,r_ratio);
			y = vmlal_u8(y,GreenData,g_ratio);
			y = vmlal_u8(y,BlueData,b_ratio);
			uint8x8_t ret = vshrn_n_u16(y,8);
			
			uint8x8x3_t result;
			result.val[0] = ret;
			result.val[1] = result.val[0];
			result.val[2] = result.val[1];
			
			vst3_u8(src.data+i*ColNums+j,result);
			
		}
	}
	::end  = chrono::steady_clock::now();//设置计时器停止计时
	
	namedWindow("转灰度后图像",WINDOW_FREERATIO);
	imshow("转灰度后图像",src);//imshow 第一个参数表示弹出窗口的名称，src表示调用哪个图像矩阵
	waitKey(0);
	return 0;
}
*/


int main(int argc,char* argv[])
{	
	
	//test(argc,argv);
	//cout <<"c++shift: "<< (::end-start).count()/10000000000. <<"s"<< endl;
	
	test1(argc,argv);
	cout <<"neon_shift: "<< (::end-start).count()/1000000000000. <<"s"<< endl;
	
	//test2(argc,argv);
	//cout <<"c++mul: "<< (::end-start).count()/10000000000. <<"s"<< endl;
	
	//test3(argc,argv);
	//cout <<"c++mul2: "<< (::end-start).count()/10000000000. <<"s"<< endl;
	
	//test4(argc,argv);
	//cout <<"neon_mul: "<< (::end-start).count()/10000000000. <<"s"<< endl;
	
	while(1);	
}


















