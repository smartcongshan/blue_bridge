#include "stc15f2k60s2.h"
#include "onewire.h"

code unsigned char du[] = { 0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
float t;

/* 延时函数 */
void delay(int z)
{
		int x,y;
		for(x = z ;x >0 ;x--)
			for(y = 110;y > 0;y--);
}

/* 关蜂鸣器 */
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

/* 数码管显示函数 */

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

unsigned int ds18b20_rd(void)
{
		unsigned char h,l;
		unsigned int temp;
		
		Init_DS18B20();
		Write_DS18B20(0xcc);
		Write_DS18B20(0x44);
		Delay_OneWire(200);
		
		Init_DS18B20();
		Write_DS18B20(0xcc);
		Write_DS18B20(0xbe);
		
		l = Read_DS18B20();
		h = Read_DS18B20();
	
		temp = (h<<8) | l;
		
		t = temp*0.0625;
		temp = t*10 + 0.5;
	
		return temp;
}

void main()
{
		unsigned int fah;
		buzzer_off();
		while(1)
		{
				fah = ds18b20_rd();
				display(fah);
				delay(5);
		}
}
