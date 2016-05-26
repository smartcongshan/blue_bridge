#include "stc15f2k60s2.h"
#include <stdio.h>
#include "iic.h"
							
#define FOSC	11059200
#define BUAD	115200

int value;

/* ???? */
void delay(int z)
{
		int x,y;
		for(x = z;x > 0;x--)
			for(y = 110;y > 0;y--);
}

/* ????? */
void buzzer_off(void)
{
		P2 &= 0x0;
		P2  = 0xa0;
		P0 &= 0x0;
		P2 &= 0x1f;
}

/* ??????? */
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
void PCF_8591_init(void)
{
	IIC_Start();
	IIC_SendByte(0x90);
	IIC_WaitAck();
	IIC_SendByte(0x03);
	IIC_WaitAck();
	IIC_Stop();
	delay(10);
}
int PCF_8591_ad(void)
{
	unsigned char ret;
	IIC_Start();
	IIC_SendByte(0x91);
	IIC_WaitAck();
	ret = IIC_RecByte();
	IIC_Ack(0);
	IIC_Stop();
	return (int)ret;
}
int main()
{
			uart_init();
	buzzer_off();

	PCF_8591_init();
	while(1)
	{
			value = PCF_8591_ad();
			printf("%d\n",value);
			delay(100);
	}
	return 0;
	
}
void uart1(void)interrupt 4
{
	if(RI)
	{
		
		RI = 0;
		
	}
	if(TI)
	{
		//TI = 0;
	
	}
}
