#include "full_can.h"
#include "LPC17XX.H" 
#include "main.h"
#include "uart0.h"
#include "eeprom_map.h"
#include "control.h"
#include "25lc640.h"
//#include <string.h>

short volatile gCANFilter = 0;
char ptr_can1_tx_wr,ptr_can1_tx_rd;
long can1_info[8];
long can1_id[8];
long can1_data[8];
long can1_datb[8];
//unsigned short rotor_can[6];

// FullCAN Message List
FULLCAN_MSG volatile gFullCANList[MAX_FILTERS];

char bR;
char RXBUFF[40],TXBUFF[40];
char bIN;
char bd_dumm[25];
char bd[25];
char TX_len;
char bOUT;
char can_tx_cnt;
char bOUT_FREE=1;
char rotor_rotor_rotor[2];


volatile uint32_t CANStatus;

//-----------------------------------------------
char CRC1_in(void)
{
char r,j,lb;
lb=(RXBUFF[1]&0x1f)+0x04;
r=RXBUFF[0];
for(j=1;j<(lb+1);j++)
	{
	r=(RXBUFF[j]^Table87[r]);
	}
if(r==0)r=0xFF;
return r;	
} 

//-----------------------------------------------
char CRC2_in(void)
{
char r,j,lb;
lb=(RXBUFF[1]&0x1f)+0x04;
r=RXBUFF[0];
for(j=1;j<(lb+1);j++)
	{
	r=(RXBUFF[j]^Table95[r]);
	}
if(r==0)r=0xFF;
return r;	
}  

//-----------------------------------------------
char CRC1_out(void)
{
char r,j,lb;
lb=(TXBUFF[1]&0x1f)+0x04;
r=TXBUFF[0];
for(j=1;j<(lb+1);j++)
	{
	r=(TXBUFF[j]^Table87[r]);
	}
if(r==0)r=0xFF;
return r;	
} 

//-----------------------------------------------
char CRC2_out(void)
{
char r,j,lb;
lb=(TXBUFF[1]&0x1f)+0x04;
r=TXBUFF[0];
for(j=1;j<(lb+1);j++)
	{
	r=(TXBUFF[j]^Table95[r]);
	}
if(r==0)r=0xFF;
return r;	
}

//-----------------------------------------------
void can1_out_adr(char* ptr,char num)
{
plazma_can4++;
if(num<=8)
	{
	can1_info[ptr_can1_tx_wr]=(((long)num)<<16)&0x000f0000UL;
	can1_id[ptr_can1_tx_wr]=0x0000009eUL;
	*((char*)&can1_data[ptr_can1_tx_wr])=ptr[0];
	*(((char*)&can1_data[ptr_can1_tx_wr])+1)=ptr[1];
	*(((char*)&can1_data[ptr_can1_tx_wr])+2)=ptr[2];
	*(((char*)&can1_data[ptr_can1_tx_wr])+3)=ptr[3];
	*((char*)&can1_datb[ptr_can1_tx_wr])=ptr[4];
	*(((char*)&can1_datb[ptr_can1_tx_wr])+1)=ptr[5];
	*(((char*)&can1_datb[ptr_can1_tx_wr])+2)=ptr[6];
	*(((char*)&can1_datb[ptr_can1_tx_wr])+3)=ptr[7];	
	ptr_can1_tx_wr++;
	if(ptr_can1_tx_wr>=8)ptr_can1_tx_wr=0;
	}
	
else if(num<=16)
	{
	can1_info[ptr_can1_tx_wr]=(8UL<<16)&0x000f0000UL;
	can1_id[ptr_can1_tx_wr]=0x0000009eUL;
	*((char*)&can1_data[ptr_can1_tx_wr])=ptr[0];
	*(((char*)&can1_data[ptr_can1_tx_wr])+1)=ptr[1];
	*(((char*)&can1_data[ptr_can1_tx_wr])+2)=ptr[2];
	*(((char*)&can1_data[ptr_can1_tx_wr])+3)=ptr[3];
	*((char*)&can1_datb[ptr_can1_tx_wr])=ptr[4];
	*(((char*)&can1_datb[ptr_can1_tx_wr])+1)=ptr[5];
	*(((char*)&can1_datb[ptr_can1_tx_wr])+2)=ptr[6];
	*(((char*)&can1_datb[ptr_can1_tx_wr])+3)=ptr[7];	
	ptr_can1_tx_wr++;
	if(ptr_can1_tx_wr>=8)ptr_can1_tx_wr=0;
	
	can1_info[ptr_can1_tx_wr]=(((long)(num-8))<<16)&0x000f0000UL;
	can1_id[ptr_can1_tx_wr]=0x0000009eUL;
	*((char*)&can1_data[ptr_can1_tx_wr])=ptr[8];
	*(((char*)&can1_data[ptr_can1_tx_wr])+1)=ptr[9];
	*(((char*)&can1_data[ptr_can1_tx_wr])+2)=ptr[10];
	*(((char*)&can1_data[ptr_can1_tx_wr])+3)=ptr[11];
	*((char*)&can1_datb[ptr_can1_tx_wr])=ptr[12];
	*(((char*)&can1_datb[ptr_can1_tx_wr])+1)=ptr[13];
	*(((char*)&can1_datb[ptr_can1_tx_wr])+2)=ptr[14];
	*(((char*)&can1_datb[ptr_can1_tx_wr])+3)=ptr[15];	
	ptr_can1_tx_wr++;
	if(ptr_can1_tx_wr>=8)ptr_can1_tx_wr=0;	
	}	

else if(num<=24)
	{
	can1_info[ptr_can1_tx_wr]=(8UL<<16)&0x000f0000UL;
	can1_id[ptr_can1_tx_wr]=0x0000009eUL;
	*((char*)&can1_data[ptr_can1_tx_wr])=ptr[0];
	*(((char*)&can1_data[ptr_can1_tx_wr])+1)=ptr[1];
	*(((char*)&can1_data[ptr_can1_tx_wr])+2)=ptr[2];
	*(((char*)&can1_data[ptr_can1_tx_wr])+3)=ptr[3];
	*((char*)&can1_datb[ptr_can1_tx_wr])=ptr[4];
	*(((char*)&can1_datb[ptr_can1_tx_wr])+1)=ptr[5];
	*(((char*)&can1_datb[ptr_can1_tx_wr])+2)=ptr[6];
	*(((char*)&can1_datb[ptr_can1_tx_wr])+3)=ptr[7];	
	ptr_can1_tx_wr++;
	if(ptr_can1_tx_wr>=8)ptr_can1_tx_wr=0;

	can1_info[ptr_can1_tx_wr]=(8UL<<16)&0x000f0000UL;
	can1_id[ptr_can1_tx_wr]=0x0000009eUL;
	*((char*)&can1_data[ptr_can1_tx_wr])=ptr[8];
	*(((char*)&can1_data[ptr_can1_tx_wr])+1)=ptr[9];
	*(((char*)&can1_data[ptr_can1_tx_wr])+2)=ptr[10];
	*(((char*)&can1_data[ptr_can1_tx_wr])+3)=ptr[11];
	*((char*)&can1_datb[ptr_can1_tx_wr])=ptr[12];
	*(((char*)&can1_datb[ptr_can1_tx_wr])+1)=ptr[13];
	*(((char*)&can1_datb[ptr_can1_tx_wr])+2)=ptr[14];
	*(((char*)&can1_datb[ptr_can1_tx_wr])+3)=ptr[15];	
	ptr_can1_tx_wr++;
	if(ptr_can1_tx_wr>=8)ptr_can1_tx_wr=0;
	
	can1_info[ptr_can1_tx_wr]=(((long)(num-16))<<16)&0x000f0000UL;
	can1_id[ptr_can1_tx_wr]=0x0000009eUL;
	*((char*)&can1_data[ptr_can1_tx_wr])=ptr[16];
	*(((char*)&can1_data[ptr_can1_tx_wr])+1)=ptr[17];
	*(((char*)&can1_data[ptr_can1_tx_wr])+2)=ptr[16];
	*(((char*)&can1_data[ptr_can1_tx_wr])+3)=ptr[19];
	*((char*)&can1_datb[ptr_can1_tx_wr])=ptr[20];
	*(((char*)&can1_datb[ptr_can1_tx_wr])+1)=ptr[21];
	*(((char*)&can1_datb[ptr_can1_tx_wr])+2)=ptr[22];
	*(((char*)&can1_datb[ptr_can1_tx_wr])+3)=ptr[23];	
	ptr_can1_tx_wr++;
	if(ptr_can1_tx_wr>=8)ptr_can1_tx_wr=0;	
	}	

else if(num<=32)
	{
	can1_info[ptr_can1_tx_wr]=(8UL<<16)&0x000f0000UL;
	can1_id[ptr_can1_tx_wr]=0x0000009eUL;
	*((char*)&can1_data[ptr_can1_tx_wr])=ptr[0];
	*(((char*)&can1_data[ptr_can1_tx_wr])+1)=ptr[1];
	*(((char*)&can1_data[ptr_can1_tx_wr])+2)=ptr[2];
	*(((char*)&can1_data[ptr_can1_tx_wr])+3)=ptr[3];
	*((char*)&can1_datb[ptr_can1_tx_wr])=ptr[4];
	*(((char*)&can1_datb[ptr_can1_tx_wr])+1)=ptr[5];
	*(((char*)&can1_datb[ptr_can1_tx_wr])+2)=ptr[6];
	*(((char*)&can1_datb[ptr_can1_tx_wr])+3)=ptr[7];	
	ptr_can1_tx_wr++;
	if(ptr_can1_tx_wr>=8)ptr_can1_tx_wr=0;

	can1_info[ptr_can1_tx_wr]=(8UL<<16)&0x000f0000UL;
	can1_id[ptr_can1_tx_wr]=0x0000009eUL;
	*((char*)&can1_data[ptr_can1_tx_wr])=ptr[8];
	*(((char*)&can1_data[ptr_can1_tx_wr])+1)=ptr[9];
	*(((char*)&can1_data[ptr_can1_tx_wr])+2)=ptr[10];
	*(((char*)&can1_data[ptr_can1_tx_wr])+3)=ptr[11];
	*((char*)&can1_datb[ptr_can1_tx_wr])=ptr[12];
	*(((char*)&can1_datb[ptr_can1_tx_wr])+1)=ptr[13];
	*(((char*)&can1_datb[ptr_can1_tx_wr])+2)=ptr[14];
	*(((char*)&can1_datb[ptr_can1_tx_wr])+3)=ptr[15];	
	ptr_can1_tx_wr++;
	if(ptr_can1_tx_wr>=8)ptr_can1_tx_wr=0;
	
	can1_info[ptr_can1_tx_wr]=(8UL<<16)&0x000f0000UL;
	can1_id[ptr_can1_tx_wr]=0x0000009eUL;
	*((char*)&can1_data[ptr_can1_tx_wr])=ptr[16];
	*(((char*)&can1_data[ptr_can1_tx_wr])+1)=ptr[17];
	*(((char*)&can1_data[ptr_can1_tx_wr])+2)=ptr[18];
	*(((char*)&can1_data[ptr_can1_tx_wr])+3)=ptr[19];
	*((char*)&can1_datb[ptr_can1_tx_wr])=ptr[20];
	*(((char*)&can1_datb[ptr_can1_tx_wr])+1)=ptr[21];
	*(((char*)&can1_datb[ptr_can1_tx_wr])+2)=ptr[22];
	*(((char*)&can1_datb[ptr_can1_tx_wr])+3)=ptr[23];	
	ptr_can1_tx_wr++;
	if(ptr_can1_tx_wr>=8)ptr_can1_tx_wr=0;
	
	can1_info[ptr_can1_tx_wr]=(((long)(num-24))<<16)&0x000f0000UL;
	can1_id[ptr_can1_tx_wr]=0x0000009eUL;
	*((char*)&can1_data[ptr_can1_tx_wr])=ptr[24];
	*(((char*)&can1_data[ptr_can1_tx_wr])+1)=ptr[25];
	*(((char*)&can1_data[ptr_can1_tx_wr])+2)=ptr[26];
	*(((char*)&can1_data[ptr_can1_tx_wr])+3)=ptr[27];
	*((char*)&can1_datb[ptr_can1_tx_wr])=ptr[28];
	*(((char*)&can1_datb[ptr_can1_tx_wr])+1)=ptr[29];
	*(((char*)&can1_datb[ptr_can1_tx_wr])+2)=ptr[30];
	*(((char*)&can1_datb[ptr_can1_tx_wr])+3)=ptr[31];	
	ptr_can1_tx_wr++;
	if(ptr_can1_tx_wr>=8)ptr_can1_tx_wr=0;	
	}	


if(bOUT_FREE)
	{
	LPC_CAN1->TFI1=can1_info[ptr_can1_tx_rd];
     LPC_CAN1->TID1=can1_id[ptr_can1_tx_rd];
     LPC_CAN1->TDA1=can1_data[ptr_can1_tx_rd];
     LPC_CAN1->TDB1=can1_datb[ptr_can1_tx_rd];
     LPC_CAN1->CMR=0x00000021;
     ptr_can1_tx_rd++;
     if(ptr_can1_tx_rd>=8)ptr_can1_tx_rd=0;
     bOUT_FREE=0;	
	}
}	

//-----------------------------------------------
void can_adr_hndl(void)
{
	TXBUFF[2]=RXBUFF[3];
	TXBUFF[3]=RXBUFF[2];
	TXBUFF[4]=((RXBUFF[4]&0xF0)>>4)|((RXBUFF[4]&0x0f)<<4);
	TXBUFF[5]=((RXBUFF[5]&0xF0)>>4)|((RXBUFF[5]&0x0f)<<4);	
}	

//-----------------------------------------------
void can_in_an(void)
{
if(!bIN) goto CAN_IN_AN_end; 
plazma_can3++;

#ifdef OLD_CAN_PROTOKOL

// Телеметрия
if((RXBUFF[0]==0x30)&&((RXBUFF[1]&0xc0)==0x40)&&
	((RXBUFF[4]&0xf0)==0xe0)&&((RXBUFF[5]&0xf0)==0x20)&&(RXBUFF[6]==0x01))
	{ 
rotor_can[4]++;
    	TXBUFF[0]=0x30;
	TXBUFF[1]=(RXBUFF[1]&0xa0)+21;
	can_adr_hndl();
	TXBUFF[6]=0x01;
	TXBUFF[7]=bd[0];
	TXBUFF[8]=bd[1];
	TXBUFF[9]=bd[2];
	TXBUFF[10]=bd[3];
	TXBUFF[11]=bd[4];
	TXBUFF[12]=bd[5];
	TXBUFF[13]=bd[6];
	TXBUFF[14]=bd[7];
	TXBUFF[15]=bd[8];													
	TXBUFF[16]=bd[9];
	TXBUFF[17]=bd[10];
	TXBUFF[18]=bd[11];
	//rotor_rotor_rotor[0]=bd[11];
	//rotor_rotor_rotor[1]++;
	TXBUFF[19]=bd[12];
	TXBUFF[20]=bd[13];
	TXBUFF[21]=bd[14];
	TXBUFF[22]=bd[15];
	TXBUFF[23]=bd[16]; 
 	TXBUFF[24]=bd[17];
 	TXBUFF[25]=bd[18];	
	TXBUFF[26]=CRC1_out();
	TXBUFF[27]=CRC2_out();
	//TX_len=28;
/*
  	TXBUFF[0]=0x30;
	TXBUFF[1]=0x35;
	TXBUFF[2]=0x01;
	TXBUFF[3]=0x01;
	TXBUFF[4]=0x1E;
	TXBUFF[5]=0x02;
	TXBUFF[6]=0x01;
	TXBUFF[7]=0x81;
	TXBUFF[8]=0x80;
	TXBUFF[9]=0x82;
	TXBUFF[10]=0x9A;
	TXBUFF[11]=0xAc;
	TXBUFF[12]=0xc4;
	TXBUFF[13]=0x98;
	TXBUFF[14]=0x80;
	TXBUFF[15]=0xE4;													
	TXBUFF[16]=0x81;
	TXBUFF[17]=0x80;
	TXBUFF[18]=0x01;
	TXBUFF[19]=0x98;
	TXBUFF[20]=0x98;
	TXBUFF[21]=0x9B;
	TXBUFF[22]=0xE8;
	TXBUFF[23]=0x65; 
 	TXBUFF[24]=0x15;
 	TXBUFF[25]=0x5D;	
	TXBUFF[26]=0xB3;
	TXBUFF[27]=0x6D;*/

	can1_out_adr(TXBUFF,28); 
	
	/*can1_out_adr(TXBUFF,8);
	can1_out_adr(&TXBUFF[8],8); 
	can1_out_adr(&TXBUFF[16],8); 
	can1_out_adr(&TXBUFF[24],4); */ 
	}














     
// БПС1 - основной
else if((RXBUFF[0]==0x30)&&((RXBUFF[1]&0xe0)==0x60)&&
	((RXBUFF[4]&0xf0)==0xe0)&&((RXBUFF[5]&0xf0)==0x20)&&(RXBUFF[6]==0x02)
	&&(RXBUFF[7]==0x02)&&(RXBUFF[8]==0x01)&&(RXBUFF[9]==0x01))
	 
	{

     TXBUFF[0]=0x30;
	TXBUFF[1]=0x20+5;
	can_adr_hndl();
	TXBUFF[6]=0x02;
	TXBUFF[7]=0x02;
	TXBUFF[8]=0x01;
	TXBUFF[9]=0x01;
	TXBUFF[10]=CRC1_out();
	TXBUFF[11]=CRC2_out();
	TX_len=12;
	
	can1_out_adr(TXBUFF,12);
     
	lc640_write_int(EE_MAIN_IST,0);
	cnt_src[1]=10;
	St_[0]&=0x63;
	St_[1]&=0x63;
     }

// БПС2 - основной
else if((RXBUFF[0]==0x30)&&((RXBUFF[1]&0xe0)==0x60)&&
	((RXBUFF[4]&0xf0)==0xe0)&&((RXBUFF[5]&0xf0)==0x20)&&(RXBUFF[6]==0x02)
	&&(RXBUFF[7]==0x02)&&(RXBUFF[8]==0x02)&&(RXBUFF[9]==0x02))
	  
	{
     TXBUFF[0]=0x30;
	TXBUFF[1]=0x20+5;
	can_adr_hndl();
	TXBUFF[6]=0x02;
	TXBUFF[7]=0x02;
	TXBUFF[8]=0x02;
	TXBUFF[9]=0x02;
	TXBUFF[10]=CRC1_out();
	TXBUFF[11]=CRC2_out();
	
	can1_out_adr(TXBUFF,12);

    
	lc640_write_int(EE_MAIN_IST,1);
	cnt_src[0]=10;
	St_[0]&=0x63;
	St_[1]&=0x63;
     	
	}	

// Выравнивающий заряд  часа
else if((RXBUFF[0]==0x30)&&((RXBUFF[1]&0xe0)==0x60)&&
	((RXBUFF[4]&0xf0)==0xe0)&&((RXBUFF[5]&0xf0)==0x20)&&(RXBUFF[6]==0x03)
	&&(RXBUFF[7]==0x03)&&((RXBUFF[8])&&(RXBUFF[8]<25)))
 	  
	{
	if(!(St&0x03)&&(NUMBAT))
		{
		spc_stat=spc_VZ;
		cnt_vz_sec=0L;
		cnt_vz_sec_=SEC_IN_HOUR*RXBUFF[8];	
		
	/*	spc_stat=spc_VZ;
		cnt_vz_sec_=3600UL*RXBUFF[8];   */
		TXBUFF[7]=0xff;
		}
	else
		{
		TXBUFF[7]=0x01;		
		}	
     TXBUFF[0]=0x30;
	TXBUFF[1]=0x20+3;
	can_adr_hndl();
	TXBUFF[6]=0x03;
	TXBUFF[8]=CRC1_out();
	TXBUFF[9]=CRC2_out();
	
	can1_out_adr(TXBUFF,10);
	}

// Выравнивающий заряд  выключение
else if((RXBUFF[0]==0x30)&&((RXBUFF[1]&0xe0)==0x60)&&
	((RXBUFF[4]&0xf0)==0xe0)&&((RXBUFF[5]&0xf0)==0x20)&&(RXBUFF[6]==111)
	&&(RXBUFF[7]==111))
 	  
	{
	spc_stat=spc_OFF;
	TXBUFF[7]=0xff;
	
     	
     TXBUFF[0]=0x30;
	TXBUFF[1]=0x20+3;
	can_adr_hndl();
	TXBUFF[6]=0x03;
	TXBUFF[8]=CRC1_out();
	TXBUFF[9]=CRC2_out();
	
	can1_out_adr(TXBUFF,10);
	}

// Контроль ёмкости
else if((RXBUFF[0]==0x30)&&((RXBUFF[1]&0xe0)==0x60)&&
	((RXBUFF[4]&0xf0)==0xe0)&&((RXBUFF[5]&0xf0)==0x20)&&(RXBUFF[6]==0x04)
	&&(RXBUFF[7]==0x04))
	  
	{
	if(!(St&0x02)&&(NUMBAT))
		{
		spc_stat=spc_KE;
		//zar_cnt_ee_ke=0;
		zar_cnt=0L;
		TXBUFF[7]=0xff;
		}
	else
		{
		TXBUFF[7]=0x01;		
		}		
     TXBUFF[0]=0x30;
	TXBUFF[1]=0x20+3;
	can_adr_hndl();
	TXBUFF[6]=0x04;
	TXBUFF[8]=CRC1_out();
	TXBUFF[9]=CRC2_out();
	can1_out_adr(TXBUFF,10);
	}


// БПС1 - выключить
if((RXBUFF[0]==0x30)&&((RXBUFF[1]&0xe0)==0x60)&&
	((RXBUFF[4]&0xf0)==0xe0)&&((RXBUFF[5]&0xf0)==0x20)&&(RXBUFF[6]==0x05)
	&&(RXBUFF[7]==0x05)&&(RXBUFF[8]==0x01)&&(RXBUFF[9]==0x01))
	  
	{
     TXBUFF[0]=0x30;
	TXBUFF[1]=0x20+5;
	can_adr_hndl();
	TXBUFF[6]=0x05;
	TXBUFF[7]=0x05;
	TXBUFF[8]=0x01;
	TXBUFF[9]=0x01;
	TXBUFF[10]=CRC1_out();
	TXBUFF[11]=CRC2_out();
	
	can1_out_adr(TXBUFF,12);
	St_[0]|=0x20;
	St&=0xfb;
	cnt_src[0]=10;
	}

// БПС2 - выключить
else if((RXBUFF[0]==0x30)&&((RXBUFF[1]&0xe0)==0x60)&&
	((RXBUFF[4]&0xf0)==0xe0)&&((RXBUFF[5]&0xf0)==0x20)&&(RXBUFF[6]==0x05)
	&&(RXBUFF[7]==0x05)&&(RXBUFF[8]==0x02)&&(RXBUFF[9]==0x02))
	  
	{
     TXBUFF[0]=0x30;
	TXBUFF[1]=0x20+5;
	can_adr_hndl();
	TXBUFF[6]=0x05;
	TXBUFF[7]=0x05;
	TXBUFF[8]=0x02;
	TXBUFF[9]=0x02;
	TXBUFF[10]=CRC1_out();
	TXBUFF[11]=CRC2_out();
	
	can1_out_adr(TXBUFF,12);
	St_[1]|=0x20;
	St&=0xfb; 
	cnt_src[1]=10;
	}




 
if((RXBUFF[0]==0x30)&&((RXBUFF[1]&0xc0)==0x40)&&
	((RXBUFF[4]&0xf0)==0xe0)&&(((RXBUFF[4]&0x03)==0x01)||((RXBUFF[4]&0x03)==0x02))
	  &&(RXBUFF[5]==0xa1))

	{
     TXBUFF[0]=0x30;
	TXBUFF[1]=(RXBUFF[1]&0xa0)+11;
	can_adr_hndl();
	TXBUFF[6]=0xfd;
	TXBUFF[7]=0xdc;
	TXBUFF[8]=0xdc;
	TXBUFF[9]=0xd0;
	TXBUFF[10]=0x80;
	TXBUFF[11]=0xc5;
	TXBUFF[12]=0x9e;
	TXBUFF[13]=0x80;
	TXBUFF[14]=0x23;													
	TXBUFF[15]=0x14;
	TXBUFF[16]=CRC1_out();
	TXBUFF[17]=CRC2_out();
	TX_len=18;
//	bOUT=1;
	}

#endif



#ifdef NEW_CAN_PROTOKOL


//Версия XML
if((RXBUFF[0]==0x30)&&((RXBUFF[1]&0xc0)==0x40)&&
	((RXBUFF[4]&0xf0)==0xe0)&&((RXBUFF[5]&0xf0)==0x20)&&(RXBUFF[6]==0x01))
	{
     TXBUFF[0]=0x30;
	TXBUFF[1]=(RXBUFF[1]&0xa0)+4;
	can_adr_hndl();
	TXBUFF[6]=0x01;
	TXBUFF[7]=/*0x0a;//*/(char)(CAN_DEVICE_VERSION&0x00ff);
	TXBUFF[8]=/*0x05;//*/(char)((CAN_DEVICE_VERSION&0xff00)>>8);
	TXBUFF[9]=CRC1_out();
	TXBUFF[10]=CRC2_out();
     can1_out_adr(TXBUFF,11);
	
	}

//Тип устройства
else if((RXBUFF[0]==0x30)&&((RXBUFF[1]&0xc0)==0x40)&&
	((RXBUFF[4]&0xf0)==0xe0)&&((RXBUFF[5]&0xf0)==0x20)&&(RXBUFF[6]==0x11))
	{
     TXBUFF[0]=0x30;
	TXBUFF[1]=(RXBUFF[1]&0xa0)+21;
	can_adr_hndl();
	TXBUFF[6]=0x11;

                              strcpy(&TXBUFF[7],"Тип неизвестен");
	if(AUSW_MAIN==6024)		strcpy(&TXBUFF[7],"ИБЭП220/60-24A");
	else if(AUSW_MAIN==6012)	strcpy(&TXBUFF[7],"ИБЭП220/60-12A");
	else if(AUSW_MAIN==4824)	strcpy(&TXBUFF[7],"ИБЭП220/48-24A");
	else if(AUSW_MAIN==4812)	strcpy(&TXBUFF[7],"ИБЭП220/48-12A");
	else if(AUSW_MAIN==6010)	strcpy(&TXBUFF[7],"ИБЭП220/60-10A");
	else if(AUSW_MAIN==6005)	strcpy(&TXBUFF[7],"ИБЭП220/60-5A ");
	else if(AUSW_MAIN==4810)	strcpy(&TXBUFF[7],"ИБЭП220/48-10A");
	else if(AUSW_MAIN==4805)	strcpy(&TXBUFF[7],"ИБЭП220/48-5A ");
	else if(AUSW_MAIN==2424)	strcpy(&TXBUFF[7],"ИБЭП220/24-24A");
	else if(AUSW_MAIN==2412)	strcpy(&TXBUFF[7],"ИБЭП220/24-12A");
	else if(AUSW_MAIN==4840)	strcpy(&TXBUFF[7],"ИБЭП220/48-40A");
	else if(AUSW_MAIN==6040)	strcpy(&TXBUFF[7],"ИБЭП220/60-40A");
	else if(AUSW_MAIN==4820)	strcpy(&TXBUFF[7],"ИБЭП220/48-20A");
	else if(AUSW_MAIN==6020)	strcpy(&TXBUFF[7],"ИБЭП220/60-20A");
	else if(AUSW_MAIN==2450)	strcpy(&TXBUFF[7],"ИБЭП220/24-50A");
	else if(AUSW_MAIN==2425)	strcpy(&TXBUFF[7],"ИБЭП220/24-25A");	
	else if(AUSW_MAIN==2424)	strcpy(&TXBUFF[7],"ИБЭП220/24-24A");
	else if(AUSW_MAIN==2412)	strcpy(&TXBUFF[7],"ИБЭП220/24-12A");

	TXBUFF[21]=' ';
	TXBUFF[22]=' ';
	TXBUFF[23]=' '; 
 	TXBUFF[24]=' ';
 	TXBUFF[25]=' ';	
	TXBUFF[26]=CRC1_out();
	TXBUFF[27]=CRC2_out();

	can1_out_adr(TXBUFF,28);
	
	}

//серийный № устройства
else if((RXBUFF[0]==0x30)&&((RXBUFF[1]&0xc0)==0x40)&&
	((RXBUFF[4]&0xf0)==0xe0)&&((RXBUFF[5]&0xf0)==0x20)&&(RXBUFF[6]==0x12))
	{
     char* str;
     TXBUFF[0]=0x30;
	TXBUFF[1]=(RXBUFF[1]&0xa0)+13;
	can_adr_hndl();
	TXBUFF[6]=0x12;

     //str="0000000"; 

	strcpy(&TXBUFF[7],"******");
	
   if(AUSW_MAIN_NUMBER<1000000)
          {
          str="000001";
          //str[12]=AUSW_MAIN_NUMBER%10;
          //AUSW_MAIN_NUMBER/=10;
         /* str[18]=AUSW_MAIN_NUMBER%10;
          str[18]=AUSW_MAIN_NUMBER/=10;
          str[17]=AUSW_MAIN_NUMBER%10;
          str[17]=AUSW_MAIN_NUMBER/=10;
          str[16]=AUSW_MAIN_NUMBER%10;
          str[16]=AUSW_MAIN_NUMBER/=10;
          str[15]=AUSW_MAIN_NUMBER%10;
          str[15]=AUSW_MAIN_NUMBER/=10;
          str[14]=AUSW_MAIN_NUMBER%10;
          str[14]=AUSW_MAIN_NUMBER/=10;
          str[13]=AUSW_MAIN_NUMBER%10;
          str[13]=AUSW_MAIN_NUMBER/=10;*/ 
          
          } 
     strcpy(&TXBUFF[7],str);
 	TXBUFF[13]=' ';
	TXBUFF[14]=' ';
	TXBUFF[15]=' '; 
 	TXBUFF[16]=' ';
 	TXBUFF[17]=' ';	
	TXBUFF[18]=CRC1_out();
	TXBUFF[19]=CRC2_out();

	can1_out_adr(TXBUFF,20);
	
	}














// Телеметрия














     
// БПС1 - основной
else if((RXBUFF[0]==0x30)&&((RXBUFF[1]&0xe0)==0x60)&&
	((RXBUFF[4]&0xf0)==0xe0)&&((RXBUFF[5]&0xf0)==0x20)&&(RXBUFF[6]==0x02)
	&&(RXBUFF[7]==0x02)&&(RXBUFF[8]==0x01)&&(RXBUFF[9]==0x01))
	 
	{

     TXBUFF[0]=0x30;
	TXBUFF[1]=0x20+5;
	can_adr_hndl();
	TXBUFF[6]=0x02;
	TXBUFF[7]=0x02;
	TXBUFF[8]=0x01;
	TXBUFF[9]=0x01;
	TXBUFF[10]=CRC1_out();
	TXBUFF[11]=CRC2_out();
	TX_len=12;
	
	can1_out_adr(TXBUFF,12);
     
	lc640_write_int(EE_MAIN_IST,0);
	cnt_src[1]=10;
	St_[0]&=0x63;
	St_[1]&=0x63;
     }

// БПС2 - основной
else if((RXBUFF[0]==0x30)&&((RXBUFF[1]&0xe0)==0x60)&&
	((RXBUFF[4]&0xf0)==0xe0)&&((RXBUFF[5]&0xf0)==0x20)&&(RXBUFF[6]==0x02)
	&&(RXBUFF[7]==0x02)&&(RXBUFF[8]==0x02)&&(RXBUFF[9]==0x02))
	  
	{
     TXBUFF[0]=0x30;
	TXBUFF[1]=0x20+5;
	can_adr_hndl();
	TXBUFF[6]=0x02;
	TXBUFF[7]=0x02;
	TXBUFF[8]=0x02;
	TXBUFF[9]=0x02;
	TXBUFF[10]=CRC1_out();
	TXBUFF[11]=CRC2_out();
	
	can1_out_adr(TXBUFF,12);

    
	lc640_write_int(EE_MAIN_IST,1);
	cnt_src[0]=10;
	St_[0]&=0x63;
	St_[1]&=0x63;
     	
	}	

// Выравнивающий заряд  часа
else if((RXBUFF[0]==0x30)&&((RXBUFF[1]&0xe0)==0x60)&&
	((RXBUFF[4]&0xf0)==0xe0)&&((RXBUFF[5]&0xf0)==0x20)&&(RXBUFF[6]==0x03)
	&&(RXBUFF[7]==0x03)&&((RXBUFF[8])&&(RXBUFF[8]<25)))
 	  
	{
	if(!(St&0x03)&&(NUMBAT))
		{
		spc_stat=spc_VZ;
		cnt_vz_sec=0L;
		cnt_vz_sec_=SEC_IN_HOUR*RXBUFF[8];	
		
	/*	spc_stat=spc_VZ;
		cnt_vz_sec_=3600UL*RXBUFF[8];
		TXBUFF[7]=0xff;*/
		}
	else
		{
		TXBUFF[7]=0x01;		
		}	
     TXBUFF[0]=0x30;
	TXBUFF[1]=0x20+3;
	can_adr_hndl();
	TXBUFF[6]=0x03;
	TXBUFF[8]=CRC1_out();
	TXBUFF[9]=CRC2_out();
	
	can1_out_adr(TXBUFF,10);
	}

// Контроль ёмкости
else if((RXBUFF[0]==0x30)&&((RXBUFF[1]&0xe0)==0x60)&&
	((RXBUFF[4]&0xf0)==0xe0)&&((RXBUFF[5]&0xf0)==0x20)&&(RXBUFF[6]==0x04)
	&&(RXBUFF[7]==0x04))
	  
	{
	if(!(St&0x02)&&(NUMBAT))
		{
		spc_stat=spc_KE;
		//zar_cnt_ee_ke=0;
		zar_cnt=0L;
		TXBUFF[7]=0xff;
		}
	else
		{
		TXBUFF[7]=0x01;		
		}		
     TXBUFF[0]=0x30;
	TXBUFF[1]=0x20+3;
	can_adr_hndl();
	TXBUFF[6]=0x04;
	TXBUFF[8]=CRC1_out();
	TXBUFF[9]=CRC2_out();
	can1_out_adr(TXBUFF,10);
	}


// БПС1 - выключить
if((RXBUFF[0]==0x30)&&((RXBUFF[1]&0xe0)==0x60)&&
	((RXBUFF[4]&0xf0)==0xe0)&&((RXBUFF[5]&0xf0)==0x20)&&(RXBUFF[6]==0x05)
	&&(RXBUFF[7]==0x05)&&(RXBUFF[8]==0x01)&&(RXBUFF[9]==0x01))
	  
	{
     TXBUFF[0]=0x30;
	TXBUFF[1]=0x20+5;
	can_adr_hndl();
	TXBUFF[6]=0x05;
	TXBUFF[7]=0x05;
	TXBUFF[8]=0x01;
	TXBUFF[9]=0x01;
	TXBUFF[10]=CRC1_out();
	TXBUFF[11]=CRC2_out();
	
	can1_out_adr(TXBUFF,12);
	St_[0]|=0x20;
	St&=0xfb;
	cnt_src[0]=10;
	}

// БПС2 - выключить
else if((RXBUFF[0]==0x30)&&((RXBUFF[1]&0xe0)==0x60)&&
	((RXBUFF[4]&0xf0)==0xe0)&&((RXBUFF[5]&0xf0)==0x20)&&(RXBUFF[6]==0x05)
	&&(RXBUFF[7]==0x05)&&(RXBUFF[8]==0x02)&&(RXBUFF[9]==0x02))
	  
	{
     TXBUFF[0]=0x30;
	TXBUFF[1]=0x20+5;
	can_adr_hndl();
	TXBUFF[6]=0x05;
	TXBUFF[7]=0x05;
	TXBUFF[8]=0x02;
	TXBUFF[9]=0x02;
	TXBUFF[10]=CRC1_out();
	TXBUFF[11]=CRC2_out();
	
	can1_out_adr(TXBUFF,12);
	St_[1]|=0x20;
	St&=0xfb; 
	cnt_src[1]=10;
	}




 
if((RXBUFF[0]==0x30)&&((RXBUFF[1]&0xc0)==0x40)&&
	((RXBUFF[4]&0xf0)==0xe0)&&(((RXBUFF[4]&0x03)==0x01)||((RXBUFF[4]&0x03)==0x02))
	  &&(RXBUFF[5]==0xa1))

	{
     TXBUFF[0]=0x30;
	TXBUFF[1]=(RXBUFF[1]&0xa0)+11;
	can_adr_hndl();
	TXBUFF[6]=0xfd;
	TXBUFF[7]=0xdc;
	TXBUFF[8]=0xdc;
	TXBUFF[9]=0xd0;
	TXBUFF[10]=0x80;
	TXBUFF[11]=0xc5;
	TXBUFF[12]=0x9e;
	TXBUFF[13]=0x80;
	TXBUFF[14]=0x23;													
	TXBUFF[15]=0x14;
	TXBUFF[16]=CRC1_out();
	TXBUFF[17]=CRC2_out();
	TX_len=18;
//	bOUT=1;
	}

#endif

CAN_IN_AN_end:
bIN=0;
}   
/**************************************************************************
DOES:    Interrupt Service Routine for CAN receive on CAN interface 1
GLOBALS: Copies the received message into the gFullCANList[] array
         Handles semaphore bits as described in LPC user manual
RETURNS: nothing
***************************************************************************/ 
void CAN_ISR_Rx1( void )
{
unsigned int buf;
unsigned int *pDest;
char temp;
char *ptr,j;
//can_cnt++;

rotor_can[0]++;

  if (!(LPC_CAN1->RFS & 0xC0000400L))
  { // 11-bit ID, no RTR, matched a filter

rotor_can[1]++;
    // initialize destination pointer
    // filter number is in lower 10 bits of C1RFS
    pDest = (unsigned int *) &(gFullCANList[(LPC_CAN1->RFS & 0x000003FFL)].Dat1);
    
    // calculate contents for first entry into FullCAN list
    buf = LPC_CAN1->RFS & 0xC00F0000L; // mask FF, RTR and DLC
    buf |= 0x01002000L; // set semaphore to 01b and CAN port to 1
    buf |= LPC_CAN1->RID & 0x000007FFL; // get CAN message ID

    // now copy entire message to FullCAN list
    *pDest = buf; 
    pDest++; // set to gFullCANList[(C1RFS & 0x000003FFL)].DatA
    *pDest = LPC_CAN1->RDA; 
    pDest++; // set to gFullCANList[(C1RFS & 0x000003FFL)].DatB
    *pDest = LPC_CAN1->RDB; 

    // now set the sempahore to complete
    buf |= 0x03000000L; // set semaphore to 11b
    pDest -= 2; // set to gFullCANList[(C1RFS & 0x000003FFL)].Dat1
    *pDest = buf; 
    
	temp=(char)gFullCANList[0].DatA;
	if(temp==0x30) bR=0;
	else bR++;
	
	temp=(char)(((gFullCANList[0].Dat1)>>16)&0x0f); 
     
     ptr=(char*)(&gFullCANList[0].DatA);
	
	if(!bR)
		{
		for(j=0;j<temp;j++)
			{
			RXBUFF[j]=*ptr;
			ptr++;
			}
		}
	else if(bR==1)
		{
		for(j=8;j<(temp+8);j++)
			{
			RXBUFF[j]=*ptr;
			ptr++;
			}                      
			
		} 		
	
	
	
		
	temp=((RXBUFF[1]&0x1f)+4);
rotor_can[2]++;
	if((CRC1_in()==RXBUFF[temp+1])&&(CRC2_in()==RXBUFF[temp+2])&&bR)
		{
  
		bIN=1;
  		rotor_can[3]++;
  		can_in_an();
		}    
    
  }

  LPC_CAN1->CMR = 0x04; // release receive buffer
}

/**************************************************************************
DOES:    Interrupt Service Routine for CAN receive on CAN interface 1
GLOBALS: Copies the received message into the gFullCANList[] array
         Handles semaphore bits as described in LPC user manual
RETURNS: nothing
***************************************************************************/ 
void can_isr_tx1 (void) 
{
//unsigned int buf;
unsigned int *pDest;
char temp;
char *ptr,j;

plazma_can2++;

can_tx_cnt++;

rotor_can[5]++;

if(ptr_can1_tx_wr!=ptr_can1_tx_rd)
	{
	LPC_CAN1->TFI1=can1_info[ptr_can1_tx_rd];
     LPC_CAN1->TID1=can1_id[ptr_can1_tx_rd];
     LPC_CAN1->TDA1=can1_data[ptr_can1_tx_rd];
     LPC_CAN1->TDB1=can1_datb[ptr_can1_tx_rd];
     LPC_CAN1->CMR=0x00000021;
     ptr_can1_tx_rd++;
     if(ptr_can1_tx_rd>=8)ptr_can1_tx_rd=0;
	}
else bOUT_FREE=1;
temp=LPC_CAN1->ICR;

}


/**************************************************************************
Initialization of a CAN interface
as described in LPC_FullCAN_SW.h
***************************************************************************/ 
short can1_init (unsigned int can_btr)
{
unsigned int *pSFR; // pointer into SFR space
unsigned int *pSFR2; // pointer into SFR space
unsigned int offset; // offset added to pSFR
                                               
LPC_SC->PCONP |= (1<<13);  /* Enable CAN1 and CAN2 clock */

LPC_PINCON->PINSEL0 &= ~0x0000000F;  /* CAN1 is p0.0 and p0.1	*/
LPC_PINCON->PINSEL0 |= 0x00000005;

gCANFilter = 0; // Reset and disable all message filters

LPC_CANAF->AFMR = 0x00000001L; // Acceptance Filter Mode Register = off !

LPC_CAN1->MOD = 1; // Go into Reset mode

LPC_CAN1->IER = 0;// Disable All Interrupts

LPC_CAN1->GSR = 0; // Clear Status register

LPC_CAN1->BTR = can_btr; // Set bit timing

//LPC_CAN1->IER |=(1<<0)|(1<<1)|(1<<9)|(1<<10); // Enable Receive & Transmit Interrupt

LPC_CAN1->MOD = 0; // Enter Normal Operating Mode



NVIC_EnableIRQ(CAN_IRQn);
LPC_CAN1->IER =0x03;
return 1;
}

/**************************************************************************
Setting a CAN receive filter
as described in LPC_FullCAN_SW.h
***************************************************************************/ 
short FullCAN_SetFilter  (
                         unsigned short can_port, // CAN interface number
                         unsigned int CANID // 11-bit CAN ID
                         )
{
unsigned int p, n;
unsigned int buf0, buf1;
unsigned int ID_lower, ID_upper;
unsigned int candata;
unsigned int *pAddr;


// Acceptance Filter Mode Register = off !
LPC_CANAF->AFMR = 0x00000001L;

if (gCANFilter == 0)
     {    
     // First call, init entry zero
     gFullCANList[0].Dat1 = 0x000037FFL; // CAN 1, disabled and unused
     }
if (gCANFilter >= MAX_FILTERS)
     {
     return 0;
     }

CANID &= 0x000007FFL; // Mask out 11-bit ID
CANID |= (can_port << 13); // Put can_port info in bits 13-15

// Filters must be sorted by interface, then by priority
// new filter is sorted into array
p = 0;
while (p < gCANFilter) // loop through all existing filters 
     {
     if ((gFullCANList[p].Dat1 & 0x0000FFFFL) > CANID)
          {
          break;
          }
     p++;
     }

// insert new filter here
buf0 = gFullCANList[p].Dat1; // save current entry
gFullCANList[p].Dat1 = CANID; // insert the new entry

// move all remaining entries one row up
gCANFilter++;
while (p < gCANFilter)
     {
     p++;
     buf1 = gFullCANList[p].Dat1;
     gFullCANList[p].Dat1 = buf0;
     buf0 = buf1;
     }

// Now work on Acceptance Filter Configuration     
// Set CAN filter for 11-bit standard identifiers
p = 0;

// Set pointer for Standard Frame Individual
// Standard Frame Explicit
LPC_CANAF->SFF_sa = p;

pAddr = (unsigned int *) LPC_CANAF_RAM_BASE;
for (n = 0; n < ((gCANFilter+1)/2); n++)
     {
     ID_lower = gFullCANList[n * 2].Dat1 & 0x0000FFFFL;
     ID_upper = gFullCANList[n * 2 + 1].Dat1 & 0x0000FFFFL;
     candata = (ID_lower << 16) + ID_upper;
     *pAddr = candata;
     p += 4;
     pAddr++;
     }

// p is still pointing to ENDofTable;
  
// Set pointer for Standard Frame Groups
// Standard Frame Group Start Address Register
LPC_CANAF->SFF_GRP_sa = p;

// Set pointer for Extended Frame Individual
// Extended Frame Start Address Register
LPC_CANAF->EFF_sa = p;

// Set pointer for Extended Frame Groups
// Extended Frame Group Start Address Register
LPC_CANAF->EFF_GRP_sa = p;

// Set ENDofTable 
// End of AF Tables Register
LPC_CANAF->ENDofTable = p;

// Acceptance Filter Mode Register, start using filter
LPC_CANAF->AFMR = 0;
  
return 1;
}


//-----------------------------------------------
void CAN_IRQHandler(void)  
{
CANStatus = LPC_CAN1->ICR;
plazma_can++;		
if ( CANStatus & (1 << 0) )
     {
	can_isr_rx1();
     }

if ( CANStatus & (1 << 1) )
     {
	can_isr_tx1();
     }

return;
}

//-----------------------------------------------
void can_isr_rx1 (void) 
{
unsigned int buf;
unsigned int *pDest;
char temp;
char *ptr,j;
plazma_can1++;
if (!(LPC_CAN1->RFS & 0xC0000400L))
     {
      
     pDest = (unsigned int *) &(gFullCANList[(LPC_CAN1->RFS & 0x000003FFL)].Dat1);
    
     // calculate contents for first entry into FullCAN list
     buf = LPC_CAN1->RFS & 0xC00F0000L; // mask FF, RTR and DLC
     buf |= 0x01002000L; // set semaphore to 01b and CAN port to 1
     buf |= LPC_CAN1->RID & 0x000007FFL; // get CAN message ID

     // now copy entire message to FullCAN list
     *pDest = buf; 
     pDest++; // set to gFullCANList[(C1RFS & 0x000003FFL)].DatA
     *pDest = LPC_CAN1->RDA; 
     pDest++; // set to gFullCANList[(C1RFS & 0x000003FFL)].DatB
     *pDest = LPC_CAN1->RDB; 

     // now set the sempahore to complete
     buf |= 0x03000000L; // set semaphore to 11b
     pDest -= 2; // set to gFullCANList[(C1RFS & 0x000003FFL)].Dat1
     *pDest = buf; 
    
	temp=(char)gFullCANList[0].DatA;
	if(temp==0x30) bR=0;
	else bR++;
	
	temp=(char)(((gFullCANList[0].Dat1)>>16)&0x0f); 
     
     ptr=(char*)(&gFullCANList[0].DatA);
	
	if(!bR)
		{
		for(j=0;j<temp;j++)
			{
			RXBUFF[j]=*ptr;
			ptr++;
			}
		}
	else if(bR==1)
		{
		for(j=8;j<(temp+8);j++)
			{
			RXBUFF[j]=*ptr;
			ptr++;
			}                      
			
		} 		
	
	
	
		
	temp=((RXBUFF[1]&0x1f)+4);

	if((CRC1_in()==RXBUFF[temp+1])&&(CRC2_in()==RXBUFF[temp+2])&&bR)
		{
  
		bIN=1;
  		can_in_an();
          
		}    
     }

LPC_CAN1->CMR = 0x04; // release receive buffer

}








