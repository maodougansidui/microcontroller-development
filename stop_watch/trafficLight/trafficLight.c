#include <reg52.h>

sbit ADDR3=P1^3;
sbit ENLED=P1^4;

unsigned char code LedChar[]={
	0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,
	0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E
};

unsigned char LedBuff[7]={
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};

bit flag1s=1;
unsigned char T0RH=0;
unsigned char T0RL=0;

void ConfigTimer0(unsigned int ms);
void TrafficLight();

void main(){
	EA=1;
	ENLED=0;
	ADDR3=1;
	ConfigTimer0(1);

	while(1){
		if (flag1s){
			flag1s=0;
			TrafficLight();
		}
	}
}

void ConfigTimer0(unsigned int ms){
	unsigned long tmp;
	
	tmp=11059200/12;
	tmp=(tmp*ms)/1000;
	tmp=65536-tmp;
	tmp=tmp+13;
	T0RH=(unsigned char)(tmp>>8);
	T0RL=(unsigned char)tmp;

	TMOD&=0xF0;
	TMOD|=0x01;
	TH0=T0RH;
	TL0=T0RL;
	ET0=1;
	TR0=1;
}

void TrafficLight(){
	static unsigned char color=2;
	static unsigned char timer=0;

	if (timer==0){
		switch(color){
			case 0:





















