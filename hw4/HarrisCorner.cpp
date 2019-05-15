#include <stdio.h>
#include <math.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Mat image; //Mat->Matrix 宣告一個圖片變數

Mat image2;

void Sobel(Mat &src, Mat &resultX, Mat &resultY){
	//use sobel 3*3 return Dx, Dy
	resultX.create(src.size(), CV_32F);
	resultY.create(src.size(), CV_32F);
	for(int i = 0; i < src.rows; i++){
		for(int j = 0; j < src.cols; j++){
			if(i == 0 || j == 0 || i == src.rows -1 || j == src.cols -1){
				resultX.at<float>(i, j) = 0.0;
				resultY.at<float>(i, j) = 0.0;
			}else{
				float tempX = src.at<uchar>(i-1, j+1) + 2*src.at<uchar>(i, j+1) + src.at<uchar>(i+1, j+1);
				tempX = tempX - src.at<uchar>(i-1, j-1) - 2*src.at<uchar>(i, j-1) - src.at<uchar>(i+1, j-1);

				float tempY = src.at<uchar>(i+1, j-1) + 2*src.at<uchar>(i+1, j) + src.at<uchar>(i+1, j+1);
				tempY = tempY - src.at<uchar>(i-1, j-1) - 2*src.at<uchar>(i-1, j) - src.at<uchar>(i-1, j+1);

				resultX.at<float>(i, j) = tempX;
				resultY.at<float>(i, j) = tempY;
			//	dst.at<float>(i, j) = abs(tempX) + abs(tempY);
			//	dst.at<float>(i, j) = sqrt(tempX*tempX + tempY*tempY);
			}
		}
	}
}

void HarrisCorner(Mat &src, Mat &dst, int blockSize, int ksize, double k){
	dst.create(src.size(), src.type());
	Mat Dx, Dy;
	Sobel(src, Dx, Dy);

	for(int i = 0; i < src.rows; i++){
		for(int j = 0; j < src.cols; j++){

		}
	}
}

int main(int argc, char** argv )
{
	image = imread( "test.jpg", 0);//讀圖
								//(檔案名稱, flag < 0原圖; flag=0 灰階; flag>0 BGR) 

	//變成一樣大小
	//resize(image, image, Size(), 0.5, 0.5); //(int , out, size, fx, fy, interpolation 方法)
	//resize(image, image, Size(image.rows*0.5, image.cols*0.5), 0, 0)

	cout << "cols:" << image.cols << endl;
	cout << "rows:" << image.rows << endl;
	cout << image.size() << endl;
	cout << "depth: " << image.depth() << endl;//0~4
	cout << "type: " << image.type() << endl;
	cout << "channels:" << image.channels() << endl;


	namedWindow("Display Image", WINDOW_AUTOSIZE);
	imshow("Display Image", image);

	Mat tempX, tempY;
	/*
	Sobel(image, tempX, tempY);
	tempX.convertTo(tempX, CV_8U);
	tempY.convertTo(tempY, CV_8U);
	imshow("X", tempX);
	imshow("Y", tempY);*/

	waitKey(0);
	return 0;
}
