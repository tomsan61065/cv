#include <stdio.h>
#include <math.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
using namespace cv;

Mat image; //Mat->Matrix 宣告一個圖片變數


void Question(Mat &image, Mat &result, int Qtype){
	//turn to frequency domain
	Mat planes[] = {Mat_<float>(image), Mat::zeros(image.size(), CV_32F)}; //建立一個 planes[2] = {padded, zero}
	if(Qtype == 3){
		Mat temp;
		resize(planes[0], temp, Size(), 0.5, 0.5);//內差法
		planes[0] = temp;
		planes[1] = Mat::zeros(temp.size(), CV_32F);
	}

	result.create(image.dims, image.size, image.type());
	
	Mat complexI;
	merge(planes, 2, complexI);// complexI = planes[2]

	dft(complexI, complexI); 
	split(complexI, planes);

	Mat tempMag;
	Mat tempAng;
	magnitude(planes[0], planes[1], tempMag);// planes[0] = magnitude
	phase(planes[0], planes[1], tempAng);


	//Quesiton Type
	if(Qtype == 1){
		vector<float> magCount(65536, 0.0); //256*256
		for(int i = 0; i < tempMag.rows; i++){
			for(int j = 0; j < tempMag.cols; j++){
				magCount[i*256 + j] = tempMag.at<float>(i, j);
			}
		}
		sort(magCount.begin(), magCount.end());
		//	for(int i = 0; i < 10; i++){
		//		cout << magCount[65536/4*3 + i - 5] << " ";
		//	}cout << endl;
		for(int i = 0; i < tempMag.rows; i++){
			for(int j = 0; j < tempMag.cols; j++){
				if(tempMag.at<float>(i, j) < magCount[65536/4*3]){
					tempMag.at<float>(i, j) = 0.0;
				}
			}
		}
	}else if(Qtype == 2){
		vector<vector<float>> magCount(256, vector<float>(256, 0.0));// 256 * (16*16)
		/*
		00 01 02 03  04 05 06 07  08 ... 015 (0, 16)
		10 11 12 13  14 15 16 17  18 ... 115
		20 21 22 23  24 25 26 27  28 ... 215
		30 31 32 33  34 35 36 37  38 ... 315
		...

		150 ..                           1515 (15,16) ~ (15, 31)
		*/
		for(int i = 0; i < tempMag.rows; i++){
			for(int j = 0; j < tempMag.cols; j++){
				magCount[(i / 16)*16 + (j/ 16)][(i % 16)*16 + j % 16] = tempMag.at<float>(i, j);
			}
		}
		for(int i = 0; i < 256; i++){
			sort(magCount[i].begin(), magCount[i].end());
		}

	/*	for(int i = 0; i < 10; i++){
			cout << magCount[0][256/4*3 + i - 5] << " ";
		}cout << endl;*/
		for(int i = 0; i < tempMag.rows; i++){
			for(int j = 0; j < tempMag.cols; j++){
			/*	if(i < 16 && j < 16){
					cout << tempMag.at<float>(i, j) << " ";
					if(j == 15){
						cout << endl;
					}
				}*/
				if(tempMag.at<float>(i, j) < magCount[(i / 16)*16 + (j/ 16)][256/4*3] ){
					tempMag.at<float>(i, j) = 0.0;
				}
			}
		}
	}

	//turn back to spatial domain
	Mat tempX;
	Mat tempY;
	if(Qtype == 3){
		copyMakeBorder(tempMag, tempMag, 64, 64, 64, 64, BORDER_CONSTANT, Scalar::all(0));
		copyMakeBorder(tempAng, tempAng, 64, 64, 64, 64, BORDER_CONSTANT, Scalar::all(0));
		/*for(int i = 0; i < 70; i++){
			for(int j = 0; j < 70; j++){
				cout << tempMag.at<float>(i, j) << " ";
			}cout << endl;
		}*/
	}

	polarToCart(tempMag, tempAng, tempX, tempY);
	Mat tempResult[] = {tempX, tempY};
	merge(tempResult, 2, result);
	//沒有 DFT_REAL 就會回傳實部虛部 2 channel 的; !!注意一定要 DFT_SCALE，看上課講義 dft 或 idft 總有一邊有除以項數
	dft(result, result, DFT_INVERSE + DFT_REAL_OUTPUT + DFT_SCALE); 

	normalize(result, result, 0, 255, NORM_MINMAX);//正規化，不然顏色會稍微怪一點
	result.convertTo(result, CV_8U);

}


void QuestionB(Mat &image, Mat &result){
	//turn to frequency domain
	result.create(image.dims, image.size, image.type());
	Mat complexI;
	Mat tempMag;
	Mat tempAng;
	Mat tempX;
	Mat tempY;

	for(int i = 0; i < 16; i++){
		for(int j = 0; j < 16; j++){
			Rect crop(i*16, j*16, 16, 16);//start x, start y, width, hight
			Mat rez = image(crop);
			Mat planes[] = {Mat_<float>(rez), Mat::zeros(rez.size(), CV_32F)}; //建立一個 planes[2] = {padded, zero}
			//cout << planes[0].size();
			merge(planes, 2, complexI);// complexI = planes[2]

			dft(complexI, complexI); 
			split(complexI, planes);

			magnitude(planes[0], planes[1], tempMag);// planes[0] = magnitude
			phase(planes[0], planes[1], tempAng);

			vector<float> magCount(256, 0.0); //16*16
			for(int m = 0; m < tempMag.rows; m++){
				for(int n = 0; n < tempMag.cols; n++){
					magCount[m*16 + n] = tempMag.at<float>(m, n);
				}
			}
			sort(magCount.begin(), magCount.end());
			//	for(int i = 0; i < 10; i++){
			//		cout << magCount[256/4*3 + i - 5] << " ";
			//	}cout << endl;
			for(int m = 0; m < tempMag.rows; m++){
				for(int n = 0; n < tempMag.cols; n++){
					if(tempMag.at<float>(m, n) < magCount[256/4*3]){
						tempMag.at<float>(m, n) = 0.0;
					}
				}
			}

			
			//turn back to spatial domain
			polarToCart(tempMag, tempAng, tempX, tempY);
			Mat tempResult[] = {tempX, tempY};
			Mat idftResult;
			merge(tempResult, 2, idftResult);
			//沒有 DFT_REAL 就會回傳實部虛部 2 channel 的; !!注意一定要 DFT_SCALE，看上課講義 dft 或 idft 總有一邊有除以項數
			dft(idftResult, idftResult, DFT_INVERSE + DFT_REAL_OUTPUT + DFT_SCALE); 
			idftResult.copyTo(result(crop));
		}
	}	
	normalize(result, result, 0, 255, NORM_MINMAX);//正規化，不然顏色會稍微怪一點
	result.convertTo(result, CV_8U);

}

int MSE(Mat &x, Mat &y){
	int total = 0;
	int temp = 0;
	for(int i = 0; i < x.rows; i++){
		for(int j = 0; j < x.cols; j++){
			temp = x.at<uchar>(i, j) - y.at<uchar>(i,j);
			total += (temp * temp);
		}
	}
	total /= x.rows * x.cols;
	return total;
}

int main(int argc, char** argv )
{
	image = imread( "bridge.jpg", 0);//讀圖
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

	Mat resultA;
	Question(image, resultA, 1);
	cout << "QuestionA MSE: " << MSE(image, resultA) << endl;
	imshow("QuestionA", resultA);
	imwrite( "./resultA.png", resultA);


	Mat resultB;
	//Question(image, resultB, 2);
	QuestionB(image, resultB);
	cout << "QuestionB MSE: " << MSE(image, resultB) << endl;
	imshow("QuestionB", resultB);
	imwrite( "./resultB.png", resultB);


	Mat resultC;
	Question(image, resultC, 3);
	cout << "QuestionC MSE: " << MSE(image, resultC) << endl;
	imshow("QuestionC", resultC);
	imwrite( "./resultC.png", resultC);

	waitKey(0);
	return 0;
}
