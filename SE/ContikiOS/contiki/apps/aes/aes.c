#include "aes.h"

#include "contiki.h"

#include <stdio.h>
#include <string.h>

AES_INTERNAL_DATA AES_data;

void AES_set_key(const char key[16]){
	unsigned short i;

	TRXPR &= ~(1 << SLPTR);

	for(i=0;i<16;i++){
		AES_data.key[i] = key[i];
		AES_KEY = AES_data.key[i];
	}

	AES_CTRL &= ~(1 << AES_MODE); // ECB
	AES_CTRL &= ~(1 << AES_DIR); // CRYPT MODE

	for (i=0;i<16;i++){ // PUT DATA TO CRYPT INTO THE REGISTRY
		AES_STATE = 'a';
	}

	AES_CTRL |= (1 << AES_REQUEST); // CRYPT REQUEST

	while ((AES_STATUS & 0x01) != 0x01); // WAIT UNTIL DONE

	for(i=0;i<16;i++){ // GET CRYPTED DATA
		AES_data.unkey[i] = AES_KEY;
	}
}

void AES_crypt_data(const char *data, unsigned short len){
	AES_data.done = 0;
	TRXPR &= ~(1 << SLPTR);
	unsigned short i, cur=0, pos=0;


	for(i=0;i<16;i++){
		AES_KEY = AES_data.key[i];
	}

	AES_CTRL &= ~(1 << AES_MODE); // ECB
	AES_CTRL &= ~(1 << AES_DIR); // CRYPT MODE
	//AES_CTRL |= (1 << AES_IM); // IRQ READY ENABLED

	while(cur < len){
		for (i=0;i<16;i++){ // PUT DATA TO CRYPT INTO THE REGISTRY
			if(cur < len)
				AES_STATE = data[cur++];
			else AES_STATE = '\0';
		}
		AES_CTRL |= (1 << AES_REQUEST); // CRYPT REQUEST

		while ((AES_STATUS & 0x01) != 0x01); // WAIT UNTIL DONE

		if((AES_STATUS & (1 << AES_ER)) == (1 << AES_ER))
			printf("ERROR CRYPT\n");

		for(i=0;i<16;i++){ // GET CRYPTED DATA
			AES_data.data[pos++] = AES_STATE;
		}
	}
	AES_data.data[pos] = '\0';

	AES_data.done = 1;
}

void AES_decrypt_data(const char *data, unsigned short len){
	AES_data.done = 0;
	TRXPR &= ~(1 << SLPTR);
	unsigned short i, cur=0, pos=0;

	for(i=0;i<16;i++){
		AES_KEY = AES_data.unkey[i];
	}

	AES_CTRL &= ~(1 << AES_MODE); // ECB
	AES_CTRL |= (1 << AES_DIR); // DECRYPT MODE
	//AES_CTRL |= (1 << AES_IM); // IRQ READY ENABLED

	while(cur < len){
		for (i=0;i<16;i++){ // PUT DATA TO DECRYPT INTO THE REGISTRY
			if(cur < len)
				AES_STATE = data[cur++];
			else AES_STATE = '\0';
		}
		AES_CTRL |= (1 << AES_REQUEST); // DECRYPT REQUEST

		while ((AES_STATUS & 0x01) != 0x01); // WAIT UNTIL DONE

		if((AES_STATUS & (1 << AES_ER)) == (1 << AES_ER))
			printf("ERROR DECRYPT\n");

		for(i=0;i<16;i++){ // GET DECRYPTED DATA
			AES_data.data[pos++] = AES_STATE;
		}
	}
	AES_data.data[pos] = '\0';

	AES_data.done = 1;
}

char * AES_get_result(){
	while(AES_data.done != 1);
	return AES_data.data;
}
