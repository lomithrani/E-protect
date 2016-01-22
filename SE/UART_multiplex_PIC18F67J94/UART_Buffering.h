#ifndef UART_BUFFERING
#define UART_BUFFERING

#define BUFFER_SIZE 64

void InitBuffers(void);

void Reset1(void);
void Reset2(void);
void Reset3(void);
void Reset4(void);

void AppendBuffer1(char);
char GetNext1(void);
unsigned int Data1Available(void);

void AppendBuffer2(char);
char GetNext2(void);
unsigned int Data2Available(void);

void AppendBuffer3(char);
char GetNext3(void);
unsigned int Data3Available(void);

void AppendBuffer4(char);
char GetNext4(void);
unsigned int Data4Available(void);

#endif