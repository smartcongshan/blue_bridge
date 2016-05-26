#include "stc15f2k60s2.h"

/* 延时函数 */
void delay(int a)
{
	int x,y;
	for(x = a;x > 0;x--)
		for(y = 110;y > 0;y--);
}
/* 关闭蜂鸣器 */
void buzzer_off(void)
{
		P2 &= 0x0;
		P2  = 0xa0;
		P0 &= 0X0;
		P2 &= 0X1F;
}
/* 控制led函数 */
void led(int x)
{
		P2 &= 0x0;
		P2  = 0x80;
		if(x == 0)
			P0 = 0x00;
		else
		{
			P0 |= 0xff;
			P0 = ~(1<<(x - 1));  
		}
		P2 &= 0x1f;
}

void main()
{
		int x;
		buzzer_off();
		x = 1;
		while(1)
		{
			led(x);
			delay(100);
		}
}