
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
	Mat imgafterthresh;
	Mat imgafterthreshcont;
	
	while(true){
		
		if(!cam.getVideoFrame(img, 1000)){
			cout << "CAM ERROR" << endl;
			while(1);
		}
		
		// image is fine...
		cvtColor(img, imgafterthresh,COLOR_BGR2GRAY);
		GaussianBlur(imgafterthresh, imgafterthresh, Size(11,11), 0 );
		Canny(imgafterthresh, imgafterthresh, 255/2 - 10, 255/2+10, 3);
		
		
		vector<vector<Point>> contours;
		findContours(imgafterthresh, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
		imgafterthreshcont = img.clone();
		cout << "nextfilter" << endl;
		for(auto& cnt : contours){
			
			cout << contourArea(cnt) << endl;
			if(contourArea(cnt) < 10 || contourArea(cnt) > 630){
				drawContours(imgafterthreshcont,vector<vector<Point>>(1,cnt),-1,Scalar(0,0,225), 2);
				continue;
			}
			Rect br = boundingRect(cnt);
			cout << br.height/(float)br.width << endl;
			if(br.height/(float)br.width < 1 || br.height/(float)br.width > 1.5 ){
				drawContours(imgafterthreshcont,vector<vector<Point>>(1,cnt),-1,Scalar(0,0,225), 2);
				continue;
				
			}
			//Now we have gotten rid of any random objects
			
		}
		
		
		
		imshow("img", img);
		imshow("img after thresh", imgafterthresh);
		imshow("img after thresh cont", imgafterthreshcont);
		
		int input = waitKey(1);
		if(input == 'q'){
			break;
			
		}
	}
	
	destroyAllWindows();
	cam.stopVideo();
	
	return 0;
}

