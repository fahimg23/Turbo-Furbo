#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>

#include <signal.h>

#include <unistd.h>

#include "Server.h"
#include "Client.h"

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

const char pi_ip[IP_ADDR_LEN] = "74.105.118.23"; // pi ip address
int camera_port = 65535;

int pi;
int run_status;

const string im_ext = ".jpg"; // image extension
const string im_name = "video stream";

int main(int argc, char* argv[])
{
	int stat;

	// Video stream
	Mat recieve_frame;
	uchar recieve_buf[MAX_BUFFER];
	vector<uchar> im_buf;

	run_status = 1;

	printf("%s\n", pi_ip);

	Client pi_cli_cam(camera_port, pi_ip);

	pi_cli_cam.initialize();

	while(!pi_cli_cam.connect_serv())
		continue;

	printf("Connected to camera server!\n");

	namedWindow(im_name, WINDOW_AUTOSIZE);

	while(run_status) {
		pi_cli_cam.read_data((char*)recieve_buf);

		vector<uchar> im_dec_buf((uchar*)recieve_buf, (uchar*)recieve_buf + MAX_BUFFER);
		Mat dec_frame(im_dec_buf);

		// Convert image buffer into image and show image
		try {
			imdecode(dec_frame, 1, &recieve_frame);
		} catch (const cv::Exception& e) {
			printf("Recieved bad frame data... Displaying previous frame. \n");
		}

		imshow(im_name, recieve_frame);

		waitKey(5);
	}

	pi_cli_cam.terminate();
}
