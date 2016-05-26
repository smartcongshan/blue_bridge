#include "stc15f2k60s2.h"
#include <intrins.h>

sbit TX = P1^0;
sbit RX = P1^1;

code unsigned char du[] = { 0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};

/* 关蜂鸣器 */
void buzzer_off(void)
{
		P2 &= 0x0;
		P2  = 0xa0;
		P0 &= 0x0;
		P2 &= 0x1f;
}
/* 延时函数 */
void delay(int z)
{
		int x,y;
		for(x = z;x > 0;x--)
			for(y = 110;y > 0;y--);
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
		P0  = (1<<x);
		P2 &= 0x1f;
}
/* 显示 */
void display(int x)
{
		int i;
		for(i = 0;i < 8;i++)
		{
				wei(i);
				duan(x%10);
				delay(10);
				x = x/10;
		}
}
/* 超声波发送函数 */
void send_wave(void)
{
		int i;
		for(i = 0;i < 16;i++)
		{
				TX = ~TX;
				_nop_();_nop_();_nop_();_nop_();
		}
}
/* 定时器0初始化 */
void time0_init(void)
{
		 AUXR |= 0x80;
		 TMOD  = 0X00;
		 TL0 = 0;
		 TH0 = 0;
		 TR0 = 1;
		 ET0 = 1;
		 EA = 1;
}
void main()
{	
			float len;
			int a,b;
			buzzer_off();
			time0_init();
			while(1)
			{
					send_wave();
					TR0 = 1;
					while((RX == 1)&&(TF0 == 0));
					TR0 = 0;
					if(TF0 == 1)
					{
							TF0 = 0;
							len = 999;
					}
					else
					{
							a = TL0;
							b = TH0;
							len = (b*256 + a) * 0.0172;
							TH0 = 0;
							TL0 = 0;
					}
					display((int)(len/10));
					delay(10);
			}
}
