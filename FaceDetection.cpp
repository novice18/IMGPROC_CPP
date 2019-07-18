// FaceDetection.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

Mat image = imread("selena.jpeg", 1);

CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;

void detectFace();

int main()
{
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

	detectFace();
	waitKey(0);
	return 0;
}

void detectFace() {
	Mat image_gray;
	cvtColor(image, image_gray, COLOR_BGR2GRAY);
	equalizeHist(image_gray, image_gray);

	vector<Rect> faces;
	face_cascade.detectMultiScale(image_gray, faces);

	for (size_t i = 0; i < faces.size(); i++) {
		int x = faces[i].x, y = faces[i].y;
		int w = faces[i].width, h = faces[i].height;

		rectangle(image, Point(x, y), Point(x + w, y + h), Scalar(0, 0, 255), 3);

		Mat faceROI = image_gray(faces[i]);

		vector<Rect> eyes;
		eyes_cascade.detectMultiScale(faceROI, eyes);

		for (size_t j = 0; j < eyes.size(); j++) {
			int ex = faces[i].x + eyes[j].x, ey = faces[i].y + eyes[j].y;
			int ew = eyes[j].width, eh = eyes[j].height;
			rectangle(image, Point(ex, ey), Point(ex + ew, ey + eh), Scalar(255, 255, 0), 3);
		}

		imshow("Face", image);

	}
}

