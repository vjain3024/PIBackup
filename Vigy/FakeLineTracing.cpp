
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


using namespace std::chrono_literals;
using namespace std;
using namespace cv;


// buffer for sprintf
char buf[100];
int stopmotor = 0;

//Paste code underneath
int main(){
	lccv::PiCamera cam;
	cam.options->camera = 0;
	cam.options->video_width = 320;
	cam.options->video_height = 240;
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
	//int data[2];
	while(true){
		//Vid
		if(!cam.getVideoFrame(img, 1000)){
			cout << "CAM ERROR" << endl;
			while(1);
		}
		
		// image is fine...
		
		
		

		
		
		int input = waitKey(1);
		if(input == 'q'){
			destroyAllWindows();
			cam.stopVideo();
		}
		else if(input == ' '){
			stopmotor = 1;
		}
		else if(input == 'g'){
			stopmotor = 0;
		}
		else if(input == 'w'){
			motor_speed[0] = 20;
			motor_speed[1] = 20;
		}
		else if(input == 'a'){
			motor_speed[0] = -20;
			motor_speed[1] = 20;
		}
		else if(input == 'd'){
			motor_speed[0] = 20;
			motor_speed[1] = -20;
		}
		else if(input == 's'){
			motor_speed[0] = -20;
			motor_speed[1] = -20;
		}
		//Data transfer
		
		//centroid_read(img);
		imshow("img", img);
		cout << "Left Motor: " <<  motor_speed[0] << endl;
		cout << "Right Motor: " <<  motor_speed[1] << endl;
		
		
		
		if(stopmotor == 1){
			motor_speed[0] = 0;
			motor_speed[1] = 0;
		}
		sprintf(tx_buffer, "[%d,%d]", motor_speed[0], motor_speed[1]); 
		write(uart0_filestream, &tx_buffer[0], strlen(tx_buffer));
		//this_thread::sleep_for(500ms);
		
	}
	
	return 0;
}
