#include "avar.h"
#include "LPC17XX.H" 
#include "main.h"
#include "control.h"
#include "eeprom_map.h"
#include "25lc640.h"
#include "uart0.h"

//-----------------------------------------------
//Аварии источников
char cnt_av_umax[2],cnt_av_umin[2],cnt_av_t[2];

//-----------------------------------------------
//Флаги аварий
unsigned short av_beep,av_rele,av_stat;


//-----------------------------------------------
void avar_src_reset(char in)
{
cnt_av_umax[in]=0;
cnt_av_umin[in]=0;
cnt_av_t[in]=0;
St_[in]&=0xe3;
cnt_src[0]=10;
cnt_src[1]=10;
} 

//-----------------------------------------------
void avar_bat_hndl(char in)
{
char data[4];
unsigned int event_ptr,lc640_adr,event_ptr_find,event_cnt;

if(in==1)
	{

	St|=0x02;
	av_stat|=0x0002;
	av_rele|=0x0002;
	av_beep|=0x0002; 
    
	event_ptr=lc640_read_int(PTR_EVENT_LOG);
	event_ptr++;	
	if(event_ptr>63)event_ptr=0;	
	lc640_write_int(PTR_EVENT_LOG,event_ptr);	
	
     event_cnt=lc640_read_int(CNT_EVENT_LOG);
	if(event_cnt!=63)event_cnt=event_ptr;
	lc640_write_int(CNT_EVENT_LOG,event_cnt); 
	
	lc640_adr=EVENT_LOG+(lc640_read_int(PTR_EVENT_LOG)*32);
	
	data[0]='B';
	data[1]=0;
	data[2]='C';
	data[3]=0;
	lc640_write_long_ptr(lc640_adr,data);

	data[0]=0;
	data[1]=0;
	data[2]=0;
	data[3]=0;
	lc640_write_long_ptr(lc640_adr+4,data);

	data[0]=LPC_RTC->YEAR;
	data[1]=LPC_RTC->MONTH;
	data[2]=LPC_RTC->DOM;
	data[3]=0;
	lc640_write_long_ptr(lc640_adr+8,data);

	data[0]=LPC_RTC->HOUR;
	data[1]=LPC_RTC->MIN;
	data[2]=LPC_RTC->SEC;
	data[3]=0;
	lc640_write_long_ptr(lc640_adr+12,data);
	
	data[0]='A';
	data[1]='A';
	data[2]='A';
	data[3]='A';
	lc640_write_long_ptr(lc640_adr+16,data);
	
	data[0]='A';
	data[1]='A';
	data[2]='A';
	data[3]='A';
	lc640_write_long_ptr(lc640_adr+20,data);
	
	data[0]='A';
	data[1]='A';
	data[2]='A';
	data[3]='A';
	lc640_write_long_ptr(lc640_adr+24,data);
	
	data[0]='A';
	data[1]='A';
	data[2]='A';
	data[3]='A';
	lc640_write_long_ptr(lc640_adr+28,data);				
	
	snmp_trap_send("Battery alarm, not founded",2,2,2);
	}

else if(in==0)
	{

	St&=0xfd;
	av_stat&=0xfffd;
	av_rele&=0xfffd;
	av_beep&=0xfffd;
		
     event_ptr=lc640_read_int(PTR_EVENT_LOG);
	if(event_ptr>63) event_ptr=63;
	event_ptr_find=event_ptr;
	
avar_bat_hndl_lbl1: 

	lc640_adr=EVENT_LOG+(event_ptr_find*32);

     lc640_read_long_ptr(lc640_adr,data);
     
     if(!((data[0]=='B')&&(data[1]==0)&&(data[2]=='C')))
     	{        
     	if(event_ptr_find)event_ptr_find--;
     	else event_ptr_find=63;
     	if(event_ptr_find==event_ptr)goto avar_bat_hndl_end;
     	else goto avar_bat_hndl_lbl1;
     	}
     else 
     	{
     	lc640_read_long_ptr(lc640_adr+16,data);
     	if(!((data[0]=='A')&&(data[1]=='A')&&(data[2]=='A')&&(data[3]=='A')))
     		{        
     		if(event_ptr_find)event_ptr_find--;
         		else event_ptr_find=63;
         	    	if(event_ptr_find==event_ptr)goto avar_bat_hndl_end;
     		else goto avar_bat_hndl_lbl1;
     		}

     	}
     		
	data[0]=LPC_RTC->YEAR;
	data[1]=LPC_RTC->MONTH;
	data[2]=LPC_RTC->DOM;
	data[3]=0;
	lc640_write_long_ptr(lc640_adr+16,data);

	data[0]=LPC_RTC->HOUR;
	data[1]=LPC_RTC->MIN;
	data[2]=LPC_RTC->SEC;
	data[3]=0;
	lc640_write_long_ptr(lc640_adr+20,data);
	
	data[0]='B';
	data[1]='B';
	data[2]='B';
	data[3]='B';
	lc640_write_long_ptr(lc640_adr+24,data);
	
	data[0]='B';
	data[1]='B';
	data[2]='B';
	data[3]='B';
	lc640_write_long_ptr(lc640_adr+28,data);
	
	snmp_trap_send("Battery alarm cleared",2,2,2);
	
	}
	
avar_bat_hndl_end:
__nop();
}


//-----------------------------------------------
void avar_bat_as_hndl(void)
{
char data[4];
unsigned short event_ptr,lc640_adr,event_ptr_find,event_cnt;


	{
	St|=0x80;
	av_stat|=0x8000;
	av_rele|=0x8000;
	av_beep|=0x8000; 
    
	event_ptr=lc640_read_int(PTR_EVENT_LOG);
	event_ptr++;	
	if(event_ptr>63)event_ptr=0;	
	lc640_write_int(PTR_EVENT_LOG,event_ptr);	
	
     event_cnt=lc640_read_int(CNT_EVENT_LOG);
	if(event_cnt!=63)event_cnt=event_ptr;
	lc640_write_int(CNT_EVENT_LOG,event_cnt); 
	
	lc640_adr=EVENT_LOG+(lc640_read_int(PTR_EVENT_LOG)*32);
	
	data[0]='B';
	data[1]=0;
	data[2]='S';
	data[3]=0;
	lc640_write_long_ptr(lc640_adr,data);

	data[0]=0;
	data[1]=0;
	data[2]=0;
	data[3]=0;
	lc640_write_long_ptr(lc640_adr+4,data);

	data[0]=LPC_RTC->YEAR;
	data[1]=LPC_RTC->MONTH;
	data[2]=LPC_RTC->DOM;
	data[3]=0;
	lc640_write_long_ptr(lc640_adr+8,data);

	data[0]=LPC_RTC->HOUR;
	data[1]=LPC_RTC->MIN;
	data[2]=LPC_RTC->SEC;
	data[3]=0;
	lc640_write_long_ptr(lc640_adr+12,data);
	
	data[0]='A';
	data[1]='A';
	data[2]='A';
	data[3]='A';
	lc640_write_long_ptr(lc640_adr+16,data);
	
	data[0]='A';
	data[1]='A';
	data[2]='A';
	data[3]='A';
	lc640_write_long_ptr(lc640_adr+20,data);
	
	data[0]='A';
	data[1]='A';
	data[2]='A';
	data[3]='A';
	lc640_write_long_ptr(lc640_adr+24,data);
	
	data[0]='A';
	data[1]='A';
	data[2]='A';
	data[3]='A';
	lc640_write_long_ptr(lc640_adr+28,data);				

	snmp_trap_send("Battery alarm, asimetry",5,1,2);
	}


avar_bat_as_hndl_end:
__nop();		
}

//-----------------------------------------------
void avar_fuse_hndl(char in)
{
char data[4];
unsigned short event_ptr,lc640_adr,event_ptr_find,event_cnt;


	{
  
	event_ptr=lc640_read_int(PTR_EVENT_LOG);
	event_ptr++;	
	if(event_ptr>63)event_ptr=0;	
	lc640_write_int(PTR_EVENT_LOG,event_ptr);	
	
     event_cnt=lc640_read_int(CNT_EVENT_LOG);
	if(event_cnt!=63)event_cnt=event_ptr;
	lc640_write_int(CNT_EVENT_LOG,event_cnt); 
	
	lc640_adr=EVENT_LOG+(lc640_read_int(PTR_EVENT_LOG)*32);
	
	data[0]='F';
	data[1]=in;
	data[2]=0;
	data[3]=0;
	lc640_write_long_ptr(lc640_adr,data);

	data[0]=0;
	data[1]=0;
	data[2]=0;
	data[3]=0;
	lc640_write_long_ptr(lc640_adr+4,data);

	data[0]=LPC_RTC->YEAR;
	data[1]=LPC_RTC->MONTH;
	data[2]=LPC_RTC->DOM;
	data[3]=0;
	lc640_write_long_ptr(lc640_adr+8,data);

	data[0]=LPC_RTC->HOUR;
	data[1]=LPC_RTC->MIN;
	data[2]=LPC_RTC->SEC;
	data[3]=0;
	lc640_write_long_ptr(lc640_adr+12,data);
	
	data[0]='A';
	data[1]='A';
	data[2]='A';
	data[3]='A';
	lc640_write_long_ptr(lc640_adr+16,data);
	
	data[0]='A';
	data[1]='A';
	data[2]='A';
	data[3]='A';
	lc640_write_long_ptr(lc640_adr+20,data);
	
	data[0]='A';
	data[1]='A';
	data[2]='A';
	data[3]='A';
	lc640_write_long_ptr(lc640_adr+24,data);
	
	data[0]='A';
	data[1]='A';
	data[2]='A';
	data[3]='A';
	lc640_write_long_ptr(lc640_adr+28,data);				

	if(in==0)		snmp_trap_send("Alarm Fuse #1 broken",5,1,2);
	else if(in==1)	snmp_trap_send("Alarm Fuse #2 broken",5,1,2);
	else if(in==2)	snmp_trap_send("Alarm Fuse #3 broken",5,1,2);

	}


avar_fuse_hndl_end:
__nop();		
}

//-----------------------------------------------
void wrk_mem_hndl(unsigned short in)
{
char data[4];
unsigned short event_ptr,lc640_adr,event_ptr_find,event_cnt;
unsigned char temp,temp_;
unsigned short tempUI;
unsigned long tempUL; 
signed short temp_temp;

if(ibat_integr==0) goto wrk_mem_hndl_end;

temp_temp=ubat_old[((ubat_old_cnt+1)&0x07)]; 


event_ptr=lc640_read_int(PTR_EVENT_LOG);
event_ptr++;	
if(event_ptr>63)event_ptr=0;	
lc640_write_int(PTR_EVENT_LOG,event_ptr);	
	
event_cnt=lc640_read_int(CNT_EVENT_LOG);
if(event_cnt!=63)event_cnt=event_ptr;
lc640_write_int(CNT_EVENT_LOG,event_cnt); 
	
lc640_adr=EVENT_LOG+(lc640_read_int(PTR_EVENT_LOG)*32);
	
data[0]='B';
data[1]=0; 
data[2]='W';
data[3]=0;

lc640_write_long_ptr(lc640_adr,data);

data[0]=*((char*)&ibat_integr);
data[1]=*(((char*)(&ibat_integr))+1);
data[2]=*((char*)&temp_temp);
data[3]=*(((char*)(&temp_temp))+1);
lc640_write_long_ptr(lc640_adr+4,data);

lc640_write_long_ptr(lc640_adr+8,(char*)&wrk_date[0]);
	
lc640_write_long_ptr(lc640_adr+12,(char*)&wrk_date[1]);

data[0]=LPC_RTC->YEAR;
data[1]=LPC_RTC->MONTH;
data[2]=LPC_RTC->DOM;
data[3]=0;
lc640_write_long_ptr(lc640_adr+16,data);

data[0]=LPC_RTC->HOUR;
data[1]=LPC_RTC->MIN;
data[2]=LPC_RTC->SEC;
data[3]=0;
lc640_write_long_ptr(lc640_adr+20,data);
	


	
wrk_mem_hndl_end:
	
}

//-----------------------------------------------
void vz_mem_hndl(unsigned short in)
{
char data[4];
unsigned int event_ptr,lc640_adr,event_ptr_find,event_cnt;
char avar_simbol;

if(in==1)
	{
	event_ptr=lc640_read_int(PTR_EVENT_LOG);
	event_ptr++;	
	if(event_ptr>63)event_ptr=0;	
	lc640_write_int(PTR_EVENT_LOG,event_ptr);	
	
     event_cnt=lc640_read_int(CNT_EVENT_LOG);
	if(event_cnt!=63)event_cnt=event_ptr;
	lc640_write_int(CNT_EVENT_LOG,event_cnt); 
	
	lc640_adr=EVENT_LOG+(lc640_read_int(PTR_EVENT_LOG)*32);
	
	data[0]='B';
	data[1]=0; 
	data[2]='Z';
	data[3]=0;
	lc640_write_long_ptr(lc640_adr,data);

	data[0]=0;
	data[1]=0;
	data[2]=0;
	data[3]=0;
	lc640_write_long_ptr(lc640_adr+4,data);

	data[0]=LPC_RTC->YEAR;
	data[1]=LPC_RTC->MONTH;
	data[2]=LPC_RTC->DOM;
	data[3]=0;
	lc640_write_long_ptr(lc640_adr+8,data);

	data[0]=LPC_RTC->HOUR;
	data[1]=LPC_RTC->MIN;
	data[2]=LPC_RTC->SEC;
	data[3]=0;
	lc640_write_long_ptr(lc640_adr+12,data);
	
	data[0]='A';
	data[1]='A';
	data[2]='A';
	data[3]='A';
	lc640_write_long_ptr(lc640_adr+16,data);
	
	data[0]='A';
	data[1]='A';
	data[2]='A';
	data[3]='A';
	lc640_write_long_ptr(lc640_adr+20,data);
	
	data[0]='A';
	data[1]='A';
	data[2]='A';
	data[3]='A';
	lc640_write_long_ptr(lc640_adr+24,data);
	
	data[0]='A';
	data[1]='A';
	data[2]='A';
	data[3]='A';
	lc640_write_long_ptr(lc640_adr+28,data);		
 	
	}

else if(in==0)
	{      
     
vz_mem_hndl_lbl1: 

	lc640_adr=EVENT_LOG+(event_ptr_find*32);

     lc640_read_long_ptr(lc640_adr,data);
     
     if(!((data[0]=='B')&&(data[2]=='Z')))
     	{        
     	if(event_ptr_find)event_ptr_find--;
     	else event_ptr_find=63;
     	if(event_ptr_find==event_ptr)goto vz_mem_hndl_end;
     	else goto vz_mem_hndl_lbl1;
     	}
     else 
     	{
     	lc640_read_long_ptr(lc640_adr+16,data);
     	if(!((data[0]=='A')&&(data[1]=='A')&&(data[2]=='A')&&(data[3]=='A')))
     		{        
     		if(event_ptr_find)event_ptr_find--;
         		else event_ptr_find=63;
         		if(event_ptr_find==event_ptr)goto vz_mem_hndl_end;
     		else goto vz_mem_hndl_lbl1;
     		}

     	}	


	
	data[0]=LPC_RTC->YEAR;
	data[1]=LPC_RTC->MONTH;
	data[2]=LPC_RTC->DOM;
	data[3]=0;
	lc640_write_long_ptr(lc640_adr+16,data);

	data[0]=LPC_RTC->HOUR;
	data[1]=LPC_RTC->MIN;
	data[2]=LPC_RTC->SEC;
	data[3]=0;
	lc640_write_long_ptr(lc640_adr+20,data);
	
	data[0]='B';
	data[1]='B';
	data[2]='B';
	data[3]='B';
	lc640_write_long_ptr(lc640_adr+24,data);
	
	data[0]='B';
	data[1]='B';
	data[2]='B';
	data[3]='B';
	lc640_write_long_ptr(lc640_adr+28,data);
	
	}

vz_mem_hndl_end:
__nop(); 

}

//-----------------------------------------------
void ke_mem_hndl(unsigned short in)
{
/*unsigned char temp,temp_;
unsigned int tempUI;
unsigned long tempUL;*/
/*
unsigned short temp_S,temp_S1;

//temp_S=lc640_read_int(KE_PTR);
temp_S++;
if(temp_S>63)temp_S=0;
//lc640_write_int(KE_PTR,temp_S); 
	
//temp_S1=lc640_read_int(KE_CNT);
if(temp_S1!=63)
	{
	temp_S1=temp_S;
//	lc640_write_int(KE_CNT,temp_S1); 
	}
		
//temp_S=KE+(lc640_read_int(KE_PTR)*4);
lc640_write_long(temp_S,time_union.time_long);

//temp_S=KE_AH+(lc640_read_int(KE_PTR)*2);
lc640_write_int(temp_S,in); */

char data[4];
unsigned int event_ptr,lc640_adr,event_ptr_find,event_cnt;
unsigned char temp,temp_;
unsigned int tempUI;
unsigned long tempUL; 
signed temp_temp;

temp_temp=ubat_old[((ubat_old_cnt+1)&0x07)]; 

event_ptr=lc640_read_int(PTR_EVENT_LOG);
event_ptr++;	
if(event_ptr>63)event_ptr=0;	
lc640_write_int(PTR_EVENT_LOG,event_ptr);	
	
event_cnt=lc640_read_int(CNT_EVENT_LOG);
if(event_cnt!=63)event_cnt=event_ptr;
lc640_write_int(CNT_EVENT_LOG,event_cnt); 
	
lc640_adr=EVENT_LOG+(lc640_read_int(PTR_EVENT_LOG)*32);
	
data[0]='B';
data[1]=0; 
data[2]='K';
data[3]=0;

lc640_write_long_ptr(lc640_adr,data);

data[0]=*((char*)&in);
data[1]=*(((char*)(&in))+1);
data[2]=*((char*)&temp_temp);
data[3]=*(((char*)(&temp_temp))+1);
lc640_write_long_ptr(lc640_adr+4,data);

lc640_write_long_ptr(lc640_adr+8,(char*)&ke_date[0]);
	
lc640_write_long_ptr(lc640_adr+12,(char*)&ke_date[1]);

data[0]=LPC_RTC->YEAR;
data[1]=LPC_RTC->MONTH;
data[2]=LPC_RTC->DOM;
data[3]=0;
lc640_write_long_ptr(lc640_adr+16,data);

data[0]=LPC_RTC->HOUR;
data[1]=LPC_RTC->MIN;
data[2]=LPC_RTC->SEC;
data[3]=0;
lc640_write_long_ptr(lc640_adr+20,data);
 
}

//-----------------------------------------------
void avar_unet_hndl(char in)
{ 
//unsigned short temp_S,temp_S1; 
char data[4];
unsigned short event_ptr,lc640_adr,event_ptr_find,event_cnt;
  
if(in) 
	{
	St|=0x01;
	av_stat|=0x0001;
	av_rele|=0x0001;
	av_beep|=0x0001;		
	tzas_cnt=5*TZAS;

	event_ptr=lc640_read_int(PTR_EVENT_LOG);
	event_ptr++;	
	if(event_ptr>63)event_ptr=0;	
	lc640_write_int(PTR_EVENT_LOG,event_ptr);	
	
     event_cnt=lc640_read_int(CNT_EVENT_LOG);
	if(event_cnt!=63)event_cnt=event_ptr;
	lc640_write_int(CNT_EVENT_LOG,event_cnt); 
	
	lc640_adr=EVENT_LOG+(lc640_read_int(PTR_EVENT_LOG)*32);
	
	data[0]='P';
	data[1]=0;
	data[2]='A';
	data[3]=0;
	lc640_write_long_ptr(lc640_adr,data);

	data[0]=0;//*((char*)&Unet_store);
	data[1]=0;//*(((char*)&Unet_store)+1);
	data[2]=0;
	data[3]=0;
	lc640_write_long_ptr(lc640_adr+4,data);

	data[0]=LPC_RTC->YEAR;
	data[1]=LPC_RTC->MONTH;
	data[2]=LPC_RTC->DOM;
	data[3]=0;
	lc640_write_long_ptr(lc640_adr+8,data);

	data[0]=LPC_RTC->HOUR;
	data[1]=LPC_RTC->MIN;
	data[2]=LPC_RTC->SEC;
	data[3]=0;
	lc640_write_long_ptr(lc640_adr+12,data);
	
	data[0]='A';
	data[1]='A';
	data[2]='A';
	data[3]='A';
	lc640_write_long_ptr(lc640_adr+16,data);
	
	data[0]='A';
	data[1]='A';
	data[2]='A';
	data[3]='A';
	lc640_write_long_ptr(lc640_adr+20,data);
	
	data[0]='A';
	data[1]='A';
	data[2]='A';
	data[3]='A';
	lc640_write_long_ptr(lc640_adr+24,data);
	
	data[0]='A';
	data[1]='A';
	data[2]='A';
	data[3]='A';
	lc640_write_long_ptr(lc640_adr+28,data);				

     		
 	
	memo_out[0]=0x55;
     memo_out[1]=0x20+2;
     memo_out[2]=0;
     memo_out[3]=0;
     memo_out[4]=((UIB0[4]>>4)&0x0f)+((UIB0[4]<<4)&0xf0);
     memo_out[5]=0x55;
     memo_out[6]=0x55; 
     	
     memo_out[7]=crc_87(memo_out,7);
	memo_out[8]=crc_95(memo_out,7);
     //uart_out_adr0(memo_out,11); 		
	snmp_trap_send("Main power is off",2,2,2);
	
	
	}
else if(in==0)
	{
	St&=0xfe;
	av_stat&=0xfffe;
	av_rele&=0xfffe;
	av_beep&=0xfffe; 
	avar_src_reset(0);
	blok_src_reset(0);
	avar_src_reset(1);
	blok_src_reset(1);	
	
	
	memo_out[0]=0x55;
     memo_out[1]=0x20+2;
     memo_out[2]=0;
     memo_out[3]=0;
     memo_out[4]=((UIB0[4]>>4)&0x0f)+((UIB0[4]<<4)&0xf0);
     memo_out[5]=0xAA;
     memo_out[6]=0xAA; 
   	
     memo_out[7]=crc_87(memo_out,7);
	memo_out[8]=crc_95(memo_out,7);
     uart_out_adr0(memo_out,11); 		
		
	
     event_ptr=lc640_read_int(PTR_EVENT_LOG);
	event_ptr_find=event_ptr;
avar_unet_hndl_lbl1:
	lc640_adr=EVENT_LOG+(event_ptr_find*32);

     lc640_read_long_ptr(lc640_adr,data);
     
     if(!((data[0]=='P')&&(data[1]==0)&&(data[2]=='A')))
     	{        
     	if(event_ptr_find)event_ptr_find--;
     	else event_ptr_find=63;
     	if(event_ptr_find==event_ptr)goto avar_unet_hndl_end;
     	else goto avar_unet_hndl_lbl1;
     	}
     else 
     	{
     	lc640_read_long_ptr(lc640_adr+16,data);
     	if(!((data[0]=='A')&&(data[1]=='A')&&(data[2]=='A')&&(data[3]=='A')))
     		{        
     		if(event_ptr_find)event_ptr_find--;
         		else event_ptr_find=63;
         		if(event_ptr_find==event_ptr)goto avar_unet_hndl_end;
     		else goto avar_unet_hndl_lbl1;
     		}

     	}	
	
	data[0]=LPC_RTC->YEAR;
	data[1]=LPC_RTC->MONTH;
	data[2]=LPC_RTC->DOM;
	data[3]=0;
	lc640_write_long_ptr(lc640_adr+16,data);

	data[0]=LPC_RTC->HOUR;
	data[1]=LPC_RTC->MIN;
	data[2]=LPC_RTC->SEC;
	data[3]=0;
	lc640_write_long_ptr(lc640_adr+20,data); 
	
	data[0]=*((char*)(&unet_store));
	data[1]=*(((char*)(&unet_store))+1);
	data[2]='B';
	data[3]='B';
	lc640_write_long_ptr(lc640_adr+24,data);
	
	data[0]='B';
	data[1]='B';
	data[2]='B';
	data[3]='B';
	lc640_write_long_ptr(lc640_adr+28,data);

	snmp_trap_send("Main power is on",2,2,2);
	}
	
avar_unet_hndl_end:
//#asm("nop")
__nop();	
}

//-----------------------------------------------
void avar_bps_hndl(char bps, char in, char v)
{
char data[4]; 
unsigned short temp_S,temp_S1;
char avar_simbol;
unsigned short event_ptr,lc640_adr,event_ptr_find,event_cnt;

if(v==0)avar_simbol='T';
else if(v==1)avar_simbol='U';
else if(v==2)avar_simbol='u';
else if(v==3)avar_simbol='L';

   
if(in==1)
	{
	//src[bps]._last_avar=v;

	event_ptr=lc640_read_int(PTR_EVENT_LOG);
	event_ptr++;	
	if(event_ptr>63)event_ptr=0;	
	lc640_write_int(PTR_EVENT_LOG,event_ptr);	
	
     event_cnt=lc640_read_int(CNT_EVENT_LOG);
	if(event_cnt!=63)event_cnt=event_ptr;
	lc640_write_int(CNT_EVENT_LOG,event_cnt); 
	
	lc640_adr=EVENT_LOG+(lc640_read_int(PTR_EVENT_LOG)*32);
	
	data[0]='S';
	data[1]=bps; 
	data[2]=avar_simbol;
	data[3]=0;
	lc640_write_long_ptr(lc640_adr,data);

	data[0]=0;
	data[1]=0;
	data[2]=0;
	data[3]=0;
	lc640_write_long_ptr(lc640_adr+4,data);

	data[0]=LPC_RTC->YEAR;
	data[1]=LPC_RTC->MONTH;
	data[2]=LPC_RTC->DOM;
	data[3]=0;
	lc640_write_long_ptr(lc640_adr+8,data);

	data[0]=LPC_RTC->HOUR;
	data[1]=LPC_RTC->MIN;
	data[2]=LPC_RTC->SEC;
	data[3]=0;
	lc640_write_long_ptr(lc640_adr+12,data);
	
	data[0]='A';
	data[1]='A';
	data[2]='A';
	data[3]='A';
	lc640_write_long_ptr(lc640_adr+16,data);
	
	data[0]='A';
	data[1]='A';
	data[2]='A';
	data[3]='A';
	lc640_write_long_ptr(lc640_adr+20,data);
	
	data[0]='A';
	data[1]='A';
	data[2]='A';
	data[3]='A';
	lc640_write_long_ptr(lc640_adr+24,data);
	
	data[0]='A';
	data[1]='A';
	data[2]='A';
	data[3]='A';
	lc640_write_long_ptr(lc640_adr+28,data);
	
	if(bps==0)snmp_trap_send("Alarm, PSU #1 disabled",2,2,2);
	if(bps==1)snmp_trap_send("Alarm, PSU #2 disabled",2,2,2);

	}

else if(in==0)
	{      
     
/*     if(v==0) src[ist]._av_temper=0;
     else if(v==1) src[ist]._av_u_max=0;
	else if(v==2) src[ist]._av_u_min=0;
	else if(v==3) src[ist]._av_net=0;
     
	if((src[ist]._av_temper==0)&&(src[ist]._av_u_max==0)&&(src[ist]._av_u_min==0)&&(src[ist]._av_net==0))
	     {
	     a_.avar_stat&=((1<<(3+ist))^0xffff);
	     if(K[MNL]!=ON)a.avar_stat&=((1<<(3+ist))^0xffff);
	     }
 	*/
	event_ptr=lc640_read_int(PTR_EVENT_LOG);
	event_ptr_find=event_ptr;
	
avar_src_hndl_lbl1: 

	lc640_adr=EVENT_LOG+(event_ptr_find*32);

     lc640_read_long_ptr(lc640_adr,data);
     
     if(!((data[0]=='S')&&(data[1]==bps)&&(data[2]==avar_simbol)))
     	{        
     	if(event_ptr_find)event_ptr_find--;
     	else event_ptr_find=63;
     	if(event_ptr_find==event_ptr)goto avar_src_hndl_end;
     	else goto avar_src_hndl_lbl1;
     	}
     else 
     	{
     	lc640_read_long_ptr(lc640_adr+16,data);
     	if(!((data[0]=='A')&&(data[1]=='A')&&(data[2]=='A')&&(data[3]=='A')))
     		{        
     		if(event_ptr_find)event_ptr_find--;
         		else event_ptr_find=63;
         		if(event_ptr_find==event_ptr)goto avar_src_hndl_end;
     		else goto avar_src_hndl_lbl1;
     		}

     	}	


	
	data[0]=LPC_RTC->YEAR;
	data[1]=LPC_RTC->MONTH;
	data[2]=LPC_RTC->DOM;
	data[3]=0;
	lc640_write_long_ptr(lc640_adr+16,data);

	data[0]=LPC_RTC->HOUR;
	data[1]=LPC_RTC->MIN;
	data[2]=LPC_RTC->SEC;
	data[3]=0;
	lc640_write_long_ptr(lc640_adr+20,data);
	
	data[0]='B';
	data[1]='B';
	data[2]='B';
	data[3]='B';
	lc640_write_long_ptr(lc640_adr+24,data);
	
	data[0]='B';
	data[1]='B';
	data[2]='B';
	data[3]='B';
	lc640_write_long_ptr(lc640_adr+28,data);

	if(bps==0)snmp_trap_send("PSU #1 alarm cleared",2,2,2);
	if(bps==1)snmp_trap_send("PSU #2 alarm cleared",2,2,2);
	
	}
	
avar_src_hndl_end:
__nop();
}

//-----------------------------------------------
void reload_hndl(void)
{
char data[4];
unsigned int event_ptr,lc640_adr/*,event_ptr_find*/,event_cnt;

event_ptr=lc640_read_int(PTR_EVENT_LOG);
event_ptr++;	
if(event_ptr>63)event_ptr=0;	
lc640_write_int(PTR_EVENT_LOG,event_ptr);	
	
event_cnt=lc640_read_int(CNT_EVENT_LOG);
if(event_cnt!=63)event_cnt=event_ptr;
lc640_write_int(CNT_EVENT_LOG,event_cnt); 
	
lc640_adr=EVENT_LOG+(lc640_read_int(PTR_EVENT_LOG)*32);
	
data[0]='U';
data[1]=0;
data[2]='R';
data[3]=0;
lc640_write_long_ptr(lc640_adr,data);

data[0]=0;
data[1]=0;
data[2]=0;
data[3]=0;
lc640_write_long_ptr(lc640_adr+4,data);

data[0]=LPC_RTC->YEAR;
data[1]=LPC_RTC->MONTH;
data[2]=LPC_RTC->DOM;
data[3]=0;
lc640_write_long_ptr(lc640_adr+8,data);

data[0]=LPC_RTC->HOUR;
data[1]=LPC_RTC->MIN;
data[2]=LPC_RTC->SEC;
data[3]=0;
lc640_write_long_ptr(lc640_adr+12,data);
	
data[0]='A';
data[1]='A';
data[2]='A';
data[3]='A';
lc640_write_long_ptr(lc640_adr+16,data);
	
data[0]='A';
data[1]='A';
data[2]='A';
data[3]='A';
lc640_write_long_ptr(lc640_adr+20,data);
	
data[0]='A';
data[1]='A';
data[2]='A';
data[3]='A';
lc640_write_long_ptr(lc640_adr+24,data);
	
data[0]='A';
data[1]='A';
data[2]='A';
data[3]='A';
lc640_write_long_ptr(lc640_adr+28,data);				
	
snmp_trap_send("Power unit started",2,2,2);	
}
