
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
using namespace std::chrono_literals;
using namespace std;
using namespace cv;
int motor_speed[2];
int main(){
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
	
	motor_speed[0] =20;
	motor_speed[1] =20;
	sprintf(tx_buffer, "[%d,%d]", motor_speed[0], motor_speed[1]); 
	write(uart0_filestream, &tx_buffer[0], strlen(tx_buffer));
	this_thread::sleep_for(1850ms);
	motor_speed[0] =-20;
	motor_speed[1] =20;
	sprintf(tx_buffer, "[%d,%d]", motor_speed[0], motor_speed[1]); 
	write(uart0_filestream, &tx_buffer[0], strlen(tx_buffer));
	this_thread::sleep_for(850ms);
	motor_speed[0] =20;
	motor_speed[1] =-20;
	sprintf(tx_buffer, "[%d,%d]", motor_speed[0], motor_speed[1]); 
	write(uart0_filestream, &tx_buffer[0], strlen(tx_buffer));
	this_thread::sleep_for(850ms);
	motor_speed[0] =-20;
	motor_speed[1] =-20;
	sprintf(tx_buffer, "[%d,%d]", motor_speed[0], motor_speed[1]); 
	write(uart0_filestream, &tx_buffer[0], strlen(tx_buffer));
	this_thread::sleep_for(850ms);
}	
