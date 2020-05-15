#include <reg52.h>

sbit ADDR3=P1^3;
sbit ENLED=P1^4;
sbit KEY1=P2^4;
sbit KEY2=P2^5;
sbit KEY3=P2^6;
sbit KEY4=P2^7;

unsigned char code LedChar[]={
	0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,
	0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E
};

unsigned char LedBuff[6]={
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};

unsigned char KeySta[4]={
	1,1,1,1
};


bit StopWatchRefresh=1;			// 1 meaning fresh, 0 meaning not to fresh.
bit StopWatchRunning=1;			// 1 meaning runing, 0 meaning not running.
unsigned char decimalPart=0;
unsigned int integerPart=0;
unsigned char T0RH=0;
unsigned char T0RL=0;


void ConfigTimer(unsigned int ms);
void StopWatchDisplay();
void KeyDriver();

void main(){
	EA=1;
	ADDR3=1;
	ENLED=0;
	ConfigTimer(2);
	P2=0xFE;			// set the P2.0 to be the output.

	while(1){
		if (StopWatchRefresh){
			StopWatchRefresh=0;
			StopWatchDisplay();
		}
		KeyDriver();
	}
}

void ConfigTimer(unsigned int ms){
	unsigned long tmp;
	
	tmp= 11059200/12;
	tmp= (tmp*ms)/1000;

	tmp=65536-tmp;
	tmp=tmp+18;
	T0RH= (unsigned char)(tmp>>8);
	T0RL= (unsigned char)tmp;
	TMOD&=0xF0;
	TMOD|=0x01;
	TH0=T0RH;
	TL0=T0RL;
	ET0=1;
	TR0=1;
}

void StopWatchAction(){
	if (StopWatchRunning){
		StopWatchRunning=0;
	}else{
		StopWatchRunning=1;
	}
}

void StopWatchReset(){
	decimalPart=0;
	integerPart=0;
	StopWatchRunning=0;
	StopWatchRefresh=1;
}


void StopWatchDisplay(){
	static unsigned char buff[4]={0,0,0,0};
	signed char i;

	LedBuff[0]=LedChar[decimalPart%10];
	LedBuff[1]=LedChar[decimalPart/10%10];
	buff[0]=integerPart%10;
	buff[1]=integerPart/10%10;
	buff[2]=integerPart/100%10;
	buff[3]=integerPart/1000%10;

	for (i=3;i>=1;i--){
		if (buff[i]==0){
			LedBuff[i+2]=0xFF;
		}else{
			break;
		}
	}

	for (; i>=0;i--){
		LedBuff[i+2]=LedChar[buff[i]];
	}
	LedBuff[2]&=0x7F;
}

void KeyDriver(){
	static unsigned char backup[4]={1,1,1,1};
	unsigned char i;

	for (i=0;i<=3;i++){
		if (backup[i]!=KeySta[i]){
			if (backup[i]!=0){
				if (i==1){
					StopWatchReset();
				}else if (i==2){
					StopWatchAction();
				}
			}
			backup[i]=KeySta[i];
		}
	}
}


void StopWatchCount(){
	if (StopWatchRunning){
		decimalPart++;
		if (decimalPart>=100){
			decimalPart=0;
			integerPart++;
			if (integerPart>=10000){
				integerPart=0;
			}
		}
		StopWatchRefresh=1;
	}
}

void KeyScan(){
	unsigned char i;
	static unsigned char keybuf[4]={0xFF,0xFF,0xFF,0xFF};

	keybuf[0]=(keybuf[0]<<1) | KEY1;
	keybuf[1]=(keybuf[1]<<1) | KEY2;
	keybuf[2]=(keybuf[2]<<1) | KEY3;
	keybuf[3]=(keybuf[3]<<1) | KEY4;

	for (i=0;i<4;i++){
		if ((keybuf[i] & 0x0F)==0x00){
			KeySta[i]=0;
		}else if ((keybuf[i] & 0x0F)==0x0F){
			KeySta[i]=1;
		}
	}
}

void LedScan(){
	static unsigned char i=0;
	P0=0xFF;
	P1= (P1 & 0xF8) | i;
	P0=LedBuff[i];
	i++;
	if (i>=6){
		i=0;
	}
}	

void interruptTimer0() interrupt 1{
	static unsigned char tmr10s=0;
	TH0=T0RH;
	TL0=T0RL;
	LedScan();
	KeyScan();

	tmr10s++;
	if (tmr10s>=5){
		tmr10s=0;
		StopWatchCount();
	}

}

