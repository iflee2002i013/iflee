
#include <iostream>
#include <arm_neon.h>

	using namespace std;
/*
int main1(int argc,char* argv[])
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

	long int c;
	long int index = 0;
	
	for ( int i = 0; i < RowNums; ++i)
	{
		for(int j = 0; j < ColNums; ++j)
		{
			unsigned int addColor = 0;
			index++;	//像素索引，每替换一个像素后+1

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
	
	waitKey(0);
	while(1);
	return 0;
}
*/
int main()
{
	uint8_t mainarry[16]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
	std::cout <<"0"<<endl;
	uint8x16_t reg = vld1q_u8(mainarry);
	std::cout <<"a"<<endl;
	uint16x8_t reg1 = vmovl_u8(*reinterpret_cast<uint8x8_t *>(&reg));
	
	uint16x8_t reg2 = vqshlq_n_u16(reg1,1);
	
	uint16_t outmainarray[8];
	std::cout <<"b"<<endl;
	vst1q_u16 (outmainarray,reg2);
	std::cout <<"c"<<endl;
	for (auto i:outmainarray)
	{
		std::cout << i <<",";
	}
	std::cout << std::flush;
	while(1);
	return 0;
}


















