
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
		cvtColor(img, imgthresh,COLOR_BGR2GRAY);
		GaussianBlur(imgthresh, imgthresh, Size(11,11), 0 );
		threshold(imgthresh, imgthresh, 140, 255, THRESH_BINARY);
		bitwise_not(imgthresh, imgwline);
		
		cvtColor(img, imgwgreen,COLOR_BGR2HSV);
		
		GaussianBlur(imgwgreen, imgwgreen, Size(11,11), 0 );
		inRange(imgwgreen,LowerGreen, HighGreen, imgwgreen);
		
		
		imshow("imgwgreen", imgwgreen);
		imshow("imgwline", imgwline);
		return 0;
}

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
		
		green_square(img);
		
		
		int input = waitKey(1);
		if(input == 'q'){
			destroyAllWindows();
			cam.stopVideo();
			break;
		
		}
	}
	return 0;
}
