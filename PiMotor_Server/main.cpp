#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>

#include <signal.h>

#include <unistd.h>

#include <pigpio.h>
#include <pigpiod_if2.h>

#include "DcMotor.h"
#include "Server.h"
#include "Client.h"

using namespace std;

#define len_arr(arr) sizeof(arr) / sizeof(arr[0])

#define WHEEL_GPIO 18 // pin 12

const char pi_ip[IP_ADDR_LEN] = "192.168.1.164"; // pi ip address
int motor_control_port = 65534;

int pi;
int run_status;

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
	char read_data[MAX_BUFFER];
	char ip[IP_ADDR_LEN];
	char move;


	strcpy(ip, pi_ip);
	run_status = 1;
	pi = pigpio_start(ip, NULL);

	if(pi) {
		if(gpioInitialise() != PI_INIT_FAILED) {

			printf("GPIO initialization succeeded.\n");

			gpioSetSignalFunc(SIGINT, sigintHandler);

			DcMotor* wheel_motor = new DcMotor(WHEEL_GPIO);

			wheel_motor->set_pwm_freq(400);
			wheel_motor->set_pwm_dc(20000);

			printf("%s\n", pi_ip);

			// Initialize server for motor control
			Server pi_serv_motor(motor_control_port, pi_ip);

			pi_serv_motor.initialize();
			pi_serv_motor.listen_conn(3);

			pi_serv_motor.accept_conn();

			printf("Connected to client!\n");

			while(run_status) {

				pi_serv_motor.read_data(read_data);

				printf(" printing move %c\n", read_data[0]);

				move = read_data[0];

				if(move == 'm'){ // 'm' for move
					if (!wheel_motor->get_state()) // if motor is off, turn on
						wheel_motor->start();
				}

				else {
					/* If 'm' is not continuously recieved (basically user is holding down the button and lets go), stop */
					if(wheel_motor->get_state())
						wheel_motor->stop();
				}

				sleep(0.05); // wait 50 milliseconds for the next 'm' to be received
			}
			wheel_motor->stop();
			terminate_gpio();
			pi_serv_motor.terminate();
    }
  }
}
