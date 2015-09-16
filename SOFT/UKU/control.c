#include "control.h"
#include "LPC17XX.H" 
#include "main.h"
#include "avar.h"	 
#include "gran.h"
#include "25lc640.h"
#include "eeprom_map.h"
//#include "pcf8563.h"
//#include "tlv2542.h"
#include "uart0.h"
//#include "uart1.h"
//#include "spa_uart.h"
#include "beep.h"
#include "full_can.h"

//-----------------------------------------------
//АПВ
unsigned main_apv_cnt,hour_apv_cnt[2],reset_apv_cnt[2];
unsigned short apv_cnt_sec[2],apv_cnt[2];
char apv_cnt_1;
short adc_buff_out_[3];

//-----------------------------------------------
//Управление источниками
char cnt_src[2];
signed short cntrl_stat=600,old_cntrl_stat,cntrl_stat1,cntrl_stat2;
signed short u_necc,u_necc_,u_necc_up,u_necc_dn;
char cnt_blok;
enum_ibs Ibs=iiM,Ibso=iiM;
char bmm_cnt,bmp_cnt;
char bS=1;
signed char num_necc;
signed char cnt_num_necc;
char tzas_cnt;
char bp_tumbler[2];

//-----------------------------------------------
//Выравнивание токов
#if(UKU_VERSION==300)
short avg_main_cnt=20;
#endif
#if(UKU_VERSION==900)
short avg_main_cnt=6;
#endif
short i_avg_max,i_avg_min; 
short avg;
char bAVG;
char avg_cnt;  
char avg_num; 
signed short x_=0; 
char avg_cnt_3;

//-----------------------------------------------
//Контроль наличия батареи
char kb_cnt[2];
char kb_full_ver;
signed short bat_ver_cnt=150;
signed short ibat_,ibat__;

//-----------------------------------------------
//Спецфункции
enum_spc_stat spc_stat=spc_OFF;
unsigned long cnt_vz_sec,cnt_vz_sec_;
unsigned long ke_date[2];
char bAVZ=0;

//-----------------------------------------------
//Математика
char bit_minus;

//-----------------------------------------------
//Звук
char cnt_beep;


//-----------------------------------------------
//Контроль заряда
char sign_U,sign_I;
char superviser_cnt;

//-----------------------------------------------
//Контроль предохранителей
const char FUSE_CONST[3]={16,5,8};
char fuse_av_cnt[3];
char fuse_av_stat[3];
char fuse_av_stat_old[3];

//-----------------------------------------------
//Контроль реле аварий
char rele_av_stat[3];

//-----------------------------------------------
//Контроль сухих контактов
char sk_stat_cnt[4]={10,10,10,10};
const char SK_CONST[4]={9,6,3,0};

//-----------------------------------------------
void apv_drv(void)
{
char i;
for(i=0;i<2;i++) 
	{
	if(apv_cnt_sec[i])
		{
		apv_cnt_sec[i]--;
		if(apv_cnt_sec[i]==0)
			{
			cnt_av_umax[i]=0;
			cnt_av_umin[i]=0;
			reset_apv_cnt[i]=600;
			}
		}
	
	if(reset_apv_cnt[i])
		{
		reset_apv_cnt[i]--;
		if(reset_apv_cnt[i]==0)
			{
			apv_cnt[i]=0;
			}
		}	
		
	if(hour_apv_cnt[i])
		{
		hour_apv_cnt[i]--;
		if(hour_apv_cnt[i]==0)
			{
			apv_cnt[i]=0;
			avar_src_reset(i);
			}
		}			
	}




if(apv_cnt_1)
	{
	apv_cnt_1--;
	if(!apv_cnt_1) 
		{
		avar_src_reset(0);
		avar_src_reset(1);
		//cntrl_stat=0;
		}
	}		
}

//-----------------------------------------------
void avg_hndl(void)
{ 
#if(UKU_VERSION==300)
#define AVGCNTMAX	10
#endif
#if(UKU_VERSION==900)
#define AVGCNTMAX	10
#endif

if(++avg_cnt_3<AVGCNTMAX)goto avg_hndl_end;

avg_cnt_3=0;

if(avg_main_cnt)
	{
	avg_main_cnt--;
	goto avg_hndl_end;
	}                 

if(!(((!(St_[0]&0x3c))&&(!OFFBP1))&&((!(St_[1]&0x3c))&&(!OFFBP2)))) goto avg_hndl_end;
	
i_avg_min=Is[0];
i_avg_max=Is[0];

if(Is[1]>Is[0])
	{
	i_avg_max=Is[1];
	}               
else		   
	{
	i_avg_min=Is[1];
	}       

if(i_avg_min==0)i_avg_min=1;

avg=i_avg_max;
avg*=100;
avg/=i_avg_min;


if(avg>150) bAVG=1;
if(avg<130) bAVG=0;

avg=Is[1];
avg*=100;
avg/=Is[0];

#if(UKU_VERSION==300)
if((avg>100)||(avg<70)) bAVG=1;
if((avg<100)&&(avg>70)) bAVG=0;
#endif

#if(UKU_VERSION==900) 
if((avg>100)||(avg<70)) bAVG=1;
if((avg<100)&&(avg>70)) bAVG=0;
/*if((avg>300)||(avg<100)) bAVG=1;
if((avg<300)&&(avg>100)) bAVG=0;*/
#endif

if(bAVG==1)
	{
	if(Is[1]<Is[0]) x_++;
	else if(Is[1]>Is[0]) x_--;
	
	}
gran(&x_,-50,50);


avg_hndl_end:

__nop();
}

//-----------------------------------------------
void cntrl_drv(void)
{
signed short temp_SS;
temp_SS=1000-cntrl_stat1;
gran(&temp_SS,30,1010);
LPC_PWM1->MR2=temp_SS;

temp_SS=1000-cntrl_stat2;
gran(&temp_SS,30,1010);
LPC_PWM1->MR3=temp_SS;

LPC_PWM1->LER=12;

}

//-----------------------------------------------
void cntrl_hndl(void)
{
//signed long temp_L;

//plazma=0;


if(St&0x01)
	{
	cntrl_stat=0;
     old_cntrl_stat=0;
     }
else if((kb_cnt[0]>15)&&(kb_cnt[0]<=30))
	{
	cntrl_stat=old_cntrl_stat-30;
	gran(&cntrl_stat,30,970);
	}
else if((kb_cnt[0]<=15)&&(kb_cnt[0]>0))
	{
	cntrl_stat=old_cntrl_stat+30;
	gran(&cntrl_stat,30,970);
	} 
else if((kb_cnt[1]>15)&&(kb_cnt[1]<=30))
	{
	cntrl_stat=old_cntrl_stat-100;
	gran(&cntrl_stat,30,970);
	}
else if((kb_cnt[1]<=15)&&(kb_cnt[1]>0))
	{
	cntrl_stat=old_cntrl_stat+100;
	gran(&cntrl_stat,30,970);
	}
else if(kb_full_ver)
	{
	cntrl_stat-=5;
	if(Uload<(USIGN))
		{
		if((av_stat&0x0002)==0)avar_bat_hndl(1);
		kb_full_ver=0;
		gran(&TBAT,5,60);
     	bat_ver_cnt=TBAT*300;
		//zar_superviser_start();
		}
	gran(&cntrl_stat,30,970);
	} 	 	


else if((MSG_IND2PWM_SRC1==0x5555)||(MSG_IND2PWM_SRC2==0x5555))
	{
 	if(Uload>u_necc)
		{
		if(((Uload-u_necc)>10)&&(cntrl_stat>0))cntrl_stat-=50;
		else if(cntrl_stat)cntrl_stat--;
		}
	else if(Uload<u_necc)
		{
		if(((u_necc-Uload)>10)&&(cntrl_stat<1015))cntrl_stat+=50;
		else	if(cntrl_stat<1020)cntrl_stat++;
		}		
	}

else if(spc_stat==spc_KE)
	{
	cntrl_stat=0;
	old_cntrl_stat=0;
	} 

else 
	{ 
     if(((Ibat/10)>IZMAX)&&!bit_minus)
     	{
               	
     	if((Ibat/10)>=(2*IZMAX))
     		{  
          	//plazma=100;	
     		if((cntrl_stat>50)&&!cnt_blok)
     			{
     			cntrl_stat-=50;
     			
                    }
               else if(cntrl_stat)cntrl_stat--;     
     		}     
     	else
    			{
    			Ibs=iiM;
     		if(cntrl_stat>10)
     			{
     			cntrl_stat-=10;
     			//plazma=101;
                    }
     		}          	 	   
     	}	
	else if(((Ibat)<((IZMAX*9)))||bit_minus)
		{
				
     	if(bit_minus)
     		{
     		if((Uload<u_necc)&&((u_necc-Uload)>=((UB0-UB20)/4)))
     			{
     			if((cntrl_stat<1000)&&!cnt_blok)
     				{
     				cntrl_stat+=50;
     				//plazma=50;
                    	}
                    else
    					{
     				if(cntrl_stat<1020)
     					{
     					cntrl_stat++;
     					//plazma=51;
                    		}
     				} 	
         			}
    			else if((Uload<u_necc)&&((u_necc-Uload)<((UB0-UB20)/4)))
     			{
     			if(cntrl_stat<1020)
     				{
     				cntrl_stat++;
     				//plazma=52;
                    	}
         			}      
  			else if((Uload>u_necc)&&((Uload-u_necc)>=((UB0-UB20)/4)))
     			{
     			if((cntrl_stat>20)&&!cnt_blok)
     				{
     				cntrl_stat-=20;
     				//plazma=103;
                    	}
                    else
    					{
     				if(cntrl_stat)
     					{
     					cntrl_stat--;
     					//plazma=104;
                    		}
     				} 	
         			}
                    	
         		
    			else if((Uload>u_necc)&&((Uload-u_necc)<((UB0-UB20)/4)))
     			{
     			if(cntrl_stat)
     				{
     				cntrl_stat--;
     				//plazma=105;
                    	}
         			}               			   			
     		}
 
 
     		
     	else 
     		{
     		if(Uload<(u_necc-1))
     			{
     			if(cntrl_stat<1020)
     				{
     				cntrl_stat++;
     				//plazma=55;
                    	}
     			}
    			else if(Uload>(u_necc+1))
     			{
     			if(cntrl_stat)
     				{
     				cntrl_stat--;
     				//plazma=106;
                    	}
     			}     			
     		}	
     	}
 	old_cntrl_stat=cntrl_stat;
     }		

Ibso=Ibs;
		
cntrl_stat1=cntrl_stat;
cntrl_stat2=cntrl_stat+x_;

//cntrl_stat1=1010;
//cntrl_stat2=1010;

if((MSG_IND2PWM_SRC1)&&(MSG_IND2PWM_SRC1!=0x5555))cntrl_stat1=MSG_IND2PWM_SRC1;
if((MSG_IND2PWM_SRC2)&&(MSG_IND2PWM_SRC2!=0x5555))cntrl_stat2=MSG_IND2PWM_SRC2;
MSG_IND2PWM_SRC1=0;
MSG_IND2PWM_SRC2=0;


cntrl_stat1++;
cntrl_stat2++;


gran(&cntrl_stat1,30,970);
gran(&cntrl_stat2,30,970);

}

//-----------------------------------------------
void zar_drv(void)
{
signed short tempSS;
//signed short b_zar; 
//char i;

if(spc_stat==spc_KE)
	{
	if(Ibat>0)tempSS=0;
	else tempSS=Ibat;
	zar_cnt_ke-=tempSS;
	if(zar_cnt_ke>=SEC_IN_HOUR*10UL)
		{
		lc640_write_int(EE_ZAR_CNT_KE,lc640_read_int(EE_ZAR_CNT_KE)+1);
		zar_cnt_ke-=SEC_IN_HOUR*10UL;
		}
	}

	
if(Ibat>0)tempSS=(signed short)(((long)Ibat*KOEFPOT)/100);	
else tempSS=Ibat;
zar_cnt+=tempSS;
if(zar_cnt<=(-(SEC_IN_HOUR*10L)))
	{
	zar_cnt+=(SEC_IN_HOUR*10L);
	if(lc640_read_int(EE_ZAR_CNT)>0) lc640_write_int(EE_ZAR_CNT,lc640_read_int(EE_ZAR_CNT)-1);
	}
else if(zar_cnt>=((SEC_IN_HOUR*10L)))
	{
	zar_cnt-=(SEC_IN_HOUR*10L);
	if(BAT_C_REAL==0x5555) 
		{
		if(lc640_read_int(EE_ZAR_CNT)<(BAT_C_NOM*10)) lc640_write_int(EE_ZAR_CNT,lc640_read_int(EE_ZAR_CNT)+1);
		}
	else if(lc640_read_int(EE_ZAR_CNT)<(BAT_C_REAL)) lc640_write_int(EE_ZAR_CNT,lc640_read_int(EE_ZAR_CNT)+1);

	}
	
 /*    
if((abs(Ibat))<2000)
	{
	if(bit_minus)
		{
		
		zar_cnt-=Ibat/10;
		}
	else 
		{
		if(lc640_read_int(EE_ZAR_CNT)<C_BAT)
		     {
		     zar_cnt+=(signed short)(((long)Ibat*KOEFPOT)/1000);
		     }
		}
	}		
		
if(zar_cnt<=-3600L)
	{
	zar_cnt=0;
	if(lc640_read_int(EE_ZAR_CNT))lc640_write_int(EE_ZAR_CNT,lc640_read_int(EE_ZAR_CNT)-1);
	if(spc_stat==spc_KE)lc640_write_int(EE_ZAR_CNT_KE,lc640_read_int(EE_ZAR_CNT_KE)+1);
	}
else if(zar_cnt>=3600L)
	{
	zar_cnt=0;
	if(lc640_read_int(EE_ZAR_CNT)<C_BAT)lc640_write_int(EE_ZAR_CNT,lc640_read_int(EE_ZAR_CNT)+1);
	}
	*/
//zar_percent=(char)(((zar_cnt_ee*360L)+(zar_cnt/100L))/(CBAT*36L));
//zar_percent=(char)(((lc640_read_int(EE_ZAR_CNT)*100L)+(zar_cnt/36L))/(C_BAT));

if(BAT_C_REAL==0x5555) 
	{
	zar_percent=(lc640_read_int(EE_ZAR_CNT)/BAT_C_NOM)*100;
	}
else	zar_percent=(char)((lc640_read_int(EE_ZAR_CNT)*100)/BAT_C_REAL);
gran_char(&zar_percent,0,100);	
		



if(Ibat>=(-IKB))
	{
	if(cnt_wrk<10)
		{
		cnt_wrk++;
		if((cnt_wrk>=10)&&(wrk!=wrkOFF)) 
			{
			wrk=wrkOFF;
			//beep_init(0x7L,'O');
			wrk_mem_hndl(ibat_integr);
			}
		}
	else cnt_wrk=10;	
	}	

else if((Ibat<(-IKB))&&(spc_stat!=spc_KE/*p1*/))
	{
	if(cnt_wrk)
		{
		cnt_wrk--;
		if((cnt_wrk==0)&&(wrk!=wrkON)) 
			{
			signed char temp;
			signed short temp_temp;
			temp_temp=ubat_old[((ubat_old_cnt+1)&0x07)];
			 
			wrk=wrkON;
			
			temp=LPC_RTC->YEAR;
			gran_char(&temp,1,99);
			*((char*)(&(wrk_date[0])))=temp;
			
			temp=LPC_RTC->MONTH;
			gran_char(&temp,1,12);
			*(((char*)(&(wrk_date[0])))+1)=temp;
			
			temp=LPC_RTC->DOM;
			gran_char(&temp,1,31);
			*(((char*)(&(wrk_date[0])))+2)=temp;			
				
			*(((char*)(&(wrk_date[0])))+3)=*((char*)&temp_temp);

			temp=LPC_RTC->HOUR;
			gran_char(&temp,0,23);
			*((char*)(&(wrk_date[1])))=temp;
               
			temp=LPC_RTC->MIN;
			gran_char(&temp,0,59);
			*(((char*)(&(wrk_date[1])))+1)=temp;
	          
			temp=LPC_RTC->SEC;
			gran_char(&temp,0,59);
			*(((char*)(&(wrk_date[1])))+2)=temp;
			
			*(((char*)(&(wrk_date[1])))+3)=*(((char*)&temp_temp)+1);

			
			//beep_init(0xFL,'O'); 
			ibat_integr=0;
			ibat_integr_=0;
			}
		}
	else cnt_wrk=0;	 
	
	}
if(wrk==wrkON)
	{
	ibat_integr_+=-Ibat;
	if(ibat_integr_>=SEC_IN_HOUR*10L)
		{
		ibat_integr_-=SEC_IN_HOUR*10L;
		ibat_integr++;
		}
	}						

if((BAT_IS_ON==bisON)&&!(St&0x02))
	{
	cnt_resurs++;
	if(cnt_resurs>=SEC_IN_HOUR)
		{
		cnt_resurs=0;
		lc640_write_int(EE_BAT_RESURS,lc640_read_int(EE_BAT_RESURS)+1);
		}
	}
		
}


//-----------------------------------------------
void zar_superviser_drv(void)
{

if(((Ubat>u_necc_up) || (Ubat<u_necc_dn))&&(bat_ver_cnt<1400/*((TBAT*300)-50)*/)&&(bat_ver_cnt>10/*((TBAT*300)-50)*/)) sign_U=0;

if(((Ibat>(2*IKB)) || (Ibat<(-IKB*2))) &&(bat_ver_cnt<1400/*((TBAT*300)-50)*/)&&(bat_ver_cnt>10/*((TBAT*300)-50)*/)) sign_I=0;

if(bat_ver_cnt==((TBAT*300)-10)) 
	{
	if((sign_U==1) && (sign_I==1) && (lc640_read_int(EE_ZAR_CNT)!=BAT_C_REAL) && (NUMBAT) && (!(St&0x02)))
		{
		lc640_write_int(EE_ZAR_CNT,BAT_C_REAL);
		superviser_cnt++;
		}
	zar_superviser_start();
	}

}

//-----------------------------------------------
void zar_superviser_start(void)
{
sign_U=1;
sign_I=1;

}

//-----------------------------------------------
void spc_drv(void)
{
static char cnt_spc_drv_ke;
if(spc_stat==spc_VZ)
	{
	cnt_vz_sec++;
	cnt_vz_sec_--;
	if(!cnt_vz_sec_)
		{
		spc_stat=spc_OFF;
		vz_mem_hndl((unsigned short)cnt_vz_sec/SEC_IN_HOUR);
		}
	}
else if(spc_stat==spc_KE/*p1*/)
	{
	if((Ubat<=(USIGN))&&(main_cnt>10))
		{
		cnt_spc_drv_ke++;
		if(cnt_spc_drv_ke>=10)
			{
			spc_stat=spc_OFF;
			St_[0]&=0xc3;
			St_[1]&=0xc3; 
			lc640_write_int(EE_BAT_C_REAL,lc640_read_int(EE_ZAR_CNT_KE));
			lc640_write_int(EE_ZAR_CNT,0);
			zar_cnt=0;
			ke_mem_hndl(lc640_read_int(EE_ZAR_CNT_KE));
			} 
		}
	else cnt_spc_drv_ke=0;	
	}	
}

//-----------------------------------------------
void cntrl_blok_hndl(void)
{
if(bmm_cnt)bmm_cnt--;
if(bmp_cnt)bmp_cnt--;
if(cnt_blok)cnt_blok--;

if((bS!=bit_minus)&&(main_cnt>3))
	{
	if(bit_minus) 
		{
		bmm_cnt=10;
		if(bmp_cnt)cnt_blok=60;
		}
	else if(!bit_minus)
		{
		bmp_cnt=10;
		if(bmm_cnt)cnt_blok=60;
		}
	}

bS=bit_minus;
} 

//-----------------------------------------------
void ext_drv(void)
{
char i;
for (i=0;i<1;i++)
	{
	if(tout[i]>TMAX_EXT[i])
		{
		if(tout_max_cnt[i]<10)
			{
			tout_max_cnt[i]++;
			if((tout_max_cnt[i]>=10)&&(!TMAX_EXT_EN[i])&&(!ND_out[i]))
				{
				tout_stat[i]=tMAX;
				if(!T_EXT_SNMP_TRAP_EN[i])//rs232_transmit_of_temper(i,1,tout[i]);
					{
					if(i==0)snmp_trap_send("DT #1 is on",2,2,2);
					if(i==1)snmp_trap_send("DT #2 is on",2,2,2);
					if(i==2)snmp_trap_send("DT #3 is on",2,2,2);
					if(i==3)snmp_trap_send("DT #4 is on",2,2,2);
					}
				if(!T_EXT_REL_EN[i])av_rele|=(1<<(8+i));
				if(!T_EXT_ZVUK_EN[i])av_beep|=(1<<(8+i));
				}
			}
		else if(tout_max_cnt[i]>10)tout_max_cnt[i]=5;	
		}
	else 
		{
		if(tout_max_cnt[i]>0)
			{
			tout_max_cnt[i]--;
			if((tout_max_cnt[i]<=0)&&(!TMAX_EXT_EN[i]))
				{
				tout_stat[i]=tNORM;
				if(!T_EXT_SNMP_TRAP_EN[i])//rs232_transmit_of_temper(i,0,tout[i]);
					{
					if(i==0)snmp_trap_send("DT #1 is off",2,2,2);
					if(i==1)snmp_trap_send("DT #2 is off",2,2,2);
					if(i==2)snmp_trap_send("DT #3 is off",2,2,2);
					if(i==3)snmp_trap_send("DT #4 is off",2,2,2);
					}
				if(AV_OFF_AVT)
					{
					av_rele&=((1<<(8+i))^0xffffU);
					av_beep&=((1<<(8+i))^0xffffU);	
					}			
				}
			}
		else if(tout_max_cnt[i]<0) tout_max_cnt[i]=5;			
		}	
		
	if(tout[i]<TMIN_EXT[i])
		{
		if(tout_min_cnt[i]<10)
			{
			tout_min_cnt[i]++;
			if((tout_min_cnt[i]>=10)&&(!TMIN_EXT_EN[i]))
				{
				tout_stat[i]=tMIN;
				if(!T_EXT_SNMP_TRAP_EN[i])//rs232_transmit_of_temper(i,2,tout[i]);
					{
					}
				if(!T_EXT_REL_EN[i])av_rele|=(1<<(8+i));
				if(!T_EXT_ZVUK_EN[i])av_beep|=(1<<(8+i));
				}
			}
		else if(tout_min_cnt[i]>10)tout_min_cnt[i]=5;	
		}
	else 
		{
		if(tout_min_cnt[i]>0)
			{
			tout_min_cnt[i]--;
			if((tout_min_cnt[i]<=0)&&(!TMIN_EXT_EN[i]))
				{
				tout_stat[i]=tNORM;
				if(!T_EXT_SNMP_TRAP_EN[i])//rs232_transmit_of_temper(i,0,tout[i]);
					{
					}
				if(AV_OFF_AVT)
					{
					av_rele&=((1<<(8+i))^0xffffU);
					av_beep&=((1<<(8+i))^0xffffU);	
					}
				}
			}
		else if(tout_min_cnt[i]<0) tout_min_cnt[i]=5;			
		}			
	}

for(i=0;i<4;i++)
	{
	if((adc_buff_[SK_CONST[i]])<100)
		{
		if(sk_stat_cnt[i]<20)sk_stat_cnt[i]++;
		}
	else if((adc_buff_[SK_CONST[i]])>1500)
		{
		if(sk_stat_cnt[i])sk_stat_cnt[i]--;
		}
	if(sk_stat_cnt[i]<2)in_stat_out[i]=255;
	else if(sk_stat_cnt[i]>18)in_stat_out[i]=0;
	}
	
	
for(i=0;i<4;i++)
	{
	
	if(in_stat_out_old[i]!=in_stat_out[i])
		{
		if(((!SK_SIGN[i])&&(in_stat_out[i]==255))||(((SK_SIGN[i])&&(in_stat_out[i]==0))))
			{
			sk_stat[i]=skON;
			if(!SK_SNMP_TRAP_EN[i])//rs232_transmit_of_sk(i,1);
				{
				if(i==0)snmp_trap_send("SK #1 is on",2,2,2);
				if(i==1)snmp_trap_send("SK #2 is on",2,2,2);
				if(i==2)snmp_trap_send("SK #3 is on",2,2,2);
				if(i==3)snmp_trap_send("SK #4 is on",2,2,2);
				}
			if(!SK_REL_EN[i])av_rele|=(1<<(11+i));
			if(!SK_ZVUK_EN[i])av_beep|=(1<<(11+i));
			}
		else
			{
			sk_stat[i]=skOFF;
			if(!SK_SNMP_TRAP_EN[i])//rs232_transmit_of_sk(i,0);
				{
				if(i==0)snmp_trap_send("SK #1 is off",2,2,2);
				if(i==1)snmp_trap_send("SK #2 is off",2,2,2);
				if(i==2)snmp_trap_send("SK #3 is off",2,2,2);
				if(i==3)snmp_trap_send("SK #4 is off",2,2,2);
				}
			if(AV_OFF_AVT)
				{
				av_rele&=((1<<(11+i))^0xffffU);
				av_beep&=((1<<(11+i))^0xffffU);	
				}
			}	
		}
		
	
	in_stat_out_old[i]=in_stat_out[i];
	}	
	  
}



//-----------------------------------------------
void rs232_transmit_of_temper(char in1,char in2,short in3)
{
}

//-----------------------------------------------
void rs232_transmit_of_sk(char in1,char in2)
{
}

//-----------------------------------------------
void bat_drv(void)
{

if((Ubat<200)&&(main_cnt>5)&&(NUMBAT))
	{ 
	if(cnt_ubat<AV_UBAT_ON)
		{
		cnt_ubat++;
		if(cnt_ubat>=AV_UBAT_ON)
			{
			avar_bat_hndl(1);
			}
		}
	else if(cnt_ubat>AV_UBAT_ON) cnt_ubat=0;	
	}
else if(Ubat>200)
	{ 
	if(cnt_ubat)
		{
		cnt_ubat--;
		if(cnt_ubat==0)
			{
			avar_bat_hndl(0);
			}
		}
	else if(cnt_ubat>AV_UBAT_ON) cnt_ubat=0;	
	}
	 

if(abs(Ibat)>IKB)
	{ 
	if(cnt_ibat<AV_IBAT_ON)
		{
		cnt_ibat++;
		if(cnt_ibat>=AV_IBAT_ON)
			{
			avar_bat_hndl(0);
			}
		}
	else if(cnt_ibat<AV_IBAT_ON)cnt_ibat=0;	
	}
else cnt_ibat=0;	  	  


if((Ubat<=(USIGN))&&(Ubat>100)&&(NUMBAT))
	{
     bUrazr=1;
     }
else 
	{
	bUrazr=0;
	bUOFF=0;
	}     



if((bit_minus)&&(Ibat>(IKB*10)))
	{         
	if(cnt_irazr<50)
		{
		cnt_irazr++;
	
		}       			
	}
else 
	{
	if(cnt_irazr>5) cnt_irazr-=5;
	else if(cnt_irazr) cnt_irazr--;
	}	
if(cnt_irazr>48) bIrazr=1;        
else if(cnt_irazr<=5)
	{
	bIrazr=0;
	bIOFF=0;
	} 
	
if(UBM_AV)
	{
	signed short ubat_avg;
	signed short ubat_avg_up;
	signed short ubat_avg_down;
	signed long temp_SL;
	static signed short	ubm_av_cnt;

	char bAV;

	ubat_avg=Ubat/5;
	if((AUSW_MAIN/100)==48)
		{
		ubat_avg=Ubat/4;
		}

	temp_SL=(signed long)ubat_avg;
    	temp_SL*=(100L + ((signed long)UBM_AV));
	temp_SL/=100L;
	ubat_avg_up=(signed short)temp_SL;

	temp_SL=(signed long)ubat_avg;
    	temp_SL*=(100L - ((signed long)UBM_AV));
	temp_SL/=100L;
	ubat_avg_down=(signed short)temp_SL;

	bAV=0;

	if ((Ubat_e[0]>ubat_avg_up) || (Ubat_e[0]<ubat_avg_down)) bAV=1;
	if ((Ubat_e[1]>ubat_avg_up) || (Ubat_e[1]<ubat_avg_down)) bAV=1;
	if ((Ubat_e[2]>ubat_avg_up) || (Ubat_e[2]<ubat_avg_down)) bAV=1;
	if ((Ubat_e[3]>ubat_avg_up) || (Ubat_e[3]<ubat_avg_down)) bAV=1;
	if((AUSW_MAIN/100)!=48)
		{
		if((Ubat_e[4]>ubat_avg_up) || (Ubat_e[4]<ubat_avg_down)) bAV=1;
		}

	if((bAV)&&(!(LPC_GPIO0->FIOPIN&(1UL<<22))))
		{
		if(ubm_av_cnt<50)
			{
			ubm_av_cnt++;
			if(ubm_av_cnt>=50)
				{
				if(!(St&0x80))avar_bat_as_hndl();
				}
			}
		}
	else 
		{
		ubm_av_cnt=0;
		}
	}
	  
}

//-----------------------------------------------
void beep_hndl(void) 
{ 
static char bcnt;
bcnt++; 
if(bcnt>9)bcnt=0;
/*if(K[MNL]==ON)
	{ */
if((((av_beep&0xffff)||(fuse_av_stat[0])||(fuse_av_stat[1])||(fuse_av_stat[2]))&&(ind!=iK)&&(ind!=iTst))
	/*||((!T_EXT_ZVUK_EN[0])&&((tout_stat[0]==tMIN)||(tout_stat[0]==tMAX)))*/)beep_init(0x55555555,'R'); 

else if(bUrazr&&(!bUOFF))beep_init(0x00010001,'R');

else if(bIrazr&&(cnt_beep==0))beep_init(0x01010101,'R');

/*
else if(bTsi||bTsb)
	{
	if(!bcnt)beep_init(0x00000001,'O');
     }

else if(bIbr) beep_init(0x00000001,'R');
*/


else beep_init(0x00000000,'S');

}

//-----------------------------------------------
void kb_drv(void)
{

//char temp;

if(kb_cnt[0]) kb_cnt[0]--;
if(kb_cnt[1]) kb_cnt[1]--;

if((kb_cnt[0]==15)||(kb_cnt[1]==15))
	{
	ibat_=abs(Ibat);
	}

if(kb_cnt[0]==1)
	{
	ibat__=abs(Ibat);
	if((abs(ibat_)+abs(ibat__))<IKB)kb_start(1);
     else 
     	{
     	avar_bat_hndl(0);
     	gran(&TBAT,5,60);
     	bat_ver_cnt=TBAT*300;
	//	zar_superviser_start();
     	}
	}	

if(kb_cnt[1]==1)
	{
	ibat__=abs(Ibat);
	if((abs(ibat_)+abs(ibat__))<IKB)kb_start(2);//avar_bat_hndl(1);
	else     	
		{
     	avar_bat_hndl(0);
     	gran(&TBAT,5,60);
     	bat_ver_cnt=TBAT*300;
		//zar_superviser_start();
     	}
	}	
}

//U1 - adc_buff_[2];
//U2 - adc_buff_[0];
//Ub - tlv_buff_[1];
//Ib - tlv_buff_[0];
//I1 - adc_buff_[3];
//I2 - adc_buff_[1];
//T1 - adc_buff_[6];
//T2 - adc_buff_[7];
//Tb - adc_buff_[5];
//-----------------------------------------------
void matemat(void)
{
//signed short temp_SS;
signed long temp_SL;
char temp;

signed char temp_s;

static char plpl;
char i;

temp_SL=(signed long)adc_buff_[17];		 
temp_SL*=Kubat;							 
temp_SL/=5000L;
Ubat=(signed short)temp_SL;

temp_SL=(signed long)adc_buff_[7];		 
temp_SL*=Kubat_part[0];							 
temp_SL/=5000L;
Ubat_part[0]=(signed short)temp_SL;

temp_SL=(signed long)adc_buff_[4];		 
temp_SL*=Kubat_part[1];							 
temp_SL/=5000L;
Ubat_part[1]=(signed short)temp_SL;

temp_SL=(signed long)adc_buff_[1];		 
temp_SL*=Kubat_part[2];							 
temp_SL/=5000L;
Ubat_part[2]=(signed short)temp_SL;

temp_SL=(signed long)adc_buff_[10];		 
temp_SL*=Kubat_part[3];							 
temp_SL/=5000L;
Ubat_part[3]=(signed short)temp_SL;

temp_SL=(signed long)adc_buff_[25];
temp_SL-=Kibat0;
temp_SL*=-Kibat1;
temp_SL/=1000L;
if(!bI) Ibat=(signed short)temp_SL; 

//Ibat=-120;

Ubat_e[0]=Ubat_part[0];

Ubat_e[1]=Ubat_part[1]-Ubat_part[0];
if(Ubat_e[1]<0)Ubat_e[1]=0;

Ubat_e[2]=Ubat_part[2]-Ubat_part[1];
if(Ubat_e[2]<0)Ubat_e[2]=0;

if((AUSW_MAIN/100)==48)Ubat_e[3]=Ubat-Ubat_part[2];
else Ubat_e[3]=Ubat_part[3]-Ubat_part[2];
if(Ubat_e[3]<0)Ubat_e[3]=0;

if((AUSW_MAIN/100)==48)Ubat_e[4]=0;
else Ubat_e[4]=Ubat-Ubat_part[3];
if(Ubat_e[4]<0)Ubat_e[4]=0;

if(Ibat<0)bit_minus=1;
else bit_minus=0;

if((kb_full_ver)&&(abs(Ibat)>IKB))
	{
	kb_full_ver=0;
	avar_bat_hndl(0);
	gran(&TBAT,5,60);
     bat_ver_cnt=TBAT*300;
	//zar_superviser_start();
	}


if((Ubat>=100)&&(MSG_IND2OUT_DIS_BAT==0)) Uload=Ubat;
else if (Us[0]>=100) Uload=Us[0];
else Uload=Us[1];

iload=Is[0]+Is[1]-(Ibat/10);
if(iload<0) iload=0;

temp_SL=(signed long)adc_buff_[24];
temp_SL*=Kunet;
temp_SL/=8000L;
Unet=(signed short)temp_SL;
/*
temp_SL=(signed long)adc_buff_[11];
temp_SL*=Kuload;
temp_SL/=8000L;
Uload=(signed short)temp_SL;
*/
temp_SL=(signed long)adc_buff_[12];
temp_SL*=Kusrc[0];
temp_SL/=4000L;
Us[0]=(signed short)temp_SL;
 
temp_SL=(signed long)adc_buff_[18];
temp_SL*=Kusrc[1];
temp_SL/=4000L;
Us[1]=(signed short)temp_SL;

temp_SL=(signed long)adc_buff_[15];
temp_SL-=Ki0src[0];
if(temp_SL<0)temp_SL=0;
temp_SL*=Kisrc[0];
temp_SL/=8000L;
Is[0]=(signed short)temp_SL;
		
temp_SL=(signed long)adc_buff_[21];
temp_SL-=Ki0src[1];
if(temp_SL<0)temp_SL=0;
temp_SL*=Kisrc[1];
temp_SL/=8000L;
Is[1]=(signed short)temp_SL;


if((adc_buff_[22]>800)&&(adc_buff_[22]<3800))ND[0]=0;
else ND[0]=0xff;
temp_SL=(signed long)adc_buff_[22];
temp_SL*=Ktsrc[0];
temp_SL/=20000L;
temp_SL-=273L;
t_i[0]=(signed short)temp_SL;


if((adc_buff_[19]>800)&&(adc_buff_[19]<3800))ND[1]=0;
else ND[1]=0xff;
temp_SL=(signed long)adc_buff_[19];
temp_SL*=Ktsrc[1];
temp_SL/=20000L;
temp_SL-=273L;
t_i[1]=(signed short)temp_SL;


if((adc_buff_[14]>800)&&(adc_buff_[14]<3800))NDB=0;
else NDB=0xff;
temp_SL=(signed long)adc_buff_[14];
temp_SL*=Ktbat;
temp_SL/=20000L;
temp_SL-=273L;
t_b=(signed short)temp_SL;


if((adc_buff_[13]>800)&&(adc_buff_[13]<3800))ND_out[0]=0;
else ND_out[0]=0xff;

temp_SL=(signed long)adc_buff_[13];
temp_SL*=Ktout[0];
temp_SL/=20000L;
temp_SL-=273L;
tout[0]=(signed short)temp_SL;

/*
if((adc_buff_out_[1]>200)&&(adc_buff_out_[1]<800))ND_out[1]=0;
else ND_out[1]=0xff;

temp_SL=(signed long)adc_buff_out_[1];
temp_SL*=Ktout[1];
temp_SL/=2000L;
temp_SL-=273L;
tout[1]=(signed short)temp_SL;


if((adc_buff_out_[2]>200)&&(adc_buff_out_[2]<800))ND_out[2]=0;
else ND_out[2]=0xff;

temp_SL=(signed long)adc_buff_out_[2];
temp_SL*=Ktout[2];
temp_SL/=2000L;
temp_SL-=273L;
tout[2]=(signed short)temp_SL;
*/

if((Is[0]+Is[1])>IMAX)
	{
	num_necc=2;
	cnt_num_necc=25;
	}
	
else if(((Is[0]+Is[1])*10)<(IMAX*KIMAX))
	{
	if(cnt_num_necc)
		{
		cnt_num_necc--;
		if(cnt_num_necc==0) num_necc=1;
		}
	}	

data_rs[0]=0x80;
if(MAIN_BPS) data_rs[0]|=0x01;
else if(!MAIN_BPS) data_rs[0]&=0xfe;

if(St&0x02)data_rs[0]&=0xfd;
else if(!(St&0x02))  data_rs[0]|=0x02;

if(spc_stat==spc_VZ) data_rs[0]|=BIN8(100);
else if(spc_stat==spc_OFF) data_rs[0]&=BIN8(11111011);

if((spc_stat==spc_KE)) data_rs[0]|=BIN8(1000);
else if(spc_stat==spc_OFF) data_rs[0]&=BIN8(11110111);


data_rs[1]=*((char*)&Unet);
data_rs[2]=*(((char*)&Unet)+1);

data_rs[3]=*((char*)&fnet);
data_rs[4]=*(((char*)&fnet)+1);


data_rs[5]=*((char*)&Ubat);
data_rs[6]=*(((char*)&Ubat)+1);
data_rs[7]=*((char*)&Ibat);
data_rs[8]=*(((char*)&Ibat)+1);
data_rs[9]=t_b;

data_rs[10]=*((char*)&BAT_C_REAL);
data_rs[11]=*(((char*)&BAT_C_REAL)+1);

data_rs[12]=*((char*)&zar_percent);
data_rs[13]=*(((char*)&zar_percent)+1);


data_rs[14]=BIN8(10000000);
if(OFFBP1)data_rs[14]&=BIN8(1111111);

data_rs[14]|=St_[0]&(BIN8(11100));
if(St&0x01)data_rs[14]|=BIN8(1);

data_rs[15]=*((char*)&Us[0]);
data_rs[16]=*(((char*)&Us[0])+1);
data_rs[17]=*((char*)&Is[0]);
data_rs[18]=*(((char*)&Is[0])+1);
data_rs[19]=t_i[0];

data_rs[20]=BIN8(10000000);
if(OFFBP2)data_rs[20]&=BIN8(1111111);

data_rs[20]|=St_[1]&(BIN8(11100));
if(St&0x01)data_rs[20]|=BIN8(1);

data_rs[21]=*((char*)&Us[1]);
data_rs[22]=*(((char*)&Us[1])+1);
data_rs[23]=*((char*)&Is[1]);
data_rs[24]=*(((char*)&Is[1])+1);
data_rs[25]=t_i[1];





data_rs0[0]=0x80;
if((!(St_[0]&0x01))||(St&0x04)) data_rs0[0]|=0x01;
else if(St_[0]&0x01) data_rs0[0]&=0xfe;

if(St_[0]&0x02) data_rs0[0]|=0x02;
else if(!(St_[0]&0x02)) data_rs0[0]&=0xfd;

if(St_[0]&0x04) data_rs0[0]|=0x04;
else if(!(St_[0]&0x04)) data_rs0[0]&=0xfb;

if(St_[0]&0x08) data_rs0[0]|=0x08;
else if(!(St_[0]&0x08)) data_rs0[0]&=0xf7;

if(St_[0]&0x10) data_rs0[0]|=0x10;
else if(!(St_[0]&0x10)) data_rs0[0]&=0xef;

if(St&0x01) data_rs0[0]|=0x20;
else if(!(St&0x01)) data_rs0[0]&=0xdf;
                                  


data_rs0[1]=0x80;
if((!(St_[1]&0x01))||(St&0x04)) data_rs0[1]|=0x01;
else if(St_[1]&0x01) data_rs0[1]&=0xfe;

if(St_[1]&0x02) data_rs0[1]|=0x02;
else if(!(St_[1]&0x02)) data_rs0[1]&=0xfd;

if(St_[1]&0x04) data_rs0[1]|=0x04;
else if(!(St_[1]&0x04)) data_rs0[1]&=0xfb;

if(St_[1]&0x08) data_rs0[1]|=0x08;
else if(!(St_[1]&0x08)) data_rs0[1]&=0xf7;

if(St_[1]&0x10) data_rs0[1]|=0x10;
else if(!(St_[1]&0x10)) data_rs0[1]&=0xef;



data_rs0[0]=0x80;
if(MAIN_BPS) data_rs0[0]|=0x01;
else if(!MAIN_BPS) data_rs0[0]&=0xfe;

if(St&BIN8(10))data_rs0[0]&=0xfd;
else if(!(St&BIN8(10)))  data_rs0[0]|=0x02;

if(spc_stat==spc_VZ) data_rs0[0]|=0x04;
else if(spc_stat==spc_OFF) data_rs0[0]&=0xfb;

if(spc_stat==spc_KE) data_rs0[0]|=0x08;
else if(spc_stat!=spc_KE) data_rs0[0]&=0xf7;

data_rs0[1]=0x80;
data_rs0[2]=0x80;
data_rs0[3]=0xc0;
data_rs0[1]|=(Us[0]&BIN8(1111111));
data_rs0[2]|=(Us[1]&BIN8(1111111));
data_rs0[3]|=(((Us[0]>>7)&BIN8(111)));
data_rs0[3]|=(((Us[1]>>4)&BIN8(111000)));
		
data_rs0[4]=0x80;
data_rs0[5]=0x80;
data_rs0[6]=0xc0;
data_rs0[4]|=(Ubat&BIN8(1111111));
data_rs0[5]|=(Uload&BIN8(1111111));
data_rs0[6]|=(((Ubat>>7)&BIN8(111)));
data_rs0[6]|=(((Uload>>4)&BIN8(111000))); 

data_rs0[7]=0x80;
if(Is[0]>=127)data_rs0[7]=0xff;
else data_rs0[7]|=(Is[0]&BIN8(1111111));

data_rs0[8]=0x80;
if(Is[1]>=127)data_rs0[8]=0xff;
else data_rs0[8]|=(Is[1]&BIN8(1111111));
//bit_minus=1;


temp=(char)(Ibat/10);
if(!temp) temp=0x80;
//else if(!bit_minus) temp&=0x7f;
//else if(bit_minus)  temp=0x80|(((temp&0x7f)^0x7f)+1);
data_rs0[9]=temp;
//data_rs0[11]=0xf6;
//data_rs0[9]=44;

temp=(char)t_i[0]&0x7f;
if (temp<=0) data_rs0[10]=0x80;
else if(temp>0) data_rs0[10]=0x80+temp;

temp=(char)t_i[1]&0x7f;
if (temp<=0) data_rs0[11]=0x80;
else if(temp>0) data_rs0[11]=0x80+temp;

temp=(char)t_b&0x7f;
if (temp<=0) data_rs0[12]=0x80;
else if(temp>0) data_rs0[12]=0x80+temp;

data_rs0[13]=(char)(Unet+1);

//data_rs0[14]=101;
if(iload>=254)data_rs0[14]=0xff;
else data_rs0[14]=(char)iload+1;

data_rs0[15]=(char)(((BAT_C_REAL/10)+1)&0x7F);

data_rs0[16]=(char)((zar_percent+1)&0x7f);


for(i=0;i<7;i++)
	{
	lakb[i]._rat_cap= (lakb_damp[i][13]*256)+ lakb_damp[i][14];
	lakb[i]._max_cell_volt= (lakb_damp[i][0]*256)+ lakb_damp[i][1];
	lakb[i]._min_cell_volt= (lakb_damp[i][2]*256)+ lakb_damp[i][3];
	lakb[i]._max_cell_temp= lakb_damp[i][4];
	lakb[i]._min_cell_temp= lakb_damp[i][5];
	lakb[i]._tot_bat_volt= (lakb_damp[i][6]*256)+ lakb_damp[i][7];
	lakb[i]._ch_curr= (lakb_damp[i][8]*256)+ lakb_damp[i][8];
	lakb[i]._dsch_curr= (lakb_damp[i][10]*256)+ lakb_damp[i][11];
	lakb[i]._s_o_c= lakb_damp[i][12];
	lakb[i]._r_b_t= lakb_damp[i][15];
	lakb[i]._c_c_l_v= (lakb_damp[i][16]*256)+ lakb_damp[i][17];
	lakb[i]._s_o_h= lakb_damp[i][18];

	if(lakb[i]._rat_cap==0)
		{
		if(lakb[i]._isOnCnt)
			{
			lakb[i]._isOnCnt--;
			if(lakb[i]._isOnCnt==0)
				{
				if(lakb[i]._battIsOn!=0) bLAKB_KONF_CH=1;
				}
			}
		}
	else 
		{
		if(lakb[i]._isOnCnt<50)
			{
			lakb[i]._isOnCnt++;
			if(lakb[i]._isOnCnt==50)
				{
				if(lakb[i]._battIsOn!=1) bLAKB_KONF_CH=1;
				}
			}
		}
	gran(&lakb[i]._isOnCnt,0,50);

	

	}

if(lakb_damp[0][34]==100)
	{
	bRS485ERR=1;
	}
if(lakb_damp[0][34]==0)
	{
	if(bRS485ERR)bLAKB_KONF_CH=1;
	bRS485ERR=0;
	}



}

//-----------------------------------------------
void out_out(void)
{
//char temp;
 
if((MSG_IND2OUT_DIS_SRC1)&&(!MSG_IND2OUT_DIS_SRC1_OLD))cnt_src[0]=10; 
if((MSG_IND2OUT_DIS_SRC2)&&(!MSG_IND2OUT_DIS_SRC2_OLD))cnt_src[1]=10;

if(cnt_src[0])cnt_src[0]--;
if(cnt_src[1])cnt_src[1]--;
       



if((tzas_cnt)&&(!(St&0x01)))tzas_cnt--;

if(OFFBP1) cnt_src[0]=0;
if(OFFBP2) cnt_src[1]=0;

if((ind==iTst_BPS1)/*&&(sub_ind==2)*/)
	{
	bp_on(1);
	if(cnt_src[1])bp_on(2);
	else bp_off(2);
	} 
else if((ind==iTst_BPS2)/*&&(sub_ind==3)*/)
	{
	bp_on(2);           
	if(cnt_src[0])bp_on(1);
	else bp_off(1);
	}
else if(ind==iTst_BPS12)
	{
	bp_on(1);
	bp_on(2);           
	}	
else 
	{
	if(NUMIST<2)
		{
		if((main_cnt>10)/*&&(ind!=iK)*/)
			{
			if((spc_stat==spc_KE) ||(tzas_cnt)
		//	#if(UKU_VERSION==300) 
			||(St_[0]&0x2c)
		//	#endif
		//	#if(UKU_VERSION==900)
		//	||(St_[0]&0x3c)
		//	#endif 
			||((St&0x01)&&(Us[0]<200)) ||((St_[0]&0x20)) ||(apv_cnt_sec[0])||(hour_apv_cnt[0])) bp_off(1);
			else bp_on(1);
			}
		else bp_on(1);
		}
	else
		{	
	     if((main_cnt>10)&&(cnt_src[0]==0)&&((!((MAIN_BPS==1)&&(St_[1]&0x3c)))||(hour_apv_cnt[0]))) //условия, разрешающие выключение источника
		     {
		     if((spc_stat==spc_KE) ||((MAIN_BPS==1)&&(num_necc<2)&&(!OFFBP2))||(St_[0]&0x3c)||(apv_cnt_sec[0])) bp_off(1); //условия, вызывающие выключение источника
		     else bp_on(1);
		     }
	     else bp_on(1);
		}
 	
 	if(NUMIST<2)bp_off(2); 
	else
	     {
	     if((main_cnt>10)&&(cnt_src[1]==0)/*&&(ind!=iK)*/&&((!((MAIN_BPS==0)&&(St_[0]&0x3c)))||(hour_apv_cnt[1]))) //условия, разрешающие выключение источника
		     {
		     if((spc_stat==spc_KE) ||((MAIN_BPS==0)&&(num_necc<2)&&(!OFFBP1))||(St_[1]&0x3c)||(apv_cnt_sec[1])) bp_off(2);//условия, вызывающие выключение источника
			else bp_on(2);
			}
		else bp_on(2);
		}
	//#endif		
	}
//bp_off(1);	

if((cnt_src[0]==0)&&(MSG_IND2OUT_DIS_SRC1))bp_off(1);        
if((cnt_src[1]==0)&&(MSG_IND2OUT_DIS_SRC2))bp_off(2);

if(MSG_IND2OUT_EN_SRC1)bp_on(1);        
if(MSG_IND2OUT_EN_SRC2)bp_on(2);

MSG_IND2OUT_DIS_SRC1_OLD=MSG_IND2OUT_DIS_SRC1;
MSG_IND2OUT_DIS_SRC2_OLD=MSG_IND2OUT_DIS_SRC2;
MSG_IND2OUT_DIS_SRC1=0;
MSG_IND2OUT_DIS_SRC2=0;
MSG_IND2OUT_EN_SRC1_OLD=MSG_IND2OUT_EN_SRC1;
MSG_IND2OUT_EN_SRC2_OLD=MSG_IND2OUT_EN_SRC2;
MSG_IND2OUT_EN_SRC1=0;
MSG_IND2OUT_EN_SRC2=0;

//IO0DIR_bit.P0_10=1;
//IO0DIR_bit.P0_11=1;
SET_REG(LPC_GPIO2->FIODIR,1,0,1);
SET_REG(LPC_GPIO2->FIODIR,1,1,1);

if(main_cnt<10)
	{
	SET_REG(LPC_GPIO2->FIOCLR,1,0,1);
	SET_REG(LPC_GPIO2->FIOCLR,1,1,1);
	}
else
	{
	if((bp_tumbler[0]==0)/*||(BPS1_spa_blok)*/)SET_REG(LPC_GPIO2->FIOSET,1,0,1);//IO0SET_bit.P0_10=1;
	else SET_REG(LPC_GPIO2->FIOCLR,1,0,1);//IO0CLR_bit.P0_10=1;

	if((bp_tumbler[1]==0)/*||(BPS2_spa_blok)*/)SET_REG(LPC_GPIO2->FIOSET,1,1,1);//IO0SET_bit.P0_11=1;
	else SET_REG(LPC_GPIO2->FIOCLR,1,1,1);//IO0CLR_bit.P0_11=1;
	}

//SET_REG(FIO0SET,1,9,1);
//SET_REG(FIO0SET,1,8,1);


//SET_REG(LPC_GPIO0->FIOPIN,0,8,1);
//SET_REG(LPC_GPIO0->FIOPIN,0,9,1);

}


//-----------------------------------------------
void rele_drv(void)
{
LPC_GPIO0->FIODIR|=(1UL<<19)|(1UL<<20)|(1UL<<21)|(1UL<<22);


rele_av_stat[0]=0;
if((REL_AV[0]&0x01)&&(St&0x01))rele_av_stat[0]=1;
if((REL_AV[0]&0x02)&&(av_rele&0x8002))rele_av_stat[0]=1;
if((REL_AV[0]&0x04)&&((St_[0]&0x3c)||(St_[1]&0x3c)))rele_av_stat[0]=1;
if((REL_AV[0]&0x08)&&((fuse_av_stat[0])||(fuse_av_stat[1])||(fuse_av_stat[2])))rele_av_stat[0]=1;
if((REL_AV[0]&0x10)&&(av_rele&(0x000f<<11)))rele_av_stat[0]=1;
if((REL_AV[0]&0x20)&&(av_rele&(0x0001<<8)))rele_av_stat[0]=1;

rele_av_stat[1]=0;
if((REL_AV[1]&0x01)&&(St&0x01))rele_av_stat[1]=1;
if((REL_AV[1]&0x02)&&(av_rele&0x8002))rele_av_stat[1]=1;
if((REL_AV[1]&0x04)&&((St_[0]&0x3c)||(St_[1]&0x3c)))rele_av_stat[1]=1;
if((REL_AV[1]&0x08)&&((fuse_av_stat[0])||(fuse_av_stat[1])||(fuse_av_stat[2])))rele_av_stat[1]=1;
if((REL_AV[1]&0x10)&&(av_rele&(0x000f<<11)))rele_av_stat[1]=1;
if((REL_AV[1]&0x20)&&(av_rele&(0x0001<<8)))rele_av_stat[1]=1;

rele_av_stat[2]=0;
if((REL_AV[2]&0x01)&&(St&0x01))rele_av_stat[2]=1;
if((REL_AV[2]&0x02)&&(av_rele&0x8002))rele_av_stat[2]=1;
if((REL_AV[2]&0x04)&&((St_[0]&0x3c)||(St_[1]&0x3c)))rele_av_stat[2]=1;
if((REL_AV[2]&0x08)&&((fuse_av_stat[0])||(fuse_av_stat[1])||(fuse_av_stat[2])))rele_av_stat[2]=1;
if((REL_AV[2]&0x10)&&(av_rele&(0x000f<<11)))rele_av_stat[2]=1;
if((REL_AV[2]&0x20)&&(av_rele&(0x0001<<8)))rele_av_stat[2]=1;


if(tst_state[0]==tstOFF)	LPC_GPIO0->FIOSET=(1UL<<21);
else if(tst_state[0]==tstON) LPC_GPIO0->FIOCLR=(1UL<<21);
else
	{
	if(rele_av_stat[0])LPC_GPIO0->FIOCLR=(1UL<<21);
	else LPC_GPIO0->FIOSET=(1UL<<21);
	}

if(tst_state[1]==tstOFF)	LPC_GPIO0->FIOSET=(1UL<<20);
else if(tst_state[1]==tstON) LPC_GPIO0->FIOCLR=(1UL<<20);
else
	{
	if(rele_av_stat[1])LPC_GPIO0->FIOCLR=(1UL<<20);
	else LPC_GPIO0->FIOSET=(1UL<<20);
	}

if(tst_state[2]==tstOFF)	LPC_GPIO0->FIOSET=(1UL<<19);
else if(tst_state[2]==tstON) LPC_GPIO0->FIOCLR=(1UL<<19);
else
	{
	if(rele_av_stat[2])LPC_GPIO0->FIOCLR=(1UL<<19);
	else LPC_GPIO0->FIOSET=(1UL<<19);
	}

if(tst_state[3]==tstOFF)	LPC_GPIO0->FIOSET=(1UL<<22);
else if(tst_state[3]==tstON) LPC_GPIO0->FIOCLR=(1UL<<22);
else
	{
	if((MSG_IND2OUT_DIS_BAT)&&(!MSG_IND2OUT_DIS_BAT_OLD))cnt_bat=10; 

	if(cnt_bat)cnt_bat--;


	if((cnt_bat==0)&&(MSG_IND2OUT_DIS_BAT))LPC_GPIO0->FIOSET=(1UL<<22);
	if(MSG_IND2OUT_EN_BAT)LPC_GPIO0->FIOCLR=(1UL<<22);

	}
MSG_IND2OUT_DIS_BAT_OLD=MSG_IND2OUT_DIS_BAT;
MSG_IND2OUT_DIS_BAT=0;
MSG_IND2OUT_EN_BAT_OLD=MSG_IND2OUT_EN_BAT;
MSG_IND2OUT_EN_BAT=0;
}

//-----------------------------------------------
void samokalibr_drv(void)
{  
skb_cnt++;


if(skb_cnt>18000) skb_cnt=0;
if((skb_cnt>=50)&&(skb_cnt<=75)) 
	{
	kz_on();
	bI=1;
	}
else 
	{
	if((ind==iTst_klbr)&&(sub_ind==0))
		{
		if(tst_state[4]==tstOFF)kz_off();
		else if(tst_state[4]==tstON)kz_on();
		}
	else if(ind!=iK)
		{
		kz_off();
		bI=0;
		}
	}

//if(skb_cnt==75) lc640_write_int(EE_KIBAT0,tlv_buff_[0]);
if(MSG_IND2OUT_EN_RELSAM)kz_on();
MSG_IND2OUT_EN_RELSAM=0;
}

//-----------------------------------------------
void time_drv(void)
{   
static char _5hz_cnt_;

hour_cnt_5hz++;
if(hour_cnt_5hz>=18000) hour_cnt_5hz=0;

_5hz_cnt_++;
if(_5hz_cnt_>=5)
	{
	_5hz_cnt_=0;
	hour_cnt_1hz++;
	if(hour_cnt_1hz>=3600)hour_cnt_1hz=0;
	if(main_cnt<200) main_cnt++;
	
	cnt_ind++;
	if(cnt_ind>9)cnt_ind=0;
	}  

//if(RTC_HOUR_cnt_5hz==150) kb_start(0);
	
if(bat_ver_cnt)
	{
	bat_ver_cnt--;
	if(bat_ver_cnt==0) kb_start(0);
	}	

//if(kb_cnt[in]) kb_cnt[in]--;	
}

//-----------------------------------------------
void u_necc_hndl(void)
{
signed long temp_L;


if(t_b<=0) temp_L=UB0;
else if(t_b>=40) temp_L=(UB20*2U)-UB0;
else
	{
	temp_L=(UB20-UB0)*10;
	temp_L*=t_b;
	temp_L/=200;
	temp_L+=UB0;
	}
	
if(spc_stat==spc_VZ) 
	{
	temp_L*=KVZ;
	temp_L/=1000;
	} 
	
u_necc_=(signed int)temp_L;


if((ind==iTst_BPS1)&&(sub_ind==0))
	{
	if(tst_state[5]==tstOFF)u_necc=400;
	else if(tst_state[5]==tstON)u_necc=800;
	else if(tst_state[5]==tstU)u_necc=UB20;
	}
else if((ind==iTst_BPS2)&&(sub_ind==0))
	{
	if(tst_state[6]==tstOFF)u_necc=400;
	else if(tst_state[6]==tstON)u_necc=800;
	else if(tst_state[6]==tstU)u_necc=UB20;
	}
else if((ind==iTst_BPS12)&&(sub_ind==0))
	{
	if(tst_state[7]==tstOFF)u_necc=400;
	else if(tst_state[7]==tstON)u_necc=800;
	else if(tst_state[7]==tstU)u_necc=UB20;
	}				

else if(!NUMBAT) u_necc=U0B;	
else u_necc=u_necc_;		

temp_L=(signed long) u_necc;
temp_L*=99L;
temp_L/=100L;
u_necc_dn=(signed short)temp_L;

temp_L=(signed long) u_necc;
temp_L*=101L;
temp_L/=100L;
u_necc_up=(signed short)temp_L;
     
}

//-----------------------------------------------
void kz_on(void)
{
LPC_GPIO0->FIODIR|=1UL<<27;
LPC_GPIO0->FIOSET|=1UL<<27;
}
 
//-----------------------------------------------
void kz_off(void)
{
LPC_GPIO0->FIODIR|=1UL<<27;
LPC_GPIO0->FIOCLR|=1UL<<27;
} 

//-----------------------------------------------
void bp_on(char in)
{
bp_tumbler[in-1]=1;
}

//-----------------------------------------------
void bp_off(char in)
{
bp_tumbler[in-1]=0;
}

//-----------------------------------------------
void kb_start(char in) 
{
if(((St&0x02)||(abs(Ibat)<IKB))&&(spc_stat!=spc_KE/*p1)&&(spc_stat!=spc_KEp2*/)&&(!(St&0x01))&&(BAT_IS_ON==bisON)&&(!St_[0]||!(St_[1]))&&(Ubat>=200))
	{
	if(in<2)kb_cnt[in]=30;
	else if(in==2)kb_full_ver=1;
	}
else 
	{
	bat_ver_cnt=TBAT*300;	
	//zar_superviser_start();
	}
}

//-----------------------------------------------
void unet_drv(void)
{
if((Unet<UMN)&&(main_cnt>5))
	{
	if(unet_drv_cnt<AVAR_UNET_ON)
		{
		unet_drv_cnt++;
		if(unet_drv_cnt>=AVAR_UNET_ON)
			{
			unet_store=Unet;
			avar_unet_hndl(1);
			}
		}
	else if(unet_drv_cnt>AVAR_UNET_ON) unet_drv_cnt=AVAR_UNET_ON;
	if(Unet<unet_store) unet_store=Unet;
	}

else if(Unet>UMN)
	{                 
	if(unet_drv_cnt)
		{
		unet_drv_cnt--;
		if(unet_drv_cnt==0)
			{
			avar_unet_hndl(0);
			
			}
		}
	}
}

//-----------------------------------------------
void src_drv(char in)
{        
char off_bp;
if(in==0) off_bp=OFFBP1;
else off_bp=OFFBP2;

if((in==1)&&(NUMIST<2)&&(main_cnt>10))
	{
	St_[1]&=0xe3;
	St_[1]|=0x20;
	} 
else
     {
     if((!off_bp)&&(!cnt_src[in])&&(!(St_[in]&0x04))&&(Us[in]>UMAX)&&(!(St&0x01)))
	     { 
	     if(cnt_av_umax[in]<AV_UMAX_ON)
		     {
		     cnt_av_umax[in]++;
		     if(cnt_av_umax[in]>=AV_UMAX_ON)
			     {
			   	if((apv_on1!=apvON)||(apv_cnt[in]==3))
			          {
			          unsigned short mask;
			     	St_[in]|=0x04;
			     	if(in==0)mask=4;
			     	else if(in==1)mask=32;
		 	     	av_stat|=mask;
			     	av_rele|=mask;
			     	av_beep|=mask;
			     	cnt_src[in]=10;
			     	if(apv_on2==apvON)apv2_hndl(in);
			     	blok_src_reset((~in)&0x01);
			          avar_bps_hndl(in,1,1);
			          reset_apv_cnt[in]=0;
			          }	
			 	else 
			 		{
			 		cnt_src[in]=10;
			 		apv_cnt_sec[in]=60;
			 		apv_cnt[in]++;
			 		reset_apv_cnt[in]=0;
			 		}
			     
			     
//			     
//	
//			     else
//			          {     
//			     unsigned short mask;
//			     St_[in]|=0x04;
//			     if(in==0)mask=4;
//			     else if(in==1)mask=32;
//		 	     av_stat|=mask;
//			     av_rele|=mask;
//			     av_beep|=mask;
//			     cnt_src[in]=10;
//			     apv_hndl(in);
//			     blok_src_reset((~in)&0x01);
//			          }
			     }
		     }	 
	     else cnt_av_umax[in]=0;	
	     }


     else cnt_av_umax[in]=0;	 


     if((!off_bp)&&(!cnt_src[in])&&(!(St_[in]&0x08))&&(Us[in]<(UMAX/2))&&(!(St&0x01))&&(main_cnt>10))
	     { 
	     if(cnt_av_umin[in]<AV_UMIN_ON)
	     	{
	     	cnt_av_umin[in]++;
	     	if(cnt_av_umin[in]>=AV_UMIN_ON)
	     		{
	     		if((apv_on1!=apvON)||(apv_cnt[in]==3))
			          {
			          unsigned short mask;
			     	St_[in]|=0x08;
			     	if(in==0)mask=8;
	     			else if(in==1)mask=64;
		 	     	av_stat|=mask;
			     	av_rele|=mask;
			     	av_beep|=mask;
			     	cnt_src[in]=10;
			     	if(apv_on2==apvON)apv2_hndl(in);
			     	blok_src_reset((~in)&0x01);
			          avar_bps_hndl(in,1,2);
			          reset_apv_cnt[in]=0;
			          }	
			 	else 
			 		{
			 		cnt_src[in]=10;
			 		apv_cnt_sec[in]=60;
			 		apv_cnt[in]++;
			 		reset_apv_cnt[in]=0;
			 		}
			     
//	     		unsigned short mask;
//	     		St_[in]|=0x08;
//	     		if(in==0)mask=8;
//	     		else if(in==1)mask=64;
//	     		av_stat|=mask;
//	     		av_rele|=mask;
//	     	  	av_beep|=mask; 			
//	     		cnt_src[in]=10;
//	     		apv_hndl(in);
//	     		blok_src_reset((~in)&0x01);
//		     	if(apv_on1!=apvON)
//			          {
//			          avar_src_hndl(in,1,0x66);
//			          }		     		 
	     		}
	     	}
	     else cnt_av_umin[in]=0;	
	     }


     else cnt_av_umin[in]=0;	  



     if((t_i[in]>TMAX)&&(!ND[in]))
     	{ 
     	if(cnt_av_t[in]<AV_T_ON)
     		{
     		cnt_av_t[in]++;
	     	if(cnt_av_t[in]>=AV_T_ON)
	     		{
	     		unsigned short mask;
	     		St_[in]|=0x10;
     			if(in==0)mask=16;
	     		else if(in==1)mask=128;
	     		av_stat|=mask;
     			av_rele|=mask;
				av_beep|=mask;
          		cnt_src[in]=10;
     			blok_src_reset((~in)&0x01);
     			avar_bps_hndl(in,1,0);
	     		}
     		}
	     else if(cnt_av_t[in]>AV_T_ON) cnt_av_t[in]=0;		
	     }
	
 

     if((t_i[in]<(TMAX-5))||(ND[in]))
	     { 
	     if(cnt_av_t[in])
	     	{
     		cnt_av_t[in]--;
     		if(cnt_av_t[in]==0)
     			{
     			unsigned short mask;
     			St_[in]&=0xef;
		     	if(in==0)mask=239;
	     		else if(in==1)mask=127;
	     		av_stat&=mask;
	     		av_rele&=mask;
	     		av_beep&=mask;
	     		avar_bps_hndl(in,0,0);
	     		}
     		}
     	}
     }	
}

//-----------------------------------------------
void blok_src_reset(char in)
{
St_[in]&=0xdf;
cnt_src[0]=10;
cnt_src[1]=10;
}

//-----------------------------------------------
void apv2_hndl(char in)
{
hour_apv_cnt[in]=60*apv_on2_time;
reset_apv_cnt[in]=0;	
}

//-----------------------------------------------
void fuse_drv(void)
{
char i;

for (i=0;i<3;i++)
	{
	if(adc_buff_[FUSE_CONST[i]]>300)
		{
		if(fuse_av_cnt[i]<10)
			{
			fuse_av_cnt[i]++;
			if(fuse_av_cnt[i]>=9)
				{
				fuse_av_stat[i]=1;
				if(fuse_av_stat_old[i]!=fuse_av_stat[i]) avar_fuse_hndl(i);
				}
			}
		}
	else if(adc_buff_[FUSE_CONST[i]]<200)
		{
		if(fuse_av_cnt[i])
			{
			fuse_av_cnt[i]--;
			if(fuse_av_cnt[i]<=1)fuse_av_stat[i]=0;
			}
		}

	
	fuse_av_stat_old[i]=fuse_av_stat[i];
	}


}

//-----------------------------------------------
void adc_drv(void)
{
int temp_S;
char i;

if (GET_REG(LPC_ADC->ADGDR,31,1))//ADDR_bit.DONE ниче не делать если не готово
	{
	adc_self_ch_buff[adc_self_ch_cnt]=(LPC_ADC->ADGDR>>4) & 0xFFF;/* Read Conversion Result             */
	adc_self_ch_cnt++;
	if(adc_self_ch_cnt>=3)
		{
		adc_self_ch_disp[0]=abs_pal(adc_self_ch_buff[1]-adc_self_ch_buff[0]);
		adc_self_ch_disp[1]=abs_pal(adc_self_ch_buff[2]-adc_self_ch_buff[1]);
		adc_self_ch_disp[2]=abs_pal(adc_self_ch_buff[2]-adc_self_ch_buff[0]);

		adc_result=(adc_self_ch_disp[0]+adc_self_ch_disp[1]+adc_self_ch_disp[2])/3;
		if(adc_self_ch_disp[2]<300)
			{
			adc_result=adc_self_ch_buff[2];
			} 
		else if(adc_self_ch_disp[1]<300)
			{
			adc_result=adc_self_ch_buff[1];
			}
		else if(adc_self_ch_disp[0]<300)
			{
			adc_result=adc_self_ch_buff[0];
			}
		else adc_result=(adc_self_ch_disp[0]+adc_self_ch_disp[1]+adc_self_ch_disp[2])/3;
		//adc_result=adc_ch*100;

		adc_buff[adc_ch][adc_cnt1]=adc_result;

		adc_self_ch_cnt=0;
		
		adc_ch++;

		if(adc_ch>=26)
			{
			adc_ch=0;
			adc_cnt1++;
			if(adc_cnt1>=16)adc_cnt1=0;
			}
		if((adc_cnt1&0x03)==0)
			{
			signed long temp_SL;
			char i;
			temp_SL=0;
			for(i=0;i<16;i++)
				{
				temp_SL+= (signed long)adc_buff[adc_ch][i];
				}
			temp_SL>>=4;
			adc_buff_[adc_ch]=(signed short)temp_SL;
			}

		}

 	}





//adc_self_ch_disp[0]=adc_self_ch_buff[0]&0x0ff0;
//adc_self_ch_disp[1]=adc_self_ch_buff[1]&0x0ff0;
//adc_self_ch_disp[2]=adc_self_ch_buff[2]&0x0ff0;



/*




	adc_chk_buff[adc_check]=GET_REG(LPC_ADC->ADGDR,4,12);//слкладываем данные во времен массив		
	if(++adc_check>=3)	// при заполнении массива выбираем истинное значение
		{
		//метод 3й лишний
//			if (adc_chk_buff[0]>adc_chk_buff[1]) temp_S=adc_chk_buff[0]-adc_chk_buff[1];
//			else temp_S=adc_chk_buff[1]-adc_chk_buff[0];
//			if(temp_S<100) temp_S= adc_chk_buff[0];	 // отсев с погрешностью больше 100
//			else temp_S = adc_chk_buff[2];
			
			//метод выбор наименьшего
		if (adc_chk_buff[0]>adc_chk_buff[1])
				{
				if (adc_chk_buff[1]>adc_chk_buff[2]) temp_S=adc_chk_buff[2];
				else temp_S=adc_chk_buff[1];
				}
			else if (adc_chk_buff[0]>adc_chk_buff[2]) temp_S=adc_chk_buff[2];
			else temp_S=adc_chk_buff[0];
			
			// процедура выбора переменной для хранения значения и усреднения
			if(adc_cnt<20&&adc_ch<9) //convert multiplex1 data
				{
				adc_buff[adc_ch][adc_cnt1]=	temp_S;
				if((adc_cnt1&0x03)==0)
					{
					temp_S=0;
					for(i=0;i<16;i++)
						{
						temp_S+=adc_buff[adc_ch][i];
						} 
					adc_buff_[adc_ch]=temp_S>>4;
					}
				}
			else if(adc_cnt<40&&adc_ch<9) //convert multiplex2 data
				{
				adc_buff1[adc_ch][adc_cnt1]= temp_S;
				if((adc_cnt1&0x03)==0)
					{
					temp_S=0;
					for(i=0;i<16;i++)
						{
						temp_S+=adc_buff1[adc_ch][i];
						} 
					adc_buff1_[adc_ch]=temp_S>>4;
					}
				}
			else if(adc_ch<9) //convert multiplex3 data
				{
				adc_buff2[adc_ch][adc_cnt1]= temp_S;
				if((adc_cnt1&0x03)==0)
					{
					temp_S=0;
					for(i=0;i<16;i++)
						{
						temp_S+=adc_buff2[adc_ch][i];
						} 
					adc_buff2_[adc_ch]=temp_S>>4;
					}
				}
			else 	 //convert input p1.31 data
				{
				unet_buff[adc_cnt1]= temp_S;
				if((adc_cnt1&0x03)==0)
					{
					temp_S=0;
					for(i=0;i<16;i++)
						{
						temp_S+=unet_buff[i];
						unet_buff_=temp_S>>4;
						}
					}		
				}	 

			++adc_cnt;
			adc_check=0;		
			}
		}
	}
else ++adc_cnt;	// данные получены предыдущем вызове, щя перекл канал
		
if(adc_cnt>=60) 
	{
	adc_cnt=0;
	adc_cnt1++;
	if(adc_cnt1>=16)
		{
		adc_cnt1=0;
		}
	}
*/
/*
if (adc_cnt<20) adc_ch=adc_cnt>>1;
else if (adc_cnt<40) adc_ch=(adc_cnt-20)>>1;
else adc_ch=(adc_cnt-40)>>1;
*/	

//Устанавливаем режимы работы ног АЦП
SET_REG(LPC_PINCON->PINSEL1,1,(23-16)*2,2);	   //P0.23
SET_REG(LPC_PINCON->PINSEL1,1,(24-16)*2,2);	   //P0.24
SET_REG(LPC_PINCON->PINSEL1,1,(25-16)*2,2);	   //P0.25
//SET_REG(LPC_PINCON->PINSEL1,1,(26-16)*2,2);	   //P0.26
SET_REG(LPC_PINCON->PINSEL3,3,(30-16)*2,2);	   //P1.30
SET_REG(LPC_PINCON->PINSEL3,3,(31-16)*2,2);	   //P1.31
SET_REG(LPC_PINCON->PINMODE1,2,(23-16)*2,2);   //P0.23
SET_REG(LPC_PINCON->PINMODE1,2,(24-16)*2,2);   //P0.24
SET_REG(LPC_PINCON->PINMODE1,2,(25-16)*2,2);   //P0.25
//SET_REG(LPC_PINCON->PINMODE1,2,(26-16)*2,2);   //P0.25
SET_REG(LPC_PINCON->PINMODE3,2,(30-16)*2,2);   //P1.30
SET_REG(LPC_PINCON->PINMODE3,2,(31-16)*2,2);   //P1.31

//SET_REG(LPC_GPIO0->FIODIR,7,5,3);
//SET_REG(LPC_GPIO0->FIOPIN,adc_ch,5,3);


// Устанавливаем адреса на коммутаторы
SET_REG(LPC_GPIO2->FIODIR,7,10,3);
SET_REG(LPC_GPIO2->FIOPIN,adc_ch/3,10,3);
/*
if(!(adc_cnt&0x01))
     {
//     if(adc_ch<=7)SET_REG(LPC_ADC->ADCR,1<<5,0,8);		  //choose AD05
//     else if(adc_ch==8) SET_REG(LPC_ADC->ADCR,1<<2,0,8);  //choose AD02
//     else SET_REG(LPC_ADC->ADCR,1<<4,0,8);				  //choose AD04
     if(adc_cnt<20&&adc_ch<=7)SET_REG(LPC_ADC->ADCR,1,0,8);		  		//choose AD00
     else if(adc_cnt<40&&adc_ch<=7)SET_REG(LPC_ADC->ADCR,1<<1,0,8);		//choose AD01
     else if(adc_ch<=7) SET_REG(LPC_ADC->ADCR,1<<2,0,8);		 			//choose AD02
     else if(adc_ch==8) SET_REG(LPC_ADC->ADCR,1<<4,0,8);  				//choose AD04 
     else SET_REG(LPC_ADC->ADCR,1<<5,0,8);				  				//choose AD05

     }*/

if(adc_ch==24)SET_REG(LPC_ADC->ADCR,1<<5,0,8);
else if(adc_ch==25)SET_REG(LPC_ADC->ADCR,1<<4,0,8);
else if((adc_ch%3)==0)SET_REG(LPC_ADC->ADCR,1<<0,0,8);
else if((adc_ch%3)==1)SET_REG(LPC_ADC->ADCR,1<<1,0,8);
else if((adc_ch%3)==2)SET_REG(LPC_ADC->ADCR,1<<2,0,8);




SET_REG(LPC_ADC->ADCR,1,21,1);	 //ADC on
SET_REG(LPC_ADC->ADCR,0,16,1);	 //software controlled conversion
SET_REG(LPC_ADC->ADCR,4,8,8);	 //clkdiv

SET_REG(LPC_ADC->ADCR,1,24,3);	 //ADC start

}



//-----------------------------------------------
char ke_start(void)
{
if((St&0x02)||(!NUMBAT))
	{
	return 1;
	}     
else if(St&0x01)
	{
	return 2;
	}	 
else if(Ibat<-(IKB*10))
	{
	return 3;
	}	  
else if(Ibat>(IKB*10))
	{
	return 4;
	}		
else 
	{
	spc_stat=spc_KE/*p1*/;
	//zar_cnt_ee_ke=0;
	//zar_cnt=0L;
	zar_cnt_ke=0;
	lc640_write_int(EE_ZAR_CNT_KE,0);


		{					
		signed temp_temp;
		char temp;
		temp_temp=ubat_old[((ubat_old_cnt+1)&0x07)]; 
		    
		temp=LPC_RTC->YEAR;
		gran_char(&temp,1,99);
		*((char*)(&(ke_date[0])))=temp;
			
		temp=LPC_RTC->MONTH;
		gran_char(&temp,1,12);
		*(((char*)(&(ke_date[0])))+1)=temp;
		
		temp=LPC_RTC->DOM;
		gran_char(&temp,1,31);
		*(((char*)(&(ke_date[0])))+2)=temp;			
				
		*(((char*)(&(ke_date[0])))+3)=*((char*)&temp_temp);
		temp=LPC_RTC->HOUR;
		gran_char(&temp,0,23);
		*((char*)(&(ke_date[1])))=temp;
               
		temp=LPC_RTC->MIN;
		gran_char(&temp,0,59);
		*(((char*)(&(ke_date[1])))+1)=temp;
	          
		temp=LPC_RTC->SEC;
		gran_char(&temp,0,59);
		*(((char*)(&(ke_date[1])))+2)=temp;
			
		*(((char*)(&(ke_date[1])))+3)=*(((char*)&temp_temp)+1);
				
		}
	return 0;
	}
}

//-----------------------------------------------
void next_date_hndl(char period)
{
unsigned short tempUS;
if((LPC_RTC->MONTH+period)>12)tempUS=LPC_RTC->YEAR+1;
else tempUS=LPC_RTC->YEAR;
lc640_write_int(EE_YEAR_AVZ,tempUS);

tempUS=LPC_RTC->MONTH+period;
if(tempUS>12)tempUS-=12;                                                  
lc640_write_int(EE_MONTH_AVZ,tempUS);

if(LPC_RTC->DOM>28)tempUS=28;
else tempUS=LPC_RTC->DOM;
lc640_write_int(EE_DATE_AVZ,tempUS);

tempUS=LPC_RTC->HOUR;
lc640_write_int(EE_HOUR_AVZ,tempUS);

tempUS=LPC_RTC->MIN;
lc640_write_int(EE_MIN_AVZ,tempUS);

tempUS=LPC_RTC->SEC;
lc640_write_int(EE_SEC_AVZ,tempUS);
}

//-----------------------------------------------
void def_set(int umax__,int ub0__,int ub20__,int usign__,int imax__,int uob__,int numi)
{
//NUMBAT=1;
lc640_write_int(EE_NUMIST,numi);
lc640_write_int(EE_MAIN_BPS,0);
lc640_write_int(EE_UMAX,umax__);
lc640_write_int(EE_UB0,ub0__);
lc640_write_int(EE_UB20,ub20__);
lc640_write_int(EE_TMAX,60);
//lc640_write_int(EE_C_BAT,180);
lc640_write_int(EE_USIGN,usign__);
lc640_write_int(EE_UMN,187);
lc640_write_int(EE_ZV_ON,0xff);
lc640_write_int(EE_IKB,10);
lc640_write_int(EE_KVZ,1030);
lc640_write_int(EE_IMAX,imax__);
lc640_write_int(EE_KIMAX,8);
lc640_write_int(EE_APV_ON,0xff);
lc640_write_int(EE_IZMAX,20);
lc640_write_int(EE_U0B,uob__);
lc640_write_int(EE_TZAS,3);

lc640_write_int(EE_MNEMO_ON,mnON);
lc640_write_int(EE_MNEMO_TIME,30);	

lc640_write_int(EE_AV_OFF_AVT,1);
lc640_write_int(EE_APV_ON1,apvOFF);
lc640_write_int(EE_NUMDT,0);
lc640_write_int(EE_NUMSK,0);
}


//-----------------------------------------------
char ubat_old_drv(void)
{        
ubat_old_cnt++;
gran_ring(&ubat_old_cnt,0,8);

ubat_old[ubat_old_cnt]=Ubat;
}

//-----------------------------------------------
void avz_hndl(void)
{
if(LPC_RTC->DOM!=/*lc640_read_int(EE_*/DATE_AVZ/*)*/)goto avz_lbl_001;
if(LPC_RTC->MONTH!=/*lc640_read_int(EE_*/MONTH_AVZ/*)*/)goto avz_lbl_001;
if(LPC_RTC->YEAR!=/*lc640_read_int(EE_*/YEAR_AVZ/*)*/)goto avz_lbl_001;
if(LPC_RTC->HOUR!=/*lc640_read_int(EE_*/HOUR_AVZ/*)*/)goto avz_lbl_001;
if(LPC_RTC->MIN!=/*lc640_read_int(EE_*/MIN_AVZ/*)*/)goto avz_lbl_001;
if(LPC_RTC->SEC!=/*lc640_read_int(EE_*/SEC_AVZ/*)*/)goto avz_lbl_001;

bAVZ=1;
avz_lbl_000:
avz_lbl_001:

if((bAVZ)&&(!(St&0x03))&&(spc_stat==spc_OFF))
	{
	spc_stat=spc_VZ;
	cnt_vz_sec=0L;
	cnt_vz_sec_=(SEC_IN_HOUR*AVZ_TIME);
	bAVZ=0;
	}
}

//-----------------------------------------------
void mnemo_hndl(void)
{
if((ind==iMn)&&(sub_ind==0)&&(MNEMO_ON==mnON))
	{
	if(mnemo_cnt)mnemo_cnt--;
	}
else mnemo_cnt=MNEMO_TIME;
}
