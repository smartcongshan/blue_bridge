#include "stc15f2k60s2.h"

code unsigned char du[] = { 0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};

/* ��ʱ���� */
void delay(int z)
{
		int x,y;
		for(x = z ;x >0 ;x--)
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
		P0 |= 0xff;
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
		P0 |= (1<<x);
		P2 &= 0x1f;
		P0 &= 0x00;
}

/* �������ʾ���� */

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


void main()
{
		buzzer_off();
		while(1)
		{
		display(12345678);
			//delay(10);
		}
}
