#include "stc15f2k60s2.h"
#include "ds1302.h"

code unsigned char du[] = { 0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};

unsigned char sec,hr,min;
unsigned int second,hour,minute;
unsigned long int dis;

/* 延时函数 */
void delay(int z)
{
		int x,y;
		for(x = z ;x >0 ;x--)
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

/* 段选 */
void duan(int x)
{
		P2 &= 0x0; 
		P2  = 0xe0;
		P0 |= 0xff;
		P0  = du[x];
		P2 &= 0x1f;
}

/* 位选 */
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

/* 显示函数 */

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
/* 时钟芯片的初始化 */
void ds1302_init(void)
{
		Ds1302_Single_Byte_Write(0x8e,0x00);
		Ds1302_Single_Byte_Write(ds1302_sec_addr,(0));
		Ds1302_Single_Byte_Write(ds1302_min_addr,((58/10)<<4)|(58%10));
		Ds1302_Single_Byte_Write(ds1302_hr_addr ,((11/10)<<4)|(11%10));
		Ds1302_Single_Byte_Write(0x8e,0x80);
	
}

/* 时钟芯片读取函数 */
void ds1302_rd(void)
{
		sec = Ds1302_Single_Byte_Read(0x81);
		Ds1302_Single_Byte_Write(0x00,0x00);
		second = ((sec&0x70)>>4)*10 + (sec&0x0f);
		min = Ds1302_Single_Byte_Read(0x83);
		Ds1302_Single_Byte_Write(0x00,0x00);
		minute = ((min&0x70)>>4)*10 + (min&0x0f);
		hr  = Ds1302_Single_Byte_Read(0x85);
		Ds1302_Single_Byte_Write(0x00,0x00);
		hour = ((hr&0x70)>>4)*10 + (hr&0x0f);
}

void main()
{
		ds1302_init();
		buzzer_off();
		while(1)
		{
				ds1302_rd();
				dis = hour*1000000 + minute*1000 + second;
				display(dis);
		}
}
