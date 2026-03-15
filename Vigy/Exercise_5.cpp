#include "opencv2/opencv.hpp"

#include <lccv.hpp>



using namespace std;
using namespace cv;




int main(){
	lccv::PiCamera cam;
	cam.options->camera = 0;
	cam.options->video_width = 320;
	cam.options->video_height = 240;
	cam.options->framerate = 15;
	cam.options->verbose = false;
	cam.startVideo();
	
	Mat img;
	
	while(true){
		
		if(!cam.getVideoFrame(img, 1000)){
			cout << "CAM ERROR" << endl;
			while(1);
		}
		
		// image is fine...
		
		
		flip(img, img, 1); //flip the image
		
		Rect r(Point(60,50), Point(70, 20));//add a rectangle
		rectangle(img, r, Scalar(0, 0, 0), -1);
		
		//4 corners
		Mat mid = img(Rect(img.rows/2-20, img.cols/2-20, 30, 30));
		Rect TL(0, 0, 30, 30);
		Rect TR(img.cols - 30, 0, 30, 30);
		Rect BL(0, img.rows - 30, 30, 30);
		Rect BR(img.cols - 30, img.rows - 30, 30, 30);
	
		mid.copyTo(img(BR));
		mid.copyTo(img(BL));
		mid.copyTo(img(TR));
		mid.copyTo(img(TL));
		
		Mat slice = img(Rect(100, 70, 50, 50));
		slice.copyTo(img(Rect(34,68,50,50)));//Move slice
	
		Rect r2(Point(100,70), Point(150, 120));//Erase slice
		rectangle(img, r2, Scalar(255, 255, 255), -1);
		
		imshow("img", img);
		
		Mat bgr[3];
		split(img, bgr);
		
		imshow("Blue Channel", bgr[0]);
		imshow("Green Channel", bgr[1]);
		imshow("Red Channel", bgr[2]);
		
		
		int input = waitKey(1);
		if(input == 'q'){
			break;
		}
	}
	
	destroyAllWindows();
	cam.stopVideo();
	
	return 0;
}
