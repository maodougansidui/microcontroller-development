#include <reg52.h>

sbit PWMOUT= P0^0;
sbit ADDR0= P1^0;
sbit ADDR1= P1^1;
sbit ADDR2= P1^2;
sbit ADDR3= P1^3;
sbit ENLED= P1^4;

unsigned char HighRH=0;
unsigned char HighRL=0;
unsigned char LowRH =0;
unsigned char LowRL =0;

void ConfigPWM(unsigned int fr, unsigned char dc);
void ClosePWM();

void main(){
	unsigned int i;

	EA=1;
	ENLED=0;
	ADDR3=1;
	ADDR2=1;
	ADDR1=1;
	ADDR0=0;

	while(1){
		ConfigPWM(100,10);
		for (i=0;i<40000;i++);
		ClosePWM();
		ConfigPWM(100,40);
		for (i=0;i<40000;i++);
		ClosePWM();
		ConfigPWM(100,90);
		for (i=0;i<40000;i++);
		ClosePWM();
		for (i=0;i<40000;i++);
	}
}

void ConfigPWM(unsigned int fr, unsigned char dc){
	unsigned int high, low;
	unsigned long tmp;

	tmp = (11059200/12)/fr;
	high = (tmp*dc)/100;
	low = tmp -high;
	high = 65536 - high +12;
	low = 65536 - low + 12;
	HighRH= (unsigned char)(high>>8);
	HighRL= (unsigned char)high;
	LowRH= (unsigned char)(low>>8);
	LowRL= (unsigned char)low;
	TMOD &=0xF0;
	TMOD |=0x01;
	TH0 = HighRH;
	TL0= HighRL;
	ET0= 1;
	TR0 =1;
	PWMOUT=1;
}

void ClosePWM(){
	TR0=0;
	ET0=0;
	PWMOUT=1;
}

void InterruptTimer0() interrupt 1{
	if (PWMOUT==1){
		TH0=LowRH;
		TL0=LowRL;
		PWMOUT=0;
	}else{
		TH0=HighRH;
		TL0=HighRL;
		PWMOUT=1;
	}

}



