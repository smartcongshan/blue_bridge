#include <REGX52.H>
#include "stc15f2k60s2"
#include "iic.h"

code unsigned char du[] = { 0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};

/* ???? */
void delay(int z)
{
		int x,y;
		for(x = z ;x >0 ;x--)
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

/* ?? */
void duan(int x)
{
		P2 &= 0x0; 
		P2  = 0xe0;
		P0 |= 0xff;
		P0  = du[x];
		P2 &= 0x1f;
}

/* ?? */
void wei(int x)
{
		x = 7 - x;
		P2 &= 0x0;
		P2  = 0xc0;
		P0 &= 0x0;
		P0 |= (1<<x);
		P2 &= 0x1f;
		P0 &= 0x00;
}

/* ??????? */

void display(long int x)
{
		int i;
		int k = 1;
		for(i = 0;i < 8;i++)
		{
				wei(i);
				duan(x%10);
				delay(20);
				x = x/10;
		}
}

void at24c02_wr(unsigned char add,unsigned char val)
{
		IIC_Start();
		IIC_SendByte(0xa0);
		IIC_WaitAck();
		IIC_SendByte(add);
		IIC_WaitAck();
		IIC_SendByte(val);
		IIC_WaitAck();
		IIC_Stop();
}

unsigned char at24c02_rd(unsigned char add)
{
		unsigned char ret;
		IIC_Start();
		IIC_SendByte(0xa0);
		IIC_WaitAck();
		IIC_SendByte(add);
		IIC_WaitAck();
		IIC_Start();
		IIC_SendByte(0xa1);
		IIC_WaitAck();
		ret = IIC_RecByte();
		IIC_Ack(1);	
		IIC_Stop();
		return ret;
}

void main()
{
		unsigned char i;
		buzzer_off();
//	i = at24c02_rd(0x00);
	delay(100);
	//	at24c02_wr(0x00,100);
		delay(10);
			i = at24c02_rd(0x00);
		while(1)
		{
			
				delay(10);
				display(i);
		}
}
