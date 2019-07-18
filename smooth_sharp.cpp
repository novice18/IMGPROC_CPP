// smooth_sharp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

Mat src = imread("train.jpg", 1);
Mat smooth_dst, sharp_dst;

int blur_type = 0;
int kernel_size = 0;
const int max_blur_type = 3;
const int max_kernel_size = 15;

void smooth(int, void*);
void sharp();

int main() {
	if (src.empty()) {
		cout << "Couldn't find image\n";
		return -1;
	}
	smooth_dst = src.clone();
	sharp_dst = src.clone();

	namedWindow("Smoothening", WINDOW_AUTOSIZE);
	createTrackbar("Av/G/M/B", "Smoothening", &blur_type, max_blur_type, smooth);
	createTrackbar("Kernel", "Smoothening", &kernel_size, max_kernel_size, smooth);

	namedWindow("Sharpening", WINDOW_AUTOSIZE);
	moveWindow("Sharpening", 2 * src.cols, 0);

	smooth(0, 0);
	sharp();

	waitKey(0);
	return 0;
}

void smooth(int, void*) {
	int n = 2 * kernel_size + 1;
	if (blur_type == 0) {
		blur(src, smooth_dst, Size(n, n), Point(-1, -1));
	}
	else if (blur_type == 1) {
		GaussianBlur(src, smooth_dst, Size(n, n), 0, 0);
	}
	else if (blur_type == 2) {
		medianBlur(src, smooth_dst, n);
	}
	else if (blur_type == 3) {
		bilateralFilter(src, smooth_dst, n, n / 2, n / 2);
	}

	imshow("Smoothening", smooth_dst);
}

void sharp() {
	Mat sharp_kernel = (Mat_<char>(3, 3) << 0, -1, 0,
										  -1, 5, -1,
										  0, -1, 0);
	//Mat unsharp_kernel = (Mat_<char>(5, 5) <<
	//	-1 / 256, -4 / 256, -6 / 256, -4 / 256, -1 / 256,
	//	-4 / 256, -16 / 256, -24 / 256, -16 / 256, -4 / 256,
	//	-6 / 256, -24 / 256, 476 / 256, -24 / 256, -6 / 256,
	//	-4 / 256, -16 / 256, -24 / 256, -16 / 256, -4 / 256,
	//	-1 / 256, -4 / 256, -6 / 256, -4 / 256, -1 / 256);

	filter2D(src, sharp_dst, src.depth(), sharp_kernel);
	
	imshow("Sharpening", sharp_dst);
}