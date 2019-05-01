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


void getPhase(){ //cv phase()
	//angle(I) = atan2(y(I), x(I));
	
}

void getMagnitude(){
	//magnitude(I) = sqrt( Re(DFT(I))^2 + Im(DFT(I))^2 )i;	
	
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
	
	
	size s = getOptimalSize(image.size());



	waitKey(0);
	return 0;
}
