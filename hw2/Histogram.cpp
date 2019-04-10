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


void Question1(){
	/*** Question1 ***/
	HistogramEqualizer(image, result1);
	equalizeHist(image, cvResult1);
	Mat Q1diff;
	Mat mask;
//	subtract(result1, cvResult1, Q1diff, mask, -1); //src1, src2, dst, mask, dtype
	Q1diff = result1 - cvResult1;

	namedWindow("result1", WINDOW_AUTOSIZE);
	namedWindow("cvResult1", WINDOW_AUTOSIZE);
	namedWindow("Q1diff", WINDOW_AUTOSIZE);
	imshow("result1", result1);
	imshow("cvResult1", cvResult1);
	imshow("Q1diff", Q1diff);
}

void convertToHSV(Mat &src, Mat &dst){
	dst.create(src.size(), src.type());

	for(int i = 0; i < src.cols; i++){ //!! cols & rows 相反
		for(int j = 0; j < src.rows; j++){
			int h = 0, s = 0, v = 0;
			int min; //max = v
			int b = src.at<Vec3b>(i, j)[0];//b
			int g = src.at<Vec3b>(i, j)[1];//g
			int r = src.at<Vec3b>(i, j)[2];//r

			v = b>g?b:g;
			v = v>r?v:r;
			min = b>g?g:b;
			min = min>r?r:min;
			
			if(v == 0){
				s = 0;
			}else{
				s = (v - min) * 255 / v; //本來 s 0~1 先乘 255
			}

//			cout << "inHSV:" << r << " " << g << " " << b;
			if(v == 0){
				h = 0;
			}else if(v == r){
				h = 60*(g-b)/(v - min);
			}else if(v == g){ 
				h = 120 + 60*(b-r)/(v - min);
			}else{
				h = 240 + 60*(r-g)/(v - min);
			}
			if(h != 0){
				h %= 360;
				h += 360;
				h %= 360;
			}
//			cout << " " << h << endl;
			h /=2 ;

			dst.at<Vec3b>(i, j)[0] = h;
			dst.at<Vec3b>(i, j)[1] = s;
			dst.at<Vec3b>(i, j)[2] = v;
		}
	}
}

void hsvToBgr(Mat &src, Mat &dst){
	for(int i = 0; i < src.cols; i++){
		for(int j = 0; j < src.rows; j++){		
			int h = src.at<Vec3b>(i, j)[0];//0~180
			double s = src.at<Vec3b>(i, j)[1];//0~255
			double v = src.at<Vec3b>(i, j)[2];//0~255
			h *= 2;
			s /= 255;
			v /= 255;
			int hi = (h/60) % 6;
		/*	
			int f = h/60 - hi;
		//	int p = v * (1 - s); //v, s乘過 255 
			int p = v - v*s/255;
		//	int q = v * (1 - f * s);
			int q = v - v*f*s/255;
		//	int t = v * (1 - (1 - f) * s); //1 - s + sf
			int t = v - v*s/255 + v*s*f/255;*/

			double f = (double)h/60 - hi;
			//https://zh.wikipedia.org/wiki/HSL%E5%92%8CHSV%E8%89%B2%E5%BD%A9%E7%A9%BA%E9%97%B4
			//https://stackoverflow.com/questions/9695329/c-how-to-round-a-double-to-an-int
			int p = (int)(v * (1 - s) * 255 + 0.5); //v, s乘過 255 
			int q = (int)(v * (1 - f * s) * 255 + 0.5);
			int t = (int)(v * (1 - (1 - f) * s) * 255 + 0.5); //1 - s + sf

			v *= 255;

			int r, g, b;

			switch(hi){
				case 0:
					r = v;
					g = t;
					b = p;
					break;
				case 1:
					r = q;
					g = v;
					b = p;
					break;
				case 2:
					r = p;
					g = v;
					b = t;
					break;
				case 3:
					r = p;
					g = q;
					b = v;
					break;
				case 4:
					r = t;
					g = p;
					b = v;
					break;
				case 5:
					r = v;
					g = p;
					b = q;
					break;
			}

			dst.at<Vec3b>(i, j)[0] = b;
			dst.at<Vec3b>(i, j)[1] = g;
			dst.at<Vec3b>(i, j)[2] = r;
		}
	}
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
	
	namedWindow("Display Image2", WINDOW_AUTOSIZE);
	namedWindow("result2_1", WINDOW_AUTOSIZE);
	namedWindow("cvResult2_1", WINDOW_AUTOSIZE);
	imshow("Display Image2", image2);
	imshow("result2_1", result2_1);
	imshow("cvResult2_1", cvResult2_1);
	namedWindow("Q2_1diff", WINDOW_AUTOSIZE);
	imshow("Q2_1diff", Q2_1diff);// */



	/*** 2-2 HSV ***/
	//opencv hsv 問題
	//https://stackoverflow.com/questions/50449602/wrong-hsv-conversion-from-rgb
	//https://stackoverflow.com/questions/43307959/how-to-get-hsv-and-lab-color-space/43319977#43319977
	vector<Mat> hsv(3);
	//myHSV
	convertToHSV(image2, result2_2);
//	for(int i = 0; i < 3; i++){
//		cout << (int)image2.at<Vec3b>(0, 0)[i] << " ";
//	}cout << endl;

//	for(int i = 0; i < 3; i++){
//		cout << (int)result2_2.at<Vec3b>(0, 0)[i] << " ";
//	}cout << endl;
	split(result2_2, hsv); 
	temp[0] = hsv[0];
	temp[1] = hsv[1];
	HistogramEqualizer(hsv[2], temp[2]);
	merge(temp, result2_2);
	hsvToBgr(result2_2, result2_2);
//	cvtColor(result2_2, result2_2, COLOR_HSV2BGR);


	//opencv
	cvtColor(image2, cvResult2_2, COLOR_BGR2HSV);
//	for(int i = 0; i < 3; i++){
//		cout << (int)cvResult2_2.at<Vec3b>(0, 0)[i] << " ";
//	}cout << endl;
	split(cvResult2_2, hsv);
	temp[0] = hsv[0];
	temp[1] = hsv[1];
	equalizeHist(hsv[2], temp[2]);
	merge(temp, cvResult2_2);
	cout << result2_2.type() << endl;
	cvtColor(cvResult2_2, cvResult2_2, COLOR_HSV2BGR);

	namedWindow("result2_2", WINDOW_AUTOSIZE);
	imshow("result2_2", result2_2);
	namedWindow("cvResult2_2", WINDOW_AUTOSIZE);
	imshow("cvResult2_2", cvResult2_2);

	Mat Q2_2diff = result2_2 - cvResult2_2;
	namedWindow("Q2_2diff", WINDOW_AUTOSIZE);
	imshow("Q2_2diff", Q2_2diff);
}


void convertToYCrCb(Mat &src, Mat &dst){
	dst.create(src.size(), src.type());

	for(int i = 0; i < src.cols; i++){ 
		for(int j = 0; j < src.rows; j++){
			double Y, Cr, Cb;
			int delta = 128;
			double b = src.at<Vec3b>(i, j)[0];
			double g = src.at<Vec3b>(i, j)[1];
			double r = src.at<Vec3b>(i, j)[2];

			Y = 0.299*r + 0.587*g + 0.114*b;
			Cr = (r-Y)*0.713 + delta;
			Cb = (b-Y)*0.564 + delta;

			dst.at<Vec3b>(i, j)[0] = (int)(Y+0.5);
			dst.at<Vec3b>(i, j)[1] = (int)(Cr+0.5);
			dst.at<Vec3b>(i, j)[2] = (int)(Cb+0.5);
		}
	}
}

void YCrCbToBgr(Mat &src, Mat &dst){
	for(int i = 0; i < src.cols; i++){ 
		for(int j = 0; j < src.rows; j++){
			int r, g, b;
			int delta = 128;
			double Y = src.at<Vec3b>(i, j)[0];
			double Cr = src.at<Vec3b>(i, j)[1];
			double Cb = src.at<Vec3b>(i, j)[2];

			r = Y + 1.403*(Cr - delta) + 0.5;
			g = Y - 0.714*(Cr - delta) - 0.344*(Cb - delta) + 0.5;
			b = Y + 1.773*(Cb - delta) + 0.5;
			if(r > 255) r = 255;
			if(r < 0) r = 0;
			if(g > 255) g = 255;
			if(g < 0) g = 0;
			if(b > 255) b = 255;
			if(b < 0) b = 0;
			dst.at<Vec3b>(i, j)[0] = b;
			dst.at<Vec3b>(i, j)[1] = g;
			dst.at<Vec3b>(i, j)[2] = r;
		}
	}
}

void Question2_3(){
	/*** YCrCb ***/
	//https://docs.opencv.org/4.1.0/de/d25/imgproc_color_conversions.html#color_convert_rgb_hls
	convertToYCrCb(image2, result2_3);
/*	for(int i = 0; i < 3; i++){
		cout << (int)image2.at<Vec3b>(0, 0)[i] <<" ";
	}cout << endl;
	for(int i = 0; i < 3; i++){
		cout << (int)result2_3.at<Vec3b>(0, 0)[i] <<" ";
	}cout << endl;*/
	vector<Mat> YCrCb(3);
	split(result2_3, YCrCb);
	vector<Mat> temp(YCrCb.begin(), YCrCb.end());
	HistogramEqualizer(YCrCb[0], temp[0]);
	temp[1] = YCrCb[1];
	temp[2] = YCrCb[2];
	merge(temp, result2_3);

	YCrCbToBgr(result2_3, result2_3);
//	cvtColor(result2_3, result2_3, COLOR_YCrCb2BGR);
	namedWindow("result2_3", WINDOW_AUTOSIZE);
	imshow("result2_3", result2_3);

	//opencv
	cvtColor(image2, cvResult2_3, COLOR_BGR2YCrCb);
/*	for(int i = 0; i < 3; i++){
		cout << (int)cvResult2_3.at<Vec3b>(0, 0)[i] <<" ";
	}cout << endl;*/
	split(cvResult2_3, YCrCb);
	equalizeHist(YCrCb[0], temp[0]);
	temp[1] = YCrCb[1];
	temp[2] = YCrCb[2];
	merge(temp, cvResult2_3);

	cvtColor(cvResult2_3, cvResult2_3, COLOR_YCrCb2BGR);
/*	for(int i = 0; i < 3; i++){
		cout << (int)result2_3.at<Vec3b>(0, 0)[i] <<" ";
	}cout << endl;
	for(int i = 0; i < 3; i++){
		cout << (int)cvResult2_3.at<Vec3b>(0, 0)[i] <<" ";
	}cout << endl;*/
	namedWindow("cvResult2_3", WINDOW_AUTOSIZE);
	imshow("cvResult2_3", cvResult2_3);

	Mat Q2_3diff;
	absdiff(result2_3, cvResult2_3, Q2_3diff);
//	Mat Q2_3diff = result2_3 - cvResult2_3;
	namedWindow("Q2_3diff", WINDOW_AUTOSIZE);
	imshow("Q2_3diff", Q2_3diff);

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
	result2_1.create(image2.size(), CV_8UC3);
	cvResult2_1.create(image2.size(), CV_8UC3);

	namedWindow("Display Image", WINDOW_AUTOSIZE);
	imshow("Display Image", image);
//	textbookExample();
	Question1();// */

	namedWindow("Display Image2", WINDOW_AUTOSIZE);
	imshow("Display Image2", image2);
	Question2();
	Question2_3();


	waitKey(0);
	return 0;
}
