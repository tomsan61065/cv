#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Mat image; //Mat->Matrix 宣告一個圖片變數
Mat result1;
Mat result2;
Mat result3;

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

void verticalFlip(){
	for(int j = 0; j < image.cols; j++){
		//cout << image.at<Vec3b>(i, j);
		for(int i = 0; i < image.rows; i++){
			result1.at<Vec3b>(image.rows - i - 1, j)[0] = image.at<Vec3b>(i, j)[0];
			result1.at<Vec3b>(image.rows - i - 1, j)[1] = image.at<Vec3b>(i, j)[1];
			result1.at<Vec3b>(image.rows - i - 1, j)[2] = image.at<Vec3b>(i, j)[2];
		}
	}
}

void horizontalFlip(){
	for(int i = 0; i < image.rows; i++){
		for(int j = 0; j < image.cols; j++){
			result2.at<Vec3b>(i, image.cols - j - 1)[0] = image.at<Vec3b>(i, j)[0];
			result2.at<Vec3b>(i, image.cols - j - 1)[1] = image.at<Vec3b>(i, j)[1];
			result2.at<Vec3b>(i, image.cols - j - 1)[2] = image.at<Vec3b>(i, j)[2];
		}
	}
}

void diagonalFlip(){
	for(int i = 0; i < image.rows; i++){
		for(int j = 0; j < image.cols; j++){
			result3.at<Vec3b>(j, i)[0] = image.at<Vec3b>(i, j)[0];
			result3.at<Vec3b>(j, i)[1] = image.at<Vec3b>(i, j)[1];
			result3.at<Vec3b>(j, i)[2] = image.at<Vec3b>(i, j)[2];
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
	result2.create(512, 512, CV_8UC3);
	result3.create(512, 512, CV_8UC3);

	verticalFlip();	
	horizontalFlip();
	diagonalFlip();

	namedWindow("Display Image", WINDOW_AUTOSIZE ); //創建一個 window 並給名稱

	imshow("Display Image", image); //給定一個 window，並給要顯示的圖片
	imshow("verticalFlip", result1);
	imshow("horizontalFlip", result2);
	imshow("diagonalFlip", result3);
	/*
	imwrite("verticalFlip.png", result1);
	imwrite("horizontalFlip.png", result2);
	imwrite("diagonalFlip.png", result3);
	*/
	waitKey(0);
	return 0;
}
