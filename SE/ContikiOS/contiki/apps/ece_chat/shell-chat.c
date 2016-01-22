
#include "contiki.h"
#include "contiki-lib.h"

#include "shell.h"
#include "shell-chat.h"
#include "simple-udp.h"

#include <ctype.h>
#include <string.h>
#include <stdio.h>

#define UDP_PORT 1234

static struct simple_udp_connection broadcast_connection;

typedef struct User{
  char name[64];
}User;

User usr = {NULL};

#define __MAX_SIZE 42
char __parsed[__MAX_SIZE+1];

char * getData(const char * data, const char * key){
  char rep = 0;
  int i=0, j=0, offSet=0, dataLen = strlen(data), keyLen = strlen(key);
  __parsed[0] = 0;

  for(i=0;i<dataLen;i++){
    for(j=0;j<keyLen;j++){
      if(i+j<dataLen && data[i+j] == key[j]){rep = 1;}else{rep = 0; break;}
    } if(rep){j += i; break;}
  }
  if(rep){
    while(data[j] != ':' && data[j] != 0) j++; while(data[j] != '"' && data[j] != 0) j++; j++; i=0;
    while(data[j] != '"' && data[j] != 0 && i < (__MAX_SIZE))__parsed[i++] = data[j++];
    __parsed[i] = 0;
  }
  return __parsed;
}

PROCESS(setname_command_process, "setname");
SHELL_COMMAND(setname_command, "setname", "setname <name>: set your name for the chat",
        &setname_command_process);

PROCESS(send_command_process, "send");
SHELL_COMMAND(send_command, "send", "send <message>: Send a message (can't be done before having a name)",
        &send_command_process);

PROCESS_THREAD(setname_command_process, ev, data)
{
  struct shell_command *c;
  char line[42];
  PROCESS_BEGIN();

  sprintf(usr.name, data);
  if(usr.name == NULL || strlen(usr.name) == 0) {
    shell_output_str(&setname_command, "setname <name>: name must be given", "");
  }else{
    sprintf(line,"Your name %s has been set.",usr.name);
    shell_output_str(&setname_command, line, "");
  }
  
  PROCESS_END();
}

PROCESS_THREAD(send_command_process, ev, data)
{
  struct shell_command *c;
  char *msg = data;
  char line[42];
  uip_ipaddr_t addr;
  PROCESS_BEGIN();

  if(usr.name == NULL || strlen(usr.name) == 0) {
    shell_output_str(&send_command, "You must set a name first!", "");
  }else if(msg == NULL || strlen(msg) == 0){
    shell_output_str(&send_command, "send <message>: the message is empty!", "");
  }else{ // SEND THE MESSAGE!
    //sprintf(line,"%s > %s",usr.name,msg);
    sprintf(line,"{\"username\" : \"%s\",\"msg\" : \"%s\"}",usr.name,msg);

    uip_create_linklocal_allnodes_mcast(&addr);
    simple_udp_sendto(&broadcast_connection, "test", 4, &addr);
  printf("%s > %s\n", usr.name, msg);
  }
  
  PROCESS_END();
}

static void receiver(struct simple_udp_connection *c, const uip_ipaddr_t *sender_addr, uint16_t sender_port, const uip_ipaddr_t *receiver_addr,
                uint16_t receiver_port, const uint8_t *data, uint16_t datalen){
  char user[42];
  char msg[42];

  printf("Raw: %s\n",(const char *)data);

  //fscanf((const char*)data,"{\"username\" : \"%[^\"]s\",\"msg\" : \"%[\"]s\"}",user,msg);
  strcpy(user, getData((const char *)data,"username"));
  strcpy(msg, getData((const char *)data,"msg"));


  printf("%s > %s\n", user, msg);
}

void chat_init(void){
  shell_register_command(&setname_command);
  shell_register_command(&send_command);

  simple_udp_register(&broadcast_connection, UDP_PORT, NULL, UDP_PORT, receiver);
}