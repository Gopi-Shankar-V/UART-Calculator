#include "header.h"
#include <limits.h>
int main() {
    u8 op[50];
    long long int num[50], nn = 0, result;
    int ni = 0, oi = 0, i;
    u8 t;

    UART0_INIT(9600);

    while(1) {
        t = UART0_RX();//
        UART0_TX(t);   
	
        if(t >= '0' && t <= '9') {
			if(nn > (LLONG_MAX - (t-'0'))/10){
				UART_str("\nBuffer Overflow!\n");
				nn = 0;
				ni = oi = 0;
				continue;
			}
	            nn = (nn * 10) + (t - '0');
        }

        else if(t=='+' || t=='-' || t=='*' || t=='/' || t=='%') {
            num[ni++] = nn;
            op[oi++] = t;
            nn = 0;
        }

        else if(t == 13) {   
            num[ni++] = nn; 
			result = num[0];

            for(i = 0; i < oi; i++) {
                switch(op[i]) {
                    case '+':
							if(result > LLONG_MAX - num[i+1]){
							UART_str("\nBuffer Overflow!\n");
							break;	
							} 
							result += num[i+1]; 
							break;

                    case '-':
							if(result > LLONG_MAX + num[i+1]){
							UART_str("\nBuffer Overflow!\n");
							break;	
							}
							result -= num[i+1];
							break;

                    case '*':
							if(result > LLONG_MAX / num[i+1]){
								UART_str("\nBuffer Overflow!\n");
								break;	
							}
					 		result *= num[i+1]; 
							break;

                    case '/': 
							if(num[i+1] == 0){
								UART_str("\nDivide By Zero!\n");
								break;	
							}
							result /= num[i+1]; 
							break;

                    case '%': result %= num[i+1]; 
							break;
                }
            }
			UART_str("\n");
            UART0_int(result);
            UART_str("\n");

            ni = 0;
            oi = 0;
            nn = 0;
        }
    }
}
