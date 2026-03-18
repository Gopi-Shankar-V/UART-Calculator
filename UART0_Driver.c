#include "header.h"

void UART0_INIT(u32 baud){
	u32 result, PCLK, a[] = {15, 30, 60};
	PCLK = a[VPBDIV] * 1000000;
	result = PCLK/(16*baud);

	PINSEL0 |= 0x00000005;
	U0LCR = 0x83;
	U0DLL = (result & 0xFF);
	U0DLM = ((result >> 8) & 0xFF);
	U0LCR = 0x03;
}

#define THRE ((U0LSR >> 5) & 1)
void UART0_TX(u8 data){
	U0THR = data;
	while(THRE == 0);
}

#define RDR (U0LSR & 1)
u8 UART0_RX(void){
	while(RDR == 0);
	return U0RBR;
}

void UART_str(u8 *p){
	while(*p){
		UART0_TX(*p);
		p++;
	}
}

void UART0_int(s32 num){
	s32 a[10], i = 0;
	if(num == 0){
		UART0_TX('0');
	}
	if(num < 0){
		num = -num;
		UART0_TX('-');
	}
	while(num > 0){
		a[i] = num%10;
		num /= 10;
		i++;
	}
	for(i=i-1; i>=0; i--){
		UART0_TX(a[i]+ '0');
	}
}

void UART0_float(float num){
	s32 ipart, frac;
	ipart = (s32)num;
	frac = (num-ipart)*100;
	UART0_int(ipart);
	UART0_TX('.');
	if(frac < 10){
		UART0_TX('0');
	}
	UART0_int(frac);
}
