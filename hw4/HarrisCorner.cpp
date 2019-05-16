#include <stdio.h>
#include <math.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Mat image; //Mat->Matrix 宣告一個圖片變數

Mat image2;

void mySobel(Mat &src, Mat &resultX, Mat &resultY){
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


void HarrisCorner(Mat &src, Mat &dst, int blockSize, double k){
	dst.create(src.size(), CV_32F);
	Mat Dx, Dy;
	mySobel(src, Dx, Dy);

	for(int i = 0; i < src.rows; i++){
		for(int j = 0; j < src.cols; j++){
			if(i == 0 || j == 0 || i == src.rows -1 || j == src.cols -1){
				dst.at<float>(i, j) = 0.0;
			}else{ 
				//det(M)/trace(M) = (DxDx*DyDy - DxDy*DxDy)/(DxDx + DyDy)
				dst.at<float>(i, j) = (
					Dx.at<float>(i, j) * Dx.at<float>(i, j) 
					* Dy.at<float>(i, j) * Dy.at<float>(i, j) 
					- Dx.at<float>(i, j) );


				//det(M) - k * (trace(M))^2 = (DxDx*DyDy - DxDy*DxDy) - k * (DxDx + DyDy)^2
			}
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
	
	mySobel(image, tempX, tempY);
	tempX.convertTo(tempX, CV_8U);
	//Sobel(image, tempY, CV_32F, 1, 0,  3, 1.0, 0, BORDER_DEFAULT);
	//tempY.convertTo(tempY, CV_8U);
	imshow("X", tempX);
	//imshow("Y", tempY);
	//imshow("sobelX", tempY);
	Mat result;
		cornerHarris(image, result, 2, 3, 0.0);
	Mat dst_norm;
	normalize( result, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );
	convertScaleAbs( dst_norm, dst_norm );
	imshow("cvHarris", dst_norm);

	waitKey(0);
	return 0;
}
