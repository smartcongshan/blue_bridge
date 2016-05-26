#include "stc15f2k60s2.h"

#define FOSC	11059200L
#define T01MS	(65536 - FOSC/1000) //TIM0不分频

code unsigned char du[] = { 0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};

int i,t = 0;

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
/* 段选 */
void duan(int x)
{
		P2 &= 0x0;
		P2  = 0xe0;
		P0  = 0xff;
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
		P0  = 1<<x;
		P2 &= 0x1f;
}
/* 显示函数 */
void display(int x)
{
		int i;
		for(i = 0;i < 8;i++)
		{
				wei(i);
				duan(x%10);
				delay(20);
				x = x/10;
		}
}
/* 定时器初始化 */
void time0_init(void)
{
		AUXR |= 0x80; //定时器0不分频
		TMOD  = 0x00; //定时器0方式0
		TL0   = T01MS;
		TH0   = T01MS>>8;
		EA    = 1;
		TR0   = 1;
		ET0   = 1;
}
void main()
{
		int b;
		buzzer_off();
		time0_init();
		while(1)
		{
				display(i);
		}
}
void time0(void) interrupt 1
{
		t++;
		if(t == 1000)
		{
				t = 0;
				i++;
		}
}
