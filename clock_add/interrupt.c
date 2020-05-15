#include <reg52.h>

sbit ADDR0=P1^0;
sbit ADDR1=P1^1;
sbit ADDR2=P1^2;
sbit ADDR3=P1^3;
sbit ENLED=P1^4;

unsigned char code LedChar[]={
	0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8,
	0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E
	};

unsigned char code RegList[6][3]={
	{0,0,0}, {0,0,1}, {0,1,0}, {0,1,1}, {1,0,0}, {1,0,1}
	};

void main(){
	
	unsigned char cnt=0;
	unsigned char index=0;
	unsigned char first=0;
	unsigned int i=0;

	ENLED=0;
	ADDR3=1;
	ADDR2=RegList[0][0];
	ADDR1=RegList[0][1];
	ADDR0=RegList[0][2];
	TMOD=0x01;
	TH0=0xFC;
	TL0=0x66;
	TR0=1;

	while (1){
		while (TF0==0); // do nothing when TF0 is 0;
		TF0=0;
		TH0=0xFC;
		TL0=0x66;
		ADDR2=RegList[first][0];
		ADDR1=RegList[first][1];
		ADDR0=RegList[first][2];
		P0=LedChar[index];
		first++;
		if (first>=6){
			first=0;
			cnt++;
			if (cnt >= 166){
				cnt=0;
				P0=0xFF;
				for (i=0;i<=30000;i++);
				index++;
				if (index>=16){
					index=0;
				}
			}
		}
	}

}

