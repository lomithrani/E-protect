#ifndef _UART_FUNCTION
#define _UART_FUNCTION

#define UART_BUFFER_SIZE 64

int uart0_callback(unsigned char c);
int uart1_callback(unsigned char c);

void InitBuffers(void);


char uart0_next(void);
unsigned int uart0_available(void);
unsigned int uart0_getLine(char *data);

void uart0_setLineCallback(void(*callback)(const char*,int));
void uart1_setLineCallback(void(*callback)(const char*,int));

char uart1_next(void);
unsigned int uart1_available(void);
unsigned int uart1_getLine(char *data);


#endif