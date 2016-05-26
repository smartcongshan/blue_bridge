#include "core.h"
#include "iic.h"

extern int mod;
extern int leixing;
code unsigned char du[] = { 0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xff};

/* ��ʱ���� */
void delay(int z)
{
		int x,y;
		for(x = z ;x >0 ;x--)
			for(y = 110;y > 0;y--);
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
/* �رշ����� */
void buzzer_off(void)
{
		P2 &= 0x00;
		P2  = 0xa0;
		P0 &= 0x00;
		P2 &= 0x1f;
}
/* PCF8591��ʼ������ */
void pcf_8591_init(void)
{
		IIC_Start();
		IIC_SendByte(0x90);
		IIC_WaitAck();
		IIC_SendByte(0x03);
		IIC_WaitAck();
		IIC_Stop();
}
/* PCF8591��ȡ���� */
int pcf_8591_rd(void)
{
		unsigned char ret;
		IIC_Start();
		IIC_SendByte(0x91);
		IIC_WaitAck();
		ret = IIC_RecByte();
		IIC_Ack(1);
		IIC_Stop();
		
		return (int)ret;
}

/* ��ʱ��0��ʼ�� */
void timer0_init(void)
{
		AUXR |= 0x80;
		TMOD  = 0x00;
		TH0		= T1MS>>8;
		TL0   = T1MS;
		TR0 	= 1;
		ET0	 	= 1;
		EA  	= 1;
}

/* ��ʱ��1��ʼ�� */
void timer1_init(void)
{
		AUXR |= 0X40;
		TH1 = 0;
		TL1 = 0;
		ET1 = 1;
		TR1 = 0;
}

/* �������������� */
void sound_wave(void)
{
		int i;
		for(i = 0;i < 16;i++)
		{
				TX = ~TX;	
				_nop_();_nop_();_nop_();_nop_();
		}
}

/* ������������ */
float sound_dis(void)
{
		int a,b;
		float len;
		sound_wave();
		//TR0 = 0;
		TR1 = 1;
		while((RX == 1)&&(TF1 == 0));
		TR1 = 0;
		if(TF1 == 1)
		{
			TF1 = 0;
			len = 999;
		}
		else
		{
				a   = TL1;
				b   = TH1;
				len = (b * 256 + a)*0.0172;
				TL1 = 0;
				TH1 = 0;
		}
		return len;
}

/* �������ʾ���� 
 * ģʽ1��
 * a:ģʽ��
 * b:�������ľ���
 * c:û���õ�
 * ģʽ2��
 * a:ģʽ��
 * b:����ʣ��ʱ��
 * c:û���õ�
 * ģʽ3��
 * a:ģʽ��
 * b:����1����ʣ��ʱ��
 * c:����2����ʣ��ʱ��
 */
void display(int a,int b,int c)
{
			switch(a)
			{
					case 1:
						/* ģʽ�� */
						wei(7);
						duan(a);
						delay(20);
						
						/* ����������ľ��� */
						wei(3);
						duan(b%10);
						delay(20);
						wei(4);
						duan(b/10);
						delay(20);
						/* ������� */
						wei(0);
						if(b > 30)
						{	
								leixing = 2;
						}
						else
						{
								leixing = 1;
						}
						duan(leixing);
						delay(20);
						break;
					case 2:
						/* ģʽ�� */
						wei(7);
						duan(a);
						delay(20);
						
						/* ����ʱ�� */
						wei(1);
						duan(b/10);
						delay(20);
						wei(0);
						duan(b%10);
						delay(20);
						
						break;
					case 3:
						/* ģʽ�� */
						wei(7);
						duan(a);
						delay(20);
					
						/* ����1����Ĵ���ʱ�� */
						wei(3);
						duan(b%10);
						delay(20);
						wei(4);
						duan(b/10);
						delay(20);
					
						/* ����2����Ĵ���ʱ�� */
						wei(1);
						duan(c/10);
						delay(20);
						wei(0);
						duan(c%10);
						delay(20);
						
						break;
			}
}


/* ����ܲ���ʾ���� */
void dis_off(void)
{
			int j;
			for(j = 0;j < 8;j++)
			{
					wei(j);
					P2 &= 0x0; 
					P2  = 0xe0;
					P0 |= 0xff;
					P2 &= 0x1f;
					delay(20);
			}
}

/* ������ʾ���� */
int weight(int i,int l3)
{
		float len;
		if(i >= 192)	//����
		{
			TR0 = 1;
			l3 = 1;
			dis_off();
		}
		if(i >= 50 && i < 192) //��������
		{
				TR0 = 0;
				l3 = 0;
				P2 &= 0x0;
				P2  = 0x80;
				P0 |= 0Xff;
				P0 &= ~(1<<1);
				P2 &= 0x1f;
				buzzer_off();
				len = sound_dis();
				display(mod,(int)(len/10),0);
				if(s4 == 0)
				{
						delay(5);
						if(s4 == 0)
						{
							mod = 2;
							jidianqi_on();
							TR0 = 1;
							//TMOD = 0x40;
						}
				}
		}
		if(i < 50)	//����
		{
				TR0 = 0;
				l3 = 0;
				P2 &= 0x0;
				P2  = 0x80;
				P0 |= 0Xff;
				P0 &= ~(1<<0);
				P2 &= 0x1f;
				buzzer_off();
				dis_off();
		}
		return l3;
}

/* �򿪼̵��� */
void jidianqi_on(void)
{
			P2 &= 0x00;
		P2  = 0xa0;
		P0 = 0x10;
		P2 &= 0x1f;
}

/* L3��˸�������� */
void L3_warning(void)
{
		unsigned char tmp;
		P2 &= 0x0;
		P2  = 0x80;
		P0 = 0xff;
		if(tmp == 0)
		P0  |= (1<<2);
		else
		P0 &= ~(1<<2);
		P2 &= 0x1f;
		delay(10);	
		P2 &= 0x00;
		P2  = 0xa0;
		tmp = p02;
		P0  = 0x40; 
	
		P2 &= 0x1f;
}
