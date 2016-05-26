#include "core.h"
#include "iic.h"

extern int mod;
extern int leixing;
code unsigned char du[] = { 0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xff};

/* 延时函数 */
void delay(int z)
{
		int x,y;
		for(x = z ;x >0 ;x--)
			for(y = 110;y > 0;y--);
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
/* 关闭蜂鸣器 */
void buzzer_off(void)
{
		P2 &= 0x00;
		P2  = 0xa0;
		P0 &= 0x00;
		P2 &= 0x1f;
}
/* PCF8591初始化函数 */
void pcf_8591_init(void)
{
		IIC_Start();
		IIC_SendByte(0x90);
		IIC_WaitAck();
		IIC_SendByte(0x03);
		IIC_WaitAck();
		IIC_Stop();
}
/* PCF8591读取函数 */
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

/* 定时器0初始化 */
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

/* 定时器1初始化 */
void timer1_init(void)
{
		AUXR |= 0X40;
		TH1 = 0;
		TL1 = 0;
		ET1 = 1;
		TR1 = 0;
}

/* 超声波启动函数 */
void sound_wave(void)
{
		int i;
		for(i = 0;i < 16;i++)
		{
				TX = ~TX;	
				_nop_();_nop_();_nop_();_nop_();
		}
}

/* 超声波测距程序 */
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

/* 数码管显示函数 
 * 模式1：
 * a:模式数
 * b:超声波的距离
 * c:没有用到
 * 模式2：
 * a:模式数
 * b:传送剩余时间
 * c:没有用到
 * 模式3：
 * a:模式数
 * b:货物1传送剩余时间
 * c:货物2传送剩余时间
 */
void display(int a,int b,int c)
{
			switch(a)
			{
					case 1:
						/* 模式数 */
						wei(7);
						duan(a);
						delay(20);
						
						/* 超声波测出的距离 */
						wei(3);
						duan(b%10);
						delay(20);
						wei(4);
						duan(b/10);
						delay(20);
						/* 哪类货物 */
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
						/* 模式数 */
						wei(7);
						duan(a);
						delay(20);
						
						/* 传送时间 */
						wei(1);
						duan(b/10);
						delay(20);
						wei(0);
						duan(b%10);
						delay(20);
						
						break;
					case 3:
						/* 模式数 */
						wei(7);
						duan(a);
						delay(20);
					
						/* 类型1货物的传送时间 */
						wei(3);
						duan(b%10);
						delay(20);
						wei(4);
						duan(b/10);
						delay(20);
					
						/* 类型2货物的传送时间 */
						wei(1);
						duan(c/10);
						delay(20);
						wei(0);
						duan(c%10);
						delay(20);
						
						break;
			}
}


/* 数码管不显示函数 */
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

/* 重量提示函数 */
int weight(int i,int l3)
{
		float len;
		if(i >= 192)	//超重
		{
			TR0 = 1;
			l3 = 1;
			dis_off();
		}
		if(i >= 50 && i < 192) //正常重量
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
		if(i < 50)	//空载
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

/* 打开继电器 */
void jidianqi_on(void)
{
			P2 &= 0x00;
		P2  = 0xa0;
		P0 = 0x10;
		P2 &= 0x1f;
}

/* L3闪烁报警函数 */
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
