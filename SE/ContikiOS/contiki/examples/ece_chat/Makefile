CONTIKI = ../..
all : ece_chat
# Add your application
#APPS += ece_chat
APPS += shell
APPS += serial-shell
APPS += ece_chat
APPS += servreg-hack

WITH_UIP6 = 1
UIP_CONF_IPV6 = 1

# Set compilation optimization parameter
CFLAGS += -Os -DUIP_CONF_IPV6_RPL

# Add your project config file which contains global parameters
#CFLAGS += -DPROJECT_CONF_H=\"project-conf.h\"

# Include Contiki makefile
include $(CONTIKI)/Makefile.include
