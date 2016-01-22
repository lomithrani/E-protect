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
#include <stdio.h>
#include <string.h>

#define MY_ID 	0
#define MY_TYPE SENSOR_PIR

#define	EMERGENCY_INTERVAL 	(20 * CLOCK_SECOND)
#define SEND_INTERVAL		(3 * CLOCK_SECOND)
#define SENSOR_INTERVAL		(1 * CLOCK_SECOND)
#define SEND_TIME			(random_rand() % (2000))

static int counter=0, waitEmergency=0, status=0;

static struct simple_udp_connection broadcast_connection;
static struct timer wait_timer;

void sendSensorStatus(int status);
int atoi(const char *str);

unsigned long timeIntrusion=0;

ISR(PCINT0_vect)
{
	if(!waitEmergency)
	{
		counter++;
		if(counter>2)
		{
			if(DEBUG) printf("On detecte l'intrusion et on l'envoi à la base\n");

			waitEmergency=1;
			timeIntrusion=clock_seconds();

			counter=0;
		}
	}  
}

/*---------------------------------------------------------------------------*/
PROCESS(check_sensor_status, "check_sen_stat");
AUTOSTART_PROCESSES(&check_sensor_status);
/*---------------------------------------------------------------------------*/
static void receiver(struct simple_udp_connection *c, const uip_ipaddr_t *sender_addr, uint16_t sender_port, const uip_ipaddr_t *receiver_addr, uint16_t receiver_port, const uint8_t *data, uint16_t datalen)
{
	char cmd = jsonData(C(data), "cmd")[0];

	if(cmd == 's'){ /* REQUEST STATUS */
		if(atoi(jsonData(C(data), "id")) == MY_ID)
			sendSensorStatus(STATUS_OK);
	}else if(cmd == 'd'){ /* DISABLE SENSOR */

	}else if(cmd == 'e'){ /* ENABLE SENSOR */

	}
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(check_sensor_status, ev, data)
{
	static struct etimer periodic_timer;

	//const char * message = "Un message de longueur variable\0";
	//const char * mot_de_passe = "azertyuiopqsdfgh";

 	//char cipher[129];

	PROCESS_BEGIN();
	DDRB = 0x00;
	PORTB = 0x00;
	DDRF = 0x00;
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
	timer_set(&wait_timer, SENSOR_INTERVAL);

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
	char string6LOWPAN[90];

	/* GET SENSOR VALUE CHECK IF IT'S FOR A LONG TIME AND UPDATE */
	printf("salut\n");

	uip_create_linklocal_allnodes_mcast(&addr);

	sprintf(string6LOWPAN,"{\"cmd\":\"g\",\"id\":\"%d\",\"st\":\"%d\"}", MY_ID, status);

	simple_udp_sendto(&broadcast_connection, string6LOWPAN, strlen(string6LOWPAN), &addr);
}