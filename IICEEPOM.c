#include<reg52.h>
#define uchar unsigned char
#define uint  unsigned int
sbit scl=P2^0;
sbit sda=P2^1;
uchar code table[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
uint m,c;
void delay(uint i)
{
   		 while(i--);
}

void delay1(uchar z)
{
         uchar x,y;
		 for(x=110;x>0;x--)
		 for(y=z;y>0;y--);
}

void start()
{
	     sda=1;
         scl=1;	
		 delay(5);
		 sda=0;
		 delay(5);
}

void stop()
{
		 sda=0;
         scl=1;
		 delay(5);
		 sda=1;
		 delay(5);
}

void respon()
{
        uchar i;
		scl=1;
	//	delay(5);
		while((sda==1)&&(i<250))
		{
		    i++;
		}
		scl=0;
		delay(5);
}

void write_byte(uchar date)
{
		 uchar i;
		 for(i=0;i<8;i++)
		 {
		       scl=0;   
			   delay(5);
			   sda=date&0x80;
			   delay(5);
			   scl=1;
			   date<<=1;
			   delay(5);
		 }
		 scl=0;
		 sda=1;
		 delay(5);

}

uchar read_byte()
{
         uchar i,bb;
		 for(i=0;i<8;i++)
		 {
		        scl=1;
				bb<<=1;
				delay(5);
				if(sda==1)
				{
				      bb|=0x01;
				}
				else
				{
				      bb|=0x00;
				}
				delay(5);
				scl=0;
				delay(5);
		 }
		 return bb;
}

void write_date(uchar address,date)
{
          start();
		  write_byte(0xa0);
		  respon();
		  write_byte(address);
		  respon();
		  write_byte(date);
		  respon();
		  stop();
}

uint read_date(uchar address)
{
          uint cc;
		  start();
		  write_byte(0xa0);
		  respon();
		  write_byte(address);
		  respon();
		  start();
		  write_byte(0xa1);
		  respon();
		  cc=read_byte();
		  stop();
		  return cc;
}

void display()
{
		  uchar ge,shi,bai;
		  ge=c%10;
		  shi=c%100/10;
		  bai=c/100;

		  P2=0xc0;
		  P0=0x04;
		  P2=0x00;

		  P2=0xe0;
		  P0=table[ge];
		  P2=0x00;
		  delay1(10);
  
		  P2=0xc0;
		  P0=0x02;
		  P2=0x00;

		  P2=0xe0;
		  P0=table[shi];
		  P2=0x00;
		  delay1(10);

		  P2=0xc0;
		  P0=0x01;
		  P2=0x00;

		  P2=0xe0;
		  P0=table[bai];
		  P2=0x00;
		  delay1(10);
}

void main()
{
        P2=0xa0;
		P0=0x80;
		P2=0x00;
        TMOD=0x01;
		EA=1;
		ET0=1;
		TH0=(65535-50000)/256;
		TL0=(65535-50000)%256;
		TR0=1;
		m=read_date(2);
		if(m>1000)
		{
		       m=0;
		}
		while(1)
		{
		        c=read_date(2);
		       write_date(2,m);
			  
			   display();
		}
}

void time() interrupt 1
{
        uchar tt;
        TH0=(65535-50000)/256;
		TL0=(65535-50000)%256;
		tt++;
		if(tt==20)
		{
		       tt=0;
		       m++;
			   if(m==1000)
			   {
			         m=0;
			   }
		}
}