// Transformations.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
using namespace cv;

int scale = 0;
int type = 0;
const int max_scale = 4;
const int max_type = 2;

Mat src = imread("google.jpg", IMREAD_COLOR);
Mat dst, dst1;

void scaling(int, void*);
void transforming(int, void*);

int main()
{
	if (src.empty())
	{
		std::cout << "Cannot read image: " << std::endl;
		return -1;
	}

	namedWindow("Scaling", WINDOW_AUTOSIZE);
	createTrackbar("Scale", "Scaling", &scale, max_scale, scaling);
	createTrackbar("(type)L/C/A", "Scaling", &type, max_type, scaling);

	namedWindow("Transform", WINDOW_AUTOSIZE);
	moveWindow("Transform", src.cols, 0);
	createTrackbar("Rot/Warp", "Transform", &type, 1, transforming);
	createTrackbar("amt", "Transform", &scale, max_scale, transforming);
	waitKey(0);
	return 0;
}

void scaling(int, void*) {
	if (type == 0) {
		type = INTER_LINEAR;
	}
	else if (type == 1) {
		type = INTER_CUBIC;
	}
	else if (type == 2) {
		type = INTER_AREA;
	}
	double fx = (double(scale) + 1) * 0.5;
	resize(src, dst, Size(fx*src.cols, fx*src.rows), fx, fx, type);
	imshow("Scaling", dst);
}

void transforming(int, void*) {
	if (type == 0) {
		Point2f pc(src.cols / 2, src.rows / 2);
		double angle = scale * (-45);
		Mat rot = getRotationMatrix2D(pc, angle, 1);
		warpAffine(src, dst, rot, src.size());
	}
	else if (type == 1) {
		Point2f srcTri[3];
		srcTri[0] = Point2f(0.f, 0.f);
		srcTri[1] = Point2f(src.cols - 1.f, 0.f);
		srcTri[2] = Point2f(0.f, src.rows - 1.f);
		Point2f dstTri[3];
		dstTri[0] = Point2f(0.f, src.rows*0.33f);
		dstTri[1] = Point2f(src.cols*(0.75f - scale*0.1f), src.rows*(0.25f + scale*0.1f));
		dstTri[2] = Point2f(src.cols*0.15f, src.rows*0.7f);
		Mat warp_mat = getAffineTransform(srcTri, dstTri);
		dst = Mat::zeros(src.rows, src.cols, src.type());
		warpAffine(src, dst, warp_mat, dst.size());
	}
	imshow("Transform", dst);
}