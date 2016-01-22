#include "contiki.h"
#include "lib/random.h"
#include "sys/ctimer.h"
#include "sys/etimer.h"
#include "sys/timer.h"
#include "net/uip.h"
#include "net/uip-ds6.h"

#include "simple-udp.h"

#include "simple-json.h"

#include "uart_functions.h"

#include "dev/rs232.h"

//#include "aes.h"

#include "defines.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static const char allowedKeys[2][15] = {
	"03000897481603",
	"5500094229540F"
};
static const int numKeys = 2;

#define CHECK_INTERVAL	(CLOCK_SECOND / 2) /* 500ms CHECK INTERVAL */

static struct simple_udp_connection broadcast_connection;

void updateDataToServer();
void sendDataSIGFOX(char force);

void sigfoxUART(const char* data, int length);
void baseReadyUART(const char* data, int length);

typedef struct Sensors{
	char type;
	char status;
	char lastTime;
	char failedTimeouts;
}Sensors;

Sensors sensorsList[20];
char string6LOWPAN[90];

static char isAlarmEnabled = 0;

static unsigned long prevTimeDISCONNECTED=0;
static unsigned long prevTimeSIGFOX=0;

char isAllowed(const char * key){
	char rep = 0;
	int i=0, j=0, n=0, dataLen, keyLen = strlen(key);


	for(n=0;n<numKeys;n++){
		dataLen = 14;
		if(DEBUG) printf("[%d]%s ==? [%d]%s\n", keyLen, key, 14, allowedKeys[n]);
		for(i=0;i<keyLen;i++){
			for(j=0;j<dataLen;j++){
				if(i+j<keyLen && allowedKeys[n][j] == key[i+j]){rep = 1;}else{ rep = 0; break; }
			} if(rep == 1){ return 1; }
		}
	}
	if(DEBUG) printf("NO MATCH\n");
	return 0;
}

/*---------------------------------------------------------------------------*/
PROCESS(check_sensor_status, "check_sen_stat");
AUTOSTART_PROCESSES(&check_sensor_status);
/*---------------------------------------------------------------------------*/
static void receiver(struct simple_udp_connection *c, const uip_ipaddr_t *sender_addr, uint16_t sender_port, const uip_ipaddr_t *receiver_addr, uint16_t receiver_port, const uint8_t *data, uint16_t datalen)
{
	C(data)[datalen] = '\0';
	int id;
	char cmd = jsonData(C(data), "cmd")[0];
	uip_ipaddr_t addr;

	if(cmd == 'g'){ /* GET SENSOR STATUS */
		id = atoi(jsonData(C(data),"id"));
		sensorsList[id].status = atoi(jsonData(C(data),"st"));
		if(sensorsList[id].type == SENSOR_NONE)
			sensorsList[id].type = atoi(jsonData(C(data),"ty"));
		sensorsList[id].lastTime = 0;
		sensorsList[id].failedTimeouts = 0;

		updateDataToServer(C(data)); /* SEND DATA TO RASPBERRY PI */
	}else if(cmd == 'c'){
		id = atoi(jsonData(C(data),"id"));
		sensorsList[id].status = atoi(jsonData(C(data),"st"));
		sensorsList[id].lastTime = 0;
		sensorsList[id].failedTimeouts = 0;

		if(isAllowed(jsonData(C(data),"key")) == 0)
			return;

		uip_create_linklocal_allnodes_mcast(&addr);
		if(isAlarmEnabled == 0){ /* ALARM IS DISABLED */
			isAlarmEnabled = 1;
			sprintf(string6LOWPAN,"{\"cmd\":\"l\"}");

		}else{ /*ALARM IS ENABLED*/
			isAlarmEnabled = 0;
			sprintf(string6LOWPAN,"{\"cmd\":\"u\"}");

		}
		simple_udp_sendto(&broadcast_connection, string6LOWPAN, strlen(string6LOWPAN), &addr);

		updateDataToServer(C(data));
		updateDataToServer(C(string6LOWPAN));
	}
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(check_sensor_status, ev, data)
{
	uip_ipaddr_t addr;
	static struct etimer periodic_timer;
	int i=0;
	char globalStatus=0;

	PROCESS_BEGIN();


	for(i=0;i<20;i++){
		sensorsList[i].type = SENSOR_NONE;
		sensorsList[i].status = STATUS_OK;
		sensorsList[i].lastTime = 0;
		sensorsList[i].failedTimeouts = 0;
	}

	sensorsList[0].type = SENSOR_PIR;
	sensorsList[1].type = SENSOR_CHOCK;
	sensorsList[2].type = SENSOR_PIR;
	sensorsList[3].type = SENSOR_RFID;


	DDRB = 0x00;
	PORTB = 0x00;


	//sensorsList[0].type = SENSOR_PIR;
	//sensorsList[1].type = SENSOR_PIR;
	//sensorsList[2].type = SENSOR_CHOCK;

	//rs232_init(RS232_PORT_0, USART_BAUD_19200, USART_PARITY_NONE | USART_STOP_BITS_1 | USART_DATA_BITS_8);
	//rs232_redirect_stdout(RS232_PORT_0);
	//rs232_send(RS232_PORT_n, char c); // OR FOR STRINGS rs232_print(char *);
	rs232_set_input(RS232_PORT_0, uart0_callback);
	uart0_setLineCallback(baseReadyUART);

	rs232_init(RS232_PORT_1, USART_BAUD_9600, USART_PARITY_NONE | USART_STOP_BITS_1 | USART_DATA_BITS_8);
	rs232_set_input(RS232_PORT_1, uart1_callback);
	uart1_setLineCallback(lineReadyUART1);

	simple_udp_register(&broadcast_connection, UDP_PORT, NULL, UDP_PORT, receiver);

	etimer_set(&periodic_timer, CHECK_INTERVAL);
	while(1) {
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
		etimer_reset(&periodic_timer);


		for(i=0;i<20;i++){
			if(sensorsList[i].type != 0){
				sensorsList[i].lastTime++;

				if(sensorsList[i].lastTime == 11){ /* SECONDS / 2 => *0->11* 6 seconds without infos */
					sensorsList[i].failedTimeouts++;
					sensorsList[i].lastTime = 0;

					uip_create_linklocal_allnodes_mcast(&addr);
					sprintf(string6LOWPAN,"{\"cmd\":\"s\",\"id\":\"%d\"}",i);
					simple_udp_sendto(&broadcast_connection, string6LOWPAN, strlen(string6LOWPAN), &addr);


					if(sensorsList[i].failedTimeouts == 3){
						sensorsList[i].failedTimeouts = 0;
						sensorsList[i].status = STATUS_TIMEOUT;
						printf("{\"cmd\":\"g\",\"id\":\"%d\",\"st\":\"%d\",\"ty\":\"%d\"}\n", i, sensorsList[i].status, sensorsList[i].type);
					}
				}
			}
		}
	}

	PROCESS_END();
}
/*---------------------------------------------------------------------------*/

void sendDataSIGFOX(char force){
	int i=0,j=0;
	char dataSIGFOX[10];
	if((prevTimeSIGFOX+60) < clock_seconds() || (force & 0x01) == 0x01){
		prevTimeSIGFOX = clock_seconds();

		for(i=0;i<20;i++){
			if(sensorsList[i].type != SENSOR_NONE && sensorsList[i].status != STATUS_OK){
				if(j<10){
					dataSIGFOX[j++] = i;
					dataSIGFOX[j++] = sensorsList[i].status;
				}
			}
		}

	}
}

void sigfoxUART(const char* data, int length){

}

void baseReadyUART(const char* data, int length){

}

void updateDataToServer(const char * data){
	if((PORTB & 0x01) == 0x01){ /* RASPBERRY IS OK AND INTERNET OK */
		printf("%s\n",C(data));
	}else{ /* SEND DATA WITH SIGFOX*/
		sendDataSIGFOX(0);
	}
}