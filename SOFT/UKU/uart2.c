
//#include "uart0.h"
#include "LPC17XX.H" 
#include "uart2.h"
#include "main.h"
#include "control.h"
#include "cmd.c"
#include "eeprom_map.h"
#include "25lc640.h"


char bRXIN2;
char UIB2[100]={0,0,0,0,0,0,0,0,0,0};
char UIB20[30]={0,0,0,0,0,0,0,0,0,0};
char flag2;
char rx_buffer2[RX_BUFFER_SIZE2];
char tx_buffer2[TX_BUFFER_SIZE2];
unsigned short rx_wr_index2,rx_rd_index2,rx_counter2;
unsigned short tx_wr_index2,tx_rd_index2,tx_counter2;
char rx_buffer_overflow2;
char plazma_uart2;
char memo_out2[50];
char data_rs2[50];
char data_rs02[50];

volatile uint32_t UART2Status;
volatile uint8_t UART2TxEmpty = 1;
//volatile uint8_t UART0Buffer[BUFSIZE];
volatile uint32_t UART2Count = 0;



//-----------------------------------------------
void putchar2(char c)
{
while (tx_counter2 == TX_BUFFER_SIZE2);
if (tx_counter2 || ((LPC_UART2->LSR & 0x60)==0))
   {
   tx_buffer2[tx_wr_index2]=c;
   if (++tx_wr_index2 == TX_BUFFER_SIZE2) tx_wr_index2=0;
   ++tx_counter2;
   }
else LPC_UART2->THR=c;
}

//-----------------------------------------------
void uart_out2 (char num,char data0,char data1,char data2,char data3,char data4,char data5)
{
char i,t=0;
//char *ptr=&data1;
char UOB2[16]; 
UOB2[0]=data0;
UOB2[1]=data1;
UOB2[2]=data2;
UOB2[3]=data3;
UOB2[4]=data4;
UOB2[5]=data5;

for (i=0;i<num;i++)
	{
	t^=UOB2[i];
	}    
UOB2[num]=num;
t^=UOB2[num];
UOB2[num+1]=t;
UOB2[num+2]=END;

for (i=0;i<num+3;i++)
	{
	putchar2(UOB2[i]);
	}   	
}

//-----------------------------------------------
void uart_out_adr2 (char *ptr, char len)
{
char UOB2[100];
char i,t=0;

for(i=0;i<len;i++)
	{
	UOB2[i]=ptr[i];
	t^=UOB2[i];
	}
//if(!t)t=0xff;
UOB2[len]=len;
t^=len;	
UOB2[len+1]=t;	
UOB2[len+2]=END;
	
for (i=0;i<len+3;i++)
	{
	putchar2(UOB2[i]);
	}   
}


//-----------------------------------------------
void uart_out_adr2_block (unsigned long adress,char *ptr, char len)
{
char UOB2[100]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
char i,temp11,t=0;
unsigned i11;

t=0;
temp11=CMND;
t^=temp11;
putchar2(temp11);

temp11=10;
t^=temp11;
putchar2(temp11);

temp11=(*((char*)&adress));
t^=temp11;
putchar2(temp11);

temp11=(*(((char*)&adress)+1));
t^=temp11;
putchar2(temp11);

temp11=(*(((char*)&adress)+2));
t^=temp11;
putchar2(temp11);

temp11=(*(((char*)&adress)+3));
t^=temp11;
putchar2(temp11);


for(i11=0;i11<len;i11++)
	{
	temp11=ptr[i11];
	t^=temp11;
	putchar2(temp11);
	}
	
temp11=(len+6);
t^=temp11;
putchar2(temp11);

putchar2(t);

putchar2(0x0a);
	
}


//-----------------------------------------------
uint32_t UART_2_Init(uint32_t baudrate )
{
uint32_t Fdiv;
uint32_t pclkdiv, pclk;

LPC_PINCON->PINSEL4 &= ~0x000f0000;//!!!!!!!!!!!
LPC_PINCON->PINSEL4 |= 0x000a0000; //!!!!!!!!!!! 
/* RxD0 is P0.3 and TxD0 is P0.2 */
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

LPC_UART2->LCR = 0x83;		/* 8 bits, no Parity, 1 Stop bit */
Fdiv = ( pclk / 16 ) / baudrate ;	/*baud rate */
LPC_UART2->DLM = Fdiv / 256;							
LPC_UART2->DLL = Fdiv % 256;
LPC_UART2->LCR = 0x03;		/* DLAB = 0 */
LPC_UART2->FCR = 0x07;		/* Enable and reset TX and RX FIFO. */

NVIC_EnableIRQ(UART2_IRQn);

LPC_UART2->IER = IER_RBR | IER_THRE | IER_RLS;	/* Enable UART2 interrupt */
return (TRUE);
}

//-----------------------------------------------
char getchar2(void)
{
char data;
while (rx_counter2==0);
data=rx_buffer2[rx_rd_index2];
if (++rx_rd_index2 == RX_BUFFER_SIZE2) rx_rd_index2=0;
--rx_counter2;
return data;
}

//***********************************************
void UART2_IRQHandler (void) 
{
uint8_t IIRValue, LSRValue;
uint8_t Dummy = Dummy;
char status,u2iir,data;
//plazma_uart2++;
			
IIRValue = LPC_UART2->IIR;
    
IIRValue >>= 1;			/* skip pending bit in IIR */
IIRValue &= 0x07;			/* check bit 1~3, interrupt identification */
if ( IIRValue == IIR_RLS )		/* Receive Line Status */
  	{
	LSRValue = LPC_UART2->LSR;
	/* Receive Line Status */
	if ( LSRValue & (LSR_OE|LSR_PE|LSR_FE|LSR_RXFE|LSR_BI) )
		{
	  	/* There are errors or break interrupt */
	  	/* Read LSR will clear the interrupt */
	  	UART2Status = LSRValue;
	  	Dummy = LPC_UART2->RBR;		/* Dummy read on RX to clear 
							interrupt, then bail out */
	  	return;
		}
	if ( LSRValue & LSR_RDR )	/* Receive Data Ready */			
		{
		
		data=LPC_UART2->RBR;
		rx_buffer2[rx_wr_index2]=data;
   		bRXIN2=1;
   		if (++rx_wr_index2 == RX_BUFFER_SIZE2) rx_wr_index2=0;
   		if (++rx_counter2 == RX_BUFFER_SIZE2)
      		{
      		rx_counter2=0;
      		rx_buffer_overflow2=1;
      		}
		}
  	}
else if ( IIRValue == IIR_RDA )	/* Receive Data Available */
  	{
	
	data=LPC_UART2->RBR;
	rx_buffer2[rx_wr_index2]=data;
   	bRXIN2=1;
   	if (++rx_wr_index2 == RX_BUFFER_SIZE2) rx_wr_index2=0;
   	if (++rx_counter2 == RX_BUFFER_SIZE2)
      	{
      	rx_counter2=0;
      	rx_buffer_overflow2=1;
      	}


  	}
else if ( IIRValue == IIR_CTI )	/* Character timeout indicator */
  	{
	/* Character Time-out indicator */
	UART2Status |= 0x100;		/* Bit 9 as the CTI error */
  	}
else if ( IIRValue == IIR_THRE )	/* THRE, transmit holding register empty */
  	{
	/* THRE interrupt */
	
	LSRValue = LPC_UART2->LSR;		/* Check status in the LSR to see if
									valid data in U0THR or not */
	if ( LSRValue & LSR_THRE )
		{
	  	UART2TxEmpty = 1;
		if (tx_counter2)
   			{
   			--tx_counter2;
   			LPC_UART2->THR=tx_buffer2[tx_rd_index2];
   			if (++tx_rd_index2 == TX_BUFFER_SIZE2) tx_rd_index2=0;
   			}
		}
	else
		{
	  	UART2TxEmpty = 0;
		}
  	}
}



//-----------------------------------------------
void uart_in_an2(void)
{
char i;



/*if(UIB2[1]!=0x51)
     {
     for(i=0;i<24;i++)
	     {
	     UIB20[i]=UIB2[i];
	     }
     }*/
//uart2_mess[0]++;

/*if((UIB2[0]==0x55)&&(UIB2[1]==0x66))
	{
	
	uart_out2(2,0x57,0x66,0,0,0,0);
	}

else if((UIB2[0]==CMND)&&(UIB2[1]==1))
	{
	adc_buff_out_[0]=UIB2[2]+(UIB2[3]*256);
	adc_buff_out_[1]=UIB2[4]+(UIB2[5]*256);
	}

else if((UIB2[0]==CMND)&&(UIB2[1]==2))
	{
	adc_buff_out_[2]=UIB2[2]+(UIB2[3]*256);
	in_stat_out[0]=UIB2[4];
	in_stat_out[1]=UIB2[5];
	}
	
else if((UIB2[0]==CMND)&&(UIB2[1]==QWEST)&&(UIB2[2]==PUTTM))
	{
	adc_buff_out_[0]=UIB2[3]+(UIB2[4]*256);
	adc_buff_out_[1]=UIB2[5]+(UIB2[6]*256);
	adc_buff_out_[2]=UIB2[7]+(UIB2[8]*256);
	in_stat_out[0]=UIB2[9];
	in_stat_out[1]=UIB2[10];
	in_stat_out[2]=UIB2[11];
	in_stat_out[3]=UIB2[12];	
	}	


		
*/	
	
if((UIB2[0]==0x33)&&(UIB2[4+UIB2[2]]==crc_87(UIB2,4+UIB2[2]))&&(UIB2[5+UIB2[2]]==crc_95(UIB2,4+UIB2[2])))
	{
	//suzz[0]++;
     
 	if((UIB2[1]==0x51)&&(UIB2[4]==1))
		{
		unsigned char lengt,i;
          plazma_uart2++;
		lengt=37;
		//if(lc640_read(BAT1_STAT)==ON)lengt+=13;
		//if(lc640_read(BAT1_STAT)==ON)lengt+=13;
		if(BAT_IS_ON==bisON)lengt+=13;
		lengt+=NUMIST*10;
		//lengt+=NUMINV*10;
		lengt+=NUMDT*4;
		lengt+=NUMSK*2;
		
		memo_out2[0]=0x33;
		memo_out2[1]=0x61;
	    	memo_out2[2]=lengt;
	    	memo_out2[3]=0x03;		//с приоритетами
	    	
	     memo_out2[4]=0x01;       //ответ на запрос телеметрии

		memo_out2[5]=0;		//Структура - количество введенных батарей.
		/*if(lc640_read(BAT1_STAT)==ON)memo_out2[5]++;
		if(lc640_read(BAT2_STAT)==ON)*/
		if(BAT_IS_ON==bisON)memo_out2[5]++;

		memo_out2[6]=NUMIST;	//Структура - количество БПСов в структуре.
		memo_out2[7]=0/*K[NUMINV]*/;	//Структура - количество инверторов в структуре.
		memo_out2[8]=NUMDT;//Структура - количество внешних датчиков температур.
		memo_out2[9]=NUMSK;	// Структура - количество внешних логических входов(сухие контакты).
		memo_out2[10]=4;	//Структура, количество внешних исполнительных устройств
		
		memo_out2[11]=1;	//Резерв, не анализируется.
		memo_out2[12]=1;	//Резерв, не анализируется.
		memo_out2[13]=1;	//Резерв, не анализируется.


		memo_out2[14]=19;	//Номер байта, с которого начинается блок информации о первичной сети питания.
		memo_out2[15]=26;	//Номер байта, с которого начинается блок информации о нагрузке.
		
		
		lengt=32;
		memo_out2[16]=lengt;	//Номер байта, с которого начинается блок информации о батареях (Если 0xFF, то информация о батареях не передается).
	    	if(memo_out2[5]==0)memo_out2[16]=0xff;
		              
		lengt+=(13*memo_out2[5]);
		memo_out2[17]=lengt;	//Номер байта, с которого начинается блок информации о БПСах (Если 0xFF, то информация о БПСах не передается).
		if(NUMIST==0)memo_out2[17]=0xff;
		
		lengt+=(10*NUMIST);
		memo_out2[18]=lengt;//Номер байта, с которого начинается блок информации об инверторах (Если 0xFF, то информация об инверторах не передается).
          /*if(K[NUMINV]==0)*/memo_out2[18]=0xff;
           
          lengt+=0;
		memo_out2[19]=lengt;	//Номер байта, с которого начинается блок информации об внешних датчиках температуры (Если 0xFF, то информация об внешних датчиках температуры не передается).
          if(NUMDT==0)memo_out2[19]=0xff;
          
          lengt+=(4*NUMDT);
		memo_out2[20]=lengt;	//Номер байта, с которого начинается блок информации об внешних логических входах (Если 0xFF, то информация об внешних логических входах  не передается).
          if(NUMSK==0)memo_out2[20]=0xff;
 
 
          lengt+=(2*NUMSK);
		memo_out2[21]=lengt;	//Номер байта, с которого начинается блок информации об спецфункциях

          
		memo_out2[22]=memo_out2[21]+1;	//Номер байта, с которого начинается блок информации об исполнительных устройствах

		//Блок информации о первичной сети питания
                  //unet_f=231;
                  //Fnet=543;
                  
          lengt=memo_out2[14]+4;
                  
  		memo_out2[lengt++]=(char)Unet;		//Напряжение питающей сети (дискретность 1В), младший байт.
		memo_out2[lengt++]=(char)(Unet/256);	//Напряжение питающей сети (дискретность 1В), старший байт.
		memo_out2[lengt++]=(char)fnet;		//Частота питающей сети (дискретность 0,1Гц), младший байт.
		memo_out2[lengt++]=(char)(fnet/256);	//Частота питающей сети (дискретность 0,1Гц), старший байт.
		memo_out2[lengt++]=1;	//Резерв, не анализируется.
		memo_out2[lengt++]=1;	//Резерв, не анализируется.
		memo_out2[lengt]=2;	// Байт состояния питающей сети.
          if(St&0x01)memo_out2[lengt]|=0x01;
		//0	0	0	0	0	0	F	А
		
		//А: Авария(1 - активно).
		//F: 0 - частота не измеряется, байты 2 3 блока информации о нагрузке не анализируются, информация о частоте сети на экран терминальной программы не выводится.

		//Блок информации о нагрузке
		lengt=memo_out2[15]+4;

		memo_out2[lengt++]=(char)Uload; 		//Напряжение нагрузки (дискретность 100мВ), младший байт.
		memo_out2[lengt++]=(char)(Uload/256); 	//Напряжение нагрузки (дискретность 100мВ), старший байт.
		memo_out2[lengt++]=(char)iload; 		//Ток нагрузки (дискретность 100мА), младший байт.
		memo_out2[lengt++]=(char)(iload/256); 	//Ток нагрузки (дискретность 100мА), старший байт.
		memo_out2[lengt++]=1; //Резерв, не анализируется.
		memo_out2[lengt++]=1; //Резерв, не анализируется.

		//Блок информации о батареях
          
		if(memo_out2[16]!=0xff)
			{
			lengt=memo_out2[16]+4;

			for(i=0;i<memo_out2[5];i++)
				{
				char ttt;
				
				memo_out2[lengt++]=i+1; 	//Номер батареи(начиная с 1). Если 0xFF, то на экране программы батарея не нумеруется.
				memo_out2[lengt++]=(char)Ubat; 		//Напряжение батареи (дискретность 100мВ), младший байт.
				memo_out2[lengt++]=(char)(Ubat/256); //Напряжение батареи (дискретность 100мВ), старший байт.
				memo_out2[lengt++]=(char)Ibat; 		//Ток батареи (дискретность 10мА, со знаком), младший байт.
				memo_out2[lengt++]=(char)(Ibat>>8); 	//Ток батареи (дискретность 10мА, со знаком), старший байт.
				memo_out2[lengt++]=(char)tbat; 		//Температура батареи (дискретность ?С, со знаком), младший байт.
				memo_out2[lengt++]=(char)(tbat/256); //Температура батареи (дискретность ?С, со знаком), старший байт.
				
				ttt=0; 
				if(St&0x02)ttt|=0x01;
				if(Ibat>0)ttt|=0x02;
				memo_out2[lengt++]=ttt; //Байт состояния батареи.
				
				

				//0	0	0	0	0	0	Z	А
	               //А: Авария(1 - активно) - батарея не выявлена в ходе последнего контроля наличия батареи(на экране терминальной программы все графы данной батареи заменить надписью "Авария, батарея отсутствует")
				//Z: Заряд:  если 1 - батарея заряжается. 
			     
				if(BAT_C_REAL==0x5555)
					{
					memo_out2[lengt++]=(char)(BAT_C_NOM*10); //Емкость батареи (дискретность 100мА*Ч), младший байт.
					memo_out2[lengt++]=(char)((BAT_C_NOM*10)/256); //Емкость батареи (дискретность 100мА*Ч), старший байт.//Если емкость батареи 0xFFFF, то в графе "Емкость" прочерк.
					}
				else
					{	
					memo_out2[lengt++]=(char)BAT_C_REAL; //Емкость батареи (дискретность 100мА*Ч), младший байт.
					memo_out2[lengt++]=(char)(BAT_C_REAL/256); //Емкость батареи (дискретность 100мА*Ч), старший байт.//Если емкость батареи 0xFFFF, то в графе "Емкость" прочерк.
		    	    		}
		    	    	
		    	    	    	
		    	    	
		    	    	if(BAT_C_REAL==0x5555)memo_out2[lengt++]=0xff;//Заряд батареи в %..
		    	    	
		    		else memo_out2[lengt++]=zar_percent;
		    		memo_out2[lengt++]=1; //Резерв, не анализируется.
				memo_out2[lengt++]=1; //Резерв, не анализируется.
	   			}
         		}       
                
		//Блок информации об БПСах

		if(memo_out2[17]!=0xff)
			{
			lengt=memo_out2[17]+4;

			for(i=0;i<memo_out2[6];i++)
				{
				
				char src_off;
				
				if(i==0)src_off=OFFBP1;
				else src_off=OFFBP2;
				
				
				
				memo_out2[lengt++]=i+1; //Номер БПСа (начиная с 1).
	    			memo_out2[lengt++]=(char)(Us[i]); 		//Напряжение БПСа (дискретность 100мВ), младший байт.
				memo_out2[lengt++]=(char)((Us[i])>>8); 	//Напряжение БПСа (дискретность 100мВ), старший байт.
				memo_out2[lengt++]=(char)(Is[i]); 		//Ток БПСа (дискретность 100мА), младший байт.
				memo_out2[lengt++]=(char)((Is[i])>>8); 	//Ток БПСа (дискретность 100мА), старший байт.
				memo_out2[lengt++]=(char)(t_i[i]); 		//Температура БПСа (дискретность ?С, со знаком), младший байт.
				memo_out2[lengt++]=(char)((t_i[i])>>8); 	//Температура БПСа (дискретность ?С, со знаком), старший байт.
				
				memo_out2[lengt]=0; 							//Байт состояния БПСа.
                    if(St_[i]&(1<<2))memo_out2[lengt]=BIN8(1000); //авария по Umin
				else if(St_[i]&(1<<3))memo_out2[lengt]=BIN8(100); //авария по Umax
				else if(St_[i]&(1<<4))memo_out2[lengt]=BIN8(10); //авария по Tmax
				else if(St_[i]&(1<<5))memo_out2[lengt]=BIN8(100000); //заблокирован
				else if((!(St_[i]&0x3c))&&(!St&0x01)&&(!src_off))memo_out2[lengt]=BIN8(1); //Работает
				//0	0	EB 	АС	AM	AU	АT	W
                    lengt++;
				//W: Рабочее состояние (1 - активно) - БПС в работе.(На экране параметры работы БПСа)
 				//Если 0 то по-порядку анализируются биты:
				//AT: Авария по перегреву БПСа (1 - активно), ("Перегрев").
				//AU: Авария по заниженному напряжению (1 - активно), ("Занижено Uвых").
				//AM: Авария по завышенному напряжению (1 - активно), ("Завышено Uвых").
				//AC: Авария по отсутствию связи с БПС (1 - активно), ("Разрыв связи").
				//EB: Внешняя блокировка БПС (заблокирован по связи оператором), ("Заблокирован извне").
				//Если 5 вышеперечисленных битов в нуле, значит ("В резерве"). 
                   	
				memo_out2[lengt++]=1; //Резерв, не анализируется.
				memo_out2[lengt++]=1; //Резерв, не анализируется.
                    }     
   
               }
               
          lengt=memo_out2[21]+4;     
          if(spc_stat==spc_OFF) memo_out2[lengt++]=0;
          else if(spc_stat==spc_KE) memo_out2[lengt++]=1;
          else if(spc_stat==spc_VZ) memo_out2[lengt++]=10;

		memo_out2[lengt]=0;
		if(lc640_read_int(EE_IU1))memo_out2[lengt]|=0x01;;
          if(lc640_read_int(EE_IU2))memo_out2[lengt]|=0x02;
		if(lc640_read_int(EE_IU3))memo_out2[lengt]|=0x04;
		if(lc640_read_int(EE_IU4))memo_out2[lengt]|=0x08;
		     
          memo_out2[memo_out2[2]+4]=crc_87(memo_out2,memo_out2[2]+4);
		memo_out2[memo_out2[2]+5]=crc_95(memo_out2,memo_out2[2]+4);
		uart_out_adr2(memo_out2,memo_out2[2]+6);
		/*
		memo_out2[0]=0x33;
		memo_out2[1]=0x61;
		memo_out2[2]=60;
		memo_out2[3]=lengt;
		memo_out2[64]=crc_87(memo_out2,64);
		memo_out2[65]=crc_95(memo_out2,64);
		uart_out_adr2(memo_out2,66);*/
		
		          
 		}
		

/*Блок информации о внешних датчиках температуры

Байт данных [0]: Номер датчика (начиная с 1).
Байт данных [1]: Температура датчика (дискретность ?С, со знаком), младший байт.
Байт данных [2]: Температура датчика (дискретность ?С, со знаком),  старший байт.
Байт данных [3]: Байт состояния датчика.

0	0	0 	0	0	0	АT	A

А: Неисправность датчика (1 - активно), ("Датчик неисправен")
AT: Авария по температуре (1 - активно), ("Температура датчика не в допуске").

Блок информации о внешних логических входах

Байт данных [0]: Номер входа (начиная с 1).
Байт данных [1]: Байт состояния входа.

0	0	0 	0	0	0	АT	A

А: Активность входа (1 - активно), ("Состояние активное/неактивное")
AВ: Аварийность входа (1 - активно), ("Авария").*/

	else if((UIB2[1]==0x52)&&(UIB2[4]==2)&&(UIB2[5]==2))   //установить основной БПС
		{
		if((UIB2[6]==1)&&(UIB2[7]==1)) 
			{
			lc640_write_int(EE_MAIN_BPS,0);
			cnt_src[1]=10;
			}
		else if((UIB2[6]==2)&&(UIB2[7]==2))
			{
			lc640_write_int(EE_MAIN_BPS,1);
			cnt_src[0]=10;
			}
		
		St_[0]&=0x63;
		St_[1]&=0x63;
		
     	memo_out2[0]=0x33;
     	memo_out2[1]=0x62;
     	memo_out2[2]=4;
     	memo_out2[3]=0x03;
     	
     	memo_out2[4]=UIB2[4];
     	memo_out2[5]=UIB2[5];
     	memo_out2[6]=UIB2[6];
     	memo_out2[7]=UIB2[7];
     	
         	memo_out2[8]=crc_87(memo_out2,8);
		memo_out2[9]=crc_95(memo_out2,8);
     	uart_out_adr2(memo_out2,10); 		
		}			

	else if((UIB2[1]==0x52)&&(UIB2[4]==3)&&(UIB2[5]==3))   //выравнивающий заряд старт
		{
		if(!(St&0x03)&&(NUMBAT))
			{
			spc_stat=spc_VZ;
			cnt_vz_sec_=3600UL*UIB2[6];
			memo_out2[5]=0xff;
			}
		else
 			{
 			memo_out2[5]=0x01;	
 			}	
		
     	memo_out2[0]=0x33;
     	memo_out2[1]=0x62;
     	memo_out2[2]=2;
     	memo_out2[3]=0x03;
     	
     	memo_out2[4]=0x03;
     	
     	memo_out2[6]=crc_87(memo_out2,6);
		memo_out2[7]=crc_95(memo_out2,6);
     	uart_out_adr2(memo_out2,8); 		
		}			

	else if((UIB2[1]==0x52)&&(UIB2[4]==4)&&(UIB2[5]==4))	//контроль емкости старт 	
		{
	  	if(!(St&0x02)&&(NUMBAT))
			{
			spc_stat=spc_KE;
			//zar_cnt_ee_ke=0;
			zar_cnt=0L;
			memo_out2[5]=0xff;
			}
		else
			{
			memo_out2[5]=0x01;	
			}		        
		
		
		memo_out2[0]=0x33;
     	memo_out2[1]=0x62;
     	memo_out2[2]=2;
     	memo_out2[3]=0x03;
     	memo_out2[4]=4;
          	
         	memo_out2[6]=crc_87(memo_out2,6);
		memo_out2[7]=crc_95(memo_out2,6);
     	uart_out_adr2(memo_out2,8); 		
     		
		}	
	
	else if((UIB2[1]==0x52)&&(UIB2[4]==5)&&(UIB2[5]==5)&&(UIB2[6])&&(UIB2[6]<=NUMIST)&&(UIB2[6]==UIB2[7])) 	//Выключение источника 
		{
	
		if((UIB2[6]==1)&&(UIB2[7]==1)) 
			{
			St_[0]|=0x20;
			St_[1]&=0xdf;
			St&=0xfb;
			cnt_src[1]=10;
			}
			
		else if((UIB2[6]==2)&&(UIB2[7]==2))
			{
			St_[1]|=0x20;
			St_[0]&=0xdf;
			St&=0xfb;
			cnt_src[0]=10;
			}	
		
     	memo_out2[0]=0x33;
     	memo_out2[1]=0x62;
     	memo_out2[2]=4;
     	memo_out2[3]=0x03;
     	
     	memo_out2[4]=5;
     	memo_out2[5]=5;
     	memo_out2[6]=UIB2[6];
     	memo_out2[7]=UIB2[6];
         	memo_out2[8]=crc_87(memo_out2,8);
		memo_out2[9]=crc_95(memo_out2,8);
     	uart_out_adr2(memo_out2,10); 		
		}
		

	else if((UIB2[1]==0x52)&&(UIB2[4]==7)&&(UIB2[5]==7))	//Выключение спецфункций
		{
		
		spc_stat=spc_OFF;
		memo_out2[0]=0x33;
     	memo_out2[1]=0x62;
     	memo_out2[2]=2;
     	memo_out2[3]=0x03;
     	memo_out2[4]=7;
     	memo_out2[5]=0xff;
     	
         	memo_out2[6]=crc_87(memo_out2,6);
		memo_out2[7]=crc_95(memo_out2,6);
     	uart_out_adr2(memo_out2,8); 		
		}					
		
	else if((UIB2[1]==0x52)&&(UIB2[4]==8)&&(UIB2[5]==8))	//Разблокировать источники
		{      
		
		St_[0]&=0xdf;
		St_[1]&=0xdf;
		
     	memo_out2[0]=0x33;
     	memo_out2[1]=0x62;
     	memo_out2[2]=2;
     	memo_out2[3]=0x03;
     	
     	memo_out2[4]=8;
     	memo_out2[5]=0xff;
     	
         	memo_out2[6]=crc_87(memo_out2,6);
		memo_out2[7]=crc_95(memo_out2,6);
     	uart_out_adr2(memo_out2,8); 		
		}		


	else if((UIB2[1]==0x52)&&(UIB2[4]==37))	//Изменить состояние исполнительных устройств
		{      
		
		if(UIB2[5]&0x01)
			{
			if(UIB2[6]&0x01)lc640_write_int(EE_IU1,1);
			else lc640_write_int(EE_IU1,0);
			}

		if(UIB2[5]&0x02)
			{
			if(UIB2[6]&0x02)lc640_write_int(EE_IU2,1);
			else lc640_write_int(EE_IU2,0);
			}

		if(UIB2[5]&0x04)
			{
			if(UIB2[6]&0x04)lc640_write_int(EE_IU3,1);
			else lc640_write_int(EE_IU3,0);
			}

		if(UIB2[5]&0x08)
			{
			if(UIB2[6]&0x08)lc640_write_int(EE_IU4,1);
			else lc640_write_int(EE_IU4,0);
			}
		
     	memo_out2[0]=0x33;
     	memo_out2[1]=0x62;
     	memo_out2[2]=2;
     	memo_out2[3]=0x03;
     	
     	memo_out2[4]=8;
     	memo_out2[5]=0xff;
     	
         	memo_out2[6]=crc_87(memo_out2,6);
		memo_out2[7]=crc_95(memo_out2,6);
     	uart_out_adr2(memo_out2,8); 		
		}


	else if((UIB2[1]==0x51)&&(UIB2[4]==0x0c))
		{
//		char temp;
		
     	memo_out2[0]=0x33;
     	memo_out2[1]=0x61;
     	memo_out2[2]=2;
     	memo_out2[3]=0x03;
     	memo_out2[4]=0x0c;
     	memo_out2[5]=lc640_read_int(CNT_EVENT_LOG);
		plazma_suzun=memo_out2[5];
         	memo_out2[6]=crc_87(memo_out2,6);
		memo_out2[7]=crc_95(memo_out2,6);
     	uart_out_adr2(memo_out2,8); 		
		}	

	else if((UIB2[1]==0x51)&&(UIB2[4]==0x0d))
		{
		//char temp;
		unsigned int tempUI/*,tempUI_*/;
		//unsigned long tempUL;
		char av_head[4],av_data_on[8],av_data_off[8],av_data[4];		
	
		tempUI=lc640_read_int(PTR_EVENT_LOG);
		tempUI=ptr_carry(tempUI,64,-1*(((signed)UIB2[5])-1));
		tempUI*=32;
		tempUI+=EVENT_LOG;	
		
	
     	memo_out2[0]=0x33;
     	memo_out2[1]=0x61;
     	memo_out2[2]=34;
     	memo_out2[3]=0x03;
     	
     	memo_out2[4]=0x0D;
     	memo_out2[5]=UIB2[5];
     	lc640_read_long_ptr(tempUI,&memo_out2[6]);
     	lc640_read_long_ptr(tempUI+4,&memo_out2[10]);
     	lc640_read_long_ptr(tempUI+8,&memo_out2[14]);
     	lc640_read_long_ptr(tempUI+12,&memo_out2[18]);
     	lc640_read_long_ptr(tempUI+16,&memo_out2[22]);
     	lc640_read_long_ptr(tempUI+20,&memo_out2[26]);
     	lc640_read_long_ptr(tempUI+24,&memo_out2[30]);
     	lc640_read_long_ptr(tempUI+28,&memo_out2[34]);
     	
         	memo_out2[38]=crc_87(memo_out2,38);
		memo_out2[39]=crc_95(memo_out2,38);
     	uart_out_adr2(memo_out2,40); 		
		}	




	else if((UIB2[1]==0x51)&&(UIB2[4]==30)&&(UIB2[5]==30))	//Запрос номера таблицы установок и ее глубины
		{      
     	memo_out2[0]=0x33;
     	memo_out2[1]=0x61;
     	memo_out2[2]=5;
     	memo_out2[3]=0x03;
     	
     	memo_out2[4]=30;
     	memo_out2[5]=NUM_OF_SET_TABL;
     	memo_out2[6]=NUM_OF_SET_TABL;
     	memo_out2[7]=DEEP_OF_SET_TABL;
     	memo_out2[8]=DEEP_OF_SET_TABL;     	     	     	
     	
         	memo_out2[9]=crc_87(memo_out2,9);
		memo_out2[10]=crc_95(memo_out2,9);
     	uart_out_adr2(memo_out2,11); 		
		}		


	else if((UIB2[1]==0x51)&&(UIB2[4]==31))	//Запрос параметра из таблицы установок 
		{      
     	memo_out2[0]=0x33;
     	memo_out2[1]=0x61;
     	memo_out2[2]=4;
     	memo_out2[3]=0x03;
     	
     	memo_out2[4]=31;
     	memo_out2[5]=UIB2[5];
     	
     	if(UIB2[5]==20)		*((signed short*)&memo_out2[6])=MAIN_BPS;
     	else if(UIB2[5]==40)	*((signed short*)&memo_out2[6])=ZV_ON;
     	else if(UIB2[5]==42)	*((signed short*)&memo_out2[6])=TBAT;
     	else if(UIB2[5]==50)	*((signed short*)&memo_out2[6])=UBMAX;
     	else if(UIB2[5]==51)	*((signed short*)&memo_out2[6])=UB20;   	     	
     	else if(UIB2[5]==52)	*((signed short*)&memo_out2[6])=USIGN;
    		else if(UIB2[5]==53)	*((signed short*)&memo_out2[6])=UMN;     	
     	else if(UIB2[5]==54)	*((signed short*)&memo_out2[6])=U0B;
     	else if(UIB2[5]==55)	*((signed short*)&memo_out2[6])=IKB;
    		else if(UIB2[5]==56)	*((signed short*)&memo_out2[6])=IZMAX;     	
     	else if(UIB2[5]==57)	*((signed short*)&memo_out2[6])=IMAX;
     	else if(UIB2[5]==58)	*((signed short*)&memo_out2[6])=KIMAX;
     	else if(UIB2[5]==59)	*((signed short*)&memo_out2[6])=KVZ;
    		else if(UIB2[5]==60)	*((signed short*)&memo_out2[6])=TZAS;     	
     	else if(UIB2[5]==61)	*((signed short*)&memo_out2[6])=TMAX;
     	else if(UIB2[5]==71)
     		{
     		if(apv_on1==apvON) *((signed short*)&memo_out2[6])=1;
     		else *((signed short*)&memo_out2[6])=0;
     		}
    		else if(UIB2[5]==72)	
    			{
    			     		{
     		if(apv_on2==apvON) *((signed short*)&memo_out2[6])=1;
     		else *((signed short*)&memo_out2[6])=0;
     		}

    			}   	
     	else if(UIB2[5]==73)	*((signed short*)&memo_out2[6])=apv_on2_time;
     	
         	memo_out2[8]=crc_87(memo_out2,8);
		memo_out2[9]=crc_95(memo_out2,8);
     	uart_out_adr2(memo_out2,10); 		
		}	

	else if((UIB2[1]==0x51)&&(UIB2[4]==33))	//Запрос типа устройства и серийного номера 
		{      
     	memo_out2[0]=0x33;
     	memo_out2[1]=0x61;
     	memo_out2[2]=6;
     	memo_out2[3]=0x03;
     	
     	memo_out2[4]=33;
     	
		if(AUSW_MAIN==6024)		memo_out2[5]=1;
		else if(AUSW_MAIN==6012)	memo_out2[5]=2;
		else if(AUSW_MAIN==4824)	memo_out2[5]=3;
		else if(AUSW_MAIN==4812)	memo_out2[5]=4;
		else if(AUSW_MAIN==6010)	memo_out2[5]=5;
		else if(AUSW_MAIN==6005)	memo_out2[5]=6;
		else if(AUSW_MAIN==4810)	memo_out2[5]=7;
		else if(AUSW_MAIN==4805)	memo_out2[5]=8;
		else if(AUSW_MAIN==2424)	memo_out2[5]=9;
		else if(AUSW_MAIN==2412)	memo_out2[5]=10;
		else if(AUSW_MAIN==4840)	memo_out2[5]=11;
		else if(AUSW_MAIN==6030)	memo_out2[5]=12;
		else if(AUSW_MAIN==4820)	memo_out2[5]=13;
		else if(AUSW_MAIN==6015)	memo_out2[5]=14;
		else if(AUSW_MAIN==2450)	memo_out2[5]=15;
		else if(AUSW_MAIN==2425)	memo_out2[5]=16;	
     	
     	*((long*)&memo_out2[6])=AUSW_MAIN_NUMBER;
     	
     	
         	memo_out2[10]=crc_87(memo_out2,10);
		memo_out2[11]=crc_95(memo_out2,10);
     	uart_out_adr2(memo_out2,12); 		
		}	

	else if((UIB2[1]==0x51)&&(UIB2[4]==34))	//Запрос типа устройства и серийного номера 
		{
		char* ptr;
		char i;
		ptr="Мама мыла раму. У шуры шары";
		      
     	memo_out2[0]=0x34;
     	memo_out2[1]=0x61;
     	memo_out2[2]=29;
     	memo_out2[3]=0x03;
     	
     	memo_out2[4]=34;
     	memo_out2[5]=27;
     	
     	for(i=0;i<27;i++)
     		{
     		memo_out2[6+i]=ptr[i];
     		}
    	
         	memo_out2[33]=crc_87(memo_out2,33);
		memo_out2[34]=crc_95(memo_out2,33);
     	uart_out_adr2(memo_out2,35); 		
		}	


	else if((UIB2[1]==0x52)&&(UIB2[4]==32))	//Изменение параметра из таблицы установок 
		{      
     	memo_out2[0]=0x33;
     	memo_out2[1]=0x62;
     	memo_out2[2]=3;
     	memo_out2[3]=0x03;
     	
     	memo_out2[4]=32;
     	memo_out2[5]=UIB2[5];
     	
     	memo_out2[6]=0xff;
     	
     	if(UIB2[5]==20)
     		{
     		if(UIB2[6]==0x22)
     			{
     			MAIN_BPS=UIB2[7]+(UIB2[8]*256);;
     			}
     		else if( (UIB2[6]==0x33) || (UIB2[6]==0x44) || (UIB2[6]==0x55) || (UIB2[6]==0x66) )
     			{
     			if(MAIN_BPS) MAIN_BPS=0;
     			else MAIN_BPS=1;
     			}
     		
     		gran(&MAIN_BPS,0,1);
     		lc640_write_int(EE_MAIN_BPS,MAIN_BPS);
     		}

     	else if(UIB2[5]==40)
     		{
     		if(UIB2[6]==0x22)
     			{
     			ZV_ON=UIB2[7]+(UIB2[8]*256);;
     			}
     		else if( (UIB2[6]==0x33) || (UIB2[6]==0x44) || (UIB2[6]==0x55) || (UIB2[6]==0x66) )
     			{
     			if(ZV_ON) ZV_ON=0;
     			else ZV_ON=1;
     			}
    			gran(&ZV_ON,0,1);
    			lc640_write_int(EE_ZV_ON,ZV_ON);
     		}	

     	else if(UIB2[5]==42)
     		{     			
     		signed short ttt;
     		ttt=UIB2[7]+(UIB2[8]*256);;
     		
     		if(UIB2[6]==0x22)
     			{
     			if((ttt>=5)&&(ttt<=60))
     				{
     				TBAT=ttt;
     				}
     			else memo_out2[6]=0x01;	
     			}
     		
     		else if(UIB2[6]==0x33)TBAT++;
      		else if(UIB2[6]==0x44)TBAT+=ttt;
     		else if(UIB2[6]==0x55)TBAT--;
      		else if(UIB2[6]==0x66)TBAT-=ttt;
     			
			gran(&TBAT,5,60);
     		lc640_write_int(EE_TBAT,TBAT);
     		}	
     	else if(UIB2[5]==50)
     		{     			
     		signed short ttt;
     		ttt=UIB2[7]+(UIB2[8]*256);//*((unsigned short*)&UIB2[7]);
     		
     		if(UIB2[6]==0x22)
     			{
     			if((ttt>=10)&&(ttt<=1000))
     				{
     				UBMAX=ttt;
     				}
     			else memo_out2[6]=0x1;	
     			}
     		
     		else if(UIB2[6]==0x33)UBMAX++;
      		else if(UIB2[6]==0x44)UBMAX+=ttt;
     		else if(UIB2[6]==0x55)UBMAX--;
      		else if(UIB2[6]==0x66)UBMAX-=ttt;
      		
      		gran(&UBMAX,10,1000);	  			
     		lc640_write_int(EE_UBMAX,UBMAX);
     		}
     			
     	else if(UIB2[5]==51)     		
     		{     			
     		signed short ttt;
     		ttt=UIB2[7]+(UIB2[8]*256);//*((unsigned short*)&UIB2[7]);
     		//ttt=(unsigned short)(*(&UIB2[7]));
     		
     		if(UIB2[6]==0x22)
     			{
     			if((ttt>=10)&&(ttt<=1000))
     				{
     				UB20=ttt;
      				}
     			else memo_out2[6]=0x1;	
     			}
     		
     		else if(UIB2[6]==0x33)UB20++;
      		else if(UIB2[6]==0x44)UB20+=ttt;
     		else if(UIB2[6]==0x55)UB20--;
      		else if(UIB2[6]==0x66)UB20-=ttt;

     		gran(&UB20,10,1000);      			  			
     		lc640_write_int(EE_UB20,UB20);
     		}	
     		
     	else if(UIB2[5]==52)     		
     		{     			
     		signed short ttt;
     		ttt=UIB2[7]+(UIB2[8]*256);;
     		
     		if(UIB2[6]==0x22)
     			{
     			if((ttt>=10)&&(ttt<=1000))
     				{
     				USIGN=ttt;
     				}
     			else memo_out2[6]=0x01;	
     			}
     		
     		else if(UIB2[6]==0x33)USIGN++;
      		else if(UIB2[6]==0x44)USIGN+=ttt;
     		else if(UIB2[6]==0x55)USIGN--;
      		else if(UIB2[6]==0x66)USIGN-=ttt;

     		gran(&USIGN,10,1000);     			  			
     		lc640_write_int(EE_USIGN,USIGN);
     		}
     		
     	else if(UIB2[5]==53)
     		{     			
     		signed short ttt;
     		ttt=UIB2[7]+(UIB2[8]*256);;
     		
     		if(UIB2[6]==0x22)
     			{
     			if((ttt>=10)&&(ttt<=1000))
     				{
     				UMN=ttt;
     				}
     			else memo_out2[6]=0x01;	
     			}
     		
     		else if(UIB2[6]==0x33)UMN++;
      		else if(UIB2[6]==0x44)UMN+=ttt;
     		else if(UIB2[6]==0x55)UMN--;
      		else if(UIB2[6]==0x66)UMN-=ttt;
      		
     		gran(&UMN,10,1000);	      			  			
     		lc640_write_int(EE_UMN,UMN);
     		}     		     		

     	else if(UIB2[5]==54)
     		{     			
     		signed short ttt;
     		ttt=UIB2[7]+(UIB2[8]*256);;
     		
     		if(UIB2[6]==0x22)
     			{
     			if((ttt>=10)&&(ttt<=1000))
     				{
     				U0B=ttt;
     				}
     			else memo_out2[6]=0x01;	
     			}
     		
     		else if(UIB2[6]==0x33)U0B++;
      		else if(UIB2[6]==0x44)U0B+=ttt;
     		else if(UIB2[6]==0x55)U0B--;
      		else if(UIB2[6]==0x66)U0B-=ttt;
      		
     		gran(&U0B,10,1000);	      			  			
     		lc640_write_int(EE_U0B,U0B);
     		}     		
     		
     	else if(UIB2[5]==55)
     		{     			
     		signed short ttt;
     		ttt=UIB2[7]+(UIB2[8]*256);;
     		
     		if(UIB2[6]==0x22)
     			{
     			if((ttt>=1)&&(ttt<=500))
     				{
     				IKB=ttt;
     				}
     			else memo_out2[6]=0x01;	
     			}
     		
     		else if(UIB2[6]==0x33)IKB++;
      		else if(UIB2[6]==0x44)IKB+=ttt;
     		else if(UIB2[6]==0x55)IKB--;
      		else if(UIB2[6]==0x66)IKB-=ttt;
      		
     		gran(&IKB,10,1000);	      			  			
     		lc640_write_int(EE_IKB,IKB);
     		}     		

     	else if(UIB2[5]==56)
     		{     			
     		signed short ttt;
     		ttt=UIB2[7]+(UIB2[8]*256);;
     		
     		if(UIB2[6]==0x22)
     			{
     			if((ttt>=10)&&(ttt<=200))
     				{
     				IZMAX=ttt;
     				}
     			else memo_out2[6]=0x01;	
     			}
     		
     		else if(UIB2[6]==0x33)IZMAX++;
      		else if(UIB2[6]==0x44)IZMAX+=ttt;
     		else if(UIB2[6]==0x55)IZMAX--;
      		else if(UIB2[6]==0x66)IZMAX-=ttt;
      		
     		gran(&IZMAX,10,200);	      			  			
     		lc640_write_int(EE_IZMAX,IZMAX);
     		}     	     
 
      	else if(UIB2[5]==57)
     		{     			
     		signed short ttt;
     		ttt=UIB2[7]+(UIB2[8]*256);;
     		
     		if(UIB2[6]==0x22)
     			{
     			if((ttt>=1)&&(ttt<=500))
     				{
     				IMAX=ttt;
     				}
     			else memo_out2[6]=0x01;	
     			}
     		
     		else if(UIB2[6]==0x33)IMAX++;
      		else if(UIB2[6]==0x44)IMAX+=ttt;
     		else if(UIB2[6]==0x55)IMAX--;
      		else if(UIB2[6]==0x66)IMAX-=ttt;
      		
     		gran(&IMAX,1,500);	      			  			
     		lc640_write_int(EE_IMAX,IMAX);
     		}     	        		
     		
      	else if(UIB2[5]==58)
     		{     			
     		signed short ttt;
     		ttt=UIB2[7]+(UIB2[8]*256);;
     		
     		if(UIB2[6]==0x22)
     			{
     			if((ttt>=5)&&(ttt<=10))
     				{
     				KIMAX=ttt;
     				}
     			else memo_out2[6]=0x01;	
     			}
     		
     		else if(UIB2[6]==0x33)KIMAX++;
      		else if(UIB2[6]==0x44)KIMAX+=ttt;
     		else if(UIB2[6]==0x55)KIMAX--;
      		else if(UIB2[6]==0x66)KIMAX-=ttt;
      		
     		gran(&KIMAX,5,10);	      			  			
     		lc640_write_int(EE_KIMAX,KIMAX);
     		} 
     		 
     	else if(UIB2[5]==59)
     		{     			
     		signed short ttt;
     		ttt=UIB2[7]+(UIB2[8]*256);;
     		
     		if(UIB2[6]==0x22)
     			{
     			if((ttt>=1005)&&(ttt<=1030))
     				{
     				KVZ=ttt;
     				}
     			else memo_out2[6]=0x01;	
     			}
     		
     		else if(UIB2[6]==0x33)KVZ++;
      		else if(UIB2[6]==0x44)KVZ+=ttt;
     		else if(UIB2[6]==0x55)KVZ--;
      		else if(UIB2[6]==0x66)KVZ-=ttt;
      		
     		gran(&KVZ,1005,1030);	      			  			
     		lc640_write_int(EE_KVZ,KVZ);
     		}      
     		       		       		
      	else if(UIB2[5]==60)
     		{     			
     		signed short ttt;
     		ttt=UIB2[7]+(UIB2[8]*256);;
     		
     		if(UIB2[6]==0x22)
     			{
     			if((ttt>=0)&&(ttt<=3))
     				{
     				TZAS=ttt;
     				}
     			else memo_out2[6]=0x01;	
     			}
     		
     		else if(UIB2[6]==0x33)TZAS++;
      		else if(UIB2[6]==0x44)TZAS+=ttt;
     		else if(UIB2[6]==0x55)TZAS--;
      		else if(UIB2[6]==0x66)TZAS-=ttt;
      		
     		gran(&TZAS,0,3);	      			  			
     		lc640_write_int(EE_TZAS,TZAS);
     		} 
     		        
      	else if(UIB2[5]==61)
     		{     			
     		signed short ttt;
     		ttt=UIB2[7]+(UIB2[8]*256);;
     		
     		if(UIB2[6]==0x22)
     			{
     			if((ttt>=10)&&(ttt<=100))
     				{
     				TMAX=ttt;
     				}
     			else memo_out2[6]=0x01;	
     			}
     		
     		else if(UIB2[6]==0x33)TMAX++;
      		else if(UIB2[6]==0x44)TMAX+=ttt;
     		else if(UIB2[6]==0x55)TMAX--;
      		else if(UIB2[6]==0x66)TMAX-=ttt;
      		
     		gran(&TMAX,10,100);	      			  			
     		lc640_write_int(EE_TMAX,TMAX);
     		}         
     				     		     		
      	else if(UIB2[5]==71)
     		{
     		if(UIB2[6]==0x22)
     			{
     			apv_on1=UIB2[7]+(UIB2[8]*256);;
     			}
     		else if( (UIB2[6]==0x33) || (UIB2[6]==0x44) || (UIB2[6]==0x55) || (UIB2[6]==0x66) )
     			{
     			if(apv_on1) apv_on1=0;
     			else apv_on1=1;
     			}
    			gran_char(&apv_on1,0,1);
    			lc640_write_int(EE_APV_ON1,apv_on1);
     		}	

     	else if(UIB2[5]==72)
     		{
     		if(UIB2[6]==0x22)
     			{
     			apv_on2=UIB2[7]+(UIB2[8]*256);;
     			}
     		else if( (UIB2[6]==0x33) || (UIB2[6]==0x44) || (UIB2[6]==0x55) || (UIB2[6]==0x66) )
     			{
     			if(apv_on2) apv_on2=0;
     			else apv_on2=1;
     			}
    			gran_char(&apv_on1,0,1);
    			lc640_write_int(EE_APV_ON1,apv_on1);
     		}	

     	else if(UIB2[5]==73)
     		{     			
     		signed short ttt;
     		ttt=UIB2[7]+(UIB2[8]*256);;
     		
     		if(UIB2[6]==0x22)
     			{
     			if((ttt>=1)&&(ttt<=24))
     				{
     				apv_on2_time=ttt;
     				}
     			else memo_out2[6]=0x01;	
     			}
     		
     		else if(UIB2[6]==0x33)apv_on2_time++;
      		else if(UIB2[6]==0x44)apv_on2_time+=ttt;
     		else if(UIB2[6]==0x55)apv_on2_time--;
      		else if(UIB2[6]==0x66)apv_on2_time-=ttt;
      		
     		gran(&apv_on2_time,1,24);	      			  			
     		lc640_write_int(EE_APV_ON2_TIME,apv_on2_time);
     		} 
 
     				     		     		
         	memo_out2[7]=crc_87(memo_out2,7);
		memo_out2[8]=crc_95(memo_out2,7);
     	uart_out_adr2(memo_out2,9); 		
		}		
	} 


}



//-----------------------------------------------
signed short index_offset2 (signed short index,signed short offset)
{
index=index+offset;
if(index>=RX_BUFFER_SIZE2) index-=RX_BUFFER_SIZE2; 
if(index<0) index+=RX_BUFFER_SIZE2;
return index;
}

//-----------------------------------------------
char control_check2(signed short index)
{
char i=0,ii=0,iii;

if(rx_buffer2[index]!=END) goto error_cc;

ii=rx_buffer2[index_offset2(index,-2)];
iii=0;
for(i=0;i<=ii;i++)
	{
	iii^=rx_buffer2[index_offset2(index,-2-ii+i)];
	}
if (iii!=rx_buffer2[index_offset2(index,-1)]) goto error_cc;	


success_cc:
return 1;
error_cc:
return 0;

}

//-----------------------------------------------
void uart_in2(void)
{
char temp,i;



if(rx_buffer_overflow2)
	{
	rx_wr_index2=0;
	rx_rd_index2=0;
	rx_counter2=0;
	rx_buffer_overflow2=0;
	}    
	
if(rx_counter2&&(rx_buffer2[index_offset2(rx_wr_index2,-1)])==END)
	{
	
     //plazma_uart2++;

     temp=rx_buffer2[index_offset2(rx_wr_index2,-3)];
    	if(temp<100) 
    		{
    		if(control_check2(index_offset2(rx_wr_index2,-1)))
    			{
    		
    			rx_rd_index2=index_offset2(rx_wr_index2,-3-temp);
    			for(i=0;i<temp;i++)
				{
				UIB2[i]=rx_buffer2[index_offset2(rx_rd_index2,i)];
				} 
			rx_rd_index2=rx_wr_index2;
			rx_counter2=0;
			
			uart_in_an2();
    			}
 	
    		} 
    	}	


    
}  