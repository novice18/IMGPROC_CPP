// edge_detection.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

Mat image = imread("billie.jpg", 0);
Mat src, dst, detected_edges;

int low_thresh = 0;
const int max_low_thresh = 100;
int type = 0;

void CannyThreshold(int, void*);
void sobel_laplace(int, void*);

int main()
{
	if (image.empty()) {
		cout << "Could not find image\n";
		return -1;
	}

	bilateralFilter(image, src, 3, 1.5, 1.5);
	dst.create(src.size(), src.type());

	namedWindow("Sobel-Laplace", WINDOW_AUTOSIZE);
	createTrackbar("S/L", "Sobel-Laplace", &type, 1, sobel_laplace);
	
	namedWindow("Canny edge", WINDOW_AUTOSIZE);
	moveWindow("Canny edge", src.cols, 0);
	createTrackbar("Thresh", "Canny edge", &low_thresh, max_low_thresh, CannyThreshold);

	sobel_laplace(0, 0);
	CannyThreshold(0, 0);
	
	waitKey(0);
	return 0;
}

void CannyThreshold(int, void*) {

	Canny(src, detected_edges, low_thresh, low_thresh*3);

	dst = Scalar::all(0);
	src.copyTo(dst, detected_edges);
	imshow("Canny edge", dst);
}

void sobel_laplace(int, void*) {
	if (type == 0) {
		Mat grad_x, grad_y;
		Mat abs_grad_x, abs_grad_y;

		Sobel(src, grad_x, src.depth(), 1, 0);
		Sobel(src, grad_y, src.depth(), 0, 1);
		convertScaleAbs(grad_x, abs_grad_x);
		convertScaleAbs(grad_y, abs_grad_y);

		addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, dst);
	}
	else {
		//Mat abs_dst;
		Laplacian(src, dst, src.depth(), 3);

		convertScaleAbs(dst, dst);
	}
	imshow("Sobel-Laplace", dst);
}
