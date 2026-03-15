	#include "opencv2/opencv.hpp"
	#include <unistd.h>

	using namespace cv;
	using namespace std;

int main(){
	Mat img = imread("Bag.jpg");
	cvtColor(img, img, COLOR_BGR2GRAY);
	imshow("Grayscale Clock", img);
	waitKey(0);
	return 0;
}
