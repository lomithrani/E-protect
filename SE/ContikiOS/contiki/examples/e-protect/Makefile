base: e-protect-base
sensorA: e-protect-sensor-0
sensorB: e-protect-sensor-1
sensorC: e-protect-sensor-2
sensorD: e-protect-sensor-3
ilyas:	test-ilyas

#APPS=servreg-hack
#uart_functions
CONTIKI=../..

APPS+=aes
APPS+=simple-json
APPS+=uart_functions

WITH_UIP6=1
UIP_CONF_IPV6=1
CFLAGS+= -Os -DUIP_CONF_IPV6_RPL

include $(CONTIKI)/Makefile.include
