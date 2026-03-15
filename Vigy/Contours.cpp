	#include "opencv2/opencv.hpp"
	#include <unistd.h>

	using namespace cv;
	using namespace std;
	
int main(){
	Mat drawing(600,600,CV_8UC3,Scalar(0,0,0));
	Mat grayscale;
	Mat threshoutput;
	Mat drawingcopy = drawing.clone();
	
	circle(drawing,Point(100, 100),95, Scalar(0,255,255),15);
	circle(drawing, Point(200, 250), 45, Scalar(0,255,255),15);
	circle(drawing, Point(400, 400), 50, Scalar(0,255,255),15);
	
	cvtColor(drawing, grayscale,COLOR_BGR2GRAY);
	
	                                                  
	threshold(grayscale, threshoutput, 127, 255, THRESH_BINARY);
	
	vector<vector<Point>> contours;
	findContours(threshoutput, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	
	
	drawContours(drawingcopy, contours,-1, Scalar(0, 0, 255),2);
	imshow("CONTOURS", drawingcopy);
	imshow("OG", drawing);
	imshow("THRESH", threshoutput);
	cout << contours.size()<< endl;
	cout << contourArea(contours[0]) << endl;
	cout << contourArea(contours[1]) << endl;
	cout << contourArea(contours[2]) << endl;
	waitKey(0);
	return 0;
}
