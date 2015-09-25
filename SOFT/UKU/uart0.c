#include "uart0.h"
#include "LPC17XX.H" 
#include "main.h"
#include "control.h"
#include "cmd.c"
#include "eeprom_map.h"
#include "modbus.h"

char bRXIN0;
char UIB0[100]={0,0,0,0,0,0,0,0,0,0};
char flag0;
char rx_buffer0[RX_BUFFER_SIZE0];
char tx_buffer0[TX_BUFFER_SIZE0];
unsigned short rx_wr_index0,rx_rd_index0,rx_counter0;
unsigned short tx_wr_index0,tx_rd_index0,tx_counter0;
char rx_buffer_overflow0;
char plazma_uart0;
char memo_out[50];
char data_rs[50];
char data_rs0[50];
//Кодовые таблицы CRC
const char Table87[]={
0x00, 0x0E, 0x1C, 0x12, 0x38, 0x36, 0x24, 0x2A, 0x70, 0x7E, 0x6C, 0x62, 0x48, 0x46, 0x54, 0x5A,
0xE0, 0xEE, 0xFC, 0xF2, 0xD8, 0xD6, 0xC4, 0xCA, 0x90, 0x9E, 0x8C, 0x82, 0xA8, 0xA6, 0xB4, 0xBA,
0xCE, 0xC0, 0xD2, 0xDC, 0xF6, 0xF8, 0xEA, 0xE4, 0xBE, 0xB0, 0xA2, 0xAC, 0x86, 0x88, 0x9A, 0x94,
0x2E, 0x20, 0x32, 0x3C, 0x16, 0x18, 0x0A, 0x04, 0x5E, 0x50, 0x42, 0x4C, 0x66, 0x68, 0x7A, 0x74,
0x92, 0x9C, 0x8E, 0x80, 0xAA, 0xA4, 0xB6, 0xB8, 0xE2, 0xEC, 0xFE, 0xF0, 0xDA, 0xD4, 0xC6, 0xC8,
0x72, 0x7C, 0x6E, 0x60, 0x4A, 0x44, 0x56, 0x58, 0x02, 0x0C, 0x1E, 0x10, 0x3A, 0x34, 0x26, 0x28,
0x5C, 0x52, 0x40, 0x4E, 0x64, 0x6A, 0x78, 0x76, 0x2C, 0x22, 0x30, 0x3E, 0x14, 0x1A, 0x08, 0x06,
0xBC, 0xB2, 0xA0, 0xAE, 0x84, 0x8A, 0x98, 0x96, 0xCC, 0xC2, 0xD0, 0xDE, 0xF4, 0xFA, 0xE8, 0xE6,
0x2A, 0x24, 0x36, 0x38, 0x12, 0x1C, 0x0E, 0x00, 0x5A, 0x54, 0x46, 0x48, 0x62, 0x6C, 0x7E, 0x70,
0xCA, 0xC4, 0xD6, 0xD8, 0xF2, 0xFC, 0xEE, 0xE0, 0xBA, 0xB4, 0xA6, 0xA8, 0x82, 0x8C, 0x9E, 0x90,
0xE4, 0xEA, 0xF8, 0xF6, 0xDC, 0xD2, 0xC0, 0xCE, 0x94, 0x9A, 0x88, 0x86, 0xAC, 0xA2, 0xB0, 0xBE,
0x04, 0x0A, 0x18, 0x16, 0x3C, 0x32, 0x20, 0x2E, 0x74, 0x7A, 0x68, 0x66, 0x4C, 0x42, 0x50, 0x5E,
0xB8, 0xB6, 0xA4, 0xAA, 0x80, 0x8E, 0x9C, 0x92, 0xC8, 0xC6, 0xD4, 0xDA, 0xF0, 0xFE, 0xEC, 0xE2,
0x58, 0x56, 0x44, 0x4A, 0x60, 0x6E, 0x7C, 0x72, 0x28, 0x26, 0x34, 0x3A, 0x10, 0x1E, 0x0C, 0x02,
0x76, 0x78, 0x6A, 0x64, 0x4E, 0x40, 0x52, 0x5C, 0x06, 0x08, 0x1A, 0x14, 0x3E, 0x30, 0x22, 0x2C,
0x96, 0x98, 0x8A, 0x84, 0xAE, 0xA0, 0xB2, 0xBC, 0xE6, 0xE8, 0xFA, 0xF4, 0xDE, 0xD0, 0xC2, 0xCC};



const char Table95[]={
0x00, 0x2A, 0x54, 0x7E, 0xA8, 0x82, 0xFC, 0xD6, 0x7A, 0x50, 0x2E, 0x04, 0xD2, 0xF8, 0x86, 0xAC,
0xF4, 0xDE, 0xA0, 0x8A, 0x5C, 0x76, 0x08, 0x22, 0x8E, 0xA4, 0xDA, 0xF0, 0x26, 0x0C, 0x72, 0x58,
0xC2, 0xE8, 0x96, 0xBC, 0x6A, 0x40, 0x3E, 0x14, 0xB8, 0x92, 0xEC, 0xC6, 0x10, 0x3A, 0x44, 0x6E,
0x36, 0x1C, 0x62, 0x48, 0x9E, 0xB4, 0xCA, 0xE0, 0x4C, 0x66, 0x18, 0x32, 0xE4, 0xCE, 0xB0, 0x9A,
0xAE, 0x84, 0xFA, 0xD0, 0x06, 0x2C, 0x52, 0x78, 0xD4, 0xFE, 0x80, 0xAA, 0x7C, 0x56, 0x28, 0x02,
0x5A, 0x70, 0x0E, 0x24, 0xF2, 0xD8, 0xA6, 0x8C, 0x20, 0x0A, 0x74, 0x5E, 0x88, 0xA2, 0xDC, 0xF6,
0x6C, 0x46, 0x38, 0x12, 0xC4, 0xEE, 0x90, 0xBA, 0x16, 0x3C, 0x42, 0x68, 0xBE, 0x94, 0xEA, 0xC0,
0x98, 0xB2, 0xCC, 0xE6, 0x30, 0x1A, 0x64, 0x4E, 0xE2, 0xC8, 0xB6, 0x9C, 0x4A, 0x60, 0x1E, 0x34,
0x76, 0x5C, 0x22, 0x08, 0xDE, 0xF4, 0x8A, 0xA0, 0x0C, 0x26, 0x58, 0x72, 0xA4, 0x8E, 0xF0, 0xDA,
0x82, 0xA8, 0xD6, 0xFC, 0x2A, 0x00, 0x7E, 0x54, 0xF8, 0xD2, 0xAC, 0x86, 0x50, 0x7A, 0x04, 0x2E,
0xB4, 0x9E, 0xE0, 0xCA, 0x1C, 0x36, 0x48, 0x62, 0xCE, 0xE4, 0x9A, 0xB0, 0x66, 0x4C, 0x32, 0x18,
0x40, 0x6A, 0x14, 0x3E, 0xE8, 0xC2, 0xBC, 0x96, 0x3A, 0x10, 0x6E, 0x44, 0x92, 0xB8, 0xC6, 0xEC, 
0xD8, 0xF2, 0x8C, 0xA6, 0x70, 0x5A, 0x24, 0x0E, 0xA2, 0x88, 0xF6, 0xDC, 0x0A, 0x20, 0x5E, 0x74, 
0x2C, 0x06, 0x78, 0x52, 0x84, 0xAE, 0xD0, 0xFA, 0x56, 0x7C, 0x02, 0x28, 0xFE, 0xD4, 0xAA, 0x80, 
0x1A, 0x30, 0x4E, 0x64, 0xB2, 0x98, 0xE6, 0xCC, 0x60, 0x4A, 0x34, 0x1E, 0xC8, 0xE2, 0x9C, 0xB6, 
0xEE, 0xC4, 0xBA, 0x90, 0x46, 0x6C, 0x12, 0x38, 0x94, 0xBE, 0xC0, 0xEA, 0x3C, 0x16, 0x68, 0x42}; 


volatile uint32_t UART0Status;
volatile uint8_t UART0TxEmpty = 1;
//volatile uint8_t UART0Buffer[BUFSIZE];
volatile uint32_t UART0Count = 0;


//-----------------------------------------------
char crc_87(char* ptr,char num)
{
char r,j/*,lb*/;
r=*ptr;

for(j=1;j<num;j++)
	{
     ptr++;
	r=((*ptr)^Table87[r]);
	}

return r;	
} 

//-----------------------------------------------
char crc_95(char* ptr,char num)
{
char r,j/*,lb*/;
r=*ptr;

for(j=1;j<num;j++)
	{
     ptr++;
	r=((*ptr)^Table95[r]);
	}

return r;	
}


//-----------------------------------------------
void putchar0(char c)
{
while (tx_counter0 == TX_BUFFER_SIZE0);
if (tx_counter0 || ((LPC_UART0->LSR & 0x60)==0))
   {
   tx_buffer0[tx_wr_index0]=c;
   if (++tx_wr_index0 == TX_BUFFER_SIZE0) tx_wr_index0=0;
   ++tx_counter0;
   }
else LPC_UART0->THR=c;
}

//-----------------------------------------------
void uart_out0 (char num,char data0,char data1,char data2,char data3,char data4,char data5)
{
char i,t=0;
//char *ptr=&data1;
char UOB0[16]; 
UOB0[0]=data0;
UOB0[1]=data1;
UOB0[2]=data2;
UOB0[3]=data3;
UOB0[4]=data4;
UOB0[5]=data5;

for (i=0;i<num;i++)
	{
	t^=UOB0[i];
	}    
UOB0[num]=num;
t^=UOB0[num];
UOB0[num+1]=t;
UOB0[num+2]=END;

for (i=0;i<num+3;i++)
	{
	putchar0(UOB0[i]);
	}   	
}

//-----------------------------------------------
void uart_out_adr0 (char *ptr, char len)
{
char UOB[50]/*={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}*/;
char i,t=0;

for(i=0;i<len;i++)
	{
	UOB[i]=ptr[i];
	t^=UOB[i];
	}
//if(!t)t=0xff;
UOB[len]=len;
t^=len;	
UOB[len+1]=t;	
UOB[len+2]=END;
//UOB[0]=i+1;
//UOB[i]=t^UOB[0];
//UOB[i+1]=END;
	
//puts(UOB); 
for (i=0;i<len+3;i++)
	{
	putchar0(UOB[i]);
	}   
}


//-----------------------------------------------
void uart_out_adr_block (unsigned long adress,char *ptr, char len)
{
char UOB[100]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
char i,temp11,t=0;
unsigned i11;

t=0;
temp11=CMND;
t^=temp11;
putchar0(temp11);

temp11=10;
t^=temp11;
putchar0(temp11);

temp11=(*((char*)&adress));
t^=temp11;
putchar0(temp11);

temp11=(*(((char*)&adress)+1));
t^=temp11;
putchar0(temp11);

temp11=(*(((char*)&adress)+2));
t^=temp11;
putchar0(temp11);

temp11=(*(((char*)&adress)+3));
t^=temp11;
putchar0(temp11);


for(i11=0;i11<len;i11++)
	{
	temp11=ptr[i11];
	t^=temp11;
	putchar0(temp11);
	}
	
temp11=(len+6);
t^=temp11;
putchar0(temp11);

putchar0(t);

putchar0(0x0a);
	
}

/*
//-----------------------------------------------
void uart0_init(void)
{


SET_REG(PINSEL0,1,0*2,2);
SET_REG(PINSEL0,1,1*2,2);



SET_REG(U0LCR,1,7,1);//U0LCR_bit.DLAB=1;
U0DLL=70000000UL/(BAUD_RATE0*16);
U0DLM=70000000UL/(BAUD_RATE0*16*256);
SET_REG(U0LCR,0,7,1);//U0LCR_bit.DLAB=0;
U0LCR=0x03;
U0FCR=0;

VICProtection = 0;
VICIntEnClr |= (1 << VIC_UART0); 
VICIntSelect &= ~(1 << VIC_UART0);
VICVectAddr9=(unsigned)uart0_interrupt;
VICVectCntl9 = 0x20 | VIC_UART0;
VICIntEnable |= (1  << VIC_UART0);

U0IER=0x03;

}
*/
//-----------------------------------------------
uint32_t UARTInit( uint32_t PortNum, uint32_t baudrate )
{
uint32_t Fdiv;
uint32_t pclkdiv, pclk;



if ( PortNum == 0 )
  	{
	LPC_PINCON->PINSEL0 &= ~0x000000F0;
	LPC_PINCON->PINSEL0 |= 0x00000050;  /* RxD0 is P0.3 and TxD0 is P0.2 */
	/* By default, the PCLKSELx value is zero, thus, the PCLK for
	all the peripherals is 1/4 of the SystemFrequency. */
	/* Bit 6~7 is for UART0 */
	pclkdiv = (LPC_SC->PCLKSEL0 >> 6) & 0x03;
	switch ( pclkdiv )
		{
	  	case 0x00:
	  	default:
		pclk = SystemFrequency/4;
		break;
	  	case 0x01:
		pclk = SystemFrequency;
		break; 
	  	case 0x02:
		pclk = SystemFrequency/2;
		break; 
	  	case 0x03:
		pclk = SystemFrequency/8;
		break;
		}

    	LPC_UART0->LCR = 0x83;		/* 8 bits, no Parity, 1 Stop bit */
	Fdiv = ( pclk / 16 ) / baudrate ;	/*baud rate */
    	LPC_UART0->DLM = Fdiv / 256;							
    	LPC_UART0->DLL = Fdiv % 256;
	LPC_UART0->LCR = 0x03;		/* DLAB = 0 */
    	LPC_UART0->FCR = 0x07;		/* Enable and reset TX and RX FIFO. */

   	NVIC_EnableIRQ(UART0_IRQn);

    	LPC_UART0->IER = IER_RBR | IER_THRE | IER_RLS;	/* Enable UART0 interrupt */
    	return (TRUE);
  	}
else if ( PortNum == 1 )
  	{
	LPC_PINCON->PINSEL4 &= ~0x0000000F;
	LPC_PINCON->PINSEL4 |= 0x0000000A;	/* Enable RxD1 P2.1, TxD1 P2.0 */
	
	/* By default, the PCLKSELx value is zero, thus, the PCLK for
	all the peripherals is 1/4 of the SystemFrequency. */
	/* Bit 8,9 are for UART1 */
	pclkdiv = (LPC_SC->PCLKSEL0 >> 8) & 0x03;
	switch ( pclkdiv )
		{
	  	case 0x00:
	  	default:
		pclk = SystemFrequency/4;
		break;
	  	case 0x01:
		pclk = SystemFrequency;
		break; 
	  	case 0x02:
		pclk = SystemFrequency/2;
		break; 
	  	case 0x03:
		pclk = SystemFrequency/8;
		break;
		}

    	LPC_UART1->LCR = 0x83;		/* 8 bits, no Parity, 1 Stop bit */
	Fdiv = ( pclk / 16 ) / baudrate ;	/*baud rate */
    	LPC_UART1->DLM = Fdiv / 256;							
    	LPC_UART1->DLL = Fdiv % 256;
	LPC_UART1->LCR = 0x03;		/* DLAB = 0 */
    	LPC_UART1->FCR = 0x07;		/* Enable and reset TX and RX FIFO. */

   	NVIC_EnableIRQ(UART1_IRQn);

    	LPC_UART1->IER = IER_RBR | IER_THRE | IER_RLS;	/* Enable UART1 interrupt */
    	return (TRUE);
  	}
return( FALSE ); 
}

//-----------------------------------------------
char getchar0(void)
{
char data;
while (rx_counter0==0);
data=rx_buffer0[rx_rd_index0];
if (++rx_rd_index0 == RX_BUFFER_SIZE0) rx_rd_index0=0;
--rx_counter0;
return data;
}

//***********************************************
void UART0_IRQHandler (void) 
{
uint8_t IIRValue, LSRValue;
uint8_t Dummy = Dummy;
char status,u0iir,data;

uart_plazma++;
			
IIRValue = LPC_UART0->IIR;
    
IIRValue >>= 1;			/* skip pending bit in IIR */
IIRValue &= 0x07;			/* check bit 1~3, interrupt identification */
if ( IIRValue == IIR_RLS )		/* Receive Line Status */
  	{
	LSRValue = LPC_UART0->LSR;
	/* Receive Line Status */
	if ( LSRValue & (LSR_OE|LSR_PE|LSR_FE|LSR_RXFE|LSR_BI) )
		{
	  	/* There are errors or break interrupt */
	  	/* Read LSR will clear the interrupt */
	  	UART0Status = LSRValue;
	  	Dummy = LPC_UART0->RBR;		/* Dummy read on RX to clear 
							interrupt, then bail out */
	  	return;
		}
	if ( LSRValue & LSR_RDR )	/* Receive Data Ready */			
		{
		
		data=LPC_UART0->RBR;
		rx_buffer0[rx_wr_index0]=data;
   		bRXIN0=1;
   		if (++rx_wr_index0 == RX_BUFFER_SIZE0) rx_wr_index0=0;
   		if (++rx_counter0 == RX_BUFFER_SIZE0)
      		{
      		rx_counter0=0;
      		rx_buffer_overflow0=1;
      		}
	  	/* If no error on RLS, normal ready, save into the data buffer. */
	  	/* Note: read RBR will clear the interrupt */
	  /*	UART0Buffer[UART0Count] = LPC_UART0->RBR;
	  	UART0Count++;
	  	if ( UART0Count == BUFSIZE )
	  		{
			UART0Count = 0;		/* buffer overflow */
	  	/*	}*/	
		}
  	}
else if ( IIRValue == IIR_RDA )	/* Receive Data Available */
  	{
	
	data=LPC_UART0->RBR;
	rx_buffer0[rx_wr_index0]=data;
   	bRXIN0=1;
   	if (++rx_wr_index0 == RX_BUFFER_SIZE0) rx_wr_index0=0;
   	if (++rx_counter0 == RX_BUFFER_SIZE0)
      	{
      	rx_counter0=0;
      	rx_buffer_overflow0=1;
      	}

	/* Receive Data Available */
/*	UART0Buffer[UART0Count] = LPC_UART0->RBR;
	UART0Count++;
	if ( UART0Count == BUFSIZE )
		{
	  	UART0Count = 0;		/* buffer overflow */
	/*	} */

	modbus_rx_buffer[modbus_rx_buffer_ptr]=data;
	modbus_rx_buffer_ptr++;
	modbus_timeout_cnt=0;
  	}
else if ( IIRValue == IIR_CTI )	/* Character timeout indicator */
  	{
	/* Character Time-out indicator */
	UART0Status |= 0x100;		/* Bit 9 as the CTI error */
  	}
else if ( IIRValue == IIR_THRE )	/* THRE, transmit holding register empty */
  	{
	/* THRE interrupt */
	
	LSRValue = LPC_UART0->LSR;		/* Check status in the LSR to see if
									valid data in U0THR or not */
	if ( LSRValue & LSR_THRE )
		{
	  	UART0TxEmpty = 1;
		if (tx_counter0)
   			{
   			--tx_counter0;
   			LPC_UART0->THR=tx_buffer0[tx_rd_index0];
   			if (++tx_rd_index0 == TX_BUFFER_SIZE0) tx_rd_index0=0;
   			}
		}
	else
		{
	  	UART0TxEmpty = 0;
		}
  	}
}

/*
//***********************************************
__irq void uart0_interrupt(void)
{
char status,u0iir,data;

status=U0LSR;
u0iir=U0IIR;

if((u0iir&0x0f)==4)
	{
	plazma_uart0++;
	data=U0RBR;
	rx_buffer0[rx_wr_index0]=data;
   	bRXIN0=1;
   	if (++rx_wr_index0 == RX_BUFFER_SIZE0) rx_wr_index0=0;
   	if (++rx_counter0 == RX_BUFFER_SIZE0)
      	{
      	rx_counter0=0;
      	rx_buffer_overflow0=1;
      	}
   	}
else if((u0iir&0x0f)==2)
	{
	if (tx_counter0)
   		{
   		--tx_counter0;
   		U0THR=tx_buffer0[tx_rd_index0];
   		if (++tx_rd_index0 == TX_BUFFER_SIZE0) tx_rd_index0=0;
   		}
	} 
VICVectAddr = 0;	  	
}*/

//-----------------------------------------------
void uart_in_an0(void)
{



if((UIB0[0]=='r')&&(UIB0[1]=='e')&&(UIB0[2]=='a')&&(UIB0[3]=='d')&&(UIB0[6]==crc_87(UIB0,6))&&(UIB0[7]==crc_95(UIB0,6)))
	{
	unsigned short ptr;
	unsigned long data1,data2;
	char temp_out[20];
	ptr=UIB0[4]+(UIB0[5]*256U);
	data1=lc640_read_long(ptr);
	data2=lc640_read_long(ptr+4);
	temp_out[0]='r';
	temp_out[1]='e';
	temp_out[2]='a';
	temp_out[3]='d';
	temp_out[4]=*((char*)&ptr);
	temp_out[5]=*(((char*)&ptr)+1);	
	temp_out[6]=*((char*)&data1);
	temp_out[7]=*(((char*)&data1)+1);		
	temp_out[8]=*(((char*)&data1)+2);	
	temp_out[9]=*(((char*)&data1)+3);		
	temp_out[10]=*((char*)&data2);
	temp_out[11]=*(((char*)&data2)+1);		
	temp_out[12]=*(((char*)&data2)+2);	
	temp_out[13]=*(((char*)&data2)+3);	
	temp_out[14]=crc_87(temp_out,14);	
	temp_out[15]=crc_95(temp_out,14);			
	uart_out_adr0(temp_out,16);
	}


if((UIB0[0]=='w')&&(UIB0[1]=='r')&&(UIB0[2]=='i')&&(UIB0[3]=='t')&&(UIB0[4]=='e')&&(UIB0[15]==crc_87(UIB0,15))&&(UIB0[16]==crc_95(UIB0,15)))
	{
	unsigned short ptr;
	unsigned long data1,data2;
	char temp_out[20];
	ptr=UIB0[5]+(UIB0[6]*256U);
	*((char*)&data1)=UIB0[7];
	*(((char*)&data1)+1)=UIB0[8];
	*(((char*)&data1)+2)=UIB0[9];
	*(((char*)&data1)+3)=UIB0[10];
	*((char*)&data2)=UIB0[11];
	*(((char*)&data2)+1)=UIB0[12];
	*(((char*)&data2)+2)=UIB0[13];
	*(((char*)&data2)+3)=UIB0[14];	
	lc640_write_long(ptr,data1);
	lc640_write_long(ptr+4,data2);
	
	//data1=lc640_read_long(ptr);
	//data2=lc640_read_long(ptr+4);
	temp_out[0]='w';
	temp_out[1]='r';
	temp_out[2]='i';
	temp_out[3]='t';
	temp_out[4]='e';
	temp_out[5]=*((char*)&ptr);
	temp_out[6]=*(((char*)&ptr)+1);	
	/*temp_out[6]=*((char*)&data1);
	temp_out[7]=*(((char*)&data1)+1);		
	temp_out[8]=*(((char*)&data1)+2);	
	temp_out[9]=*(((char*)&data1)+3);		
	temp_out[10]=*((char*)&data2);
	temp_out[11]=*(((char*)&data2)+1);		
	temp_out[12]=*(((char*)&data2)+2);	
	temp_out[13]=*(((char*)&data2)+3);	*/
	temp_out[7]=crc_87(temp_out,7);	
	temp_out[8]=crc_95(temp_out,7);			
	uart_out_adr0(temp_out,9);
	}






			
else if((UIB0[0]==0x55)&&(!PT)&&(!UIB0[2])&&(!UIB0[3])&&
	(((UIB0[4]&0xf0)==0x10)||((UIB0[4]&0xf0)==0x20))&&((UIB0[4]&0x0f)==0x0a)
	&&(UIB0[(UIB0[1]&0x1f)+5]==crc_87(UIB0,(UIB0[1]&0x1f)+5))&&(UIB0[(UIB0[1]&0x1f)+6]==crc_95(UIB0,(UIB0[1]&0x1f)+5)))
	{
	
	if((C_D)&&(FR)&&((UIB0[1]&0x1f)==4)&&(UIB0[5]==2)&&(UIB0[6]==2))
		{
		if((UIB0[7]==1)&&(UIB0[8]==1)) 
			{
/////			MAIN_IST=0;
/////			cnt_src[1]=10;
			lc640_write_int(EE_MAIN_BPS,0);
			cnt_src[1]=10;
			}
		else if((UIB0[7]==2)&&(UIB0[8]==2))
			{
/////		MAIN_IST=1;
/////			cnt_src[0]=10;
			lc640_write_int(EE_MAIN_BPS,1);
			cnt_src[0]=10;
			}
		
		St_[0]&=0x63;
		St_[1]&=0x63;
		
     	memo_out[0]=0x55;
     	memo_out[1]=0x20+4;
     	memo_out[2]=0;
     	memo_out[3]=0;
     	memo_out[4]=((UIB0[4]>>4)&0x0f)+((UIB0[4]<<4)&0xf0);
     	memo_out[5]=0x02;
     	memo_out[6]=0x02;
     	memo_out[7]=UIB0[7];
     	memo_out[8]=UIB0[8];
     	memo_out[9]=crc_87(memo_out,9);
		memo_out[10]=crc_95(memo_out,9);
     	uart_out_adr0(memo_out,11); 
     	
		} 
	else if((C_D)&&(FR)&&((UIB0[1]&0x1f)==3)&&(UIB0[5]==3)&&(UIB0[6]==3))
		{ 

		if(!(St&0x03)&&(NUMBAT))
			{
			spc_stat=spc_VZ;
			cnt_vz_sec_=3600UL*UIB0[7];
			memo_out[6]=0xff;
			}
		else
 			{
 			memo_out[6]=0x01;	
 			}	


     	memo_out[0]=0x55;
     	memo_out[1]=0x20+2;
     	memo_out[2]=0;
     	memo_out[3]=0;
     	memo_out[4]=((UIB0[4]>>4)&0x0f)+((UIB0[4]<<4)&0xf0);
     	memo_out[5]=0x03;
     	
         	memo_out[7]=crc_87(memo_out,7);
		memo_out[8]=crc_95(memo_out,7);
     	uart_out_adr0(memo_out,9); 		
     	

		}
		
	else if((C_D)&&(FR)&&((UIB0[1]&0x1f)==2)&&(UIB0[5]==7)&&(UIB0[6]==7))
		{ 

		spc_stat=spc_OFF;
				
     	memo_out[0]=0x55;
     	memo_out[1]=0x20+2;
     	memo_out[2]=0;
     	memo_out[3]=0;
     	memo_out[4]=((UIB0[4]>>4)&0x0f)+((UIB0[4]<<4)&0xf0);
     	memo_out[5]=0x07;
     	memo_out[6]=0xff;
         	memo_out[7]=crc_87(memo_out,7);
		memo_out[8]=crc_95(memo_out,7);
     	uart_out_adr0(memo_out,9); 		
     	
  
		}			

	else if((C_D)&&(FR)&&((UIB0[1]&0x1f)==2)&&(UIB0[5]==4)&&(UIB0[6]==4))
		{ 
  		if(!ke_start()) memo_out[6]=0xff;
		else memo_out[6]=0x01;
     	memo_out[0]=0x55;
     	memo_out[1]=0x20+2;
     	memo_out[2]=0;
     	memo_out[3]=0;
     	memo_out[4]=((UIB0[4]>>4)&0x0f)+((UIB0[4]<<4)&0xf0);
     	memo_out[5]=0x04;
     	
         	memo_out[7]=crc_87(memo_out,7);
		memo_out[8]=crc_95(memo_out,7);
     	uart_out_adr0(memo_out,9); 		
     	
     	}		    

	else if((C_D)&&(FR)&&((UIB0[1]&0x1f)==2)&&(UIB0[5]==8)&&(UIB0[6]==8))
		{ 

    		spc_stat=spc_OFF;

     	memo_out[0]=0x55;
     	memo_out[1]=0x20+2;
     	memo_out[2]=0;
     	memo_out[3]=0;
     	memo_out[4]=((UIB0[4]>>4)&0x0f)+((UIB0[4]<<4)&0xf0);
     	memo_out[5]=0x08;
     	memo_out[6]=0xff;
         	memo_out[7]=crc_87(memo_out,7);
		memo_out[8]=crc_95(memo_out,7);
     	uart_out_adr0(memo_out,9); 		
     	

		}
		
	else if((C_D)&&(FR)&&((UIB0[1]&0x1f)==2)&&(UIB0[5]==6)&&(UIB0[6]==6))
		{ 
		//plazma++;
     	memo_out[0]=0x55;
     	memo_out[1]=0x20+2;
     	memo_out[2]=0;
     	memo_out[3]=0;
     	memo_out[4]=((UIB0[4]>>4)&0x0f)+((UIB0[4]<<4)&0xf0);
     	memo_out[5]=0x06;
     	memo_out[6]=0x06;
         	memo_out[7]=crc_87(memo_out,7);
		memo_out[8]=crc_95(memo_out,7);
     	uart_out_adr0(memo_out,9); 		
     	
		}	

	else if((C_D)&&(FR)&&((UIB0[1]&0x1f)==4)&&(UIB0[5]==5)&&(UIB0[6]==5)&&(UIB0[7]==1)&&(UIB0[8]==1))
		{ 
  		
		St_[0]|=0x20;
		St_[1]&=0xdf;
		St&=0xfb;
/////		cnt_imax=0;  
		cnt_src[0]=10;
/////		cnt_alias_blok=60;
		
     	memo_out[0]=0x55;
     	memo_out[1]=0x20+4;
     	memo_out[2]=0;
     	memo_out[3]=0;
     	memo_out[4]=((UIB0[4]>>4)&0x0f)+((UIB0[4]<<4)&0xf0);
     	memo_out[5]=5;
     	memo_out[6]=5; 
     	memo_out[7]=1;
     	memo_out[8]=1;     	
         	memo_out[9]=crc_87(memo_out,9);
		memo_out[10]=crc_95(memo_out,9);
     	uart_out_adr0(memo_out,11); 		
     	
     	
		}	

	else if((C_D)&&(FR)&&((UIB0[1]&0x1f)==4)&&(UIB0[5]==5)&&(UIB0[6]==5)&&(UIB0[7]==2)&&(UIB0[8]==2))
		{ 

		St_[1]|=0x20;
		St_[0]&=0xdf;
		St&=0xfb;
/////		cnt_imax=0;  
		cnt_src[0]=10;
/////		cnt_alias_blok=60;
		
     	memo_out[0]=0x55;
     	memo_out[1]=0x20+4;
     	memo_out[2]=0;
     	memo_out[3]=0;
     	memo_out[4]=((UIB0[4]>>4)&0x0f)+((UIB0[4]<<4)&0xf0);
     	memo_out[5]=5;
     	memo_out[6]=5; 
     	memo_out[7]=2;
     	memo_out[8]=2;     	
         	memo_out[9]=crc_87(memo_out,9);
		memo_out[10]=crc_95(memo_out,9);
     	uart_out_adr0(memo_out,11); 		
     	
     		
     	

		}	

	else if((C_D)&&(FR)&&((UIB0[1]&0x1f)==1)&&(UIB0[5]==1))
		{
	/*	memo_out[0]=0x55;
     	memo_out[1]=0x20+27;
     	memo_out[2]=0;
     	memo_out[3]=0;
     	memo_out[4]=((UIB0[4]>>4)&0x0f)+((UIB0[4]<<4)&0xf0); 
     	memo_out[5]=0x01;
     	memo_out[6]=data_rs[0];
		memo_out[7]=data_rs[1];
		memo_out[8]=data_rs[2];
		memo_out[9]=data_rs[3];
		memo_out[10]=data_rs[4];
		memo_out[11]=data_rs[5];
		memo_out[12]=data_rs[6];
    		memo_out[13]=data_rs[7];
		memo_out[14]=data_rs[8];													
		memo_out[15]=data_rs[9];
		memo_out[16]=data_rs[10];
		memo_out[17]=data_rs[11];
		memo_out[18]=data_rs[12];
		memo_out[19]=data_rs[13];
		memo_out[20]=data_rs[14];
		memo_out[21]=data_rs[15];
		memo_out[22]=data_rs[16]; 
		memo_out[23]=data_rs[17];
		memo_out[24]=data_rs[18];
		memo_out[25]=data_rs[19];
		memo_out[26]=data_rs[20];
		memo_out[27]=data_rs[21]; 	
		memo_out[28]=data_rs[22];
		memo_out[29]=data_rs[23];
		memo_out[30]=data_rs[24];
		memo_out[31]=data_rs[25]; 				
		memo_out[32]=crc_87(memo_out,32);
		memo_out[33]=crc_95(memo_out,32);
	
          uart_out_adr0(memo_out,34); */


		memo_out[0]=0x55;
     	memo_out[1]=0x20+18;
     	memo_out[2]=0;
     	memo_out[3]=0;
     	memo_out[4]=((UIB0[4]>>4)&0x0f)+((UIB0[4]<<4)&0xf0); 
     	memo_out[5]=0x01;
     	memo_out[6]=data_rs0[0];
		memo_out[7]=data_rs0[1];
		memo_out[8]=data_rs0[2];
		memo_out[9]=data_rs0[3];
		memo_out[10]=data_rs0[4];
		memo_out[11]=data_rs0[5];
		memo_out[12]=data_rs0[6];
    		memo_out[13]=data_rs0[7];
		memo_out[14]=data_rs0[8];													
		memo_out[15]=data_rs0[9];
		memo_out[16]=data_rs0[10];
		memo_out[17]=data_rs0[11];
		memo_out[18]=data_rs0[12];
		memo_out[19]=data_rs0[13];
		memo_out[20]=data_rs0[14];
		memo_out[21]=data_rs0[15];
		memo_out[22]=data_rs0[16]; 
		memo_out[23]=crc_87(memo_out,23);
		memo_out[24]=crc_95(memo_out,23);
	
          uart_out_adr0(memo_out,25); 




		}					
	
	}

}



//-----------------------------------------------
signed short index_offset0 (signed short index,signed short offset)
{
index=index+offset;
if(index>=RX_BUFFER_SIZE0) index-=RX_BUFFER_SIZE0; 
if(index<0) index+=RX_BUFFER_SIZE0;
return index;
}

//-----------------------------------------------
char control_check0(signed short index)
{
char i=0,ii=0,iii;

if(rx_buffer0[index]!=END) goto error_cc;

ii=rx_buffer0[index_offset0(index,-2)];
iii=0;
for(i=0;i<=ii;i++)
	{
	iii^=rx_buffer0[index_offset0(index,-2-ii+i)];
	}
if (iii!=rx_buffer0[index_offset0(index,-1)]) goto error_cc;	


//success_cc:
return 1;
error_cc:
return 0;

}

//-----------------------------------------------
void uart_in0(void)
{
char temp,i/*,count*/;

//__disable_irq();

if(rx_buffer_overflow0)
	{
	rx_wr_index0=0;
	rx_rd_index0=0;
	rx_counter0=0;
	rx_buffer_overflow0=0;
	}    
	
if(rx_counter0&&(rx_buffer0[index_offset0(rx_wr_index0,-1)])==END)
	{
	
     temp=rx_buffer0[index_offset0(rx_wr_index0,-3)];
    	if(temp<100) 
    		{
    		if(control_check0(index_offset0(rx_wr_index0,-1)))
    			{
    		
    			rx_rd_index0=index_offset0(rx_wr_index0,-3-temp);
    			for(i=0;i<temp;i++)
				{
				UIB0[i]=rx_buffer0[index_offset0(rx_rd_index0,i)];
				} 
			rx_rd_index0=rx_wr_index0;
			rx_counter0=0;
			
			uart_in_an0();
    			}
 	
    		} 
    	}	


//__enable_irq();     
}   	
