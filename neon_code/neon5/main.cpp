//vcgt_u8（a,b）表示vector compare greater than，如果a>b，则将目标寄存器相应位置置位1，否则
//置位0
//a={1,2,3,4,5,6,7,8};
//b={2,3,5,1,4,5,2,10};
//则比较后，目标寄存器的输出结果（以uint8x8_t为例）：00011110
#include <iostream>
#include <arm_neon.h>

using namespace std;

//比较部分示例代码
int main1()
{
	//定义矩阵
	uint8_t martix1[8] = {1,2,3,4,5,6,7,8};
	uint8_t martix2[8] = {2,3,5,1,4,5,2,10};
	uint8_t result[8];
	//定义并将矩阵装在到寄存器
	uint8x8_t array1;
	uint8x8_t array2;
	array1 = vld1_u8 (martix1);
	array2 = vld1_u8 (martix2);
	//比较寄存器1中比寄存器2中大的值，将较大的对应目标寄存器位置位1
	uint8x8_t cmp_result = vcgt_u8(array1,array2);
	//将比较结果st到结果矩阵中
	vst1_u8(result,cmp_result);
	//打印结果矩阵
	for(int i = 0;i < 8;i++)
	{
		cout << "结果矩阵："<< static_cast<int>(result[i]) << endl;
	}
	
	cout << endl;
	while(1);

}

//分支示例代码
int main()
{
	//定义矩阵
	uint8_t martix1[8] = {100,180,10,181,201,200,200,3};
	uint8_t martix2[8] = {180,180,180,180,180,180,180,180};
	
	uint8_t result[8];
	uint8_t result1[8];
	uint8_t result2[8];
	uint8_t result3[8];
	uint8_t result4[8];
	//定义并将矩阵装在到寄存器
	uint8x8_t array1;
	uint8x8_t array2;
	array1 = vld1_u8 (martix1);
	array2 = vld1_u8 (martix2);
	//比较寄存器1中比寄存器2中大的值，将较大的对应目标寄存器位置位1
	uint8x8_t cmp_result = vcgt_u8(array1,array2);
	vst1_u8(result,cmp_result);
	cout << "比较结果：";
	for(int i = 0;i < 8;i++)
	{
		cout << static_cast<int>(result[i])<<" ";
	}
	
	cout << endl;
	
	//a*b+a*(!b)
	//if(a>180) a = 180;
	uint8x8_t cmp_And_180;
	cmp_And_180 = vand_u8(cmp_result,array2);
	vst1_u8(result1,cmp_And_180);
	cout << "与：";
	for(int i = 0;i < 8;i++)
	{
		cout << static_cast<int>(result1[i])<<" ";
	}
	
	cout << endl;
	
	//a<180，则将比较结果取反
	uint8x8_t ncmp_result;
	ncmp_result = vmvn_u8(cmp_result);
	vst1_u8(result2,ncmp_result);
	cout << "取反：";
	for(int i = 0;i < 8;i++)
	{
		cout << static_cast<int>(result2[i]) <<" ";
	}
	
	cout << endl;
	
	//else a = a
	uint8x8_t ncmp_And_m1;
	ncmp_And_m1 = vand_u8(ncmp_result,array1);
	vst1_u8(result3,ncmp_And_m1);
	cout << "再与：";
	for(int i = 0;i < 8;i++)
	{
		cout << static_cast<int>(result3[i])<<" ";
	}
	
	cout << endl;
	
	//if + else
	uint8x8_t or_Result;
	or_Result = vorr_u8(cmp_And_180,ncmp_And_m1);

	//将分支结果st到结果矩阵中
	vst1_u8(result4,or_Result);
	cout << "结果矩阵：";
	for(int i = 0;i < 8;i++)
	{
		cout << static_cast<int>(result4[i])<<" ";
	}
	
	cout << endl;
	while(1);
}












