#include "stc15f2k60s2.h"
#include "iic.h"
#include "core.h"

int l3 = 0;
int t = 0,a = 0;
int re,mod = 1;
int leixing;
int ht1 = 2,ht2 = 3; //货物类型1的时间 货物类型2的时间

/* 1秒的延时 */
void Delay1000ms(int ht)		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 43;
	j = 6;
	k = 203;
	do
	{
		display(mod,ht,0);
		do
		{
			while (--k);
		//	
		} while (--j);
			
				display(mod,ht,0);						
	} while (--i);
	display(mod,ht,0);
}


void main()
{

		buzzer_off();
		pcf_8591_init();
		timer0_init();
		timer1_init();
		while(1)
		{
				switch(mod)
				{
						case 1:
									re =  pcf_8591_rd();
									delay(20);
									l3 = weight(re,l3);
									break;
						case 2:
									TR0 = 0;
									if(leixing == 1)
									{
										display(mod,ht1,0);
											Delay1000ms(ht1);
											ht1--;
											if(ht1 == 0)
												mod = 1;
										}
									else
									{	
										display(mod,ht2,0);
											Delay1000ms(ht2);
											ht2--;
										if(ht2 == 0)
												mod = 1;
									}
								
									
									break;
						case 3:
									break;
				}
		}
}
void time0(void) interrupt 1
{
		
				t++;
				a++;
				if(t == 500)//0.5s
				{	
						t = 0;
			
					if(l3 == 1)
						L3_warning();
				}	
				if(a == 1000)
				{
						a = 0;
						if(mod == 2)
						{
						if(leixing == 1)
						{
							ht1--;
								
									display(mod,ht1,0);
								
							if(ht1 == 0)
							{
									buzzer_off();
									mod = 1;
							}
						}
						else
						{
								ht2--;
								display(mod,ht2,0);
							if(ht2 == 0)
							{
									buzzer_off();
									mod = 1;
							}
						}
					}
				
				
				}
			
}
