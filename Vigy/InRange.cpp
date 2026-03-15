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
	
	Mat threshimg;
	Mat img;
	Scalar redLower = Scalar(0,0,90);
	Scalar redUpper = Scalar(50,50,255);
	float radiuss;
	Point2f centerpt;
	
	while(true){
		
		if(!cam.getVideoFrame(img, 1000)){
			cout << "CAM ERROR" << endl;
			while(1);
		}
		
		// image is fine...
		
		
		
		imshow("img", img);
		GaussianBlur(img, img, Size(11,11), 0 );
		imshow("Filtered",img);
		inRange(img, redLower, redUpper, threshimg);//If a pixel is between redLower and redUpper then it becomes white otherwise its black
		
		vector<vector<Point>> contours;
		findContours(threshimg, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
		if(contours.size() >= 1){//check if there are any contours and if so circle them
			minEnclosingCircle(contours[0], centerpt, radiuss);
			circle(threshimg, centerpt, radiuss, Scalar(255, 0, 0), 2);
		}
		imshow("InRange", threshimg);
		
		
		int input = waitKey(1);
		if(input == 'q'){
			break;
		}
	}
	
	destroyAllWindows();
	cam.stopVideo();
	
	return 0;
}
