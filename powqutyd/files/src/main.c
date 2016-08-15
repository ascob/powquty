/*
 * main.c
 *
 *  Created on: Jun 16, 2016
 *      Author: neez
 */
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include "PQ_App.h"
#include "mqtt.h"
#include "calculation.h"
#include "config.h"

static volatile int stop_main = 0;

void handle_signal()
{
	stop_mosquitto();
	stop_calculation();
	stop_main = 1;
	publish_device_offline();
}


int main (int argc, char *argv[]) {
	signal(SIGINT, handle_signal);
	signal(SIGTERM, handle_signal);
	char* config_file = "/etc/powqutyd/powqutyd.cfg";

	if(load_config(config_file)){
		printf("Error: could not load some config from %s\n", config_file);
		// return -1;
	}

	// PQ_ERROR err = PQ_NO_ERROR;
	printf("Starting powqutyd ...\n");
	if(!mqtt_init()) {
		printf("MQTT Thread started \n");
	} else {
		printf("couldn't start MQTT-Thread\n");
		// return -1;
	}


	if(!calculation_init()) {
		printf("Calculation Thread started\n");
		publish_device_online();
	}

	while (!stop_main){

	}

	return 0;
}

