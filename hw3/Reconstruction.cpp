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


static const float atan2_p1 = 0.9997878412794807f*(float)(180/CV_PI);
static const float atan2_p3 = -0.3258083974640975f*(float)(180/CV_PI);
static const float atan2_p5 = 0.1555786518463281f*(float)(180/CV_PI);
static const float atan2_p7 = -0.04432655554792128f*(float)(180/CV_PI);

float myFastArctan( float y, float x )
{
	//a^2 = b^2 + c^2 - 2bc * cosA
	float ax = abs(x), ay = abs(y);//首先不分象限，求得一個銳角角度
	float a, c, c2;
	if( ax >= ay ){
		c = ay/(ax + (float)DBL_EPSILON);
		c2 = c*c;
		a = (((atan2_p7*c2 + atan2_p5)*c2 + atan2_p3)*c2 + atan2_p1)*c;
	}else{
		c = ax/(ay + (float)DBL_EPSILON);
		c2 = c*c;
		a = 90.f - (((atan2_p7*c2 + atan2_p5)*c2 + atan2_p3)*c2 + atan2_p1)*c;
	}
	if( x < 0 )//求出銳角，根據 x,y 的正負确定向量的方向，即角度。
		a = 180.f - a;
	if( y < 0 )
		a = 360.f - a;
	return a;
}


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

void Quesiton1(){
	Mat padded;
	int m = getOptimalDFTSize(image.rows);//因為 dft 在 size 是 2, 3, 5 倍數會更快
	int n = getOptimalDFTSize(image.cols);
	cout << m << " " << n << endl;
	//(inputArray, OutputArray, top, bottom, left, right, borderTpe, scalar)
	copyMakeBorder(image, padded, 0, m - image.rows, 0, n - image.cols, BORDER_CONSTANT, Scalar::all(0));
	
	Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)}; //建立一個 planes[2] = {padded, zero}

	Mat complexI;
	merge(planes, 2, complexI);// complexI = planes[2]

	dft(complexI, complexI); 
	split(complexI, planes);
/*
	dft(complexI, complexI, DFT_INVERSE + DFT_REAL_OUTPUT);
	split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))

	//直接 show 會出事，因為 float 被 default 認知為 0~1，會乘上 255
	//normalize(complexI, complexI, 0, 1, NORM_MINMAX); //(in, out, 正規化下界, 正規化上界, nor, 方式)
	complexI.convertTo(complexI, CV_8U); //(out, type)
	imshow("try Reverse1", complexI); */
	

	Mat tempMag;
	Mat tempAng;
	magnitude(planes[0], planes[1], tempMag);// planes[0] = magnitude
//	getMagnitude(planes[0], planes[1], tempMag);// planes[0] = magnitude
	phase(planes[0], planes[1], tempAng);
//	getPhase(planes[0], planes[1], tempAng);
	tempAng = tempAng(Rect(0, 0, tempAng.cols & -2, tempAng.rows & -2));
	tempMag = tempMag(Rect(0, 0, tempMag.cols & -2, tempMag.rows & -2));

	Mat tempX;
	Mat tempY;
//	convertPolarToCart(tempMag, tempAng, tempX, tempY);
	polarToCart(tempMag, tempAng, tempX, tempY);
	Mat tempResult[] = {tempX, tempY};
	Mat tempReverse;
	merge(tempResult, 2, tempReverse);
	dft(tempReverse, tempReverse, DFT_INVERSE  + DFT_REAL_OUTPUT); //沒有 DFT_REAL 就會回傳實部虛部 2 channel 的
	tempReverse.convertTo(tempReverse, CV_8U);
	cout << tempReverse.channels() << endl;
	imshow("try Reverse", tempReverse);
	return;




	Mat magI = planes[0];
	
	magI += Scalar::all(1);                    // switch to logarithmic scale
	log(magI, magI);

	// crop the spectrum, if it has an odd number of rows or columns
	magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));
	// rearrange the quadrants of Fourier image  so that the origin is at the image center
	int cx = magI.cols/2;
	int cy = magI.rows/2;
	Mat q0(magI, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
	Mat q1(magI, Rect(cx, 0, cx, cy));  // Top-Right
	Mat q2(magI, Rect(0, cy, cx, cy));  // Bottom-Left
	Mat q3(magI, Rect(cx, cy, cx, cy)); // Bottom-Right
	Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);
	q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
	q2.copyTo(q1);
	tmp.copyTo(q2);

	normalize(magI, magI, 0, 1, NORM_MINMAX); // Transform the matrix with float values into a
	// viewable image form (float between values 0 and 1).
	imshow("Input Image"       , image);    // Show the result
	imshow("spectrum magnitude", magI);
	waitKey(0);
}


void Quesiton2(){
	
}

int main(int argc, char** argv )
{
	image = imread( "Fig0424(a)(rectangle).tif", 0);//讀圖
								//(檔案名稱, flag < 0原圖; flag=0 灰階; flag>0 BGR) 
	image2 = imread("Fig0427(a)(woman).tif", 0);

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

	namedWindow("Display Image", WINDOW_AUTOSIZE);
	imshow("Display Image", image);

	namedWindow("Display Image2", WINDOW_AUTOSIZE);
	imshow("Display Image2", image2);
	
	Quesiton1();

	waitKey(0);
	return 0;
}
