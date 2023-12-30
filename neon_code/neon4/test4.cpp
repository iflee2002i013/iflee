
using namespace std;
using namespace cv;
auto start = chrono::steady_clock::now();
auto end  = chrono::steady_clock::now();

int test4(int argc,char* argv[])
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
	
	start = chrono::steady_clock::now();//设置计时器开始计时
	
	uint8x8_t r_ratio = vdup_n_u8(77);
	uint8x8_t g_ratio = vdup_n_u8(151);
	uint8x8_t b_ratio = vdup_n_u8(28);
	

	for ( i = 0; i < RowNums; ++i)
	{
		for(j = 0; j < ColNums; ++j)
		{
			uint8x8x3_t rgb = vld3_u8(src.data+i*ColNums+j);
			uint8x8_t RedData = rgb.val[0];
			uint8x8_t GreenData = rgb.val[1];
			uint8x8_t BlueData = rgb.val[2];
			
			uint16x8_t y = vmull_u8(RedData,r_ratio);
			y = vmlal_u8(y,GreenData,g_ratio);
			y = vmlal_u8(y,BlueData,b_ratio);
			uint8x8_t result = vshrn_n_u16(y,8);
			
			vst1_u8(src.data+i*ColNums+j,result);
			
			
		}
	}
	::end  = chrono::steady_clock::now();//设置计时器停止计时
	
	namedWindow("转灰度后图像",WINDOW_FREERATIO);
	imshow("转灰度后图像",src);//imshow 第一个参数表示弹出窗口的名称，src表示调用哪个图像矩阵
	waitKey(0);
	return 0;
}
