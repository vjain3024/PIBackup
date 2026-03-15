	#include "opencv2/opencv.hpp"
	#include <unistd.h>

	using namespace cv;
	using namespace std;
	
int main(){
	Mat img = imread("Trump.webp");
	Mat imgcartoon = img.clone();
	
	for(int i = 0; i < 10; i = i+2){
		bilateralFilter(imgcartoon, img, 15, 30, 30);
		bilateralFilter(img, imgcartoon, 15, 30, 30);
	}
	
	imshow("Trump in a cartoon", imgcartoon);
	
	waitKey(0);
	return 0;
}
