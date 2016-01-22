#include "sensors.h"

SList<Sensor*> sensorsList;


Sensor::Sensor(char type, char status, UString name){
	this->type = type;
	this->status = status;
	this->name = name;
}