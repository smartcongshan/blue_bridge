#include "stc15f2k60s2.h"

#define CCP_S1  0x20
#define CCP_S2	0x10

void buzzer_off(void)
{
		P2 &= 0x0;
		P2  = 0xa0;
		P0 &= 0x0;
		P2 &= 0x1f;
}

void pwm_init(void)
{
		ACC = P_SW1;					//也就是AUXR1寄存器
		ACC &= ~(CCP_S1 | CCP_S2);
		/*将CCP_S1和CCP_S0变成00
		 *    5       4
		 *		0       0 
		 */
		P_SW1 = ACC;
		CCON = 0;
		CL = 0;
		CH = 0;
		CMOD = 0x02;
}

void pwm_zhankongbi(unsigned int zhan)
{
		PCA_PWM0 = 0x00;
		CCAP0H = CCAP0L = 0x20;	//计算占空比(0x100 - 0x20)/0x100
		CCAPM0 = 0x42;
		
}

void main()
{
		buzzer_off();
		pwm_init();
		CR = 1;	//启动PWM
	while(1)
	{
			pwm_zhankongbi();
	}
}
