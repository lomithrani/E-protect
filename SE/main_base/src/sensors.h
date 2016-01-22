#ifndef SENSORS_H
#define SENSORS_H

#include "utils/functions.h"

#define STATUS_OK 			0
#define STATUS_WARNING 		1
#define STATUS_LOW_BAT 		2
#define STATUS_ERROR 		3
#define STATUS_EMERGENCY 	4
#define STATUS_TIMEOUT		5

#define SENSOR_NONE			0
#define SENSOR_PIR			1
#define SENSOR_CHOCK		2
#define SENSOR_RFID			3

class Sensor{
public:
	Sensor(char type = 0, char status = 0, UString name = "");
	
	char type;
	char status;
	UString name;
};

extern SList<Sensor*> sensorsList;

#endif