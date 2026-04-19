
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
int motor_speed[2];
int rightgren =0;
int leftgren = 0;
int doublegren = 0;
int sharpturn = 0;
int newheight = 1232;
int newwidth = 1640;

using namespace std::chrono_literals;
using namespace std;
using namespace cv;


int green_square(Mat img){
		Mat imgthresh;
		Mat imgwline;
		Mat imgwgreen;
		Scalar LowerGreen = Scalar (72, 145, 105);
		Scalar HighGreen = Scalar (78, 255, 171);
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
	
		imshow("imgwgreen", imgwgreen);
	if(contours.size() != 0){
		
			for(auto& cnt : contours){
				cout << contourArea(cnt) << endl;
				cout << "COUNTOR AREA!!!!" << endl;
				Rect br = boundingRect(cnt);
				//cout << br.height/(float)br.width << endl;
				if(contourArea(cnt) < 1000|| contourArea(cnt) > 13000){
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
						leftgren = 1;
						leftgreen++;
						return 1;
					}
					else if(top == 1 && left ==1){//Right Green
						cout<<"right green"<<endl;
						rightgren = 1;
						rightgreen++;
						return 2;
					}
					
					
					if(rightgreen == 1 && leftgreen == 1){//DOuble
						cout<<"double"<<endl;
						leftgren = 0;
						rightgren = 0;
						doublegren = 1;
						return 3;
					}
				}
			}
		}
	return 0;
}

int centroid_adjust(Mat img){
	Mat imgthresh;
	Mat imgwline;
	
	cvtColor(img, imgthresh,COLOR_BGR2GRAY);
	GaussianBlur(imgthresh, imgthresh, Size(11,11), 0 );
	threshold(imgthresh, imgthresh, 127, 255, THRESH_BINARY);
	bitwise_not(imgthresh, imgwline);
		
	Point center(newwidth/8,newheight/8);
	Scalar color(255, 255, 0);
	int radius  = 5, thickness = 1;
	circle(img, center, radius, color, thickness);
	
	imshow("imgwline", imgwline);
	imshow("imgthresh", imgthresh);
	if((int)imgwline.at<uchar>(Point(center.x,center.y)) == 255){//You are on good path or need a sharp turn
		for(int i = 130; i < 159; i++){//checks wheter you need to take a sharp turn right
			if((int)imgwline.at<uchar>(Point((center.x + i),center.y)) == 255){//Line on the right
				Point center1((center.x + i),120);
				Scalar color1(0, 255, 0);
				int radius1  = 2, thickness1 = 2;
				circle(img, center1, radius1, color1, thickness1);
				//imshow("img", img);
				//cout<<"Line found at: " << center.x+i << "," << center.y<<endl;
				cout<<"Line on sharp right"<<endl;
				return 11;
			}
		for(int i = 130; i < 159; i++){//checks wheter you need to take a sharp turn left
				if((int)imgwline.at<uchar>(Point((center.x - i),center.y)) == 255){//Line on the left
					Point center2((center.x - i),120);
					Scalar color2(255, 0, 255);
					int radius2  = 2, thickness2 = 2;
					circle(img, center2, radius2, color2, thickness2);
					//imshow("img", img);
					//cout<<"Line found at: " << center.x-i << "," << center.y<<endl;
					cout<<"Line on sharp left"<<endl;
					return 12;
				}	
			}
		cout<<"Line im mid"<<endl;
		return 0;//If there is no need for a sharp turn continue forward
		}
	}
		else{
			for(int l = 0; l < 150; l++){
				for(int i = 1; i < 159; i++){
					if((int)imgwline.at<uchar>(Point((center.x + i),(center.y - l))) == 255){//Line on the right
						Point center1((center.x + i),(center.y - l));
						for(int t = 0; t < newwidth/2; t++){
							if(imgwline.at<uchar>(Point((center.x + t), (0))) != 255){//Ur off line rn not on a gap
								Scalar color1(0, 255, 0);
								int radius1  = 2, thickness1 = 2;
								circle(img, center1, radius1, color1, thickness1);
								//imshow("img", img);
								//cout<<"Line found at: " << center.x+i << "," << center.y+l<<endl;
								cout<<"Line on right HEHEIDWHEDHUIEWHDHWEHD"<<endl;
								return 1;
							}
							else{
								return 100;//GAP
							}
						}
						
						
					}
				}
				for(int i = 1; i < 159; i++){
					if((int)imgwline.at<uchar>(Point((center.x - i),(center.y - l))) == 255){//Line on the left
						for(int t = 0; t < newwidth/2; t++){
							if(imgwline.at<uchar>(Point((center.x + t), (0))) != 255){//Ur off line rn not on a gap
								Point center2((center.x - i),(center.y - l));
								Scalar color2(255, 0, 255);
								int radius2  = 2, thickness2 = 2;
								circle(img, center2, radius2, color2, thickness2);
								//imshow("img", img);
								//cout<<"Line found at: " << center.x-i << "," << center.y+l<<endl;
								cout<<"Line on left HRUUIEHHFEWFUIEUIFHE"<<endl;
								return 2;
							}
							else{
								return 100;//GAP
							}
					}	
				}
			}
		}
	}
	cout<<"Line not found"<<endl;
	return 100;
}


void centroid_read(Mat img){
	int centered = centroid_adjust(img);
	if(centered == 1){//turn right
		motor_speed[0] = 30;//left motor
		motor_speed[1] = -30;//right motor
	}
	else if(centered == 2){//turn left
		motor_speed[0] = -30;
		motor_speed[1] = 30;
	}
	else if(centered == 0){//straight
		motor_speed[0] = 40;
		motor_speed[1] = 40;
	}
	else if(centered == 11){//SHARP TURN RIGHT
		sharpturn = 1;
		motor_speed[0] = 80;
		motor_speed[1] = -40;
	}
	else if(centered == 12){//SHARP TURN LEFT
		sharpturn = 1;
		motor_speed[0] = -40;
		motor_speed[1] = 80;
	}
	else{//GAPS
		motor_speed[0] = 20;
		motor_speed[1] = 20;
	}
}


// buffer for sprintf
char buf[100];
int stopmotor = 0;


//Paste code underneath
int main(){
	lccv::PiCamera cam;
	cam.options->camera = 0;
	cam.options->video_width = newwidth;
	cam.options->video_height = newheight;
	cam.options->framerate = 60;
	cam.options->verbose = false;
	cam.startVideo();
	
	Mat img;
	
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

	
	// data recieved from the pico, start sending and recieving values
	char tx_buffer[40];
	
	char rx_buffer[256];
	int rx_length = 0;
	
	
	do{
		rx_length = read(uart0_filestream, (void*)rx_buffer, 255);
	}while(rx_length <= 0);
	
	
	//int data[2];
	while(true){
		//Vid
		if(!cam.getVideoFrame(img, 1000)){
			cout << "CAM ERROR" << endl;
			while(1);
		}
		
		// image is fine...
		resize(img,img,Size(newwidth/4,newheight/4));
		flip(img,img,-1);
		

		
		
		int input = waitKey(1);
		if(input == 'q'){
			destroyAllWindows();
			cam.stopVideo();
			break;
			
		}
		else if(input == ' '){
			stopmotor = 1;
		}
		else if(input == 'g'){
			stopmotor = 0;
		}
		
		motor_speed[0] = 0;
		motor_speed[1] = 0;
		sprintf(tx_buffer, "[%d,%d]", motor_speed[0], motor_speed[1]); 
		write(uart0_filestream, &tx_buffer[0], strlen(tx_buffer));
		
		//Data transfer
		
		green_square(img);
		if(leftgren == 1 || rightgren == 1){
			leftgren = 0;
			rightgren = 0;
			motor_speed[0] = 40;
			motor_speed[1] = 40;
			sprintf(tx_buffer, "[%d,%d]", motor_speed[0], motor_speed[1]); 
			write(uart0_filestream, &tx_buffer[0], strlen(tx_buffer));
			this_thread::sleep_for(100ms);
			green_square(img);
		}
		
		///cout<<"Double Green: "<<doublegren<<endl;
		if(doublegren == 1){
			cout<<"doublegreen"<<endl;
			doublegren = 0;
			motor_speed[0] = 40;
			motor_speed[1] = -20;
			sprintf(tx_buffer, "[%d,%d]", motor_speed[0], motor_speed[1]); 
			write(uart0_filestream, &tx_buffer[0], strlen(tx_buffer));
			//while(centroid_adjust !=  )
			this_thread::sleep_for(1500ms);
		}
		else if(rightgren == 1){
			cout<<"rightgreen"<<endl;
			rightgren = 0;
			motor_speed[0] = 40;
			motor_speed[1] = -20;
			sprintf(tx_buffer, "[%d,%d]", motor_speed[0], motor_speed[1]); 
			write(uart0_filestream, &tx_buffer[0], strlen(tx_buffer));
			this_thread::sleep_for(100ms);
		}
		else if(leftgren == 1){
			cout<<"leftgreen"<<endl;
			leftgren = 0;
			motor_speed[0] = -20;
			motor_speed[1] = 40;
			sprintf(tx_buffer, "[%d,%d]", motor_speed[0], motor_speed[1]); 
			write(uart0_filestream, &tx_buffer[0], strlen(tx_buffer));
			this_thread::sleep_for(100ms);
		}
		/*
		green_square(img);
		if(leftgren == 1 || rightgren == 1){
			leftgren = 0;
			rightgren = 0;
			motor_speed[0] = 0;
			motor_speed[1] = 0;
			sprintf(tx_buffer, "[%d,%d]", motor_speed[0], motor_speed[1]); 
			write(uart0_filestream, &tx_buffer[0], strlen(tx_buffer));
			this_thread::sleep_for(1000ms);
			motor_speed[0] = 40;
			motor_speed[1] = 40;
			sprintf(tx_buffer, "[%d,%d]", motor_speed[0], motor_speed[1]); 
			write(uart0_filestream, &tx_buffer[0], strlen(tx_buffer));
			this_thread::sleep_for(950ms);
			green_square(img);
		}
		
		///cout<<"Double Green: "<<doublegren<<endl;
		if(doublegren == 1){
			cout<<"doublegreen"<<endl;
			doublegren = 0;
			motor_speed[0] = 40;
			motor_speed[1] = -20;
			sprintf(tx_buffer, "[%d,%d]", motor_speed[0], motor_speed[1]); 
			write(uart0_filestream, &tx_buffer[0], strlen(tx_buffer));
			this_thread::sleep_for(800ms);//avoid detecting another line
			while(centroid_adjust(img) != 0){//While its not on a straight line 
				motor_speed[0] = 40;
				motor_speed[1] = -20;
				sprintf(tx_buffer, "[%d,%d]", motor_speed[0], motor_speed[1]); 
				write(uart0_filestream, &tx_buffer[0], strlen(tx_buffer));
				this_thread::sleep_for(500ms);
			}
		}
		else if(rightgren == 1){
			cout<<"rightgreen"<<endl;
			rightgren = 0;
			motor_speed[0] = 40;
			motor_speed[1] = -20;
			sprintf(tx_buffer, "[%d,%d]", motor_speed[0], motor_speed[1]); 
			write(uart0_filestream, &tx_buffer[0], strlen(tx_buffer));
			this_thread::sleep_for(300ms);
			do{
				if(!cam.getVideoFrame(img, 1000)){
					cout << "CAM ERROR" << endl;
					while(1);
				}
				motor_speed[0] = 40;
				motor_speed[1] = -20;
				sprintf(tx_buffer, "[%d,%d]", motor_speed[0], motor_speed[1]); 
				write(uart0_filestream, &tx_buffer[0], strlen(tx_buffer));
				this_thread::sleep_for(100ms);
			}while(centroid_adjust(img) != 0) ;//While its not on a straight line 
		}
		else if(leftgren == 1){
			cout<<"leftgreen"<<endl;
			leftgren = 0;
			motor_speed[0] = 0;
			motor_speed[1] = 20;
			sprintf(tx_buffer, "[%d,%d]", motor_speed[0], motor_speed[1]); 
			write(uart0_filestream, &tx_buffer[0], strlen(tx_buffer));
			this_thread::sleep_for(150ms);
			int val;
			do{//While its not on a straight line 
				if(!cam.getVideoFrame(img, 1000)){
					cout << "CAM ERROR" << endl;
					while(1);
				}
				motor_speed[0] = 0;
				motor_speed[1] = 20;
				sprintf(tx_buffer, "[%d,%d]", motor_speed[0], motor_speed[1]); 
				write(uart0_filestream, &tx_buffer[0], strlen(tx_buffer));
				this_thread::sleep_for(20ms);
				val = centroid_adjust(img);
				printf("\nCCentroid Adjust retuurned %5d\n", val);
				
			}
			while(val != 0);
		}
		*/
		
		centroid_read(img);
		
		imshow("img", img);
		
		
		
		
		if(stopmotor == 1){
			motor_speed[0] = 0;
			motor_speed[1] = 0;
		}
		cout << "Left Motor: " <<  motor_speed[0] << endl;
		cout << "Right Motor: " <<  motor_speed[1] << endl;
		cout << "Sharp: " <<  sharpturn << endl;
		
		sprintf(tx_buffer, "[%d,%d]", motor_speed[0], motor_speed[1]); 
		write(uart0_filestream, &tx_buffer[0], strlen(tx_buffer));
		if(sharpturn == 1){
			this_thread::sleep_for(200ms);
			sharpturn = 0;
		}
		
	}
	

	
	return 0;
}
