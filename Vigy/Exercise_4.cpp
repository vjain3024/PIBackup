	#include "opencv2/opencv.hpp"
	#include <unistd.h>

	using namespace cv;
	using namespace std;
	
int main(){
	Mat img = imread("Clock.jpg",0);
	if(img.empty()){
		cout << "failed" << endl;
		return 1;
	}
	imshow("IMG",img);
	Mat added;
	Mat subtracted;
	Mat M(img.rows,img.cols,CV_8UC1,Scalar(1));
	M = M * 100;
	
	add(img, M, added);
	subtract(img, M, subtracted);
	imshow("Added",added);
	imshow("Subtracted",subtracted);
	waitKey(0);
	
	return 0;
}
	
	
	
	
	
	
	
