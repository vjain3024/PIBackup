
// Line trace with pi camera


// includes for opencv stuff
// includes for the serial communication and other stuff
#include <lccv.hpp>
#include <iostream>
#include <string.h>	// c
#include <string>	// c++
#include <errno.h>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <termios.h>                                                         
#include <stdio.h>
#include <stdlib.h>	                                                     
#include <fcntl.h>                                                                                                               
#include <sys/types.h> 
#include <stdint.h>
#include <sys/signal.h>
#include <time.h>
#include <stdbool.h>	

#include <opencv2/opencv.hpp>
#include <sstream> 

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui.hpp>



#define BAUDRATE B115200                                                      
#define MODEMDEVICE "/dev/ttyAMA0"

#define DEBUG_IMSHOW(name, frame) imshow(name, frame);

//#define DEBUG_IMSHOW(name,frame)



using namespace std::chrono_literals;
using namespace std;
using namespace cv;


int green_square(Mat img){
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
					continue;
				}
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
						return 1;
					}
					else if(top == 1 && left ==1){//Right Green
						cout<<"right green"<<endl;
						rightgreen++;
						return 2;
					}
					
					
					if(rightgreen == 1 && leftgreen == 1){//DOuble
						cout<<"double"<<endl;
						return 3;
					}
				}
			}
		}
	return 0;
}



// buffer for sprintf
char buf[100];



//Paste code underneath
int main(){
	// start serial
	int uart0_filestream = open(MODEMDEVICE, O_RDWR | O_NOCTTY | O_NDELAY);		//Open in non blocking read/write mode
	if (uart0_filestream == -1)
	{
		//ERROR - CAN'T OPEN SERIAL PORT
		printf("Error - Unable to open UART.  Ensure it is not in use by another application\n");
		exit(-1);
	}
	
	// config serial settings
	struct termios options;
	tcgetattr(uart0_filestream, &options);
	options.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;		//<Set baud rate
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;
	tcflush(uart0_filestream, TCIFLUSH);
	tcsetattr(uart0_filestream, TCSANOW, &options);

this_thread::sleep_for(1000ms);
	
	// data recieved from the pico, start sending and recieving values
	char tx_buffer[40];
	int data = 30;
	while(true){
		sprintf(tx_buffer, "[%d]", data); 
		cout << "Send Data: " << data << endl;
		write(uart0_filestream, &tx_buffer[0], strlen(tx_buffer));
		data = data * -1;
		this_thread::sleep_for(500ms);
	}
	
	return 0;
}
