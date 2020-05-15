#include <reg52.h>

sbit ADDR0=P1^0;
sbit ADDR1=P1^1;
sbit ADDR2=P1^2;
sbit ADDR3=P1^3;
sbit ENLED=P1^4;

	// 15 0.0006 553 65536-553=64983 FDD7 1667
unsigned char code LedChar[]={
	0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90	
};

unsigned char code image[]={
	0xFF,0x99,0x00,0x00,0x00,0x81,0xC3,0xE7
};

unsigned char LedBuff[]={
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};


unsigned char flag0=0;
unsigned char flag1=0;

void timeCalculated();

void main(){
	
	EA=1;
	ENLED=0;
	TMOD=0x01;
	TH0=0xFD;
	TL0=0xD7;
	ET0=1;
	TR0=1;
	
	
	while(1){
		timeCalculated();	
	}
	
	
	
}

void InterruptTimer0() interrupt 1{				// led and digit led
	static unsigned char i=0;
	static unsigned char move=0;
	static unsigned int cnt=0;	
	ENLED=0; 
	
	TH0=0xFD;
	TL0=0xD7;
	
	cnt++;
	if (cnt>=1667){
		cnt=0;
		flag0=1;
		flag1=1;
	}
	
	P0=0xFF;
	switch(i){
		case 0:ADDR3=1; ADDR2=0;ADDR1=0;ADDR0=0;i++;P0=LedBuff[0];break;
		case 1:ADDR3=1; ADDR2=0;ADDR1=0;ADDR0=1;i++;P0=LedBuff[1];break;
		case 2:ADDR3=1; ADDR2=0;ADDR1=1;ADDR0=0;i++;P0=LedBuff[2];break;
		case 3:ADDR3=1; ADDR2=0;ADDR1=1;ADDR0=1;i++;P0=LedBuff[3];break;
		case 4:ADDR3=1; ADDR2=1;ADDR1=0;ADDR0=0;i++;P0=LedBuff[4];break;
		case 5:ADDR3=1; ADDR2=1;ADDR1=0;ADDR0=1;i++;P0=LedBuff[5];break;
		case 6:
			ADDR3=1;
			ADDR2=1;
			ADDR1=1;
			ADDR0=0;
			i++;
			P0=~(0x01<<move);
			if (flag0==1){
				flag0=0;
				move++;
				if (move>=8){
					move=0;
				}
			}
			break;
		case 7:ADDR3=0; ADDR2=0;ADDR1=0;ADDR0=0;i++;P0=image[0];break;
		case 8:ADDR3=0; ADDR2=0;ADDR1=0;ADDR0=1;i++;P0=image[1];break;
		case 9:ADDR3=0; ADDR2=0;ADDR1=1;ADDR0=0;i++;P0=image[2];break;
		case 10:ADDR3=0; ADDR2=0;ADDR1=1;ADDR0=1;i++;P0=image[3];break;
		case 11:ADDR3=0; ADDR2=1;ADDR1=0;ADDR0=0;i++;P0=image[4];break;
		case 12:ADDR3=0; ADDR2=1;ADDR1=0;ADDR0=1;i++;P0=image[5];break;
		case 13:ADDR3=0; ADDR2=1;ADDR1=1;ADDR0=0;i++;P0=image[6];break;
		case 14:ADDR3=0; ADDR2=1;ADDR1=1;ADDR0=1;i=0;P0=image[7];break;
		default:
			break;	
	}
	
}



void timeCalculated(){
	static unsigned long sec=0;
																														 
	
	if (flag1==1){
		flag1=0;
		sec++;
		LedBuff[0] = LedChar[sec%10];
        LedBuff[1] = LedChar[sec/10%10];
       	LedBuff[2] = LedChar[sec/100%10];
        LedBuff[3] = LedChar[sec/1000%10];
        LedBuff[4] = LedChar[sec/10000%10];
        LedBuff[5] = LedChar[sec/100000%10];

	}
}



