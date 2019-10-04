#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>

#include <signal.h>

#include <unistd.h>

#include "Server.h"
#include "Client.h"

using namespace std;

const char pi_ip[IP_ADDR_LEN] = "74.105.118.23"; // pi ip address
int motor_port = 65534;

int run_status = 1;

int main(int argc, char* argv[])
{
	int stat;
	char move;

	run_status = 1;

	printf("%s\n", pi_ip);

	Client pi_cli_mot(motor_port, pi_ip);

	pi_cli_mot.initialize();

	while(!pi_cli_mot.connect_serv())
		continue;

	printf("Connected to server!\n");

	while(run_status) {

		scanf("%c", &move);

		pi_cli_mot.write_data(&move);
	}
}
