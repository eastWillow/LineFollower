#include <STC12C5A60S2.H>
#include <intrins.h>
#define ADC_POWER 0x80
#define ADC_FLAG 0x10
#define ADC_START 0x08
#define ADC_HHSPEED 0x60
/*
CNY70 > 3F =>BLACK
*/
unsigned char ch = 0;
unsigned char cny70 = 0x00;
void setup();
void serialInit(void);
void serialSendHex(unsigned char);
void Delay500ms();
void main(){
	setup();
	serialInit();
	while(1);
}
void setup(){
	IE = 0xa0;
	P1ASF = 0x1F;
	ADC_CONTR = 0xe8;
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
}
void AnalogISR(void) interrupt 5 using 1{
	ADC_CONTR &= ~ADC_FLAG;
	if(ADC_RES >= 0xE0) cny70 |= (0x01 << ch);
	if(ADC_RES <= 0x0f) cny70 &= _crol_(0xfe,ch);
	if(++ch>7){
		ch = 0;
		serialSendHex(cny70);
	}
	ADC_CONTR = ADC_POWER|ADC_HHSPEED|ADC_START|ch;
}
void serialInit(void)		//9600bps@12.000MHz
{
	PCON &= 0x7F;		//Baudrate no doubled
	SCON = 0x50;		//8bit and variable baudrate
	AUXR |= 0x04;		//BRT's clock is Fosc (1T)
	BRT = 0xD9;		//Set BRT's reload value
	AUXR |= 0x01;		//Use BRT as baudrate generator
	AUXR |= 0x10;		//BRT running
}
void serialSendHex(unsigned char Data){
	SBUF = Data;
	while(!TI);
	TI = 0;
}
void Delay500ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 22;
	j = 3;
	k = 227;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}