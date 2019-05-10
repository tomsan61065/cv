#include <stdio.h>
#include <math.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Mat image; //Mat->Matrix 宣告一個圖片變數

Mat image2;



int main(int argc, char** argv )
{
	image = imread( "Fig0424(a)(rectangle).tif", 0);//讀圖
								//(檔案名稱, flag < 0原圖; flag=0 灰階; flag>0 BGR) 
	//image2 = imread("Fig0427(a)(woman).tif", 0);

	//變成一樣大小
	resize(image, image, Size(), 0.5, 0.5); //(int , out, size, fx, fy, interpolation 方法)
	//resize(image, image, Size(image.rows*0.5, image.cols*0.5), 0, 0)

	cout << "cols:" << image.cols << endl;
	cout << "rows:" << image.rows << endl;
	cout << image.size() << endl;
	cout << "depth: " << image.depth() << endl;//0~4
	cout << "type: " << image.type() << endl;
	cout << "channels:" << image.channels() << endl;

	cout << endl << "Image2:" << endl;
	cout << "cols:" << image2.cols << endl;
	cout << "rows:" << image2.rows << endl;
	cout << image2.size() << endl;
	cout << "depth: " << image2.depth() << endl;//0~4
	cout << "type: " << image2.type() << endl;
	cout << "channels:" << image2.channels() << endl;
	
	
	namedWindow("Display Image", WINDOW_AUTOSIZE);
	imshow("Display Image", image);

	namedWindow("Display Image2", WINDOW_AUTOSIZE);
	imshow("Display Image2", image2);
	Mat result;
	Mat result2;
	exchangePhaseAndMagnitude(image, image2, result, result2);
	imshow("result e", result);
	imshow("result f", result2);

	waitKey(0);
	return 0;
}
