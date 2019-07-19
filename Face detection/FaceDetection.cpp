// FaceDetection.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

Mat image;
int face_scale = 0;
int eye_scale = 0;
const int max_face_scale = 5;
const int max_eye_scale = 5;

CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;

void detectFace(int, void*);

int main(int argc, char** argv)
{
	if (argc != 2) {
		return -2;
	}
	image = imread(argv[1], 1);
	if (image.empty()) {
		cout << "Could not find image\n";
		return -1;
	}


	if (!face_cascade.load("C:/source/opencv/sources/data/haarcascades/haarcascade_frontalface_alt.xml")) {
		cout << "Error loading face cascade\n";
		return -1;
	}
	if (!eyes_cascade.load("C:\\source\\opencv\\sources\\data\\haarcascades\\haarcascade_eye_tree_eyeglasses.xml")) {
		cout << "Error loading eyes cascade\n";
		return -1;
	}

	namedWindow("face detection", WINDOW_AUTOSIZE);
	createTrackbar("face", "face detection", &face_scale, max_face_scale, detectFace);
	createTrackbar("eyes", "face detection", &eye_scale, max_eye_scale, detectFace);

	detectFace(0, 0);
	waitKey(0);
	return 0;
}

void detectFace(int, void*) {
	double f_scale = double(face_scale)*0.02 + 1.01;
	double e_scale = double(eye_scale)*0.02 + 1.01;

	Mat image_gray;
	cvtColor(image, image_gray, COLOR_BGR2GRAY);
	equalizeHist(image_gray, image_gray);
	Mat image2 = image.clone();

	vector<Rect> faces;
	face_cascade.detectMultiScale(image_gray, faces, f_scale, 3, 0, Size(50, 50));

	for (size_t i = 0; i < faces.size(); i++) {
		int x = faces[i].x, y = faces[i].y;
		int w = faces[i].width, h = faces[i].height;

		rectangle(image2, Point(x, y), Point(x + w, y + h), Scalar(0, 0, 255), 3);

		Mat faceROI = image_gray(faces[i]);

		vector<Rect> eyes;
		eyes_cascade.detectMultiScale(faceROI, eyes, e_scale);

		for (size_t j = 0; j < eyes.size(); j++) {
			int ex = faces[i].x + eyes[j].x, ey = faces[i].y + eyes[j].y;
			int ew = eyes[j].width, eh = eyes[j].height;
			rectangle(image2, Point(ex, ey), Point(ex + ew, ey + eh), Scalar(255, 255, 0), 3);
		}

		imshow("face detection", image2);

	}
}

