#include "opencv2/opencv.hpp"
#include <unistd.h>

using namespace cv;
using namespace std;

int main(){
	Mat img = imread("Ball.webp");
	Mat slice = img(Rect(200, 200, 100, 100));
	Mat sliced = imread("Ball.webp");
	slice.copyTo(sliced(Rect(34,68,100,100)));//Move slice
	
	Rect r(Point(200,200), Point(300, 300));//Erase slice
	rectangle(sliced, r, Scalar(255, 255, 255), -1);
	
	imshow("Image Of An Ball", img);
	imshow("Image Of An Ball Manipulated", sliced);
	imshow("Image Of A Slice of a Ball", slice);
	waitKey(0);
	
	
	return 0;
}
