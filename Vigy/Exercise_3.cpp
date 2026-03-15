#include "opencv2/opencv.hpp"
#include <unistd.h>

using namespace cv;
using namespace std;

int main(){
	Mat rect(400,400, CV_8UC1, Scalar(0));
	Mat circled(400,400, CV_8UC1, Scalar(0));
	Mat and_bit(400,400, CV_8UC1, Scalar(0));
	Mat or_bit(400,400, CV_8UC1, Scalar(0));
	Mat not_bit(400,400, CV_8UC1, Scalar(0));
	
	Rect r(Point(100,100), Point(250, 350));//RECT
	rectangle(rect, r, Scalar(255, 255, 255), -1);
	
	//CIRCLE
	int radius  = 100;
	int thickness = -1;
	Point center(200,200);
	Scalar color(255, 255, 255);	
	
	circle(circled, center, radius, color, thickness);
	
	//And Bit
	bitwise_and(rect, circled, and_bit);
	
	//Or Bit
	bitwise_or(rect, circled, or_b it);
	
	//Or Bit
	bitwise_not(or_bit, not_bit);
	
	imshow("Image Of A Circle", circled);
	imshow("Image Of A Rectangle", rect);
	imshow("Image Of And Bitwise", and_bit);
	imshow("Image Of Or Bitwise", or_bit);
	imshow("Image Of Not Bitwise", not_bit);
	waitKey(0);
	return 0;
}
