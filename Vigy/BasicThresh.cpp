	#include "opencv2/opencv.hpp"
	#include <unistd.h>

	using namespace cv;
	using namespace std;
	
int main(){
	Mat img = imread("HotAirBalloon.jpeg");
	
	Mat imgwbasicthresh = img.clone();
	Mat imgwadaptthresh = img.clone();
	Mat imgwadaptthreshgaus = img.clone();
	
	cvtColor(imgwbasicthresh, imgwbasicthresh,COLOR_BGR2GRAY);
	cvtColor(imgwadaptthresh, imgwadaptthresh,COLOR_BGR2GRAY);
	cvtColor(imgwadaptthreshgaus, imgwadaptthreshgaus,COLOR_BGR2GRAY);
	
	GaussianBlur(imgwbasicthresh, imgwbasicthresh, Size(3,3), 0 );
	//Basic Thresh
	threshold(imgwbasicthresh, imgwbasicthresh, 127, 255, THRESH_BINARY);//The max value is 225, the threshold is 127, the method is cv2.THRESH_BINARY to make black and white
	
	//Adaptive Thresh
	adaptiveThreshold(imgwadaptthresh, imgwadaptthresh, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY,11,2);
	adaptiveThreshold(imgwadaptthreshgaus, imgwadaptthreshgaus, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY,11,2);
	
	imshow("Black & White AT W Gauss", imgwadaptthreshgaus);
	imshow("Black & White AT", imgwadaptthresh);
	imshow("Black & White BT", imgwbasicthresh);
	waitKey(0);
	return 0;
}
