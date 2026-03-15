#include "opencv2/opencv.hpp"
#include <unistd.h>

using namespace cv;
using namespace std;

int main(){

	Mat img = imread("Ball.webp");
	//Print out a X on the image
	Point p1(0,0), p2(600, 600);
	Scalar color(255, 0, 0);
	int thickness = 2;
	line(img, p1, p2, color, thickness);
	
	Point p12(0,600), p22(600, 0);
	line(img, p12, p22, color, thickness);

	
	//Draw a circle surrounding the image
	Point center(300,300);
	int radius  = 300, thicknessC = 10;
	circle(img, center, radius, color, thicknessC);
	//Print Some Text
	Point origin(0, 20);
	String text = "Lacrosse Ball";
	Scalar textColor(255, 0 ,255);
	putText(img, text, origin, FONT_HERSHEY_SIMPLEX, 1, textColor);
	
	imshow("Image Of An Ball", img);

	//Print out some info about the image
	printf("Rows: %d \n", img.rows);
	printf("Collumns: %d \n", img.cols);
	Vec3b px = img.at<Vec3b>(img.rows/2, img.cols/2);
	printf("The BGR for 100,100 is : (%d, %d, %d) \n", px[0], px[1], px[2]);
	//cout << "Dim " << img.dims << endl;
	waitKey(0);
	return 0;
}
