//IMPORTANT!!!
//Rect(top left point x, top left point y, height, width)

#include "opencv2/opencv.hpp"
#include <unistd.h>

using namespace cv;
using namespace std;

int main(){
	Mat img = imread("Ball.webp");
	Mat imgchanged = imread("Ball.webp");
	
	//Print out a X on the image
	Point p1(0,0), p2(600, 600);
	Scalar color(255, 0, 0);
	int thickness = 2;
	line(imgchanged, p1, p2, color, thickness);
	
	Point p12(0,600), p22(600, 0);
	line(imgchanged, p12, p22, color, thickness);

	
	//Draw a circle surrounding the image
	Point center(300,300);
	int radius  = 300, thicknessC = 10;
	circle(imgchanged, center, radius, color, thicknessC);
	
	//Print Some Text
	Point origin(0, 20);
	String text = "Lacrosse Ball";
	Scalar textColor(0, 0 ,0);
	putText(imgchanged, text, origin, FONT_HERSHEY_SIMPLEX, 1, textColor);
	
	//Create a rectangle
	Rect r(Point(150,150), Point(250, 250));
	rectangle(imgchanged, r, Scalar(0, 0, 0), -1);
	
	//Flip!
	flip(imgchanged , imgchanged , 0 );
	
	//Resize
	Size newSize(400, 400);
	resize(imgchanged, imgchanged, newSize);
	
	// 4 Corners
	Mat mid = img(Rect(img.rows/2-20, img.cols/2-20, 100, 100));
	Rect TL(0, 0, 100, 100);
	Rect TR(imgchanged.cols - 100, 0, 100, 100);
	Rect BL(0, imgchanged.rows - 100, 100, 100);
	Rect BR(imgchanged.cols - 100, imgchanged.rows - 100, 100, 100);
	
	
	
	
	mid.copyTo(imgchanged(BR));
	mid.copyTo(imgchanged(BL));
	mid.copyTo(imgchanged(TR));
	mid.copyTo(imgchanged(TL));
	
	imshow("MIDDLE", mid);
	//imshow("Image Of An Ball", img);
	imshow("Image Of An Ball Changed", imgchanged);
	
	printf("Rows: %d \n", imgchanged.rows);
	printf("Collumns: %d \n", imgchanged.cols);
	printf("Center Pixel = (%d, %d) \n", imgchanged.rows/2, imgchanged.cols/2);
	
	Vec3b px = img.at<Vec3b>(imgchanged.rows/2, imgchanged.cols/2);
	printf("The BGR for the center pixel is : (%d, %d, %d) \n", px[0], px[1], px[2]);
	
	waitKey(0);
	return 0;
}
