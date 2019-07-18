// DisplayImage.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

Mat robo = imread("robo.png", 0);
Mat robo_noisy = imread("robo_noisy.png", 0);
Mat dst1, dst2;

int type1 = 0;
int type2 = 0;
int kernel_size1 = 0;
int kernel_size2 = 0;
int it = 1;
int op1 = 0;
int op2 = 0;

const int max_type = 2;
const int max_kernel_size = 9;
const int max_it = 5;

void erode_dilate(int, void*);
void open_close(int, void*);

int main() {
	
	if (robo.empty()) {
		cout << "Could not open or find image\n";
		return -1;
	}
	if (robo_noisy.empty()) {
		cout << "Could not open or find image\n";
		return -1;
	}

	namedWindow("Erosion & Dilation", WINDOW_AUTOSIZE);
	moveWindow("Erosion & Dilation", (robo.cols) / 2, 0);
	namedWindow("Opening & Closing", WINDOW_AUTOSIZE);
	moveWindow("Opening & Closing", 2 * robo.cols, 0);

	//trackbars for erosion & dilation
	createTrackbar("Erode/dilate", "Erosion & Dilation", &op1, 1, erode_dilate);
	createTrackbar("R/C/E", "Erosion & Dilation", &type1, max_type, erode_dilate);
	createTrackbar("Kernel", "Erosion & Dilation", &kernel_size1, max_kernel_size, erode_dilate);
	createTrackbar("Iterations", "Erosion & Dilation", &it, max_it, erode_dilate);

	//trackbars for opening & closing
	createTrackbar("Open/Close", "Opening & Closing", &op2, 1, open_close);
	createTrackbar("R/C/E", "Opening & Closing", &type2, max_type, open_close);
	createTrackbar("Kernel", "Opening & Closing", &kernel_size2, max_kernel_size, open_close);

	erode_dilate(0, 0);

	waitKey(0);
	return 0;
}

void erode_dilate(int, void*) {
	if (type1 == 0) {
		type1 = MORPH_RECT;
	}
	else if (type1 == 1) {
		type1 = MORPH_CROSS;
	}
	else if (type1 == 2) {
		type1 = MORPH_ELLIPSE;
	}

	Mat element = getStructuringElement(type1, Size(kernel_size1+1, kernel_size1+1), Point(-1, -1));
	if (op1 == 0) {
		erode(robo, dst1, element, Point(-1, -1), it);
	}
	else if (op1 == 1) {
		dilate(robo, dst1, element, Point(-1, -1), it);
	}
	imshow("Erosion & Dilation", dst1);
}

void open_close(int, void*) {
	if (type2 == 0) {
		type2 = MORPH_RECT;
	}
	else if (type2 == 1) {
		type2 = MORPH_CROSS;
	}
	else if (type2 == 2) {
		type2 = MORPH_ELLIPSE;
	}

	Mat element = getStructuringElement(type2, Size(kernel_size2 + 1, kernel_size2 + 1), Point(-1, -1));
	if (op2 == 0) {
		morphologyEx(robo_noisy, dst2, MORPH_OPEN, element);
	}
	else if (op2 == 1) {
		morphologyEx(robo_noisy, dst2, MORPH_CLOSE, element);
	}
	imshow("Opening & Closing", dst2);
}