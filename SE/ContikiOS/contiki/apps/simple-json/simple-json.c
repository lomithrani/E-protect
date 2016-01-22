#include "simple-json.h"

char __parsed[__MAX_SIZE+1];

char * jsonData(const char * data, const char * key){
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