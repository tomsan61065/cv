#include <stdio.h>
#include <math.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Mat image; //Mat->Matrix 宣告一個圖片變數
Mat result1;
Mat cvResult1;

Mat image2;
Mat result2;
Mat cvResult2;

void Question1(){
	int array[256] = {0}; //0~255
	int cdf[256] = {0};
	int T[256] = {0};

	for(int i = 0; i < image.rows; i++){
		for(int j = 0; j < image.cols; j++){
			array[image.at<uchar>(i, j)]++;
		}
	}

	int cdfMin = array[0];
	cdf[0] = array[0];
	for(int i = 1; i < 256; i++){
		if(cdfMin == 0 && cdf[i] != 0){
			cdfMin = cdf[i];
		}
		cdf[i] = cdf[i-1] + array[i];
	}

/*
	for(int i = 0; i < 256; i++){
		if(array[i] != 0)
			cout << "at color " << i <<" :" << array[i] << endl;
	}*/

	for(int i = 0; i < 256; i++){
//		T[i] = round( (double)(cdf[i] - cdfMin)/(double)(image.cols * image.rows - cdfMin) * (256 - 1) );
		T[i] = round( (double)(cdf[i])/(double)(image.cols * image.rows) * (256 - 1) );
		//(L - 1) / M*N * sigma(0~now)
	}

	for(int i = 0; i < image.rows; i++){
		for(int j = 0; j < image.cols; j++){
			result1.at<uchar>(i, j) = T[ image.at<uchar>(i, j) ];
		}
	}
	
}


void Question2(){

}


int main(int argc, char** argv )
{
	if ( argc != 2 )
	{
		printf("usage: DisplayImage.out <Image_Path>\n");
		return -1;
	}

	image = imread( "mp2.jpg", 0 );//讀圖
								//(檔案名稱, flag < 0原圖; flag=0 灰階; flag>0 BGR) 
	image2 = imread("mp2a.jpg", 1);

	if ( !image.data || !image2.data )
	{
		printf("No fully image data \n");
		return -1;
	}

	cout << "cols:" << image.cols << endl;
	cout << "rows:" << image.rows << endl;
	cout << image.size() << endl;
	cout << "depth: " << image.depth() << endl;//0~4
	cout << "type: " << image.type() << endl;
	cout << "channels:" << image.channels() << endl;

/*	for(int i = 0; i < image.rows; i++){
		for(int j = 0; j < image.cols; j++){
			cout << image.at<Vec3b>(i, j);
		}
		cout << endl;
	}*/

	result1.create(image.size(), CV_8U);
	cvResult1.create(image.size(), CV_8U);
	result2.create(image2.size(), CV_8UC3);
	cvResult2.create(image2.size(), CV_8UC3);

	Question1();

	namedWindow("Display Image", WINDOW_AUTOSIZE ); //創建一個 window 並給名稱
	namedWindow("result1", WINDOW_AUTOSIZE);

	imshow("Display Image", image); //給定一個 window，並給要顯示的圖片
	imshow("result1", result1);

	waitKey(0);
	return 0;
}
