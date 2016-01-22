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
#include <avr/io.h>
#include "contiki-conf.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/pgmspace.h>

#define MY_ID 	3
#define MY_TYPE SENSOR_RFID

#define SEND_INTERVAL		(3 * CLOCK_SECOND)
#define SENSOR_INTERVAL		(1 * CLOCK_SECOND)
#define SEND_TIME			(random_rand() % (2000))
#define LED_GREEN_ON		(PORTF=1)
#define LED_RED_ON			(PORTF=2)


static unsigned long timeIntrusion=0, timeSending=0;
static int waitEmergency=0, status=0, led=0;
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
		LED_GREEN_ON;
	}else if(cmd == 'l'){
		// LED ON == LOCKED
		PORTF=1;
		if(DEBUG) printf("[Message  from base] Lock \n");
		LED_RED_ON;
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

	//turn on Green led
	LED_GREEN_ON;

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
		
		if(!sending)
		{
			sendSensorStatus(status);
		}else{
			if(((timeSending+3)<clock_seconds()))
				sending=0;
		}

		/*
		if(led==0)
		{
			printf("GREEN LED ON\n");
			LED_GREEN_ON;
			led=1;
		}else{
			printf("RED LED ON\n");
			LED_RED_ON;
			led=0;
		}*/
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
	if(sending)
		return;

	sending=1;

	timeSending=clock_seconds();

	if(DEBUG){
		printf("RFID code received %s ", data);
		if(DEBUG==2) printf("HEX: %X",(unsigned int)data);
		printf("\n");
	}

	uip_ipaddr_t addr;
	//char string6LOWPAN[90];

	/* GET SENSOR VALUE CHECK IF IT'S FOR A LONG TIME AND UPDATE */
	if(DEBUG) printf("send code ...\n");

	uip_create_linklocal_allnodes_mcast(&addr);

	if(DEBUG==2) printf("string6Lowpan: %s\n", string6LOWPAN);
	sprintf(string6LOWPAN,"{\"cmd\":\"c\",\"id\":\"%d\",\"st\":\"%d\",\"key\":\"%s\",\"ty\":\"%d\"}", MY_ID, STATUS_OK, data, MY_TYPE);

	simple_udp_sendto(&broadcast_connection, string6LOWPAN, strlen(string6LOWPAN), &addr);
}