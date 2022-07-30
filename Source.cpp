#include <opencv2/core/utility.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <cstdio>
#include <iostream>

using namespace cv;
using namespace std;

Vec3b RandomColor(int value);

int main(int argc, char** argv)
{
	clock_t start, end;
	start = clock();

	Mat img = imread("imagen1.jpg");
	imshow("Imagen Original", img);

	Mat imgGray, imgCanny;
	cvtColor(img, imgGray, COLOR_BGR2GRAY);
	GaussianBlur(imgGray, imgGray, Size(5, 5), 2);
	Canny(imgGray, imgCanny, 40, 100);

	imshow("Imagen en Gris", imgGray);
	imshow("Imagen Canny", imgCanny);

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	findContours(imgCanny, contours, hierarchy, RETR_LIST, CHAIN_APPROX_SIMPLE, Point());
	Mat imageContours = Mat::zeros(img.size(), CV_8UC1);
	Mat marks(img.size(), CV_32S);
	marks = Scalar::all(0);
	int index = 0;
	int compCount = 0;
	for (; index >= 0; index = hierarchy[index][0], compCount);
	{
		drawContours(marks, contours, index, Scalar::all(compCount + 1), 1, 8, hierarchy);
		drawContours(imageContours, contours, index, Scalar(255), 1, 8, hierarchy);
	}
	Mat marksShows;
	convertScaleAbs(marks, marksShows);
	imshow("Marks Shows", marksShows);
	imshow("Imagen Contours", imageContours);
	watershed(img, marks);

	Mat afwatershed;
	convertScaleAbs(marks, afwatershed);
	imshow("Despues Watershed", afwatershed);

	Mat PerspectiveImg = Mat::zeros(img.size(), CV_8UC3);
	for (int i = 0; i < marks.rows; i++) {
		for (int j = 0; j < marks.cols; j++) {
			int index = marks.at<int>(i, j);
			if (marks.at<int>(i, j) == 1) {
				PerspectiveImg.at<Vec3b>(i, j) = Vec3b(255, 255, 255);
			}
			else {
				PerspectiveImg.at<Vec3b>(i, j) = RandomColor(index);

			}
		}
	}
	imshow("Despues ColorFill", PerspectiveImg);

	Mat wshed;
	addWeighted(img, 0.4, PerspectiveImg, 0.6, 0, wshed);
	imshow("ADDWeighted Image", wshed);

	end = clock();
	double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
	cout << "El tiempo que ha tomado el programa es: " << fixed << time_taken;
	cout << "segundos" << endl;

	waitKey(0);
	return 0;
}

Vec3b RandomColor(int value){
	value = value % 255;
	RNG rng;
	int aa = rng.uniform(0, value);
	int bb = rng.uniform(0, value);
	int cc = rng.uniform(0, value);
	return Vec3b(aa, bb, cc);
}
