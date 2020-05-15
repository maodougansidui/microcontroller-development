#include <reg52.h>

sbit ADDR0=P1^0;
sbit ADDR1=P1^1;
sbit ADDR2=P1^2;
sbit ADDR3=P1^3;
sbit ENLED=P1^4;

sbit KEY4=P2^7;
sbit KEY2=P2^5;

unsigned char code LedChar[]={
	0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,
	0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E
};

bit KeySta=1;
bit KeySta1=1;

void main(){
	bit backup=1;
	bit backup1=1;
	signed char cnt=0;

	EA=1;
	ENLED=0;
	ADDR3=1;
	ADDR2=0;
	ADDR1=0;
	ADDR0=0;
	TMOD=0x01;
	TH0=0xF8;
	TL0=0xCD;
	ET0=1;
	TR0=1;
	P2=0xF7;
	P0=LedChar[cnt];

	while(1){
		if (KeySta !=backup){
			if (backup==0){
				cnt++;
				if (cnt>=10){
					cnt=0;
				}
				P0=LedChar[cnt];
			}
			backup=KeySta;			
		}
		if (KeySta1 !=backup1){
			if (backup1==0){
				cnt--;
				if (cnt<0){
					cnt=9;
				}
				P0=LedChar[cnt];
			}
			backup1=KeySta1;			
		}
	}
		 
}

void InterruptTimer0() interrupt 1{
	static unsigned char keybuf=0xFF;
	static unsigned char keybuf1=0xFF;

	TH0=0xF8;
	TL0=0xCD;

	keybuf=(keybuf<<1) | KEY4;
	keybuf1=(keybuf1<<1) | KEY2;
	if (keybuf==0x00){
		KeySta=0;
	}else if (keybuf==0xFF){
		KeySta=1;
	}else{
	}

	if (keybuf1==0x00){
		KeySta1=0;
	}else if (keybuf1==0xFF){
		KeySta1=1;
	}else{
	}
	
}