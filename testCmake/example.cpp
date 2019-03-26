#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv )
{
	if ( argc != 2 )
	{
		printf("usage: DisplayImage.out <Image_Path>\n");
		return -1;
	}
	Mat image; //Mat->Matrix 宣告一個圖片變數 
	image = imread( argv[1], 1 );//讀圖
	//(檔案名稱, flag < 0原圖; flag=0 灰階; flag>0 BGR) 
	if ( !image.data )
	{
		printf("No image data \n");
		return -1;
	}

	cout << image.rows << endl;
	cout << image.cols << endl;
	cout << image.size() << endl;

	image.at<Vec3b>(256, 256)[0] = 0;
	image.at<Vec3b>(256, 256)[1] = 0;
	image.at<Vec3b>(256, 256)[2] = 0;
	//	for(int i = 0; i < ; i++){
	//	
	//	}


	namedWindow("Display Image", WINDOW_AUTOSIZE ); //創建一個 window 並給名稱
	imshow("Display Image", image); //給定一個 window，並給要顯示的圖片
	waitKey(0);
	return 0;
}
