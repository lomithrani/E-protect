#include "contiki.h"
#include "lib/random.h"
#include "sys/ctimer.h"
#include "sys/etimer.h"
#include "sys/timer.h"
#include "net/uip.h"
#include "net/uip-ds6.h"
#include <avr/interrupt.h>
#include "simple-udp.h"
#include "simple-json.h"
#include "defines.h"
#include "uart_functions.h"
#include "dev/rs232.h"
#include "dev/slip.h"
#include "dev/spi.h"
#include <avr/io.h>
#include "contiki-conf.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/pgmspace.h>

#define MY_ID 	1
#define MY_TYPE SENSOR_CHOCK

#define SEND_INTERVAL		(3 * CLOCK_SECOND)
#define SENSOR_INTERVAL		(1 * CLOCK_SECOND)
#define SEND_TIME			(random_rand() % (2000))

static unsigned long timeIntrusion=0, timeSending=0;
static int waitEmergency=0, status=0;
static char sending=0, string6LOWPAN[90];

static struct simple_udp_connection broadcast_connection;
static struct etimer periodic_timer;

void sendSensorStatus(int status);
void lineReadyUART0(const char* data, int length);


/*---------------------------------------------------------------------------*/
	PROCESS(check_sensor_status, "check_sen_stat");
	AUTOSTART_PROCESSES(&check_sensor_status);
/*---------------------------------------------------------------------------*/
	static void receiver(struct simple_udp_connection *c, const uip_ipaddr_t *sender_addr, uint16_t sender_port, const uip_ipaddr_t *receiver_addr, uint16_t receiver_port, const uint8_t *data, uint16_t datalen)
	{
		C(data)[datalen] = '\0';
		char cmd = jsonData(C(data), "cmd")[0];

	if(cmd == 's'){ /* REQUEST STATUS */
		if(atoi(jsonData(C(data), "id")) == MY_ID)
			sendSensorStatus(STATUS_OK);
	}else if(cmd == 'u'){
		// LED OFF
		PORTF=0;
		if(DEBUG) printf("[Message from base] Unlock \n");
	}else if(cmd == 'l'){
		// LED ON == LOCKED
		PORTF=1;
		if(DEBUG) printf("[Message from base] Lock \n");
	}
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(check_sensor_status, ev, data)
{

	//const char * message = "Un message de longueur variable\0";
	//const char * mot_de_passe = "azertyuiopqsdfgh";

 	//char cipher[129];

	PROCESS_BEGIN();
	DDRB = 0x00;
	PORTB = 0x00;
	DDRF = 0xFF;
	PORTF = 0x00;

	//Activation of Interrupt
	PCICR |= (1<<PCIE0);
	//Enable interrupt when PORTB.0 state change 
	PCMSK0 |= (1<<PCINT0);

/*
	AES_set_key(mot_de_passe);


	printf("Raw message [%d]: %s\n", strlen(message), message);
	AES_crypt_data(message, strlen(message));

	strcpy(cipher, AES_get_result());
	printf("Crypt data [%d]: %s\n", strlen(cipher), cipher);

	AES_decrypt_data(cipher, strlen(cipher));
	printf("Decrypted data: %s\n", AES_get_result());
*/

	simple_udp_register(&broadcast_connection, UDP_PORT, NULL, UDP_PORT, receiver);

	etimer_set(&periodic_timer, SEND_INTERVAL);
	
	rs232_init(RS232_PORT_0, USART_BAUD_9600, USART_PARITY_NONE | USART_STOP_BITS_1 | USART_DATA_BITS_8);
	rs232_set_input(RS232_PORT_0, uart0_callback);
	uart0_setLineCallback(lineReadyUART0);
		

	while(1) {

		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
		etimer_reset(&periodic_timer);
		
		if(waitEmergency)
		{
			status=STATUS_EMERGENCY;
			if(((timeIntrusion+20)>clock_seconds()))
			{
				if(DEBUG) printf("Je suis dans le waitEmergency Time: %lu  timeIntrusion: %lu diff: %lu\n", 
							clock_seconds(),
							timeIntrusion,
							((timeIntrusion+20)-clock_seconds()));
			}else{
				if(DEBUG) printf("On est en dehors de la boucle waitEmergency %lu",clock_seconds());
				waitEmergency=0;
				status=STATUS_OK;
			}
		}
		sendSensorStatus(status);
	}

	PROCESS_END();
}
/*---------------------------------------------------------------------------*/

void sendSensorStatus(int status){
	uip_ipaddr_t addr;
	//char string6LOWPAN[90];

	/* GET SENSOR VALUE CHECK IF IT'S FOR A LONG TIME AND UPDATE */
	if(DEBUG==2) printf("send status...\n");

	uip_create_linklocal_allnodes_mcast(&addr);

	sprintf(string6LOWPAN,"{\"cmd\":\"g\",\"id\":\"%d\",\"st\":\"%d\",\"ty\":\"%d\"}", MY_ID, status, MY_TYPE);

	simple_udp_sendto(&broadcast_connection, string6LOWPAN, strlen(string6LOWPAN), &addr);
}

void lineReadyUART0(const char* data, int length)
{
	C(data)[6]=0;

	if(DEBUG){
		printf("%s tap detected\n", data);
	}

	if(!strcmp(data,"single") || !strcmp(data,"double"))
	{
		if(DEBUG) printf("Choc détecté \n");
		waitEmergency=1;
		timeIntrusion=clock_seconds();
	}
}