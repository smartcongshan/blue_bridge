#include "stc15f2k60s2.h"
#include "iic.h"
#include <stdio.h>

typedef unsigned char uchar;
typedef unsigned int uint;
#define FOSC	11059200
#define T1MS	(65536 - (FOSC/1000))
#define BUAD	115200

uchar du[] = { 0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
int t,write;
unsigned char cnt;

/* ???? */
void delay(int z)
{
	while(z--);
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
void start()
{
	     SDA=1;
         SCL=1;	
		 delay(5);
		 SDA=0;
		 delay(5);
}

void stop()
{
		 SDA=0;
         SCL=1;
		 delay(5);
		 SDA=1;
		 delay(5);
}

void respon()
{
        uchar i;
		SCL=1;
	//	delay(5);
		while((SDA==1)&&(i<250))
		{
		    i++;
		}
		SCL=0;
		delay(5);
}

void write_byte(uchar date)
{
		 uchar i;
		 for(i=0;i<8;i++)
		 {
		       SCL=0;   
			   delay(5);
			   SDA=date&0x80;
			   delay(5);
			   SCL=1;
			   date<<=1;
			   delay(5);
		 }
		 SCL=0;
		 SDA=1;
		 delay(5);

}

uchar read_byte()
{
         uchar i,bb;
		 for(i=0;i<8;i++)
		 {
		        SCL=1;
				bb<<=1;
				delay(5);
				if(SDA==1)
				{
				      bb|=0x01;
				}
				else
				{
				      bb|=0x00;
				}
				delay(5);
				SCL=0;
				delay(5);
		 }
		 return bb;
}
void write_date(uchar address,date)
{
          start();
		  write_byte(0xa0);
		  respon();
		  write_byte(address);
		  respon();
		  write_byte(date);
		  respon();
		  stop();
}

uint read_date(uchar address)
{
          uint cc;
		  start();
		  write_byte(0xa0);
		  respon();
		  write_byte(address);
		  respon();
		  start();
		  write_byte(0xa1);
		  respon();
		  cc=read_byte();
		  stop();
		  return cc;
}

//void operate_delay(unsigned char t)
//{
//	unsigned char i;
//	
//	while(t--){
//		for(i=0; i<112; i++);
//	}
//}

///* 写eeprom */
//void write_eeprom(unsigned char add,unsigned char val)
//{
//	 IIC_Start();
//    IIC_SendByte(0xa0);
//    IIC_WaitAck();
//    IIC_SendByte(add);
//    IIC_WaitAck();
//    IIC_SendByte(val);
//    IIC_WaitAck();
//    IIC_Stop();
//	//operate_delay(10);
//}
///* 读eeprom */
//unsigned char read_eeprom(unsigned char add)
//{
//		unsigned char ret;
//	
//		IIC_Start();
//		IIC_SendByte(0Xa0);
//		IIC_WaitAck();
//		IIC_SendByte(add);
//		IIC_Start();
//		IIC_SendByte(0xa1);
//		IIC_WaitAck();
//		ret = IIC_RecByte();
//		//IIC_Ack(1);
//		IIC_Stop();
//		return ret;
//}

/* 定时器0初始化 */
void timer0_init(void)
{
	
		AUXR |= 0x80;
		TMOD = 0X00;
		TH0  = T1MS>>8;
		TL0  = T1MS;
		//EA   = 1;
		ET0  = 1;
		TR0  = 1;
		
}
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
		//REN  = 1;
}
void main()
{
		unsigned char x = 0;
		uart_init();	
		buzzer_off();
		timer0_init();
		//init();

		write_date(2,x);
		delay(10);
		cnt = read_date(2);
//		if(cnt > 50)
//		{
//				cnt = 0;
//		}
		while(1)
		{	
				//printf("%d\n",cnt);
				
				//delay(10);
				display(cnt);
					if(write == 1)
					{
						write = 0;
						write_date(2,cnt);
					}		
		}
		
}
void timer0(void) interrupt 1
{
		t++;
		if(t == 1000)
		{
				t = 0;
				cnt++;
				write = 1;
				if(cnt > 100)
					cnt = 0;
		}
}
void uart1(void)interrupt 4
{
	if(RI)
	{
		
		RI = 0;
		
	}
	if(TI)
	{
		TI = 0;
	
	}
}
