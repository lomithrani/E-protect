#include <p18f67j94.h> // OR PIC18F65J94
#include "Cfg_18F.h"
#include <usart.h>
#include <stdio.h>

#include <delays.h>

#include "UART_Buffering.h"

#define _XTAL_FREQ 24000000
#define USE_AND_MASKS

#define SPBRG(baud) (int)((((double)_XTAL_FREQ / (double)baud) / 16.0) - 1.0)

// Xtal 20MHz BRG16=0, BRGH=1
#define BAUD115200Q20	10
#define BAUD115200Q24 	12 // SPBRG(115200)
#define BAUD57600Q24	25
#define BAUD9600Q24	   155




/************************************************************************************************************************************************************


                                                         1 - LCDBIAS2/RP29/!WR/RE1							33 - SEG13/AN9/RP11/CTED7/RC2
														 2 - LCDBIAS1/RP28/!RD/RE0							34 - SEG17/SCL1/RP15/CTED8/RC3
														 3 - COM4/SEG28/AN8/RP46/RG0						35 - SEG16/SDA1/RP17/CTED9/RC4
														 4 - COM5/SEG29/AN19/RP39/RG1						36 - SEG12/RP16/CTED10/RC5
														 5 - COM6/SEG30/AN18/C3INA/RP42/RG2					37 - CTED6/PGD/RB7
														 6 - COM7/SEG31/AN17/C3INB/RP43/RG3					38 - VDD
			 6 6 6 6 6 5 5 5 5 5 5 5 5 5 5 4		     7 - !MCLR											39 - OSC1/CLKI/RP10/RA7
		     4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9             8 - SEG26/AN16/C3INC/RP44/RTCC/RG4					40 - OSC2/CLKO/RP6/RA6
		     # # # # # # # # # # # # # # # # 			 9 - VSS											41 - VSS
		 1 #                                 # 48		10 - VCAP											42 - CTED5/PGC/RB6
		 2 #                                 # 47		11 - SEG25/AN5/RP38/RF7								43 - SEG8/RP13/CTED4/RB5
		 3 #                                 # 46		12 - SEG24/AN11/C1INA/RP40/RF6						44 - SEG11/RP12/CTED3/RB4
		 4 #                                 # 45		13 - SEG23/CVREF/AN10/C1INB/RP35/RF5				45 - SEG10/RP7/CTED2/RB3
		 5 #                                 # 44		14 - D+/RF4											46 - SEG9/RP14/CTED1/RB2
		 6 #                                 # 43		15 - D-/RF3											47 - VLCAP2/RP9/RB1
		 7 #                                 # 42		16 - SEG20/AN7/CTMUI/C2INB/RP36/RF2					48 - VLCAP1/RP8/CTED13/INT0/RB0
		 8 #           PIC18F67J94           # 41		17 - VUSB3V3										49 - SEG7/RP27/REFO2/PSP7/RD7
		 9 #                                 # 40		18 - VBAT											50 - SEG6/SCL2/RP26/PSP6/RD6
		10 #                                 # 39		19 - AVDD											51 - SEG5/SDA2/RP25/PSP5/RD5
		11 #                                 # 38		20 - AVSS											52 - SEG4/RP24/PSP4/RD4
		12 #                                 # 37		21 - VREF+/AN3/RP3/RA3								53 - SEG3/RP23/PSP3/RD3
		13 #                                 # 36		22 - SEG21/VREF-/AN2/RP2/RA2						54 - SEG2/RP22/PSP2/RD2
		14 #                                 # 35		23 - SEG18/AN1/RP1/RA1								55 - SEG1/RP21/PSP1/RD1
		15 #                                 # 34		24 - SEG19/AN0/AN1-/RP0/RA0							56 - VSS
		16 #                                 # 33		25 - VSS											57 - VDD
		     # # # # # # # # # # # # # # # # 			26 - VDD											58 - SEG0/RP20/PSP0/RD0
             1 1 1 2 2 2 2 2 2 2 2 2 2 3 3 3			27 - SEG15/AN4/LVDIN/C1INA/C2INA/C3INA/RP5/RA5		59 - LCDBIAS0/RP31/RE7
             7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2			28 - SEG14/AN6/RP4/RA4								60 - COM3/RP34/RE6
														29 - SOSCI/RC1										61 - COM2/RP37/RE5
														30 - SOSCO/SCLKI/PWRLCLK/RC0						62 - COM1/RP32/RE4
														31 - SEG27/RP18/!UOE/CTED11/RC6						63 - COM0/RP33/REFO1/RE3
														32 - SEG22/RP19/CTED12/RC7							64 - LCDBIAS3/RP30/CS/RE2





************************************************************************************************************************************************************/

void HISR(void);

#pragma code InterruptVectorHigh = 0x08
void InterruptVectorHigh (void)
{  _asm
    goto HISR	//Appel du service d'interruption de priorité haute
  _endasm
}


#pragma code

void init(void){
    // RB INPUT
    TRISB = 0xf;

    // RD OUTPUT
    TRISD = 0;
	PORTD = 0;

OSCCON2bits.IOLOCK = 0; // UNLOCK PPS

    // U1RX -> RP7  -------> I/O 45
	RPINR0_1  |= 0x01;
    RPINR0_1  &= 0xf1;
	RPOR6_7   |= 0x10;
    RPOR6_7   &= 0x1f;

    // U1TX -> RP22 -------> I/O 54
	RPINR0_1  |= 0x50;
    RPINR0_1  &= 0x5f;
	RPOR22_23 |= 0x01;
    RPOR22_23 &= 0xf1;

    // U2RX -> RP14 -------> I/O 46
    RPINR2_3  |= 0x03;
    RPINR2_3  &= 0xf3;
    RPOR14_15 |= 0x02;
    RPOR14_15 &= 0xf2;

    // U2TX -> RP23 -------> I/O 53
    RPINR2_3  |= 0x50;
    RPINR2_3  &= 0x5f;
    RPOR22_23 |= 0x20;
    RPOR22_23 &= 0x2f;

    // U3RX -> RP12 -------> I/O 44
    RPINR4_5  |= 0x03;
    RPINR4_5  &= 0xf3;
    RPOR12_13 |= 0x02;
    RPOR12_13 &= 0xf2;

    // U3TX -> RP25 -------> I/O 51
    RPINR4_5  |= 0x60;
    RPINR4_5  &= 0x6f;
    RPOR24_25 |= 0x20;
    RPOR24_25 &= 0x2f;

OSCCON2bits.IOLOCK = 1; // LOCK PPS

	/************************************
	 * TX no interruptions
	 * RX interruptions
	 * Asynchronous mode
	 * 8bits
	 * Continuous RX
	 * High speed
	 * 115200baud // 57600baud
	*************************************/

	Open1USART(USART_TX_INT_OFF & USART_RX_INT_ON & USART_ASYNCH_MODE &  USART_EIGHT_BIT & USART_CONT_RX & USART_BRGH_HIGH, BAUD115200Q24);
	Open2USART(USART_TX_INT_OFF & USART_RX_INT_ON & USART_ASYNCH_MODE &  USART_EIGHT_BIT & USART_CONT_RX & USART_BRGH_HIGH, BAUD115200Q24);
	Open3USART(USART_TX_INT_OFF & USART_RX_INT_ON & USART_ASYNCH_MODE &  USART_EIGHT_BIT & USART_CONT_RX & USART_BRGH_HIGH, BAUD57600Q24);

	
	INTCONbits.PEIE = 1;	// Autorise Interruption Perif
	INTCONbits.GIE  = 1;
	RCONbits.IPEN   = 0;
}

void main(void){
	init();
	InitBuffers();

	Delay10KTCYx(100);

	while(1){

		if(Data1Available() > 1){
			switch(GetNext1()){
			case 1:
				while(Busy2USART());
				putc2USART(GetNext1());
				break;
			case 2:
				while(Busy3USART());
				putc3USART(GetNext1());
				break;
			default:
				break;
			}
		}

		if(Data2Available() > 0){
			while(Busy1USART());
			putc1USART(1);
			while(Busy1USART());
			putc1USART(GetNext2());
		}

		if(Data3Available() > 0){
			while(Busy1USART());
			putc1USART(2);
			while(Busy1USART());
			putc1USART(GetNext3());
		}

	}

	Close1USART();
	Close2USART();
	Close3USART();
}

/************************************************************************
 * 
 * If data is available coming from the RaspberryPi we enter a loop and
 * send at most 5 times of packets of data
 * Bytes are send as couple: 1st byte device address, 2nd data
 * 
*************************************************************************/

#pragma interrupt HISR	// Interruptions de priorité Haute
void HISR(void)
{ 
	if(DataRdy1USART()){
		AppendBuffer1(getc1USART());
	}

	if(DataRdy2USART()){ 
		AppendBuffer2(getc2USART());
	}
	
	if(DataRdy3USART()){
		AppendBuffer3(getc3USART());
	}
}

