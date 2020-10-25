#include "pch.h"
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace cv;
using namespace std;

float minmumColor = 0;
float maximumColor = 0;

float measureBlur(Mat src) {
	Mat grad_x, grad_y;
	Mat grad;
	Mat abs_grad_x, abs_grad_y;

	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;

	Sobel(src, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_x, abs_grad_x);

	Sobel(src, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_y, abs_grad_y);


	convertScaleAbs(grad_x, abs_grad_x);
	convertScaleAbs(grad_y, abs_grad_y);

	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);

	//imshow("edges", grad);

	int blurredPixels = 0;

	for (int i = 0; i < grad.cols; i++) {
		for (int j = 0; j < grad.rows; j++) {
			int valueAt = (int)grad.at<uchar>(j, i);

			if (valueAt < 20) {
				blurredPixels++;

			}
		}
	}

	float bluredPixelsPercentage = ((float)blurredPixels / (grad.cols*grad.rows)) * 100;
	cout << "Blurness percentage: " << bluredPixelsPercentage << "\n";
	return bluredPixelsPercentage;


}

Mat unsharpImage(Mat src) {

	Mat smoothed = src.clone();
	Mat result = Mat::zeros(src.rows, src.cols, src.type());
	//cout << "In unsharp" << "\n";
	
	float percentageBlur = measureBlur(src);
	cout << "Checking if blur ..." << endl;
	if (percentageBlur < 60) {
		cout << "No Blurness found"<<"\n"; 
		return src;
	}
	cout << "Blurness found" << "\n";
	
	//smooth Image
	blur(src, smoothed, Size(9, 9));
	cout << "moothing Done" << "\n";
	
	Mat edge = Mat::zeros(src.rows, src.cols, src.type());
	
	//calculate edge	
	for (int i = 0; i < src.cols; i++) {
		for (int j = 0; j < src.rows; j++) {
			edge.at<char>(j, i) = src.at<char>(j, i) - smoothed.at<char>(j, i);			
		}
	}
	
	//unsharp
	for (int i = 0; i < src.cols; i++) {
		for (int j = 0; j < src.rows; j++) {
			int valueAt = (int)src.at<uchar>(j, i);
			if (valueAt <= 240) {
				result.at<char>(j, i) = src.at<char>(j, i) + 0.7*(edge.at<char>(j, i));				
			}
			else {
				result.at<char>(j, i) = src.at<char>(j, i);
			}
				
		}
	}
	cout << "unsharp done" << "\n";
	
	return result;
	

}



float total = 0;
//calculate pixel`s Variance
float Vpp(Mat img) {
	float total = 0;
	float mean = 0;
	float variance = 0;
	for (int i = 0; i<img.rows; i++) {
		for (int j = 0; j<img.cols; j++) {
			total += img.at<float>(i, j);
		}
	}
	float m = img.cols*img.rows;
	mean = total / m;
	for (int i = 0; i<img.rows; i++) {
		for (int j = 0; j<img.cols; j++) {
			variance += pow((img.at<float>(i, j)) - mean, 2);
		}
	}

	return  variance;
}

// Output matrix of variance at each pexil
Mat MeasureVariance(Mat img) {
	Mat variance = Mat::zeros(img.rows, img.cols, CV_32FC1);
	for (int i = 0; i<img.rows; i++) {
		for (int j = 0; j<img.cols; j++) {
			if (i == 0) {
				if (j == 0) {
					Mat kernel = Mat(2, 2, CV_32F);
					kernel.at<float>(0, 0) = (float)img.at<uchar>(i, j);
					kernel.at<float>(0, 1) = (float)img.at<uchar>(i, j + 1);
					kernel.at<float>(1, 0) = (float)img.at<uchar>(i + 1, j);
					kernel.at<float>(1, 1) = (float)img.at<uchar>(i + 1, j);
					variance.at<float>(i, j) = Vpp(kernel);
				}
				else if (j<img.cols - 1) {
					Mat kernel = Mat(2, 3, CV_32F);
					kernel.at<float>(0, 0) = (float)img.at<uchar>(i, j - 1);
					kernel.at<float>(0, 1) = (float)img.at<uchar>(i, j);
					kernel.at<float>(0, 2) = (float)img.at<uchar>(i, j + 1);
					kernel.at<float>(1, 0) = (float)img.at<uchar>(i + 1, j - 1);
					kernel.at<float>(1, 1) = (float)img.at<uchar>(i + 1, j);
					kernel.at<float>(1, 2) = (float)img.at<uchar>(i + 1, j + 1);
					variance.at<float>(i, j) = Vpp(kernel);
				}
				else {
					Mat kernel = Mat(2, 2, CV_32F);
					kernel.at<float>(0, 0) = (float)img.at<uchar>(i, j - 1);
					kernel.at<float>(0, 1) = (float)img.at<uchar>(i, j);
					kernel.at<float>(1, 0) = (float)img.at<uchar>(i + 1, j - 1);
					kernel.at<float>(1, 1) = (float)img.at<uchar>(i + 1, j);
					variance.at<float>(i, j) = Vpp(kernel);
				}
			}
			else if (i>0 && i<img.rows - 1) {
				if (j == 0) {
					Mat kernel = Mat(3, 2, CV_32F);
					kernel.at<float>(0, 0) = (float)img.at<uchar>(i - 1, j);
					kernel.at<float>(0, 1) = (float)img.at<uchar>(i - 1, j + 1);
					kernel.at<float>(1, 0) = (float)img.at<uchar>(i, j);
					kernel.at<float>(1, 1) = (float)img.at<uchar>(i, j + 1);
					kernel.at<float>(2, 0) = (float)img.at<uchar>(i + 1, j);
					kernel.at<float>(2, 1) = (float)img.at<uchar>(i + 1, j + 1);
					variance.at<float>(i, j) = Vpp(kernel);
				}
				else if (j<img.cols - 1) {
					Mat kernel = Mat(3, 3, CV_32F);
					kernel.at<float>(0, 0) = (float)img.at<uchar>(i - 1, j - 1);
					kernel.at<float>(0, 1) = (float)img.at<uchar>(i - 1, j);
					kernel.at<float>(0, 2) = (float)img.at<uchar>(i - 1, j + 1);
					kernel.at<float>(1, 0) = (float)img.at<uchar>(i, j - 1);
					kernel.at<float>(1, 1) = (float)img.at<uchar>(i, j);
					kernel.at<float>(1, 2) = (float)img.at<uchar>(i, j + 1);
					kernel.at<float>(2, 0) = (float)img.at<uchar>(i + 1, j - 1);
					kernel.at<float>(2, 1) = (float)img.at<uchar>(i + 1, j);
					kernel.at<float>(2, 2) = (float)img.at<uchar>(i + 1, j + 1);
					variance.at<float>(i, j) = Vpp(kernel);
				}
				else {
					Mat kernel = Mat(3, 2, CV_32F);
					kernel.at<float>(0, 0) = (float)img.at<uchar>(i - 1, j - 1);
					kernel.at<float>(0, 1) = (float)img.at<uchar>(i - 1, j);
					kernel.at<float>(1, 0) = (float)img.at<uchar>(i, j - 1);
					kernel.at<float>(1, 1) = (float)img.at<uchar>(i, j);
					kernel.at<float>(2, 0) = (float)img.at<uchar>(i + 1, j - 1);
					kernel.at<float>(2, 1) = (float)img.at<uchar>(i + 1, j);
					variance.at<float>(i, j) = Vpp(kernel);
				}

			}
			else {
				if (j == 0) {
					Mat kernel = Mat(2, 2, CV_32F);
					kernel.at<float>(0, 0) = (float)img.at<uchar>(i - 1, j);
					kernel.at<float>(0, 1) = (float)img.at<uchar>(i - 1, j + 1);
					kernel.at<float>(1, 0) = (float)img.at<uchar>(i, j);
					kernel.at<float>(1, 1) = (float)img.at<uchar>(i, j + 1);
					variance.at<float>(i, j) = Vpp(kernel);

				}
				else if (j<img.cols - 1) {
					Mat kernel = Mat(2, 3, CV_32F);
					kernel.at<float>(0, 0) = (float)img.at<uchar>(i - 1, j - 1);
					kernel.at<float>(0, 1) = (float)img.at<uchar>(i - 1, j);
					kernel.at<float>(0, 2) = (float)img.at<uchar>(i - 1, j + 1);
					kernel.at<float>(1, 0) = (float)img.at<uchar>(i, j - 1);
					kernel.at<float>(1, 1) = (float)img.at<uchar>(i, j);
					kernel.at<float>(1, 2) = (float)img.at<uchar>(i, j + 1);
					variance.at<float>(i, j) = Vpp(kernel);

				}
				else {
					Mat kernel = Mat(2, 2, CV_32F);
					kernel.at<float>(0, 0) = (float)img.at<uchar>(i - 1, j - 1);
					kernel.at<float>(0, 1) = (float)img.at<uchar>(i - 1, j);
					kernel.at<float>(1, 0) = (float)img.at<uchar>(i, j - 1);
					kernel.at<float>(1, 1) = (float)img.at<uchar>(i, j);
					variance.at<float>(i, j) = Vpp(kernel);
				}
			}
		}

	}
	return variance;
}
//Calculate the percent of variance pixels greater than the threshhold
float percentofNoisePixels(int th, Mat var) {
	float p = 0;
	float total = 0;
	for (int i = 0; i<var.rows; i++) {
		for (int j = 0; j<var.cols; j++) {
			if (var.at<float>(i, j)>th)
				total++;
		}
	}
	p = (total) / (var.rows*var.cols);
	return p * 100;
}
//Correct Noise
Mat correctNoise(Mat img) {
	Mat variance = MeasureVariance(img);
	float noisePercent = percentofNoisePixels(1000, variance);
	cout << "Checking Noise ...." << "\n";
	cout << "Noise Percentage: " << noisePercent << "\n";
	if (noisePercent>55) {
		cout << "Noise detected" << endl;
		Mat imgCorrected = Mat(img.rows, img.cols, 0);
		medianBlur(img, imgCorrected, 5);
		return imgCorrected;
	}
	else {
		cout << " No Noise detected" << endl;
		return img;
	}

}

void histogram(Mat img) {
	int bins = 256;
	int hsize[] = { bins };
	float xranges[] = { 0, 256 };
	const float* ranges[] = { xranges };
	MatND hist;
	calcHist(&img, 1, 0, Mat(), hist, 1, hsize, ranges, true, false);

	int hist_w = 512;
	int hist_h = 400;
	int bin_w = cvRound((double)hist_w / bins);
	

	minmumColor = 0;
	maximumColor = 0;
	for (int h = 1; h < bins; h++) {
		float binVal = hist.at<float>(h);
		if (binVal > 0) {
			minmumColor = h;
			break;
		}
		//cout << " " << binVal;
	}
	cout <<"Minimum Color: " << minmumColor << "\n";

	for (int h = 255; h > 0; h--) {
		float binVal = hist.at<float>(h);
		if (binVal > 0) {
			maximumColor = h;
			break;
		}
		//cout << " " << binVal;
	}
	cout << "Maximum Color: "<< maximumColor << "\n";

	
}

//check color coverage
float percentOfCoverage(Mat img) {
	histogram(img);
	for (int i = 0; i<img.rows; i++) {
		for (int j = 0; j<img.cols; j++) {
			if ((float)img.at<uchar>(i, j)<minmumColor)
				minmumColor = (float)img.at<uchar>(i, j);
			if ((float)img.at<uchar>(i, j)>maximumColor)
				maximumColor = (float)img.at<uchar>(i, j);
		}
	}
	return (((maximumColor - minmumColor) + 1) / 256) * 100;
}
//check color collapsed correction

Mat correctColor(Mat img) {
	cout << "Checking Color Contrast ...." << "\n";
	float pC = percentOfCoverage(img);
	float collapsedColorValue = 100 - pC;
	cout << "Percentage: " << pC << endl;
	
	if (collapsedColorValue>0) {
		cout << "Image is color collapsed" << endl;
		Mat imgCorrected = Mat(img.rows, img.cols, 0);
		for (int i = 0; i < img.rows; i++) {
			for (int j = 0; j < img.cols; j++) {
				imgCorrected.at<uchar>(i, j) = ((int)img.at<uchar>(i, j) - minmumColor)*((255 - 0) / (maximumColor - minmumColor)) + 0;
			}
		}
		//medianBlur(img, imgCorrected, 5);
		return imgCorrected;
	}
	cout << "Image is not color collapsed" << endl;
	return img;
}


int main(int argc, char** argv) {
	Mat img2 = imread("imagesA2/2.jpg", 0);
	Mat img3 = imread("3.jpg", 0);
	Mat img4 = imread("imagesA2/4.jpg", 0);
	Mat img5 = imread("imagesA2/5.jpg", 0);
	Mat img6 = imread("imagesA2/6.jpg", 0);
	Mat img7 = imread("imagesA2/7.jpg", 0);
	Mat img8 = imread("imagesA2/8.jpg", 0);

	imshow("Original Image 2", img2);
	Mat correctedNoise2 = correctNoise(img2);
	imshow("Corrected Noise 2", correctedNoise2);
	Mat correctedBlur2 = unsharpImage(correctedNoise2);
	imshow("Corrected Blur 2", correctedBlur2);
	Mat correctedColor = correctColor(correctedBlur2);
	imshow("Result 2", correctedColor);

	imshow("Original Image 3", img3);
	Mat correctedNoise3 = correctNoise(img3);
	imshow("Corrected Noise 3", correctedNoise3);
	Mat correctedBlur3 = unsharpImage(correctedNoise3);
	imshow("Corrected Blur 3", correctedBlur3);
	Mat correctedColor3 = correctColor(correctedBlur3);
	imshow("Result 3", correctedColor3);

	imshow("Original Image 4", img4);
	Mat correctedNoise4 = correctNoise(img4);
	imshow("Corrected Noise 4", correctedNoise4);
	Mat correctedBlur4 = unsharpImage(correctedNoise4);
	imshow("Corrected Blur 4", correctedBlur4);
	Mat correctedColor4 = correctColor(correctedBlur4);
	imshow("Result 4", correctedColor4);

	imshow("Original Image 5", img5);
	Mat correctedNoise5 = correctNoise(img5);
	imshow("Corrected Noise 5", correctedNoise5);
	Mat correctedBlur5 = unsharpImage(correctedNoise5);
	imshow("Corrected Blur 5", correctedBlur5);
	Mat correctedColor5 = correctColor(correctedBlur5);
	imshow("Result 5", correctedColor5);

	
	imshow("Original Image 6", img6);
	Mat correctedNoise6 = correctNoise(img6);
	imshow("Corrected Noise 6", correctedNoise6);
	Mat correctedBlur6 = unsharpImage(correctedNoise6);
	imshow("Corrected Blur 6", correctedBlur6);
	Mat correctedColor6 = correctColor(correctedBlur6);
	imshow("Result 6", correctedColor6);

	imshow("Original Image 7", img7);
	Mat correctedNoise7 = correctNoise(img7);
	imshow("Corrected Noise 7", correctedNoise7);
	Mat correctedBlur7 = unsharpImage(correctedNoise7);
	imshow("Corrected Blur 7", correctedBlur7);
	Mat correctedColor7 = correctColor(correctedBlur7);
	imshow("Result 7", correctedColor7);

	imshow("Original Image 8", img8);
	Mat correctedNoise8 = correctNoise(img8);
	imshow("Corrected Noise 8", correctedNoise8);
	Mat correctedBlur8 = unsharpImage(correctedNoise8);
	imshow("Corrected Blur 8", correctedBlur8);
	Mat correctedColor8 = correctColor(correctedBlur8);
	imshow("Result 8", correctedColor8);
		
	waitKey();
	return 0;
}




