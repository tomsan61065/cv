#include <iostream>
#include <opencv2/opencv.hpp>

#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

int main(){
//	cvNamedWindow("show Image1", CV_WINDOW_AUTOSIZE);
	Mat image1;
	Mat image2;

	image1 = imread("./luna.jpg");
	if(image1.empty()){
		printf("Error loading image1\n");
		return 0;
	}

	return 0;
}
