
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
		
		
		
		imshow("img", img);
		
		
		int input = waitKey(1);
		if(input == 'q'){
			break;
		}
	}
	
	destroyAllWindows();
	cam.stopVideo();
	
	return 0;
}
