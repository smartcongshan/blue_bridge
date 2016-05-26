#include "stc15f2k60s2.h"
#include <stdio.h>
							
#define FOSC	11059200
#define BUAD	115200

/* 延时函数 */
void delay(int z)
{
		int x,y;
		for(x = z;x > 0;x--)
			for(y = 110;y > 0;y--);
}

/* 关闭蜂鸣器 */
void buzzer_off(void)
{
		P2 &= 0x0;
		P2  = 0xa0;
		P0 &= 0x0;
		P2 &= 0x1f;
}

/* 串口初始化函数 */
void uart_init(void)
{
		SCON = 0x50;
		AUXR = 0x40;
		TMOD = 0x00;
		TL1  = (65536 - (FOSC/4/BUAD));
		TH1  = (65536 - (FOSC/4/BUAD))>>8;
		EA   = 1;
		ES   = 1;
		TR1  = 1;
		TI   = 1;
		//REN  = 1;
}

void main()
{
		buzzer_off();
		uart_init();
		while(1)
		{
				printf("abc\n");
				delay(10);
		}
}
/*
void uart(void) interrupt 4 using 1
{
		if(RI)
		{
			RI = 0;
			P0 = SBUF;
		}
		if(TI)
		{
			TI = 0;
			//busy = 0;
		}
}
*/
