#include "stc15f2k60s2.h"

#define FOSC	11059200L
#define T01MS	(65536 - FOSC/1000) //TIM0����Ƶ

code unsigned char du[] = { 0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};

int i,t = 0;

/* ��ʱ���� */
void delay(int z)
{
		int x,y;
		for(x = z;x > 0;x--)
			for(y = 110;y > 0;y--);
}
/* �رշ����� */
void buzzer_off(void)
{
		P2 &= 0x0;
		P2  = 0xa0;
		P0 &= 0x0;
		P2 &= 0x1f;
}
/* ��ѡ */
void duan(int x)
{
		P2 &= 0x0;
		P2  = 0xe0;
		P0  = 0xff;
		P0  = du[x];
		P2 &= 0x1f;
}
/* λѡ */
void wei(int x)
{
		x = 7 - x;
		P2 &= 0x0;
		P2  = 0xc0;
		P0 &= 0x0;
		P0  = 1<<x;
		P2 &= 0x1f;
}
/* ��ʾ���� */
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
/* ��ʱ����ʼ�� */
void time0_init(void)
{
		AUXR |= 0x80; //��ʱ��0����Ƶ
		TMOD  = 0x00; //��ʱ��0��ʽ0
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
