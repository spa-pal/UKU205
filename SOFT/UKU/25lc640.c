#include "25lc640.h"
#include "LPC17xx.H"
#include "main.h"

#ifndef SPI1_DEFINED
#define SPI1_DEFINED



char spi1(char in)
{

 

LPC_SPI->SPDR=in;
while(!(LPC_SPI->SPSR&(1<<7)));
return LPC_SPI->SPDR;

 

}

#endif

//----------------------------------------------- 
//настройка SPI1
void spi1_config(void)
{ 

SET_REG( LPC_PINCON->PINSEL0, 3, 15*2, 2);
SET_REG( LPC_PINCON->PINSEL1, 0, (16-16)*2, 2);	 //?
SET_REG( LPC_PINCON->PINSEL1, 3, (17-16)*2, 2);
SET_REG( LPC_PINCON->PINSEL1, 3, (18-16)*2, 2); 
/*
S1SPCCR=100;
S1SPCR=0x3f; */

LPC_SPI->SPCCR=8;
LPC_SPI->SPCR=0x20;
}

//----------------------------------------------- 
//выключение SPI1
void spi1_unconfig(void)
{ 

SET_REG( LPC_PINCON->PINSEL0, 0, 15*2, 2);
SET_REG( LPC_PINCON->PINSEL1, 0, (16-16)*2, 2);
SET_REG( LPC_PINCON->PINSEL1, 0, (17-16)*2, 2);
SET_REG( LPC_PINCON->PINSEL1, 0, (18-16)*2, 2);

LPC_SPI->SPCR=0x00;
}

//----------------------------------------------- 
//Разрешение записи
void lc640_wren(void)
{

spi1_config();

CS_ON

spi1(0x06); 

CS_OFF

spi1_unconfig();
}

//-----------------------------------------------
//Чтение из м-мы регистра состояния
char lc640_rdsr(void)
{
char temp;

spi1_config();
CS_ON
spi1(0x05);
temp=spi1(0xff);
CS_OFF
spi1_unconfig();
return temp;
}

//----------------------------------------------- 
//Чтение из м-мы байта по адр. ADR
int lc640_read(int ADR)
{
int temp;
temp=0;

while(lc640_rdsr()&0x01)
	{
	}
spi1_config();
CS_ON
CS_ON
//temp_short[0]=PINSEL1;	
//
//IO0DIR|=1UL<<17;
//IO0CLR|=1UL<<17;
spi1(0x03);
temp=*(((char*)&ADR)+1);
spi1(temp);
temp=*((char*)&ADR);
spi1(temp);
temp=spi1(0xff);
//IO0SET|=1UL<<17;
CS_OFF
CS_OFF
spi1_unconfig();
return temp;

}

//----------------------------------------------- 
//Чтение из м-мы слова по адр. ADR
int lc640_read_int(int ADR)
{



char temp;
int temp_i;

//LPC_GPIO0->FIODIR|=0x00000002;
//LPC_GPIO0->FIOSET|=0x00000002;
while(lc640_rdsr()&0x01)
	{
	}

//lc640_rdsr();
//IO0DIR_bit.P0_11=1;
//IO0SET_bit.P0_11=1;
spi1_config();
CS_ON
spi1(0x03);
temp=*(((char*)&ADR)+1);
spi1(temp);
temp=*((char*)&ADR);
spi1(temp);
temp=spi1(0xff);
temp_i=spi1(0xff);
temp_i<<=8;
temp_i+=temp;
CS_OFF
CS_OFF
spi1_unconfig();

//LPC_GPIO0->FIOCLR|=0x00000002;
return temp_i;
}

//----------------------------------------------- 
//Чтение из м-мы 4 байт по адр. ADR
long lc640_read_long(int ADR)
{
char temp0,temp1,temp2;
long temp_i;
while(lc640_rdsr()&0x01)
	{
	}
spi1_config();
CS_ON
spi1(0x03);
temp0=*(((char*)&ADR)+1);
spi1(temp0);
temp0=*((char*)&ADR);
spi1(temp0);
temp0=spi1(0xff);
temp1=spi1(0xff);
temp2=spi1(0xff);
temp_i=spi1(0xff);
temp_i<<=8;
temp_i+=temp2;
temp_i<<=8;
temp_i+=temp1;
temp_i<<=8;
temp_i+=temp0;
CS_OFF
CS_OFF
spi1_unconfig();
return temp_i;
}

//----------------------------------------------- 
//Чтение из м-мы 4 байт по адр. ADR
void lc640_read_long_ptr(int ADR,char* out_ptr)
{
char temp0/*,temp1,temp2*/;
//long temp_i;
while(lc640_rdsr()&0x01)
	{
	}
spi1_config();
CS_ON
spi1(0x03);
temp0=*(((char*)&ADR)+1);
spi1(temp0);
temp0=*((char*)&ADR);
spi1(temp0);
out_ptr[0]=spi1(0xff);
out_ptr[1]=spi1(0xff);
out_ptr[2]=spi1(0xff);
out_ptr[3]=spi1(0xff);
CS_OFF
CS_OFF
spi1_unconfig();
}

//----------------------------------------------- 
//Чтение из м-мы N байт по адр. ADR
void lc640_read_str(int ADR, char* ram_ptr, char num)
{
char temp0,i;
while(lc640_rdsr()&0x01)
	{
	}
spi1_config();
CS_ON
spi1(0x03);
temp0=*(((char*)&ADR)+1);
spi1(temp0);
temp0=*((char*)&ADR);
spi1(temp0);

for(i=0;i<num;i++)
	{
	*ram_ptr++=spi1(0xff);
	}
CS_OFF
CS_OFF
spi1_unconfig();
}

//-----------------------------------------------
//Запись байта in по адресу ADR
char lc640_write(int ADR,char in)
{
char temp; 

while(lc640_rdsr()&0x01)
	{
	}
lc640_wren();
spi1_config();	
CS_ON
spi1(0x02);
temp=*(((char*)&ADR)+1);
spi1(temp);
temp=*((char*)&ADR);
spi1(temp);
temp=spi1(in);
CS_OFF
CS_OFF
spi1_unconfig(); 
return temp;
}

//-----------------------------------------------
//Запись слова in по адресу ADR
char lc640_write_int(short ADR,short in)
{
char temp; 
while(lc640_rdsr()&0x01)
	{
	}
lc640_wren();
spi1_config();	
CS_ON
spi1(0x02);
temp=*(((char*)&ADR)+1);
spi1(temp);
temp=*((char*)&ADR);
spi1(temp);
temp=*((char*)&in);
spi1(temp);
temp=*(((char*)&in)+1);
spi1(temp);
CS_OFF
CS_OFF
spi1_unconfig();
return temp;
}  

//-----------------------------------------------
//Запись 4 байт in по адресу ADR
char lc640_write_long(int ADR,long in)
{
char temp; 

while(lc640_rdsr()&0x01)
	{
	}
lc640_wren();	
spi1_config();
CS_ON
spi1(0x02);
temp=*(((char*)&ADR)+1);
spi1(temp);
temp=*((char*)&ADR);
spi1(temp);
temp=*((char*)&in);
spi1(temp);
temp=*(((char*)&in)+1);
spi1(temp);
temp=*(((char*)&in)+2);
spi1(temp);
temp=*(((char*)&in)+3);
spi1(temp);           
CS_OFF
CS_OFF  
spi1_unconfig();
return temp;
}

//-----------------------------------------------
//Запись 4 байт in по адресу ADR
char lc640_write_long_ptr(int ADR,char* in)
{
char temp; 

while(lc640_rdsr()&0x01)
	{
	}
lc640_wren();	
spi1_config();
CS_ON
spi1(0x02);
temp=*(((char*)&ADR)+1);
spi1(temp);
temp=*((char*)&ADR);
spi1(temp);
temp=in[0];
spi1(temp);
temp=in[1];
spi1(temp);
temp=in[2];
spi1(temp);
temp=in[3];
spi1(temp);

CS_OFF
CS_OFF  
spi1_unconfig();
return temp;
}		
