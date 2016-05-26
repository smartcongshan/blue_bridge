#include "stc15f2k60s2.h"

sbit p36 = P3^6;
sbit p37 = P3^7;
sbit p42 = P4^2;
sbit p44 = P4^4;

code unsigned char du[] = { 0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
int i;

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

/* 显示 */
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

/* 按键检测函数 */
void key(void)
{
		int a,b,c;
		
		P3 = 0x0f;
		p44 = 0;
		p42 = 0;
		if(P3 != 0x0f)
		{
				delay(5);
				if(P3 != 0x0f)
				{
						a = P3;		
						p44 = 1;
						p42 = 1;
						P3 = 0xf0;
						p36 = p42;
						p37 = p44;
						b = P3 & 0xf0;
						c = a|b;
				}
			}
		switch(c)
		{
				case 0x7e: i = 1;  break;
				case 0xbe: i = 2;  break;
				case 0xde: i = 3;  break;
				case 0xee: i = 4;  break;
				case 0x7d: i = 5;  break;
				case 0xbd: i = 6;  break;
				case 0xdd: i = 7;  break;
				case 0xed: i = 8;  break;
				case 0x7b: i = 9;  break;
				case 0xbb: i = 10; break;
				case 0xdb: i = 11; break;
				case 0xeb: i = 12; break;
				case 0x77: i = 13; break;
				case 0xb7: i = 14; break;
				case 0xd7: i = 15; break;
				case 0xe7: i = 16; break;
				default: break;
		}
}

void main()
{
		buzzer_off();
		while(1)
		{
				key();
				display(i);
		}
}
