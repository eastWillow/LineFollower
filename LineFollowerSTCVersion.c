#include <STC12C5A60S2.H>
#include <intrins.h>
#define ADC_POWER 0x80
#define ADC_FLAG 0x10
#define ADC_START 0x08
#define ADC_HHSPEED 0x60
#define FULL 65535
#define ALL_WIDTH 20000 //20ms
sbit SERVOMOTOR1=P0^0;
sbit LIMITSWITCH=P3^2; // INT0
sbit IN1=P0^1;
sbit IN2=P0^2;
//Low Trigger
sbit CNY70_0=P1^0; //cny70.0
sbit CNY70_1=P1^1; //cny70.1
sbit CNY70_2=P1^2; //cny70.2
sbit CNY70_3=P1^3; //cny70.3
sbit CNY70_4=P1^4; //cny70.4
/*
CNY70 > 3F =>BLACK
*/
/*
TA7267
IN1 IN2 OUT1 OUT2
 1   1    L    L break
 0   1    L    H backward
 1   0    H    L forward
 0   0    Z    Z nothing
*/
bit ServoMotor;
volatile unsigned char ch = 0;
volatile unsigned char cny70 = 0xff;
volatile unsigned int data servoMotorHighTime = 1250;//volatile
void setup();
void serialInit(void);
void serialSendHex(unsigned char);
void Delay500ms();
void main(){
	setup();
	//serialInit();
	while(1){
		//if(((~cny70)&0x02) == 0x02) servoMotorHighTime =1100;
		//if(((~cny70)&0x08) == 0x08) servoMotorHighTime =1400;
	}
}
void setup(){
	IE = 0xa3;
	//PX1 = 1;
	//P1ASF = 0x1F;
	//ADC_CONTR = ADC_POWER|ADC_HHSPEED|ch;
	//_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	TR0 = 0; //reset Timer0 Switch
	TMOD = 0x11; //0010 0010
	TL0 = (FULL - ALL_WIDTH - servoMotorHighTime) %256;
	TH0 = (FULL - ALL_WIDTH - servoMotorHighTime) / 256;
	SERVOMOTOR1 = 0;
	TR0 = 1;
}
/*void AnalogISR(void) interrupt 5 using 1{
	ADC_CONTR &= ~ADC_FLAG;
	if(ADC_RES >= 0xE0) cny70 |= (0x01 << ch);
	if(ADC_RES <= 0x0f) cny70 &= _crol_(0xfe,ch);
	if(++ch>7){
		ch = 0;
	}
	ADC_CONTR = ADC_POWER|ADC_HHSPEED|ADC_START|ch;
}*/
void limitSwitch () interrupt 0 using 0{
	IN1 =1;
	IN2 =1;
}
void servoMotor ()interrupt 1 using 0{
	TR0 = 0;
	if(SERVOMOTOR1 == 1){
		TL0 = (FULL - ALL_WIDTH - servoMotorHighTime) %256;
		TH0 = (FULL - ALL_WIDTH - servoMotorHighTime) / 256;
		SERVOMOTOR1 = 0;
	}
	if(SERVOMOTOR1 == 0){
		TL0 = (FULL - servoMotorHighTime) %256;
		TH0 = (FULL - servoMotorHighTime) / 256;
		SERVOMOTOR1 = 1;
	}
	TR0 = 1;
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