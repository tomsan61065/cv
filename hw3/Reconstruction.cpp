#include <stdio.h>
#include <math.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Mat image; //Mat->Matrix 宣告一個圖片變數

Mat image2;

void getPhase(Mat &x, Mat &y, Mat &result){ //cv::phase()
	//angle(I) = atan2(y(I), x(I));
	result.create(x.dims, x.size, x.type());

	for(int i = 0; i < x.rows; i++){
		for(int j = 0; j < x.cols; j++){
			if(x.depth() == CV_32F){ //c++ atan2 return radians
				result.at<float>(i, j) = atan2(y.at<float>(i, j), x.at<float>(i, j));
			}else{
				result.at<double>(i, j) = atan2(y.at<double>(i, j), x.at<double>(i, j));
			}
		}
	}
	return;
}

void getMagnitude(Mat &x, Mat &y, Mat &result){//cv::magnitude
	//magnitude(I) = sqrt( Re(DFT(I))^2 + Im(DFT(I))^2 )i;	
	result.create(x.dims, x.size, x.type());

	for(int i = 0; i < x.rows; i++){
		for(int j = 0; j < x.cols; j++){
			if(x.depth() == CV_32F){ 
				result.at<float>(i, j) = sqrt(y.at<float>(i, j)*y.at<float>(i, j) + x.at<float>(i, j)*x.at<float>(i, j));
			}else{
				result.at<double>(i, j) = sqrt(y.at<double>(i, j)*y.at<double>(i, j) + x.at<double>(i, j)*x.at<double>(i, j));
			}
		}
	}
	return;	
}

void convertPolarToCart(Mat &mag, Mat &ang, Mat &x, Mat &y){
	//x(I) = mag(I) * cos(angle(I))
	//y(I) = mag(I) * sin(angle(I))
	x.create(mag.dims, mag.size, mag.type());
	y.create(mag.dims, mag.size, mag.type());

	for(int i = 0; i < mag.rows; i++){
		for(int j = 0; j < mag.cols; j++){
			if(mag.depth() == CV_32F){ 
				x.at<float>(i, j) = mag.at<float>(i, j) * cos(ang.at<float>(i, j));
				y.at<float>(i, j) = mag.at<float>(i, j) * sin(ang.at<float>(i, j));
			}else{
				x.at<double>(i, j) = mag.at<double>(i, j) * cos(ang.at<double>(i, j));
				y.at<double>(i, j) = mag.at<double>(i, j) * sin(ang.at<double>(i, j));		
			}
		}
	}
	return;	
}

void exchangePhaseAndMagnitude(Mat &image, Mat &image2, Mat &result, Mat &result2){
	Mat padded;
	int m = getOptimalDFTSize(image.rows);//因為 dft 在 size 是 2, 3, 5 倍數會更快
	int n = getOptimalDFTSize(image.cols);
	cout << m << " " << n << endl;
	//(inputArray, OutputArray, top, bottom, left, right, borderTpe, scalar)
	copyMakeBorder(image, padded, 0, m - image.rows, 0, n - image.cols, BORDER_CONSTANT, Scalar::all(0));
	
	Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)}; //建立一個 planes[2] = {padded, zero}
	Mat planes2[] = {Mat_<float>(image2), Mat::zeros(image2.size(), CV_32F)};


	Mat complexI;
	Mat complex2;
	merge(planes, 2, complexI);// complexI = planes[2]
	merge(planes2, 2, complex2);

	dft(complexI, complexI); 
	dft(complex2, complex2);
	split(complexI, planes);
	split(complex2, planes2);

/*
	dft(complexI, complexI, DFT_INVERSE + DFT_REAL_OUTPUT);
	split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))

	//直接 show 會出事，因為 float 被 default 認知為 0~1，會乘上 255
	normalize(complexI, complexI, 0, 1, NORM_MINMAX); //(in, out, 正規化下界, 正規化上界, nor, 方式)
	complexI.convertTo(complexI, CV_8U); //(out, type)
	imshow("try Reverse1", complexI); */
	

	Mat tempMag;
	Mat tempAng;
	Mat tempAng2;
	Mat tempMag2;
//	magnitude(planes[0], planes[1], tempMag);// planes[0] = magnitude
	getMagnitude(planes[0], planes[1], tempMag);// planes[0] = magnitude
	getMagnitude(planes2[0], planes2[1], tempMag2);
//	phase(planes[0], planes[1], tempAng);
	getPhase(planes[0], planes[1], tempAng);
	getPhase(planes2[0], planes2[1], tempAng2);


	Mat tempX;
	Mat tempY;
	convertPolarToCart(tempMag, tempAng2, tempX, tempY);
//	polarToCart(tempMag, tempAng, tempX, tempY);
	convertPolarToCart(tempMag2, tempAng, planes2[0], planes2[1]);
	Mat tempResult[] = {tempX, tempY};
	merge(tempResult, 2, result);
	merge(planes2, 2, result2);
	//沒有 DFT_REAL 就會回傳實部虛部 2 channel 的; !!注意一定要 DFT_SCALE，看上課講義 dft 或 idft 總有一邊有除以項數
	dft(result, result, DFT_INVERSE + DFT_REAL_OUTPUT + DFT_SCALE); 
	dft(result2, result2, DFT_INVERSE + DFT_REAL_OUTPUT + DFT_SCALE); 

	normalize(result, result, 0, 255, NORM_MINMAX);//正規化，不然顏色會稍微怪一點
	normalize(result2, result2, 0, 255, NORM_MINMAX);
	result.convertTo(result, CV_8U);
	result2.convertTo(result2, CV_8U);
}


int main(int argc, char** argv )
{
	image = imread( "Fig0424(a)(rectangle).tif", 0);//讀圖
								//(檔案名稱, flag < 0原圖; flag=0 灰階; flag>0 BGR) 
	image2 = imread("Fig0427(a)(woman).tif", 0);

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

	Mat image3, image4, result3, result4;
//	image3 = imread("DoW-512.png", 0);
//	image4 = imread("DoW2-512.png", 0);
	image3 = imread("sponge2.png", 0);
	image4 = imread("spongebob.png", 0);

	imshow("Q2 imag1", image3);
	imshow("Q2 imag2", image4);
	exchangePhaseAndMagnitude(image3, image4, result3, result4);
	imshow("result1", result3);
	imshow("result2", result4);


	waitKey(0);
	return 0;
}
