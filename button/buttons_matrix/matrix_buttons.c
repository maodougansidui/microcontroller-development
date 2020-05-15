/*
 ============================================================================
 Name        : matrix_buttons.c
 Author      : JianlyuMao
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <reg52.h>

sbit ADDR0 = P1 ^ 0;
sbit ADDR1 = P1 ^ 1;
sbit ADDR2 = P1 ^ 2;
sbit ADDR3 = P1 ^ 3;
sbit ENLED = P1 ^ 4;
sbit KEY_IN_1 = P2 ^ 4;
sbit KEY_IN_2 = P2 ^ 5;
sbit KEY_IN_3 = P2 ^ 6;
sbit KEY_IN_4 = P2 ^ 7;
sbit KEY_OUT_1 = P2 ^ 3;
sbit KEY_OUT_2 = P2 ^ 2;
sbit KEY_OUT_3 = P2 ^ 1;
sbit KEY_OUT_4 = P2 ^ 0;

unsigned char code LedMatrix[16][8]= {
	{0xC3,0xC9,0xCC,0xCF,0xCF,0xCF,0xCF,0x00},			// 1
	{0xC3,0x99,0x9C,0xCF,0xF3,0xF9,0xFC,0x81},			// 2
	{0xC1,0xBF,0xBF,0x8F,0xF1,0x8F,0xBF,0xC1},			// 3
	{0xE7,0xC3,0x81,0x00,0xE7,0xE7,0xE7,0xE7},			// arrow up
	{0x87,0x93,0x99,0x9E,0x00,0x9F,0x9F,0x9F},			// 4
	{0xC3,0xDB,0xFB,0xC3,0xDF,0xDD,0xE3,0xFF},			// 5
	{0xC7,0xF3,0xF9,0xED,0xD5,0xD9,0xDB,0xE7},		   	// 6
	{0xF7,0xF3,0xF1,0x00,0x00,0xF1,0xF3,0xF7},			// arrow left
	{0x81,0x9F,0x9F,0xCF,0xE7,0xF3,0xF9,0xF9},			// 7
	{0xC3,0xDB,0xDB,0xC3,0xE7,0xDB,0xDB,0xC3},			// 8
	{0x81,0xBD,0xBD,0x81,0xBF,0xBF,0xBF,0x81},			// 9
	{0xE7,0xE7,0xE7,0xE7,0x00,0x81,0xC3,0xE7},			// arrow down
	{0xC3,0x99,0xBD,0xBD,0xBD,0xBD,0x99,0xC3},			// 0
	{0x24,0xB6,0xB6,0xA4,0xA4,0xAE,0xAE,0x24},			// esc
	{0x9F,0x9F,0x9F,0x9B,0x99,0x80,0x80,0xF9},			// enter key
	{0xEF,0xCF,0x8F,0x00,0x00,0x8F,0xCF,0xEF}			// arrow right

};

unsigned char KeySta[4][4] = { { 1, 1, 1, 1 }, { 1, 1, 1, 1 }, { 1, 1, 1, 1 }, {
		1, 1, 1, 1 } };

unsigned char index;

void main() {
	unsigned char i, j;
	unsigned char backup[4][4] = { { 1, 1, 1, 1 }, { 1, 1, 1, 1 },
			{ 1, 1, 1, 1 }, { 1, 1, 1, 1 } };

	EA = 1;
	ENLED = 0;
	ADDR3 = 0;
	ADDR2 = 0;
	ADDR1 = 0;
	ADDR0 = 0;
	TMOD = 0x01;
	TH0 = 0xFC;
	TL0 = 0x67;
	ET0 = 1;
	TR0 = 1;

	while (1) {
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (backup[i][j] != KeySta[i][j]) {
					if (backup[i][j] != 0) {
						index=i*4 +j;
					}
					backup[i][j] = KeySta[i][j];
				}
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
		case 1: KEY_OUT_1=1; KEY_OUT_2=0; break;
		case 2: KEY_OUT_2=1; KEY_OUT_3=0; break;
		case 3: KEY_OUT_3=1; KEY_OUT_4=0; break;
		default: break;
	}
}

void LedScan(){
	static unsigned char i=0;
	
	P0=0xFF;
	switch(i){
		case 0: ADDR2=0; ADDR1=0; ADDR0=0; i++; P0=LedMatrix[index][0];break;
		case 1: ADDR2=0; ADDR1=0; ADDR0=1; i++; P0=LedMatrix[index][1];break;
		case 2: ADDR2=0; ADDR1=1; ADDR0=0; i++; P0=LedMatrix[index][2];break;
		case 3: ADDR2=0; ADDR1=1; ADDR0=1; i++; P0=LedMatrix[index][3];break;
		case 4: ADDR2=1; ADDR1=0; ADDR0=0; i++; P0=LedMatrix[index][4];break;
		case 5: ADDR2=1; ADDR1=0; ADDR0=1; i++; P0=LedMatrix[index][5];break;
		case 6: ADDR2=1; ADDR1=1; ADDR0=0; i++; P0=LedMatrix[index][6];break;
		case 7: ADDR2=1; ADDR1=1; ADDR0=1; i=0; P0=LedMatrix[index][7];break;
		default: break;
	}
}


void InterruptTimer0() interrupt 1{
	TH0=0xFC;
	TL0=0x67;
	LedScan();
	KeyScan();
}

