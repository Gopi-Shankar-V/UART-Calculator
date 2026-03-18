#include <lpc21xx.h>

typedef unsigned char u8;
typedef unsigned short int u16;
typedef unsigned int u32;
typedef signed int s32;

typedef signed short int s16;
typedef signed char s8; 

extern void delay_ms(u32);
extern void delay_sec(u32);

extern void UART0_INIT(u32);
extern void UART0_TX(u8);
extern u8 UART0_RX(void);
extern void UART_str(u8 *);
extern void UART0_int(s32);
extern void UART0_float(float);
