#include<reg52.h>

//sbit LED=P0^0;
sbit ADDR0=P1^0;
sbit ADDR1=P1^1;
sbit ADDR2=P1^2;
sbit ADDR3=P1^3;
sbit ENLED=P1^4;

void main()
{
	unsigned int i=0;
	unsigned char dir=0;
	unsigned char shift=0x01;
	ENLED=0;
	ADDR3=1;
	ADDR2=1;
	ADDR1=1;
	ADDR0=0;
//	LED=0;

	while(1)
	{
		P0=~shift;
		for (i=0;i<30000;i++);
		if (dir==0){
			shift=shift<<1;
			if (shift==0x80){
				dir=1;
			}
		}else{
			shift=shift>>1;
			if (shift==0x01){
				dir=0;
			}
		}	
	}
}