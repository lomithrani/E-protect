#include "UART_Buffering.h"

char buffer1[BUFFER_SIZE];
char buffer2[BUFFER_SIZE];
char buffer3[BUFFER_SIZE];
char buffer4[BUFFER_SIZE];

char *buffer1Put = buffer1;
char *buffer1Get = buffer1;

char *buffer2Put = buffer2;
char *buffer2Get = buffer2;

char *buffer3Put = buffer3;
char *buffer3Get = buffer3;

char *buffer4Put = buffer4;
char *buffer4Get = buffer4;

void InitBuffers(void){
	int i=0;

	for(i=0;i<BUFFER_SIZE;i++){
		buffer1[i] = 0;
		buffer2[i] = 0;
		buffer3[i] = 0;
		buffer4[i] = 0;
	}
}

void Reset1(void){
	buffer1Put = buffer1;
	buffer1Get = buffer1;
}
void Reset2(void){
	buffer2Put = buffer2;
	buffer2Get = buffer2;
}
void Reset3(void){
	buffer3Put = buffer3;
	buffer3Get = buffer3;
}
void Reset4(void){
	buffer4Put = buffer4;
	buffer4Get = buffer4;
}

void AppendBuffer1(char c){
	*buffer1Put = c;
    buffer1Put++;

    if(buffer1Put >= (buffer1 + BUFFER_SIZE))
        buffer1Put = buffer1;
}

char GetNext1(void){
    char c = *buffer1Get;
	if(buffer1Get == buffer1Put)
		return 0;
    buffer1Get++;

    if(buffer1Get >= (buffer1 + BUFFER_SIZE))
        buffer1Get = buffer1;

	return c;
}

unsigned int Data1Available(void){
	unsigned int count = 0;
	if(buffer1Get > buffer1Put){
		count = buffer1 + BUFFER_SIZE - buffer1Put;
		count += buffer1Get - buffer1;
	}else{
		count = buffer1Put - buffer1Get;
	}
	return count;

	//if(buffer1Get == buffer1Put)
	//	return 0;
	//else return 1;
}


void AppendBuffer2(char c){
	*buffer2Put = c;
    buffer2Put++;

    if(buffer2Put >= (buffer2 + BUFFER_SIZE))
        buffer2Put = buffer2;
}

char GetNext2(void){
    char c = *buffer2Get;
	if(buffer2Get == buffer2Put)
		return 0;
    buffer2Get++;

    if(buffer2Get >= (buffer2 + BUFFER_SIZE))
        buffer2Get = buffer2;

	return c;
}

unsigned int Data2Available(void){
	unsigned int count = 0;
	if(buffer2Get > buffer2Put){
		count = buffer2 + BUFFER_SIZE - buffer2Put;
		count += buffer2Get - buffer2;
	}else{
		count = buffer2Put - buffer2Get;
	}
	return count;
}


void AppendBuffer3(char c){
	*buffer3Put = c;
    buffer3Put++;

    if(buffer3Put >= (buffer3 + BUFFER_SIZE))
        buffer3Put = buffer3;
}

char GetNext3(void){
    char c = *buffer3Get;
	if(buffer3Get == buffer3Put)
		return 0;
    buffer3Get++;

    if(buffer3Get >= (buffer3 + BUFFER_SIZE))
        buffer3Get = buffer3;

	return c;
}

unsigned int Data3Available(void){
	unsigned int count = 0;
	if(buffer3Get > buffer3Put){
		count = buffer3 + BUFFER_SIZE - buffer3Put;
		count += buffer3Get - buffer3;
	}else{
		count = buffer3Put - buffer3Get;
	}
	return count;
}


void AppendBuffer4(char c){
	*buffer4Put = c;
    buffer4Put++;

    if(buffer4Put >= (buffer4 + BUFFER_SIZE))
        buffer4Put = buffer4;
}

char GetNext4(void){
    char c = *buffer4Get;
	if(buffer4Get == buffer4Put)
		return 0;
    buffer4Get++;

    if(buffer4Get >= (buffer4 + BUFFER_SIZE))
        buffer4Get = buffer4;

	return c;
}

unsigned int Data4Available(void){
	unsigned int count = 0;
	if(buffer4Get > buffer4Put){
		count = buffer4 + BUFFER_SIZE - buffer4Put;
		count += buffer4Get - buffer4;
	}else{
		count = buffer4Put - buffer4Get;
	}
	return count;
}
