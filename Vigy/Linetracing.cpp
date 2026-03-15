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
	Mat imgthresh;
	Mat imgwline;
	Mat imgwgreen;
	Scalar LowerGreen = Scalar (76, 212, 82);
	Scalar HighGreen = Scalar (82, 255, 131);
	int leftgreen = 0;
	int rightgreen = 0;
	int top = 0;
	int bottom = 0;
	int left = 0;
	int right = 0;
	Point midpoint;
	
	
	while(true){
		
		if(!cam.getVideoFrame(img, 1000)){
			cout << "CAM ERROR" << endl;
			while(1);
		}
		
		cvtColor(img, imgthresh,COLOR_BGR2GRAY);
		GaussianBlur(imgthresh, imgthresh, Size(11,11), 0 );
		threshold(imgthresh, imgthresh, 127, 255, THRESH_BINARY);
		bitwise_not(imgthresh, imgwline);
		
		cvtColor(img, imgwgreen,COLOR_BGR2HSV);
		
		GaussianBlur(imgwgreen, imgwgreen, Size(11,11), 0 );
		inRange(imgwgreen,LowerGreen, HighGreen, imgwgreen);
		
		leftgreen = 0;
		rightgreen = 0;
		top = 0;
		bottom = 0;
	    left = 0;
		right = 0;
		vector<vector<Point>> contours;
		findContours(imgwgreen, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
		//drawContours(img,contours,-1,Scalar(0,0,255),10);
		printf("Num of contours : ");
		cout<<contours.size()<<endl;
		
		cout<<"Cycle done"<<endl;
		
	
		if(contours.size() != 0){
		
			for(auto& cnt : contours){
				cout << contourArea(cnt) << endl;
				Rect br = boundingRect(cnt);
				cout << br.height/(float)br.width << endl;
				if(contourArea(cnt) < 200 || contourArea(cnt) > 2000){
					//drawContours(img,vector<vector<Point>>(1,cnt),-1,Scalar(0,0,225), 2);
					continue;
				}
				/*else if(br.height/(float)br.width < 0.90 || br.height/(float)br.width > 1.10 ){
					drawContours(img,vector<vector<Point>>(1,cnt),-1,Scalar(0,0,225), 2);
					continue;
				
				}*/
				else{
					//If detect green
					drawContours(img,vector<vector<Point>>(1,cnt),-1,Scalar(0,255,0), 2);
					Rect bounding_rect = boundingRect(cnt);
					midpoint.x = bounding_rect.x + (bounding_rect.width)/2;
					midpoint.y = bounding_rect.y + (bounding_rect.height)/2;
					
					//get the midpoint
					Point center(midpoint.x,midpoint.y);
					int radius  = 1;
					int thickness = 1;
					Scalar color(255, 255, 0);
					circle(img, center, radius, color, thickness);
					//Detect where black is
					cout<<(int)imgthresh.at<uchar>(Point(midpoint.x,midpoint.y-(bounding_rect.height/2 + 8)))<<endl;
					cout<<(int)imgthresh.at<uchar>(Point(midpoint.x-(bounding_rect.width/2 + 8),midpoint.y))<<endl;
					cout<<(int)imgthresh.at<uchar>(Point(midpoint.x+(bounding_rect.width/2 + 8),midpoint.y))<<endl;
					cout<<(int)imgthresh.at<uchar>(Point(midpoint.x,midpoint.y+(bounding_rect.height/2 + 8)))<<endl;
					
					Point topcenter(midpoint.x,midpoint.y-(bounding_rect.height/2 + 8));
					int radiustop  = 2;
					int thicknesstop = 2;
					Scalar topcolor(255, 0, 0);
					circle(img, topcenter, radiustop, topcolor, thicknesstop);
					
					
					Point leftcenter(midpoint.x-(bounding_rect.width/2 + 8),midpoint.y);
					int radiusleft  = 2;
					int thicknessleft = 2;
					Scalar leftcolor(0, 255, 0);
					circle(img, leftcenter, radiusleft, leftcolor, thicknessleft);
								
					Point rightcenter(midpoint.x+(bounding_rect.width/2 + 8),midpoint.y);
					int radiusright  = 2;
					int thicknessright = 2;
					Scalar rightcolor(0, 0, 255);
					circle(img, rightcenter, radiusright, rightcolor, thicknessright);
								
					Point bottomcenter(midpoint.x,midpoint.y+(bounding_rect.height/2 + 8));
					int radiusbottom  = 2;
					int thicknessbottom = 2;
					Scalar bottomcolor(255, 0, 255);
					circle(img, bottomcenter, radiusbottom, bottomcolor, thicknessbottom);
					
					if((int)imgwline.at<uchar>(Point(midpoint.x,midpoint.y-(bounding_rect.height/2 + 8))) == 255){//If there is black above good
						
						
						
						top = 1;
						cout<<"line on top"<<endl;
					}
					if((int)imgwline.at<uchar>(Point(midpoint.x-(bounding_rect.width/2 + 8),midpoint.y)) == 255){//If there is black to the left
						
						
						left = 1;
						cout<<"line on left"<<endl;
					}
					if((int)imgwline.at<uchar>(Point(midpoint.x+(bounding_rect.width/2 + 8),midpoint.y)) == 255){//If there is black to the right
						
						
						right = 1;
						cout<<"line on right"<<endl;
					}
					if((int)imgwline.at<uchar>(Point(midpoint.x,midpoint.y+(bounding_rect.height/2 + 8))) == 255){//If there is black to the bottom
						

						cout<<"line on bottom"<<endl;
						bottom = 1;
					}
					
					//Detect what type of green square it is
					if(bottom == 1){//False
						cout<<"false"<<endl;
						continue;
					}
					else if(top == 1 && right == 1){//Left Green
						cout<<"left green"<<endl;
						leftgreen++;
					}
					else if(top == 1 && left ==1){//Right Green
						cout<<"right green"<<endl;
						rightgreen++;
					}
					
					if(rightgreen == 1 && leftgreen == 1){//DOuble
						cout<<"double"<<endl;
					}
				}
			}
		}
		
		
		imshow("img after thresh", imgthresh);
		imshow("img w line", imgwline);
		imshow("img", img);
		imshow("img w green", imgwgreen);
		int input = waitKey(1);
		if(input == 'q'){
			break;
		}
	}
	
	destroyAllWindows();
	cam.stopVideo();
	
	return 0;
}
