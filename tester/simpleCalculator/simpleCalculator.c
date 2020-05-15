#include <reg52.h>
#include <stdlib.h>

sbit ADDR0=P1^0;
sbit ADDR1=P1^1;
sbit ADDR2=P1^2;
sbit ADDR3=P1^3;
sbit ENLED=P1^4;
sbit KEY_IN_1=P2^4;
sbit KEY_IN_2=P2^5;
sbit KEY_IN_3=P2^6;
sbit KEY_IN_4=P2^7;
sbit KEY_OUT_1=P2^3;
sbit KEY_OUT_2=P2^2;
sbit KEY_OUT_3=P2^1;
sbit KEY_OUT_4=P2^0;

unsigned char code LedChar[]={
		0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,
		0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E
};

unsigned char LedBuff[6]={
		0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};

unsigned char code KeyCodeMap[4][4]={
		{0x31,0x32,0x33,0x26},
		{0x34,0x35,0x36,0x25},
		{0x37,0x38,0x39,0x28},
		{0x30,0x1B,0x0D,0x27}
};

unsigned char KeySta[4][4]={
		{1,1,1,1},{1,1,1,1},{1,1,1,1},{1,1,1,1}
};

void KeyDriver();
int calculate(unsigned char instruction,long result,long addend,bit first);

void main(){
	EA=1;
	ADDR3=1;
	ENLED=0;
	TMOD=0x01;
	TH0=0xFC;
	TL0=0x67;
	TR0=1;
	ET0=1;
	LedBuff[0]=LedChar[0];

	while (1){
		KeyDriver();
	}
}

void ShowNumber(long num){
	signed char i;
	unsigned char sign;
	unsigned char buf[6];

	if (num<0){
		sign=1;
		num=-num;
	}else{
		sign=0;
	}
	for (i=0;i<6;i++){
		buf[i]=num%10;
		num=num/10;
	}
	for (i=5;i>=1;i--){
		if (buf[i]==0){
			LedBuff[i]=0xFF;
		}else{
			break;
		}
	}
	if (sign!=0){
		if (i<5){
			LedBuff[i+1]=0xBF;
		}
	}
	for ( ; i>=0;i--){
		LedBuff[i]=LedChar[buf[i]];
	}
}

void KeyAction(unsigned char keycode){
	static long result=0;
	static long addend=0;
	static unsigned char instruction=0;
	static bit first=1;

	if (keycode>=0x30 && keycode<=0x39){			   // number input
		addend=(addend*10) + (keycode-0x30);
		ShowNumber(addend);
	}else if (keycode==0x26){							// add
		result=calculate(instruction,result,addend,first);
		first=0;
		addend=0;
		instruction=0;
		ShowNumber(result);
	}else if (keycode==0x28){							// subtract
		result=calculate(instruction,result,addend,first);
		first=0;
		addend=0;
		instruction=1;
		ShowNumber(result);		
	}else if (keycode==0x25){							// multiplication
		result=calculate(instruction,result,addend,first);
		first=0;
		addend=0;
		instruction=2;
		ShowNumber(result);			
	}else if (keycode==0x27){				  			// divide
		result=calculate(instruction,result,addend,first);
		first=0;
		addend=0;
		instruction=3;
		ShowNumber(result);		
	}else if (keycode==0x0D){
		result=calculate(instruction,result,addend,first);
		first=0;
		addend=0;
		instruction=4;
		ShowNumber(result);		
	}else if (keycode==0x1B){
		first=1;
		instruction=0;
		addend=0;
		result=0;
		ShowNumber(addend);
	}
}

int calculate(unsigned char instruction,long result,long addend,bit first){
	if (first){
		result+=addend;
		return result;
	}
	switch(instruction){
		case 0: result+=addend; break;
		case 1: result-=addend; break;
		case 2: result*=addend; break;
		case 3: result=result/addend; break;
		case 4: result=result;
		default: break;
	}

	return result;
	
}

void KeyDriver(){
	unsigned char i,j;
	static unsigned char backup[4][4]={
		{1,1,1,1},{1,1,1,1},{1,1,1,1},{1,1,1,1}
	};
	for (i=0;i<4;i++){
		for (j=0;j<4;j++){
			if (backup[i][j]!=KeySta[i][j]){
				if (backup[i][j]!=0){
					KeyAction(KeyCodeMap[i][j]);
				}
				backup[i][j]=KeySta[i][j];
			}
		}
	}
}

void KeyScan(){
	unsigned char i;
	static unsigned char keyout=0;
	static unsigned char keybuf[4][4]={
		{0xFF,0xFF,0xFF,0xFF},{0xFF,0xFF,0xFF,0xFF},
		{0xFF,0xFF,0xFF,0xFF},{0xFF,0xFF,0xFF,0xFF}
	};

	keybuf[keyout][0]=(keybuf[keyout][0]<<1) | KEY_IN_1;
	keybuf[keyout][1]=(keybuf[keyout][1]<<1) | KEY_IN_2;
	keybuf[keyout][2]=(keybuf[keyout][2]<<1) | KEY_IN_3;
	keybuf[keyout][3]=(keybuf[keyout][3]<<1) | KEY_IN_4;

	for (i=0;i<4;i++){
		if ((keybuf[keyout][i] & 0x0F)==0x00){
			KeySta[keyout][i]=0;
		}else if ((keybuf[keyout][i] & 0x0F)==0x0F){
			KeySta[keyout][i]=1;	
		}
	}
	keyout++;
	keyout=keyout&0x03;
	switch(keyout){
		case 0: KEY_OUT_4=1; KEY_OUT_1=0; break;
		case 1:	KEY_OUT_1=1; KEY_OUT_2=0; break;
		case 2: KEY_OUT_2=1; KEY_OUT_3=0; break;
		case 3: KEY_OUT_3=1; KEY_OUT_4=0; break;
		default:break;
	}
	
}

void LedScan(){
	static unsigned char i=0;
	P0=0xFF;
	switch(i){
		case 0: ADDR2=0; ADDR1=0; ADDR0=0; i++; P0=LedBuff[0]; break;
		case 1: ADDR2=0; ADDR1=0; ADDR0=1; i++; P0=LedBuff[1]; break;
		case 2: ADDR2=0; ADDR1=1; ADDR0=0; i++; P0=LedBuff[2]; break;
		case 3: ADDR2=0; ADDR1=1; ADDR0=1; i++; P0=LedBuff[3]; break;
		case 4: ADDR2=1; ADDR1=0; ADDR0=0; i++; P0=LedBuff[4]; break;
		case 5: ADDR2=1; ADDR1=0; ADDR0=1; i=0; P0=LedBuff[5]; break;
		default: break;
	}
}

void InterruptTimer0() interrupt 1{
	TH0=0xFC;
	TL0=0x67;
	LedScan();
	KeyScan();
}