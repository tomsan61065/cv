#include <stdio.h>
#include <math.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Mat image; //Mat->Matrix 宣告一個圖片變數
Mat result1;
Mat cvResult1;

Mat image2;
Mat result2_1;
Mat cvResult2_1;
Mat result2_2;
Mat cvResult2_2;
Mat result2_3;
Mat cvResult2_3;

void HistogramEqualizer(Mat &image, Mat &result1){
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
		// wiki 版本的公式, 有 - cdfMin
//		T[i] = round( (double)(cdf[i] - cdfMin)/(double)(image.cols * image.rows - cdfMin) * (256 - 1) ); 

		T[i] = round( (double)(cdf[i])/(double)(image.cols * image.rows) * (256 - 1) );
		//(L - 1) / M*N * sigma(0~now)
		// (cumulative/M*N) * (L-1) -> {累進數量/整體 = 累進機率(0~1)} * 區間(0~L-1)
	}

	for(int i = 0; i < image.rows; i++){
		for(int j = 0; j < image.cols; j++){
			result1.at<uchar>(i, j) = T[ image.at<uchar>(i, j) ];
		}
	}
	
}


void textbookExample(){
	int array[256] = {790, 1023, 850, 656, 329, 245, 122, 81}; //0~255
	int cdf[256] = {0};
	int T[256] = {0};

	int cdfMin = array[0];
	cdf[0] = array[0];
	for(int i = 1; i < 256; i++){
		if(cdfMin == 0 && cdf[i] != 0){
			cdfMin = cdf[i];
		}
		cdf[i] = cdf[i-1] + array[i];
	}

	for(int i = 0; i < 8; i++){
//		T[i] = round( (double)(cdf[i] - cdfMin)/(double)(64 * 64 - cdfMin) * (8 - 1) );
//		cout <<  (double)(cdf[i])/(double)(64 * 64) << " ";

		T[i] = round( (double)(cdf[i])/(double)(64 * 64) * (8 - 1) );
		//(L - 1) / M*N * sigma(0~now)
	}cout << endl;

	for(int i = 0; i < 8; i++){
		cout << T[i] << " "; 
	}
	cout << endl;
}


void Question2(){

	/*** 2-1 rgb ***/
	vector<Mat> bgr(3);
	split(image2, bgr);
	vector<Mat> temp(bgr.begin(), bgr.end());
	HistogramEqualizer(bgr[0], temp[0]);
	HistogramEqualizer(bgr[1], temp[1]);
	HistogramEqualizer(bgr[2], temp[2]);
	merge(temp, result2_1);
	equalizeHist(bgr[0], temp[0]);
	equalizeHist(bgr[1], temp[1]);
	equalizeHist(bgr[2], temp[2]);
	merge(temp, cvResult2_1);

	Mat Q2_1diff = result2_1 - cvResult2_1;
	namedWindow("Q2_1diff", WINDOW_AUTOSIZE);
	imshow("Q2_1diff", Q2_1diff);



	/*** 2-2 HSV ***/
	//opencv hsv 問題
	//https://stackoverflow.com/questions/50449602/wrong-hsv-conversion-from-rgb
	//https://stackoverflow.com/questions/43307959/how-to-get-hsv-and-lab-color-space/43319977#43319977
	vector<Mat> hsv(3);
	cvtColor(image2, cvResult2_2, COLOR_BGR2HSV);
	for(int i = 0; i < 3; i++){
		cout << (int)cvResult2_2.at<Vec3b>(0, 0)[i] << " ";
	}cout << endl;

//	cout << cvResult2_2 << endl;
	split(cvResult2_2, hsv); 
	for(int i = 0; i < 3; i++){
		hsv[i].convertTo(hsv[i], CV_16U);
		temp[i] = Mat(image2.size(), CV_16U);
	}
	temp[0] = hsv[0] * 2;
	temp[1] = hsv[1] * 100 / 255;
	temp[2] = hsv[2] * 100 / 255;
	merge(temp, result2_2);

	cout << result2_2.type() << endl;

	for(int i = 0; i < 3; i++){
		cout << (int)image2.at<Vec3b>(0, 0)[i] << " ";
	}cout << endl;

	for(int i = 0; i < 3; i++){
		cout << (int)result2_2.at<Vec3b>(0, 0)[i] << " ";
	}cout << endl;

	for(int i = 0; i < 3; i++){
		cout << (int)cvResult2_2.at<Vec3b>(0, 0)[i] << " ";
	}cout << endl;

	namedWindow("result2_2", WINDOW_AUTOSIZE);
	imshow("result2_2", result2_2);
	namedWindow("cvResult2_2", WINDOW_AUTOSIZE);
	imshow("cvResult2_2", cvResult2_2);

	


}


int main(int argc, char** argv )
{
	image = imread( "mp2.jpg", 0 );//讀圖
								//(檔案名稱, flag < 0原圖; flag=0 灰階; flag>0 BGR) 
	image2 = imread("mp2a.jpg", 1);

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
	
	
	result1.create(image.size(), CV_8U);
//	cvResult1.create(image.size(), CV_8U);

	result2_1.create(image2.size(), CV_8UC3);
	cvResult2_1.create(image2.size(), CV_8UC3);


	/*** Question1 ***/
	HistogramEqualizer(image, result1);
	equalizeHist(image, cvResult1);
	Mat Q1diff;
	Mat mask;
//	subtract(result1, cvResult1, Q1diff, mask, -1); //src1, src2, dst, mask, dtype
	Q1diff = result1 - cvResult1;
/*
	namedWindow("Display Image", WINDOW_AUTOSIZE ); //創建一個 window 並給名稱
	namedWindow("result1", WINDOW_AUTOSIZE);
	namedWindow("cvResult1", WINDOW_AUTOSIZE);
	namedWindow("Q1diff", WINDOW_AUTOSIZE);
	imshow("Display Image", image); //給定一個 window，並給要顯示的圖片
	imshow("result1", result1);
	imshow("cvResult1", cvResult1);
	imshow("Q1diff", Q1diff);*/



	Question2();
/*	namedWindow("Display Image2", WINDOW_AUTOSIZE);
	namedWindow("result2_1", WINDOW_AUTOSIZE);
	namedWindow("cvResult2_1", WINDOW_AUTOSIZE);
	imshow("Display Image2", image2);
	imshow("result2_1", result2_1);
	imshow("cvResult2_1", cvResult2_1);*/

//	textbookExample();

	waitKey(0);
	return 0;
}
