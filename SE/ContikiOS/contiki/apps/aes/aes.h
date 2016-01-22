#ifndef AES_H_
#define AES_H_

#ifndef AES_STATUS_RY
#define AES_STATUS_RY 0
#endif

#include "contiki.h"

typedef struct AES_INTERNAL_DATA{
	char done;
	char data[129];
	char key[16];
	char unkey[16];
}AES_INTERNAL_DATA;

void AES_set_key(const char key[16]);
void AES_crypt_data(const char *data, unsigned short len);
void AES_decrypt_data(const char *data, unsigned short len);
char * AES_get_result();

#endif