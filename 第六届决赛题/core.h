#ifndef __CORE_H
#define __CORE_H

#include "stc15f2k60s2.h"
sbit p02 = P0^2;
sbit TX  = P1^0;
sbit RX  = P1^1;
sbit s4  = P3^3;

#define FOSC 11059200
#define T1MS (65536 - FOSC/1000)

void delay(int z);
void duan(int x);
void wei(int x);
void buzzer_off(void);
void pcf_8591_init(void);
int pcf_8591_rd(void);
void timer0_init(void);
void timer1_init(void);
void sound_wave(void);
float sound_dis(void);
void display(int a,int b,int c);
void dis_off(void);
int weight(int i,int);
void L3_warning(void);
void jidianqi_on(void);



#endif
