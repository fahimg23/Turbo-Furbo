#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>

#include <signal.h>

#include <unistd.h>

#include <pigpio.h>
#include <pigpiod_if2.h>

#include "Server.h"
#include "Camera_Server.h"

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

const char pi_ip[IP_ADDR_LEN] = "192.168.1.164"; // pi ip address
int camera_port = 65535;

int pi;
int run_status;

const string im_ext = ".jpg"; // image extension
const string im_name = "video stream";

/* Handler for Cntrl+C keyboard event  */
void sigintHandler(int s)
{
	char ip[IP_ADDR_LEN];

	strcpy(ip, pi_ip);

	run_status = 0;

	pigpio_start(ip, NULL);
	gpioInitialise();
	pigpio_stop(pi);
	printf("\nRestarted gpio\n");
}

void terminate_gpio(void)
{
	gpioTerminate();
	pigpio_stop(pi);
	printf("Stopped gpio\n");
}

int main(int argc, char* argv[])
{
	int stat;
	char ip[IP_ADDR_LEN];

	// Video capture
	Mat frame;
	Mat recieve_frame;
  VideoCapture cap(0);
	uchar buf[MAX_BUFFER];
	vector<uchar> im_buf;
	uchar recieve_buf[MAX_BUFFER];
	int num_bytes_read;

  if(!cap.isOpened())
    printf("Error. Unable to open camera\n");

	cap.set(CAP_PROP_FRAME_WIDTH,128);
	cap.set(CAP_PROP_FRAME_HEIGHT,128);

	strcpy(ip, pi_ip);
	run_status = 1;
	pi = pigpio_start(ip, NULL);

	if(pi) {
		if(gpioInitialise() != PI_INIT_FAILED) {

			printf("GPIO initialization succeeded.\n");

			gpioSetSignalFunc(SIGINT, sigintHandler);

			// Initialize server for camera
			Camera_Server pi_serv_cam(camera_port, pi_ip, SOCK_DGRAM);

			pi_serv_cam.initialize();

			pi_serv_cam.terminate();

			pi_serv_cam.initialize();

			while(!pi_serv_cam.read_data((char*)recieve_buf, num_bytes_read))
				continue;
			//pi_serv_cam.listen_conn(3);

			//pi_serv_cam.accept_conn();

			cap >> frame;
			imencode(im_ext, frame, im_buf);

			copy(im_buf.begin(), im_buf.end(), buf);

			while(run_status) {

				// capture video frame and encode into memory buffer
				cap >> frame;
				imencode(im_ext, frame, im_buf);

				copy(im_buf.begin(), im_buf.end(), buf);

				printf("image vector size: %d\n", im_buf.size());

				pi_serv_cam.write_frame((char*)buf);
			}
			terminate_gpio();
			pi_serv_cam.terminate();
    }
  }
}
