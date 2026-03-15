	#include "opencv2/opencv.hpp"
	#include <unistd.h>

	using namespace cv;
	using namespace std;
	
int main(){
	Mat img = imread("Train(Noisy).jpg");
	Mat imgstandardclean = img.clone();
	Mat imggausclean = img.clone();
	Mat imgmedclean = img.clone();
	Mat imgbilclean = img.clone();
	
	//Standard Blurring Method
	blur(imgstandardclean, imgstandardclean, Size(11,11));
	
	imshow("Noisy Image", img);
	imshow("Cleaned Using Standard Blurring", imgstandardclean);
	
	//Gaussian Blurring
	GaussianBlur(imggausclean, imggausclean, Size(11,11), 0 );
	
	imshow("Cleaned Using Gaussian Blurring", imggausclean);
	
	//Median Blurring
	medianBlur(imgmedclean, imgmedclean, 11);
	
	imshow("Cleaned Using Median Blurring", imgmedclean);
	
	//Bilateral Filtering
	bilateralFilter(imgbilclean, img, 15, 271, 271);
	bilateralFilter(img, imgbilclean, 15, 271, 271);
	
	imshow("Cleaned Using a Bilateral Filter Blurring", imgbilclean);
	
	waitKey(0);
	return 0;
}
