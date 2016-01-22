#include "uart_functions.h"
#include <stdio.h>


char uart0_buffer[UART_BUFFER_SIZE];
char uart1_buffer[UART_BUFFER_SIZE];

char *uart0_put = uart0_buffer;
char *uart0_get = uart0_buffer;

char *uart1_put = uart1_buffer;
char *uart1_get = uart1_buffer;

void (*uart0_lineCallback)(const char*,int) = 0;
void (*uart1_lineCallback)(const char*,int) = 0;

char uart0_line[UART_BUFFER_SIZE];
char uart1_line[UART_BUFFER_SIZE];

void InitBuffers(void){
    int i=0;

    for(i=0;i<UART_BUFFER_SIZE;i++){
        uart0_buffer[i] = 0;
        uart1_buffer[i] = 0;
    }

    uart0_put = uart0_buffer;
    uart0_get = uart0_buffer;

    uart1_put = uart1_buffer;
    uart1_get = uart1_buffer;
}

void uart0_setLineCallback(void(*callback)(const char*,int)){
    uart0_lineCallback = callback;
}

void uart1_setLineCallback(void(*callback)(const char*,int)){
    uart1_lineCallback = callback;
}

int uart0_callback(unsigned char c){
    *uart0_put = c;
    uart0_put++;

    //printf("%c",c);
    if(uart0_lineCallback != 0 && c == '\n'){
        int i=0;
        while(uart0_available()){
            char n = uart0_next();
            if(n == '\n'){
                break;
            }else{
                uart0_line[i++] = n;
            }
        }
        uart0_line[i] = '\0';
        uart0_lineCallback(uart0_line,i);
    }

    if(uart0_put >= (uart0_buffer + UART_BUFFER_SIZE))
        uart0_put = uart0_buffer;
    return 0;
}

char uart0_next(){
    char c = *uart0_get;
    if(uart0_get == uart0_put)
        return 0;
    uart0_get++;

    if(uart0_get >= (uart0_buffer + UART_BUFFER_SIZE))
        uart0_get = uart0_buffer;

    return c;
}

unsigned int uart0_available(){
    unsigned int count = 0;
    if(uart0_get > uart0_put){
        count = uart0_buffer + UART_BUFFER_SIZE - uart0_put;
        count += uart0_get - uart0_buffer;
    }else{
        count = uart0_put - uart0_get;
    }
    return count;
}

unsigned int uart0_getLine(char *data){
    return 0;
}

int uart1_callback(unsigned char c){
    *uart1_put = c;
    uart1_put++;

    if(uart1_lineCallback != 0 && c == '\n'){
        int i=0;
        while(uart1_available()){
            char n = uart1_next();
            if(n == '\n'){
                break;
            }else{
                uart1_line[i++] = n;
            }
        }
        uart1_line[i] = '\0';
        uart1_lineCallback(uart1_line,i);
    }

    if(uart1_put >= (uart1_buffer + UART_BUFFER_SIZE))
        uart1_put = uart1_buffer;
    return 1;
}

char uart1_next(){
        char c = *uart1_get;
    if(uart1_get == uart1_put)
        return 0;
    uart1_get++;

    if(uart1_get >= (uart1_buffer + UART_BUFFER_SIZE))
        uart1_get = uart1_buffer;

    return c;
}

unsigned int uart1_available(){
    unsigned int count = 0;
    if(uart1_get > uart1_put){
        count = uart1_buffer + UART_BUFFER_SIZE - uart1_put;
        count += uart1_get - uart1_buffer;
    }else{
        count = uart1_put - uart1_get;
    }
    return count;
}

unsigned int uart1_getLine(char *data){
    return 0;
}