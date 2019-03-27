#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Mat image; //Mat->Matrix 宣告一個圖片變數
Mat result1;
Mat cvResult1;
Mat result2;
Mat cvResult2;

void mySharpen(){
	for(int i = 0; i < image.rows; i++){
		for(int j = 0; j < image.cols; j++){
			if(i == 0 || i == image.rows - 1 || j == 0 || j == image.cols - 1){
				result1.at<Vec3b>(i, j)[0] = 0;
				result1.at<Vec3b>(i, j)[1] = 0;
				result1.at<Vec3b>(i, j)[2] = 0;
			}else{
				int temp;
				for(int k = 0; k < 3; k++){
					temp = 5 * int(image.at<Vec3b>(i, j)[k]);
					temp -= image.at<Vec3b>(i-1, j)[k];
					temp -= image.at<Vec3b>(i, j-1)[k];
					temp -= image.at<Vec3b>(i+1, j)[k];
					temp -= image.at<Vec3b>(i, j+1)[k];
					if(temp < 0){
						result1.at<Vec3b>(i, j)[k] = 0;
					}else if(temp > 255){
						result1.at<Vec3b>(i, j)[k] = 255;
					}else{
						result1.at<Vec3b>(i, j)[k] = temp;
					}
				}
			}
			//cout << image.at<Vec3b>(i, j);
		}
		//cout << endl;
	}
}

void cvSharpen(){
	Mat kernel(3, 3, CV_32F, Scalar(0));
	kernel.at<float>(1, 1) = 5.0;
	kernel.at<float>(1, 0) = -1.0;
	kernel.at<float>(0, 1) = -1.0;
	kernel.at<float>(1, 2) = -1.0;
	kernel.at<float>(2, 1) = -1.0;
	filter2D(image, cvResult1, image.depth(), kernel);
}

void myGaussianBlur(){
	for(int i = 0; i < image.rows; i++){
		for(int j = 0; j < image.cols; j++){
			if(i == 0 || i == image.rows - 1 || j == 0 || j == image.cols - 1){
				result2.at<Vec3b>(i, j)[0] = 0;
				result2.at<Vec3b>(i, j)[1] = 0;
				result2.at<Vec3b>(i, j)[2] = 0;
			}else{
				int temp;
				for(int k = 0; k < 3; k++){
					temp = image.at<Vec3b>(i-1, j-1)[k];
					temp += image.at<Vec3b>(i-1, j+1)[k];
					temp += image.at<Vec3b>(i+1, j-1)[k];
					temp += image.at<Vec3b>(i+1, j+1)[k];

					temp += 2 * image.at<Vec3b>(i-1, j)[k];
					temp += 2 * image.at<Vec3b>(i, j-1)[k];
					temp += 2 * image.at<Vec3b>(i+1, j)[k];
					temp += 2 * image.at<Vec3b>(i, j+1)[k];
					
					temp += 4 * image.at<Vec3b>(i, j)[k];
					temp /= 16;	
					
					if(temp < 0){
						result2.at<Vec3b>(i, j)[k] = 0;
					}else if(temp > 255){
						result2.at<Vec3b>(i, j)[k] = 255;
					}else{
						result2.at<Vec3b>(i, j)[k] = temp;
					}
				}
			}
		}
	}
}


int main(int argc, char** argv )
{
	if ( argc != 2 )
	{
		printf("usage: DisplayImage.out <Image_Path>\n");
		return -1;
	}

	image = imread( argv[1], 1 );//讀圖
								//(檔案名稱, flag < 0原圖; flag=0 灰階; flag>0 BGR) 
	if ( !image.data )
	{
		printf("No image data \n");
		return -1;
	}

	cout << image.rows << endl;
	cout << image.cols << endl;
	cout << image.size() << endl;
	cout << image.depth() << endl;//0~4
	cout << image.type() << endl;
	cout << image.channels() << endl;

/*	for(int i = 0; i < image.rows; i++){
		for(int j = 0; j < image.cols; j++){
			cout << image.at<Vec3b>(i, j);
		}
		cout << endl;
	}*/

	result1.create(512, 512, CV_8UC3);
	cvResult1.create(512, 512, CV_8UC3);
	result2.create(512, 512, CV_8UC3);
	cvResult2.create(512, 512, CV_8UC3);

	mySharpen();
	cvSharpen();
	myGaussianBlur();
	GaussianBlur(image, cvResult2, Size(3, 3), 0, 0);//後兩個, x 與 y 方向標準差	

	namedWindow("Display Image", WINDOW_AUTOSIZE ); //創建一個 window 並給名稱
	namedWindow("Sharpen Image", WINDOW_AUTOSIZE ); 
	namedWindow("cvSharp Image", WINDOW_AUTOSIZE ); 
	namedWindow("Blur Image", WINDOW_AUTOSIZE ); 
	namedWindow("cvBlur Image", WINDOW_AUTOSIZE ); 

	imshow("Display Image", image); //給定一個 window，並給要顯示的圖片
	imshow("Sharpen Image", result1);
	imshow("cvSharp Image", cvResult1);
	imshow("Blur Image", result2);
	imshow("cvBlur Image", cvResult2);

	waitKey(0);
	return 0;
}
