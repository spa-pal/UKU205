//ветка главная

                  
#include "LPC17xx.H"                    /* LPC17xx definitions                */
#include "simbols.h"
#include "graphic.h"
#include "gran.h" 
#include "main.h" 
#include "control.h"
#include "common_func.h"
#include "eeprom_map.h"
#include "avar.h"
#include "25lc640.h" 
#include "beep.h"
#include "ret.h"
#include "memo.h"
#include "uart0.h"
#include "uart2.h"
#include "full_can.h"
#include "snmp_data_file.h"
#include "net_config.h"

extern U8 own_hw_adr[];
extern U8  snmp_Community[];

BOOL tick;
extern LOCALM localm[];
#define MY_IP localm[NETIF_ETH].IpAdr
#define DHCP_TOUT   50

//-----------------------------------------------
//Таймер
char b1000Hz,b100Hz,b50Hz,b10Hz,b5Hz,b2Hz,b1Hz,b33Hz;
short t0cnt,t0cnt0,t0cnt1,t0cnt2,t0cnt3,t0cnt4,t0cnt5,t0cnt6,t0_cnt7,t0_cnt8;
char bFL5,bFL2,bFL,bFL_;



//-----------------------------------------------
//АЦП
//int adc_buff[10][16],adc_buff1[10][16],adc_buff2[10][16];
//int adc_buff_[10],adc_buff1_[10],adc_buff2_[10],unet_buff[18],unet_buff_,adc_chk_buff[3];
char adc_cnt,adc_cnt1,adc_ch,adc_check;
signed short adc_self_ch_buff[3],adc_self_ch_disp[3];
signed short adc_buff[26][16];
signed short adc_buff_[26];
char adc_self_ch_cnt;
signed short adc_result;
//-----------------------------------------------
//Состояние сети
short Hz_cnt,fnet,Hz_out;
char bFF,bFF_;
signed short Unet;
char unet_drv_cnt;
signed short unet_store;

//-----------------------------------------------
//Индикация
stuct_ind a,b[10];
char dig[5];
char zero_on;
char lcd_buffer[LCD_SIZE];
char lcd_bitmap[1024];
const char ABCDEF[]={"0123456789ABCDEF"};
signed char ptr_ind=0;
char mnemo_cnt;
signed char parol[3];
char fl_simv_num,fl_simv_len;
char default_temp;
signed short av_j_si_max,av_j_si_max_;
char simax;
char phase;
const char sm_[]		={"                    "}; //
const char sm_exit[]	={" Выход              "}; //
const char smon[]	={"ВКЛ."}; 
const char smoff[]	={"ВЫК."};
const char sm_mont[13][4]={"упс","янв","фев","мар","апр","май","июн","июл","авг","сен","окт","ноя","дек"}; //
char content[63]; 
char ind_reset_cnt;
//-----------------------------------------------
//Коэффициенты, отображаемые из EEPROM
signed short Ktsrc[2];
signed short Kusrc[2];
signed short Kisrc[2];
signed short Ki0src[2];
signed short Kubat;
signed short Kubat_part[4]; 		//калибровка промежуточных напряжений батареи
signed short Kibat0;
signed short Kibat1;
signed short Ktbat;
signed short Kunet;
signed short Ktout[3];

signed short MAIN_BPS;
signed short UMAX;
signed short UB0;
signed short UB20;
signed short TMAX;
signed short AV_OFF_AVT;
signed short USIGN;
signed short UMN;
signed short ZV_ON;
signed short IKB;
signed short KVZ;
signed short IMAX;
signed short KIMAX;
signed short APV_ON;
signed short IZMAX;
signed short U0B;
signed short TZAS;
signed short VZ_HR;
signed short TBAT;
signed short UBM_AV;
signed short REL_AV[3];

signed short NUMBAT;
signed short NUMIST;
signed short NUMDT;
signed short NUMSK;
signed short BAT_C_REAL;
signed short BAT_C_NOM;
signed short BAT_RESURS;

//signed short ZAR_CNT,ZAR_CNT_KE;
//signed short BAT_RESURS;

signed short AUSW_MAIN;
signed long 	AUSW_MAIN_NUMBER;
signed short AUSW_DAY;
signed short AUSW_MONTH;
signed short AUSW_YEAR;
signed short AUSW_UKU;
signed short AUSW_UKU_SUB;
signed long AUSW_UKU_NUMBER;
signed long	AUSW_BPS1_NUMBER;
signed long  AUSW_BPS2_NUMBER;
signed short AUSW_RS232;
signed short AUSW_PDH;
signed short AUSW_SDH;
signed short AUSW_ETH;

signed short TMAX_EXT_EN[3];
signed short TMAX_EXT[3];
signed short TMIN_EXT_EN[3];
signed short TMIN_EXT[3];
signed short T_EXT_REL_EN[3];
signed short T_EXT_ZVUK_EN[3];
signed short T_EXT_LCD_EN[3];
signed short T_EXT_SNMP_TRAP_EN[3];

signed short SK_SIGN[4];
signed short SK_REL_EN[4];
signed short SK_ZVUK_EN[4];
signed short SK_LCD_EN[4];
signed short SK_SNMP_TRAP_EN[4];

enum_AVZ AVZ;
unsigned short HOUR_AVZ;
unsigned short MIN_AVZ;
unsigned short SEC_AVZ;
unsigned short DATE_AVZ;
unsigned short MONTH_AVZ;
unsigned short YEAR_AVZ;
unsigned short AVZ_TIME;

signed short ETH_IS_ON;
signed short ETH_DHCP_ON;
signed short ETH_IP_1;
signed short ETH_IP_2;
signed short ETH_IP_3;
signed short ETH_IP_4;
signed short ETH_MASK_1;
signed short ETH_MASK_2;
signed short ETH_MASK_3;
signed short ETH_MASK_4;
signed short ETH_TRAP1_IP_1;
signed short ETH_TRAP1_IP_2;
signed short ETH_TRAP1_IP_3;
signed short ETH_TRAP1_IP_4;
signed short ETH_TRAP2_IP_1;
signed short ETH_TRAP2_IP_2;
signed short ETH_TRAP2_IP_3;
signed short ETH_TRAP2_IP_4;
signed short ETH_TRAP3_IP_1;
signed short ETH_TRAP3_IP_2;
signed short ETH_TRAP3_IP_3;
signed short ETH_TRAP3_IP_4;
signed short ETH_TRAP4_IP_1;
signed short ETH_TRAP4_IP_2;
signed short ETH_TRAP4_IP_3;
signed short ETH_TRAP4_IP_4;
signed short ETH_TRAP5_IP_1;
signed short ETH_TRAP5_IP_2;
signed short ETH_TRAP5_IP_3;
signed short ETH_TRAP5_IP_4;

signed short ETH_SNMP_PORT_READ;
signed short ETH_SNMP_PORT_WRITE;

signed short ETH_GW_1;
signed short ETH_GW_2;
signed short ETH_GW_3;
signed short ETH_GW_4;

//char snmp_community[10];

signed short dumm[8];

enum_BAT_IS_ON BAT_IS_ON;
enum_apv_on apv_on1,apv_on2;

signed short BAT_DAY_OF_ON,BAT_MONTH_OF_ON,BAT_YEAR_OF_ON;
signed short apv_on2_time;

enum_MNEMO_ON MNEMO_ON;
unsigned short MNEMO_TIME;

//-----------------------------------------------
//Состояние системы
char St;
short main_cnt;

//-----------------------------------------------
//Состояние батареи
signed short t_b,Ubat,Ibat;
signed short Ubat_part[4];		//напряжения промежуточных точек батареи относительно земли
signed short Ubat_e[5];		    	//напряжения банок батареи
signed long zar_cnt,zar_cnt_ke;
signed char zar_percent;
char cnt_wrk;
enum_wrk wrk;
unsigned long wrk_date[2];
signed long ibat_integr_;
signed short ibat_integr;
signed short cnt_resurs;
char cnt_ibat,cnt_ubat;
char cnt_irazr;
char bUrazr;
char bUOFF,bIOFF;
char cnt_irazr;
char bIrazr; 
char bI;
char NDB;
signed short ubat_old[8];
signed short ubat_old_cnt;
char cnt_bat;					//счетчик задержки отключения контактора батареи

//-----------------------------------------------
//Состояние источников
char St_[2];
signed short t_i[2],Us[2],Is[2];
char ND[2];

//-----------------------------------------------
//Состояние нагрузки
signed short iload,Uload;

//-----------------------------------------------
//Состояние внешних датчиков
signed short tout[3];
char tout_max_cnt[3],tout_min_cnt[3];
enum_tout_stat tout_stat[3];
char in_stat_out_old[4];
enum_sk_stat sk_stat[4]={skOFF,skOFF};
char ND_out[3];
char in_stat_out[4];

//-----------------------------------------------
//Кнопки
char speed,l_but,n_but;
char but;                            
unsigned long but_n,but_s;
char but0_cnt;
char but1_cnt;
char but_onL_temp;


//-----------------------------------------------
//Сообщения
char MSG_IND2OUT_DIS_SRC1,MSG_IND2OUT_DIS_SRC1_OLD;
char MSG_IND2OUT_DIS_SRC2,MSG_IND2OUT_DIS_SRC2_OLD;
char MSG_IND2OUT_EN_SRC1,MSG_IND2OUT_EN_SRC1_OLD;
char MSG_IND2OUT_EN_SRC2,MSG_IND2OUT_EN_SRC2_OLD;
char MSG_IND2OUT_EN_RELSAM,MSG_IND2OUT_EN_RELSAM_OLD;
short MSG_IND2PWM_SRC1,MSG_IND2PWM_SRC2;
char MSG_IND2OUT_EN_BAT,MSG_IND2OUT_EN_BAT_OLD;
char MSG_IND2OUT_DIS_BAT,MSG_IND2OUT_DIS_BAT_OLD;

//-----------------------------------------------
//Тест
char tst_state_dumm[10];
enum_tst_state tst_state[10];
//char tst_state_dumm_[10];

//-----------------------------------------------
//Самокалибровка
unsigned short skb_cnt;

//-----------------------------------------------
//Время
unsigned short hour_cnt_5hz,hour_cnt_1hz;
char cnt_ind;

//-----------------------------------------------
//Отладка
unsigned short rotor_can[6];
unsigned short cnt_sec;
char plazma_suzun;
char rele_temp;
char uart_plazma;
char plazma_can,plazma_can1,plazma_can2,plazma_can3,plazma_can4;
short snmp_plazma;


//-----------------------------------------------
//Имя устройства

const char* DEVICE_NAME[20]={
                              "ИБЭП220/60-24A",
                              "ИБЭП220/60-12A",
                              "ИБЭП220/48-24A",
                              "ИБЭП220/48-12A",
                              "ИБЭП220/60-10A",
                              "ИБЭП220/60-5A ",
                              "ИБЭП220/48-10A",
                              "ИБЭП220/48-5A ",
                              "ИБЭП220/24-24A",
                              "ИБЭП220/24-12A",
                              "ИБЭП220/48-40A",
                              "ИБЭП220/60-40A"
                              "ИБЭП220/48-20A",
                              "ИБЭП220/60-20A",
                              "ИБЭП220/24-50A",
                              "ИБЭП220/24-25A",
                              "ИБЭП220/24-24A",
                              "ИБЭП220/24-12A"
                              };



volatile unsigned long SysTickCnt;

char bRESET_RESET=0;

//-----------------------------------------------
//Информация для передачи по SNMP
char str_of_device_name;


LOCALM const ip_config = {
  { 192,168,0,150 },             // IP address
  { 192,168,0,1 },               // Default Gateway
  { 255,255,255,0 },             // Net mask
  { 194,25,2,129 },              // Primary DNS server
  { 194,25,2,130 }               // Secondary DNS server
};



//-----------------------------------------------
void init_ETH(void)
{
localm[NETIF_ETH].IpAdr[0]=lc640_read_int(EE_ETH_IP_1);
localm[NETIF_ETH].IpAdr[1]=lc640_read_int(EE_ETH_IP_2);
localm[NETIF_ETH].IpAdr[2]=lc640_read_int(EE_ETH_IP_3);
localm[NETIF_ETH].IpAdr[3]=lc640_read_int(EE_ETH_IP_4);

localm[NETIF_ETH].NetMask[0]=lc640_read_int(EE_ETH_MASK_1);
localm[NETIF_ETH].NetMask[1]=lc640_read_int(EE_ETH_MASK_2);
localm[NETIF_ETH].NetMask[2]=lc640_read_int(EE_ETH_MASK_3);
localm[NETIF_ETH].NetMask[3]=lc640_read_int(EE_ETH_MASK_4);

localm[NETIF_ETH].DefGW[0]=lc640_read_int(EE_ETH_GW_1);
localm[NETIF_ETH].DefGW[1]=lc640_read_int(EE_ETH_GW_2);
localm[NETIF_ETH].DefGW[2]=lc640_read_int(EE_ETH_GW_3);
localm[NETIF_ETH].DefGW[3]=lc640_read_int(EE_ETH_GW_4);

}

//-----------------------------------------------
static void timer_poll () 
{
if (SysTick->CTRL & 0x10000) 
     {
     timer_tick ();
     tick = __TRUE;
     }
}

//-----------------------------------------------
signed short abs_pal(signed short in)
{
if(in<0)return -in;
else return in;
}


//-----------------------------------------------
void Delay (unsigned long tick) 
{       
unsigned long systickcnt;

systickcnt = SysTickCnt;
while ((SysTickCnt - systickcnt) < tick);
}

//-----------------------------------------------
void rtc_init (void) 
{
LPC_RTC->CCR=0x11;
}
//-----------------------------------------------
void pwm_init(void)
{
SET_REG(LPC_PINCON->PINSEL7,3,(25-16)*2,2);
SET_REG(LPC_PINCON->PINSEL7,3,(26-16)*2,2);
LPC_PWM1->PR=0;
LPC_PWM1->PCR=0x00000c00;
LPC_PWM1->MCR=2;
LPC_PWM1->MR0=1000;
LPC_PWM1->MR2=800;
LPC_PWM1->MR3=800;
LPC_PWM1->LER=1;
LPC_PWM1->TCR=2;
LPC_PWM1->TCR=9;
}

//-----------------------------------------------
void bitmap_hndl(void)
{
short x,ii,i;
unsigned int ptr_bitmap;
static char ptr_cnt,ptr_cnt1,ptr_cnt2,ptr_cnt3,ptr_cnt4;

for(ii=0;ii<488;ii++)
	{
	lcd_bitmap[ii]=0x00;
	}

if(!mnemo_cnt)
	{
	if(St&0x01)
		{
		if(bFL2)
			{
			graphic_print(3,3,50,24,50,3,sAVNET,0);
			graphic_print(3,3,50,24,50,3,sAVNET1,0);
			}
		}
	else
		{

		if(NUMIST>=1)
			{
			draw_rectangle(0,0,20,20,0,0);
			draw_rectangle(1,1,18,18,0,0);
			if(!(St_[0]&0x3c))
				{
				graphic_print(3,2,15,15,15,2,sBPS1,0);
				}
			else if(St_[0]&(1<<4))
				{
				if(bFL2)graphic_print(3,2,15,15,15,2,sAVT,0);
				}
			else if((St_[0]&(1<<3))||(St_[0]&(1<<2)))
				{
				if(bFL2)graphic_print(2,2,15,15,15,2,sAVU,0);
				}	
			
			if((!OFFBP1))
				{
				draw(9,20,0,11,0);
				draw(9,31,91,0,0);
				draw_ptr(9,19+ptr_cnt1,0,4);
				}				
			}
		if(NUMIST>=2)
			{
			draw_rectangle(23,0,20,20,0,0);
			draw_rectangle(24,1,18,18,0,0);
			if(!(St_[1]&0x3c))
				{
				graphic_print(25,2,15,15,15,2,sBPS2,0);
				}
			else if(St_[1]&(1<<4))
				{
				if(bFL2)graphic_print(25,2,15,15,15,2,sAVT,0);
				}
			else if((St_[1]&(1<<3))||(St_[1]&(1<<2)))
				{
				if(bFL2)graphic_print(25,2,15,15,15,2,sAVU,0);
				}	
			
			if((!OFFBP2))
				{
				draw(32,20,0,11,0);
				draw(32,31,68,0,0);
				draw_ptr(32,19+ptr_cnt1,0,4);
				}				
			}			
		}
	if(NUMBAT)
		{
		draw_rectangle(50,0,35,20,0,0);
		draw_rectangle(53,20,3,2,0,0);
		draw_rectangle(79,20,3,2,0,0);
		if(St&0x02)
			{
			if(bFL2)graphic_print(43,0,50,24,50,3,sAVNET1,0);
			}
		else 
			{
			draw(66,20,0,11,0);
			draw(66,31,34,0,0);
			if(Ibat<0)draw_ptr(66,19+ptr_cnt1,0,4);
			else if(Ibat>=0)draw_ptr(66,34-ptr_cnt1,2,4);
			
			if(ptr_cnt4<30)
				{
			     if(BAT_C_REAL!=0x5555)
				     {
				     signed short u;
				     u=(((signed short)zar_percent/5));
				     gran(&u,0,20);
				     draw_rectangle(51,0,32,u,1,0);
				     //zar_percent=100;
				     if(zar_percent<10)
					     {
					     draw_rectangle(61,5,12,9,1,2);
					     graphic_print_text(61,5," %",2,zar_percent,0,1,1);
					     }
				     else if(zar_percent<100)
					     {
					     draw_rectangle(58,5,18,9,1,2);
					     graphic_print_text(58,5,"  %",3,zar_percent,0,2,1);
					     }		
				     else 
					     {
					     draw_rectangle(55,5,24,9,1,2);
					     graphic_print_text(55,5,"   %",4,zar_percent,0,3,1);
					     }									
				     //draw_rectangle(59,3,18,9,1,2);
				     //graphic_print_text(53,3,"   %",4,zar_percent,0,3,1);
				     }
                    else graphic_print_text(58,5,"   A",4,Ibat/10,1,3,1);

				}				
			else
				{
				graphic_print_text(58,5,"   A",4,Ibat/10,1,3,1);
				}
			}

		}	
		

	draw_rectangle(92,2,27,14,0,0);
	draw(92,8,-4,0,0);
	draw(118,8,4,0,0);
	draw(67,31,39,0,0);
	draw(105,31,0,-16,0);	
	draw_ptr(105,34-ptr_cnt3,2,4);
	
	graphic_print_text(70,22,"    B",5,Uload,1,4,1);
	if(iload<100)graphic_print_text(93,5,"   A",4,iload,1,3,1);
	else graphic_print_text(90,5,"   A",4,iload/10,0,3,1);
			
	ptr_cnt++;
	if(ptr_cnt>=3)
		{
		ptr_cnt=0;
		ptr_cnt1++;
		if(ptr_cnt1>=13)
			{
			ptr_cnt1=0;
			}
	
		ptr_cnt2++;
		if(ptr_cnt2>=32)
			{
			ptr_cnt2=0;
			}
				
		ptr_cnt3++;
		if(ptr_cnt3>=17)
			{
			ptr_cnt3=0;
			}

		ptr_cnt4++;
		if(ptr_cnt4>=40)
			{
			ptr_cnt4=0;
			}					
		}		
		
	}
else
	{
	for(i=0;i<4;i++)
		{
		ptr_bitmap=122*(unsigned)i;
		for(x=(20*i);x<((20*i)+20);x++)
	 		{
			lcd_bitmap[ptr_bitmap++]=caracter[(unsigned)lcd_buffer[x]*6];
			lcd_bitmap[ptr_bitmap++]=caracter[((unsigned)lcd_buffer[x]*6)+1];
			lcd_bitmap[ptr_bitmap++]=caracter[((unsigned)lcd_buffer[x]*6)+2];
			lcd_bitmap[ptr_bitmap++]=caracter[((unsigned)lcd_buffer[x]*6)+3];
			lcd_bitmap[ptr_bitmap++]=caracter[((unsigned)lcd_buffer[x]*6)+4];
			lcd_bitmap[ptr_bitmap++]=caracter[((unsigned)lcd_buffer[x]*6)+5];
			} 
		}
	}	
}

//-----------------------------------------------
void ind_hndl(void)
{
//const char* ptr;
const char* ptrs[40];
const char* sub_ptrs[30];
static char sub_cnt,sub_cnt1;
char i,sub_cnt_max;

sub_cnt_max=5;
i=0;

if(spc_stat==spc_VZ)
	{
	sub_ptrs[i++]=" Выравн.заряд  z:0Z ";
	sub_cnt_max++;
	}
else if((spc_stat==spc_KE))
	{
	sub_ptrs[i++]="  Контроль емкости  ";
	sub_cnt_max++;
	}	
if(St&0x01)
	{
	sub_ptrs[i++]="   Авария сети!!!   ";
	sub_cnt_max++;	
	}
if(St&0x02)
	{
	sub_ptrs[i++]=" Авария батареи!!!  ";
	sub_cnt_max++;	
	}
	
if(St&0x80)
	{
	sub_ptrs[i++]=" Асимметрия АКБ!!!  ";
	sub_cnt_max++;	
	}
			
if(St_[0]&0x1c)
	{
	sub_ptrs[i++]=" Авария источника N1";
	sub_cnt_max++;	
	}	

if(St_[1]&0x1c)
	{
	sub_ptrs[i++]=" Авария источника N2";
	sub_cnt_max++;	
	}	
		
if((tout_stat[0]==tMAX)&&(!T_EXT_LCD_EN[0]))
	{
	sub_ptrs[i++]="Внеш.темп.N1завышена";
	sub_cnt_max++;	
	}	
else if((tout_stat[0]==tMIN)&&(!T_EXT_LCD_EN[0]))
	{
	sub_ptrs[i++]="Внеш.темп.N1занижена";
	sub_cnt_max++;	
	}	
if((sk_stat[0]==skON)&&(!SK_LCD_EN[0]))
	{
	sub_ptrs[i++]="    Авария СК №1    ";
	sub_cnt_max++;	
	}

if((sk_stat[1]==skON)&&(!SK_LCD_EN[1]))
	{
	sub_ptrs[i++]="    Авария СК №2    ";
	sub_cnt_max++;	
	}

if((sk_stat[2]==skON)&&(!SK_LCD_EN[2]))
	{
	sub_ptrs[i++]="    Авария СК №3    ";
	sub_cnt_max++;	
	}

if((sk_stat[3]==skON)&&(!SK_LCD_EN[3]))
	{
	sub_ptrs[i++]="    Авария СК №4    ";
	sub_cnt_max++;	
	}

if(fuse_av_stat[0])
	{
	sub_ptrs[i++]=" Авария предохр. №1 ";
	sub_cnt_max++;	
	}	

if(fuse_av_stat[1])
	{
	sub_ptrs[i++]=" Авария предохр. №2 ";
	sub_cnt_max++;	
	}

if(fuse_av_stat[2])
	{
	sub_ptrs[i++]=" Авария предохр. №3 ";
	sub_cnt_max++;	
	}
	
sub_cnt1++;	
if(sub_cnt1>=20)
	{
	sub_cnt1=0;
	sub_cnt++;
	if(sub_cnt>=sub_cnt_max)
		{
		sub_cnt=0;
		}
	}



if(ind==iDeb)
     {
     
	if(sub_ind==0) 
     	{
     	bgnd_par("                    ",
     	         "                    ",
     	         "                    ",
     	         "                    ");

//		int2lcdyx(bat_ver_cnt,0,3,0);
//		int2lcdyx(cntrl_stat,1,3,0);
//		int2lcdyx(but_n,0,4,0);
// 		int2lcdyx(but_s,1,4,0);
// 		int2lcdyx(but,2,4,0);
 		int2lcdyx(bFF,0,2,0);
 		int2lcdyx(Hz_out,1,2,0);
		int2lcdyx(Hz_cnt,2,2,0);
//		int2lcdyx(Kusrc[0],1,4,0);
//		int2lcdyx(temp_SL,0,4,0);
//		int2lcdyx(Us[0],3,4,0);


		int2lcdyx(u_necc_up,0,7,0);
		int2lcdyx(u_necc,1,7,0);
		int2lcdyx(u_necc_dn,2,7,0);
		int2lcdyx(Ubat,3,7,0);

		int2lcdyx(sign_U,0,10,0);
		int2lcdyx(sign_I,1,10,0);
		int2lcdyx(superviser_cnt,2,10,0);	


		//int2lcdyx(zar_percent,0,19,0);
		//int2lcdyx(BAT_C_REAL,1,19,0);
		//int2lcdyx(BAT_C_NOM,2,19,0);

		int2lcdyx(rele_av_stat[0],0,19,0);
		int2lcdyx(rele_av_stat[1],1,19,0);
		int2lcdyx(rele_av_stat[2],2,19,0);

		int2lcdyx(REL_AV[0],0,17,0);
		int2lcdyx(REL_AV[1],1,17,0);
		int2lcdyx(REL_AV[2],2,17,0);

		//rele_av_stat[0]
		//int2lcdyx(lc640_read_int(EE_ZAR_CNT),3,19,0);

		  //bat_ver_cnt



	/*	int2lcdyx(tlv_buff[1][1],0,9,0);
		int2lcdyx(tlv_buff[1][2],0,14,0);
		int2lcdyx(tlv_buff[1][3],0,19,0);

   		int2lcdyx(tlv_buff[1][4],1,4,0);
		int2lcdyx(tlv_buff[1][5],1,9,0);
		int2lcdyx(tlv_buff[1][6],1,14,0);
		int2lcdyx(tlv_buff[1][7],1,19,0);

   		int2lcdyx(tlv_buff[1][8],2,4,0);
		int2lcdyx(tlv_buff[1][9],2,9,0);
		int2lcdyx(tlv_buff[1][10],2,14,0);
		int2lcdyx(tlv_buff[1][11],2,19,0);

   		int2lcdyx(tlv_buff[1][12],3,4,0);
		int2lcdyx(tlv_buff[1][13],3,9,0);
		int2lcdyx(tlv_buff[1][14],3,14,0);
		int2lcdyx(tlv_buff[1][15],3,19,0);	*/
     	} 

	else if(sub_ind==1) 
     	{

     	bgnd_par(	"2                   ",
     			"                   "
     			,sm_,sm_);
          int2lcdyx(cntrl_stat,0,4,0);
          int2lcdyx(num_necc,1,0,0);

          int2lcdyx(Us[0],0,14,1);
     	int2lcdyx(Us[1],1,14,1);
     	int2lcdyx(Is[0],0,19,1);
     	int2lcdyx(Is[1],1,19,1);
     	char2lcdhyx(St_[0],0,10);
     	char2lcdhyx(St_[1],1,10);
          int2lcdyx(NUMIST,2,1,0);
          /*int2lcdyx(bat_ver_cnt,3,3,0);
     	int2lcdyx(kb_cnt[0],3,7,0);
     	int2lcdyx(kb_cnt[1],3,10,0);
     	*/ 


      	}
    else if(sub_ind==2)
     	{
     	bgnd_par("                    ",
     		    "                    ",
     		    "                    ",
     		    "                    ");
     		    
     	int2lcdyx(adc_buff_[0],0,4,0);
     	int2lcdyx(adc_buff_[1],1,4,0);
     	int2lcdyx(adc_buff_[2],2,4,0);
     	int2lcdyx(adc_buff_[3],3,4,0);
     	int2lcdyx(adc_buff_[4],0,9,0);
     	int2lcdyx(adc_buff_[5],1,9,0);
     	int2lcdyx(adc_buff_[6],2,9,0);
     	int2lcdyx(adc_buff_[7],3,9,0);
    		int2lcdyx(adc_buff_[8],0,14,0);
     	int2lcdyx(adc_buff_[9],1,14,0);
     	int2lcdyx(adc_buff_[10],2,14,0);
     	int2lcdyx(adc_buff_[11],3,14,0);
     	int2lcdyx(adc_buff_[12],0,19,0);
     	int2lcdyx(adc_buff_[13],1,19,0);
     	int2lcdyx(adc_buff_[14],2,19,0);
     	int2lcdyx(adc_buff_[15],3,19,0);


		//int2lcdyx(adc_buff_[8],1,19,0);
//		int2lcdyx(adc_buff1_[8],2,19,0);
		
//		int2lcdyx(tlv_buff_[1],1,17,0);
//		int2lcdyx(unet_buff_,3,19,0); 
		
     	    
	    	}    

    else if(sub_ind==3)
     	{
     	bgnd_par("                    ",
     		    "                    ",
     		    "                    ",
     		    "                    ");
     		    
      	int2lcdyx(adc_buff_[16],0,4,0);
     	int2lcdyx(adc_buff_[17],1,4,0);
     	int2lcdyx(adc_buff_[18],2,4,0);
     	int2lcdyx(adc_buff_[19],3,4,0);
     	int2lcdyx(adc_buff_[20],0,9,0);
     	int2lcdyx(adc_buff_[21],1,9,0);
     	int2lcdyx(adc_buff_[22],2,9,0);
     	int2lcdyx(adc_buff_[23],3,9,0);
    		int2lcdyx(adc_buff_[24],0,14,0);
     	int2lcdyx(adc_buff_[25],1,14,0);
//     	int2lcdyx(adc_buff_[26],2,14,0);
//     	int2lcdyx(adc_buff_[27],3,14,0);
//     	int2lcdyx(adc_buff_[28],0,19,0);
//     	int2lcdyx(adc_buff_[29],1,19,0);
//     	int2lcdyx(adc_buff_[30],2,19,0);
//     	int2lcdyx(adc_buff_[31],3,19,0);
  

		
		
     	    
	    	}    
    else if(sub_ind==4)
     	{
     	bgnd_par("                    ",
     		    "                    ",
     		    "                    ",
     		    "                    ");
     		    
     	/*int2lcdyx(adc_buff[2][0],0,3,0);
     	int2lcdyx(adc_buff[2][1],1,3,0);
     	int2lcdyx(adc_buff[2][2],2,3,0);
     	int2lcdyx(adc_buff[2][3],3,3,0);
     	int2lcdyx(adc_buff[2][4],0,7,0);
     	int2lcdyx(adc_buff[2][5],1,7,0);
     	int2lcdyx(adc_buff[2][6],2,7,0);
     	int2lcdyx(adc_buff[2][7],3,7,0);
     	int2lcdyx(adc_buff[2][8],0,11,0);
     	int2lcdyx(adc_buff[2][9],1,11,0);
     	int2lcdyx(adc_buff[2][10],2,11,0);
     	int2lcdyx(adc_buff[2][11],3,11,0);
     	int2lcdyx(adc_buff[2][12],0,15,0);
     	int2lcdyx(adc_buff[2][13],1,15,0);
     	int2lcdyx(adc_buff[2][14],2,15,0);
     	int2lcdyx(adc_buff[2][15],3,15,0);*/

 /*    	int2lcdyx(unet_buff[0],0,3,0);
     	int2lcdyx(unet_buff[1],1,3,0);
     	int2lcdyx(unet_buff[2],2,3,0);
     	int2lcdyx(unet_buff[3],3,3,0);
     	int2lcdyx(unet_buff[4],0,7,0);
     	int2lcdyx(unet_buff[5],1,7,0);
     	int2lcdyx(unet_buff[6],2,7,0);
     	int2lcdyx(unet_buff[7],3,7,0);
     	int2lcdyx(unet_buff[8],0,11,0);
     	int2lcdyx(unet_buff[9],1,11,0);
     	int2lcdyx(unet_buff[10],2,11,0);
     	int2lcdyx(unet_buff[11],3,11,0);
     	int2lcdyx(unet_buff[12],0,15,0);
     	int2lcdyx(unet_buff[13],1,15,0);
     	int2lcdyx(unet_buff[14],2,15,0);
     	int2lcdyx(unet_buff[15],3,15,0);  */
		
     	int2lcdyx(adc_cnt1,0,19,0);
     	int2lcdyx(adc_cnt,1,19,0);
     	//int2lcdyx(unet_buff[14],2,14,0);
     	//int2lcdyx(unet_buff[15],3,14,0);		
     	    
	    	}    
     else if(sub_ind==1)
     	{
     	bgnd_par("Un    ( Ш1   !Ш2   @",
     	         "Uб    < U1   #U2   $",
     	         "Iб    > I1   %I2   ^",
     	         "St      St1   St2   ");
 //    	lcd_buffer[4]='a';            
 //    	int2lcd_mmm(Ibat,'a',1);   int2lcdyx(cntrl_stat,0,9,0);          int2lcdyx(hour_apv_cnt,0,13,0);                             char2lcdhyx(St_[0],0,19);  
 //    	int2lcdyx(Ubat,1,4,0);     int2lcdyx(main_apv_cnt,1,9,0);        int2lcdyx(lc640_read_int(SRC1_AVAR_PTR),1,13,0);            char2lcdhyx(St_[1],1,19);
 //    	int2lcdyx(Us[0],2,4,0);  int2lcdyx(apv_cnt_1,2,9,0);           int2lcdyx(lc640_read_int(SRC1_AVAR_CNT),2,13,0);                                     int2lcdhyx(av_stat,2,19);
 //    	int2lcdyx(Us[1],3,4,0);  int2lcdyx(reset_apv_cnt,3,9,0);                                            int2lcdyx(plazma,3,19,0);
     	int2lcd(u_necc,'(',1);
     	int2lcd(cntrl_stat1,'!',0);
     	int2lcd(cntrl_stat2,'@',0);
     	int2lcd(Us[0],'#',1);
     	int2lcd(Us[1],'$',1);
     	int2lcd(Is[0],'%',1);
     	int2lcd(Is[1],'^',1);
     	int2lcd(Ubat,'<',1);
     	int2lcd_mmm(Ibat,'>',2);
     	char2lcdhyx(St_[0],3,13);
     	char2lcdhyx(St_[1],3,19);
     	char2lcdhyx(St,3,5);
     	}
 
     else if(sub_ind==2)
     	{

     	
     	/*int2lcd_mmm((signed)(zar_cnt/10),'<',0);
     	int2lcd_mmm((signed)(zar_cnt_ke/10),'>',0);
     	int2lcdyx(lc640_read_int(EE_ZAR_CNT),0,16,0); 
     	int2lcdyx(lc640_read_int(EE_ZAR_CNT_KE),1,16,0);*/
		   
     	/*
     	int2lcdyx(ND[0],0,7,0);
     	int2lcdyx(cnt_av_t[0],1,7,0);
     	int2lcdyx(adc_buff_[6],0,15,0);
     	
     	int2lcdyx(t_i[1],2,3,0);
     	int2lcdyx(ND[1],2,7,0);
     	int2lcdyx(cnt_av_t[1],3,7,0);
     	int2lcdyx(adc_buff_[4],1,15,0);*/
     	
     	
     	//int2lcdyx(plazma_uart1,0,2,0);
     	/*int2lcdyx(rx_wr_index1,1,2,0);
     	int2lcdyx(rx_rd_index1,2,2,0);
     	int2lcdyx(rx_counter1,3,2,0);
//     	char2lcdbyx(U1LCR,0,19);
 //    	char2lcdbyx(U1IER,1,19);
 //    	char2lcdbyx(U1IIR,2,19);
 //    	char2lcdbyx(U1LSR,3,19);
     	int2lcdyx(adc_buff_out_[0],0,19,0);
     	int2lcdyx(adc_buff_out_[1],1,19,0);
     	int2lcdyx(adc_buff_out_[2],2,19,0);
     	int2lcdyx(in_stat_out[0],0,14,0);
     	int2lcdyx(in_stat_out[1],1,14,0);*/
     	//int2lcdyx(can_cnt,1,15,0);
     	//int2lcdyx(can_tx_cnt,2,15,0);
     	//long2lcdyx(C1ICR,0,8);
     	//int2lcdyx(bOUT_FREE,1,2,0);
     	/*int2lcdyx(ptr_can1_tx_wr,2,2,0);
     	int2lcdyx(ptr_can1_tx_rd,3,2,0);
     	long2lcdyx(can1_data[0],0,19);
     	long2lcdyx(can1_datb[0],0,11);
     	long2lcdyx(can1_data[1],1,19);
     	long2lcdyx(can1_datb[1],1,11);
     	long2lcdyx(can1_data[2],2,19);
     	long2lcdyx(can1_datb[2],2,11);
     	long2lcdyx(can1_data[3],3,19);
     	long2lcdyx(can1_datb[3],3,11);
     	char2lcdhyx(bd[0],0,3);*/
     	}

	else if(sub_ind==2)
     	{
     	bgnd_par("pl    ( Ш1   !Ш2   @",
     	         "Uб    < U1   #U2   $",
     	         "Iб    > I1   %I2   ^",
     	         "St      St1   St2   ");
 //    	lcd_buffer[4]='a';            
 //    	int2lcd_mmm(Ibat,'a',1);   int2lcdyx(cntrl_stat,0,9,0);          int2lcdyx(hour_apv_cnt,0,13,0);                             char2lcdhyx(St_[0],0,19);  
 //    	int2lcdyx(Ubat,1,4,0);     int2lcdyx(main_apv_cnt,1,9,0);        int2lcdyx(lc640_read_int(SRC1_AVAR_PTR),1,13,0);            char2lcdhyx(St_[1],1,19);
 //    	int2lcdyx(Us[0],2,4,0);  int2lcdyx(apv_cnt_1,2,9,0);           int2lcdyx(lc640_read_int(SRC1_AVAR_CNT),2,13,0);                                     int2lcdhyx(av_stat,2,19);
 //    	int2lcdyx(Us[1],3,4,0);  int2lcdyx(reset_apv_cnt,3,9,0);                                            int2lcdyx(plazma,3,19,0);
     	//int2lcd(plazma,'(',0);
     	int2lcd(cntrl_stat1,'!',0);
     	int2lcd(cntrl_stat2,'@',0);
     	int2lcd(Us[0],'#',1);
     	int2lcd(Us[1],'$',1);
     	int2lcd(Is[0],'%',1);
     	int2lcd(Is[1],'^',1);
     	int2lcd(Ubat,'<',1);
     	int2lcd_mmm(Ibat,'>',2);
     	char2lcdhyx(St_[0],3,13);
     	char2lcdhyx(St_[1],3,19);
     	char2lcdhyx(St,3,5);
		}
     	
     else if(sub_ind==3)
     	{
     	bgnd_par(sm_,sm_,sm_,sm_);
          
  /*        char2lcdhyx(RXBUFF[0],0,3);
          char2lcdhyx(RXBUFF[1],0,6);
          char2lcdhyx(RXBUFF[2],0,9);
          char2lcdhyx(RXBUFF[3],0,12);
          char2lcdhyx(RXBUFF[4],0,15);
          char2lcdhyx(RXBUFF[5],0,18);

          char2lcdhyx(RXBUFF[6],1,3);
          char2lcdhyx(RXBUFF[7],1,6);
          char2lcdhyx(RXBUFF[8],1,9);
          char2lcdhyx(RXBUFF[9],1,12);
          char2lcdhyx(RXBUFF[10],1,15);
          char2lcdhyx(RXBUFF[11],1,18);

          int2lcdyx(plazma_can3,3,3,0);*/

 		}
 
 	
     	
    else if(sub_ind==5)
     	{
     	bgnd_par("                    ",
     		    "                    ",
     		    "                    ",
     		    "                    ");
     	int2lcdyx(tout_max_cnt[0],0,1,0);	
     	int2lcdyx(tout_min_cnt[0],1,1,0); 
     	int2lcdyx(tout_stat[0],2,1,0);   
     	
     	int2lcdyx(TMAX_EXT[0],0,5,0);	
     	int2lcdyx(tout[0],1,5,0); 
     	int2lcdyx(TMIN_EXT[0],2,5,0);        	
     		
/*     		char tout_max_cnt[3],tout_min_cnt[3];
enum {tNORM,tMAX,tMIN}tout_stat[3];
char in_stat_out_old[4];
enum {skON,skOFF}sk_stat[3];*/
     		    
     /*	int2lcdyx(spc_stat,0,1,0);
     	int2lcdyx(lc640_read_int(EE_BAT_C_NOM),1,4,0);	
     	int2lcdyx(lc640_read_int(EE_BAT_C_REAL),2,4,0);
     	long2lcdyx(zar_cnt_ke,0,19); 
     	long2lcdyx(zar_cnt,1,19);
      	int2lcdyx(lc640_read_int(EE_ZAR_CNT_KE),0,10,0);	
     	int2lcdyx(lc640_read_int(EE_ZAR_CNT),1,10,0);
     	int2lcdyx(zar_percent,3,3,0);    
  		int2lcdyx(lc640_read_int(KE_PTR),3,6,0);
		int2lcdyx(lc640_read_int(KE_CNT),3,9,0);*/
     	}  
	else if(sub_ind==6)
     	{
     	ptrs[0]="    ?        $ # @ !";
     	ptrs[1]="               J   j";
     	ptrs[2]="               Y   y";
     	ptrs[3]="   h   m     Q q Z z";
     	ptrs[4]="   s   S       C   D";
     	ptrs[5]="   f   F       c   d";
     	ptrs[6]="             A a B b";
     	
     	
     	bgnd_par(ptrs[index_set],
     	         ptrs[index_set+1],
     	         ptrs[index_set+2],
     	         ptrs[index_set+3]);
     	
     	int2lcd_mmm(Ibat,'?',2); 
     	int2lcd(OFFBP1,'$',0);
     	char2lcdh(St_[0],'#');
     	int2lcd(OFFBP2,'@',0);
     	char2lcdh(St_[1],'!');
     	int2lcd(Us[0],'J',0);
     	int2lcd(Us[1],'j',0);
      	int2lcd(cnt_src[0],'Y',0);
     	int2lcd(cnt_src[1],'y',0); 
     	int2lcd(hour_apv_cnt[0],'h',0);
     	int2lcd(main_apv_cnt,'m',0); 
//     	int2lcd(lc640_read_int(SRC1_AVAR_PTR),'s',0); 
//     	int2lcd(lc640_read_int(SRC1_AVAR_CNT),'S',0);
//     	int2lcd(lc640_read((lc640_read_int(SRC1_AVAR_PTR)*4)+SRC1_AVAR_DAT),'e',0); 	
//     	int2lcd(lc640_read_int(SRC2_AVAR_PTR),'f',0); 
 //    	int2lcd(lc640_read_int(SRC2_AVAR_CNT),'F',0);     	
     	int2lcd(apv_cnt[0],'Q',0);
     	int2lcd(apv_cnt_sec[0],'q',0);
		int2lcd(apv_cnt[1],'Z',0);
     	int2lcd(apv_cnt_sec[1],'z',0);
     	int2lcd(cnt_av_umax[0],'A',0);
     	int2lcd(cnt_av_umin[0],'a',0);   
     	int2lcd(cnt_av_umax[1],'B',0);
     	int2lcd(cnt_av_umin[1],'b',0); 
     	int2lcd(hour_apv_cnt[0],'C',0); 
     	int2lcd(hour_apv_cnt[1],'D',0);
     	int2lcd(reset_apv_cnt[0],'c',0); 
     	int2lcd(reset_apv_cnt[1],'d',0);     	    	  	
 //    	lcd_buffer[4]='a';            
 //    	  int2lcdyx(cntrl_stat,0,9,0);          int2lcdyx(hour_apv_cnt,0,13,0);                             char2lcdhyx(St_[0],0,19);  
 //    	int2lcdyx(Ubat,1,4,0);     int2lcdyx(main_apv_cnt,1,9,0);                    char2lcdhyx(St_[1],1,19);
 //    	int2lcdyx(Us[0],2,4,0);  int2lcdyx(apv_cnt_1,2,9,0);                                                int2lcdhyx(av_stat,2,19);
 //    	int2lcdyx(Us[1],3,4,0);  int2lcdyx(reset_apv_cnt,3,9,0);                                            int2lcdyx(plazma,3,19,0);
 /*    	int2lcd(plazma,'(',0);
     	int2lcd(cntrl_stat1,'!',0);
     	int2lcd(cntrl_stat2,'@',0);
     	int2lcd(Us[0],'#',1);
     	int2lcd(Us[1],'$',1);
     	int2lcd(Is[0],'%',1);
     	int2lcd(Is[1],'^',1);
     	int2lcd(Ubat,'<',1);
     	int2lcd_mmm(Ibat,'>',2);
     	char2lcdhyx(St_[0],3,13);
     	char2lcdhyx(St_[1],3,19);
     	char2lcdhyx(St,3,5);*/
		}     	       	
		
	else if(sub_ind==7)
     	{
     	bgnd_par(sm_,sm_,sm_,sm_);
     	//long2lcdyx(C1IER,0,10);
  		//long2lcdyx(C1GSR,1,10);
  		int2lcdyx(LPC_RTC->MONTH,0,6,0);
  		int2lcdyx(AVZ,1,6,0);
  		int2lcdyx(lc640_read_int(EE_MONTH_AVZ),2,6,0);
  		int2lcdyx(LPC_RTC->YEAR,0,16,0);
  		
  		int2lcdyx(lc640_read_int(EE_YEAR_AVZ),2,16,0);
  		//long2lcdyx(C1RFS,0,19);
  		//long2lcdyx(C1RID,1,19);
  		//long2lcdyx(C1RDA,2,19);
  		//long2lcdyx(C1RDB,3,19);
  		//long2lcdyx(plazma_can_long,3,10);
		} 
		
	//int2lcdyx(sub_ind,3,1,0);	    	       			
     }

/*
const char sm7[]	={" Источник N2        "}; //
const char sm8[]	={" Нагрузка           "}; //
const char sm9[]	={" Сеть               "}; //
const char sm10[]	={" Спецфункции        "}; // 
const char sm11[]	={" Журнал аварий      "}; //
const char sm12[]	={" Батарейный журнал  "}; //
const char sm14[]	={" Паспорт            "}; //
*/
else if(ind==iMn)
	{
	ptrs[0]=		"  Неопределенность  ";
	if((Us[0]>200)&&(Us[1]>200))ptrs[0]=" В работе БПС1,БПС2 ";
	else if(Us[0]>200)ptrs[0]="    В работе БПС1   ";
	else if(Us[1]>200)ptrs[0]="    В работе БПС2   ";
	else if(Ubat>200)ptrs[0]="  Работа от батареи ";
	ptrs[1]=		" Uб=   lВ Iб=    @А ";
     ptrs[2]=		" Uн=   #В Iн=    $А ";
     ptrs[3]=		" 0%:0^:0& 0</>  /0{ ";
     ptrs[4]=		" Батарея            ";
     ptrs[5]=		" БПС1               "; 
	ptrs[6]=		" БПС2               ";
     ptrs[7]=		" Нагрузка           ";
     ptrs[8]=		" Сеть               ";
     ptrs[9]=		" Спецфункции        ";
     ptrs[10]=		" Журнал событий     ";
	ptrs[11]=		" Батарейный журнал  ";
     //ptrs[12]=		" Паспорт            ";
     ptrs[12]=		" Установки          ";
     ptrs[13]=		" Сброс аварий       ";
     ptrs[14]=sm_exit;
     
     if(sub_ind==0)index_set=0;
	else if((index_set-sub_ind)>2)index_set=sub_ind+2;
	else if(sub_ind>index_set)index_set=sub_ind;
	if(sub_cnt<5)bgnd_par(ptrs[0],ptrs[index_set+1],ptrs[index_set+2],ptrs[index_set+3]);
	else bgnd_par(sub_ptrs[sub_cnt-5],ptrs[index_set+1],ptrs[index_set+2],ptrs[index_set+3]);

	if((index_set)&&(sub_ind))
	     {
	     if(index_set==sub_ind)lcd_buffer[60]=1;
	     else if((index_set-sub_ind)==1)lcd_buffer[40]=1;
	     else if((index_set-sub_ind)==2)lcd_buffer[20]=1;
	     }	
	
 	int2lcd(Ubat,'l',1);
 	int2lcd_mmm(Ibat,'@',2);
 	int2lcd(Uload,'#',1);
 	int2lcd(iload,'$',1); 	
 	
 	int2lcd(LPC_RTC->SEC,'&',0);
 	int2lcd(LPC_RTC->MIN,'^',0);
 	int2lcd(LPC_RTC->HOUR,'%',0);
 	
 	int2lcd(LPC_RTC->DOM,'<',0);
 	sub_bgnd(sm_mont[LPC_RTC->MONTH],'>',0);
 	int2lcd(LPC_RTC->YEAR,'{',0);
 	if(bFL2)
 	     {
 	     lcd_buffer[find(':')]=' ';
 	     lcd_buffer[find(':')]=' ';
 	     lcd_buffer[find(':')]=' ';
 	     }
 	//int2lcdyx(sub_ind,0,19,0);
 	//int2lcdyx(index_set,0,15,0);
//    	int2lcd(a,'^',0);
  //   int2lcd(55,'#',0);
  //   int2lcd(897,'@',0);
     //int2lcdyx(plazma,0,19,0);
     //int2lcdyx((int)(CTIME0&0x0000003fUL),0,19,0);
     /*int2lcdyx(adc_ch,0,19,0);*/
     /*int2lcdyx(tlv[0],0,19,0);
     int2lcdyx(tlv_buff_[0],1,19,0);
     int2lcdyx(tlv[1],2,19,0);
     int2lcdyx(tlv_buff_[1],3,19,0);*/
    /* int2lcdyx(adc_cnt1,2,19,0);*/
    /* int2lcdyx(unet_buff_,1,14,0);
     
     int2lcdyx(adc_buff_[1],1,3,0);
     int2lcdyx(adc_buff_[2],2,3,0);
     int2lcdyx(adc_buff_[3],3,3,0);
     int2lcdyx(adc_buff_[4],0,8,0);
     int2lcdyx(adc_buff_[5],1,8,0);
     int2lcdyx(adc_buff_[6],2,8,0);
     int2lcdyx(adc_buff_[7],3,8,0);*/
     //int2lcdyx(PWM1,0,6,0);
     //int2lcd_m(-200,'#',2);
     
        //  int2lcdyx(phase2,1,14,0);
     /*long2lcdyx(S1SPSR,0,7); 

     long2lcdyx(temp_short[3],3,7); 
     long2lcdyx(temp_short[4],0,16); 
     long2lcdyx(temp_short[5],1,16);
     long2lcdyx(temp_short[6],2,16);
     long2lcdyx(temp_short[7],3,16);*/ 
     
     //long2lcdyx(IO0DIR,0,7);
     //int2lcdyx(adc_ch,0,5,0);   
	//int2lcdyx(tlv_buff_[1],0,19,0); 
	//int2lcdyx((char)(but),0,5,0);

     //int2lcdyx(RTC_SEC,0,19,0);
   /*   long2lcdyx(lc640_read_long(28),2,8);
     int2lcdyx((CTIME0_bit.SEC),3,19,0); 
     char2lcdbyx(0xbe,0,19);*/ 
     
     //char2lcdbyx(I2STAT,0,7); 
     //char2lcdbyx(plazma_char[1],1,7);
     //char2lcdbyx(plazma_char[2],2,7);
     //char2lcdbyx(plazma_char[3],3,7);
     //char2lcdbyx(plazma_char[4],0,16);
     //char2lcdbyx(plazma_char[5],1,16);
     //char2lcdbyx(plazma_char[6],2,16);
     //long2lcdyx(T0EMR,2,16);
     //long2lcdyx(T0TC,3,16);
     //int2lcdyx(U0DLL,0,19,0);
     //int2lcdyx(U0DLM,1,19,0);
/*int2lcdyx(sub_ind,0,0,0);
int2lcdyx(index_set,0,1,0);*/  
  
//     char2lcdhyx(pdh_state,1,2);  
//     char2lcdhyx(sdh_state,2,2);
//int2lcdyx(sub_cnt,0,3,0);
//int2lcdyx(in_stat_out[1],0,6,0);

	int2lcd(cnt_vz_sec/SEC_IN_HOUR,'z',0);
	int2lcd((cnt_vz_sec/60)%60,'Z',0);
	
	//int2lcdyx(tlv_buff_[0],0,4,0);
	//int2lcdyx(tlv_buff_[1],1,4,0);
	//int2lcdyx(hour_apv_cnt[0],0,4,0);
     
     //char2lcdhyx(rotor_rotor_rotor[0],0,2); 
     //char2lcdhyx(rotor_rotor_rotor[1],1,2); 
     
     //char2lcdhyx(uart1_mess[0],0,2); 
     //char2lcdhyx(uart1_mess[1],0,6); 
     
     /*
     int2lcdyx(adc_buff_out_[1],0,8,0);
     int2lcdyx(adc_buff_out_[2],0,13,0);
     int2lcdyx(in_stat_out[0],1,2,0);
    */
    /* int2lcdyx(plazma,0,2,0);
     int2lcdyx(usart1_router_stat,0,6,0);
     int2lcdyx(usart1_router_cnt,0,10,0);
     int2lcdyx(usart1_router_wrk,0,14,0);*/
     //int2lcdyx(plazma,0,3,0);
    /* int2lcdyx(adc_buff_out_[0],1,3,0);*/
    /* int2lcdyx(suzz[0],0,2,0);
     int2lcdyx(suzz[1],0,6,0);
  	int2lcdyx(suzz[2],0,10,0);
      */
    // int2lcdyx(suzz[4],0,18,0);   
     //int2lcdyx(lc640_read_int(RESET_CNT),0,14,0);

/*	int2lcdyx(bat_ver_cnt,0,8,0);
	int2lcdyx(kb_cnt[0],0,14,0);
	*/
	//int2lcdyx(cntrl_stat,0,18,0);
/*	St_[1]|=0x20;
	St&=0xfb; 
	cnt_src[1]=10;*/

     //int2lcdyx(av_rele,0,4,0);
     //int2lcdyx(av_beep,0,10,0);
	//int2lcdyx(tst_state[0],0,14,0);
     //int2lcdyx(St_[0],0,12,0);
	//int2lcdyx(St_[1],0,18,0);
    	} 
     

else if (ind==iBat)
	{
	if(!NUMBAT)
		{
		bgnd_par(" БАТАРЕЯ НЕ ВВЕДЕНА ",
		         " При  необходимости ",
		         "     введите её     ",
		         "в батарейном журнале");
		} 	
	else if(St&0x02)
		{
		if(bFL2)bgnd_par("       АВАРИЯ!!       ",
		                 "       Батарея        ",
		                 "    не подключена     ",sm_);
		else bgnd_par(sm_,sm_,sm_,sm_);
		}               
	else
		{
		ptrs[0]="      БАТАРЕЯ       ";
		if(!bit_minus)
		     {
		     ptrs[1]="    заряжается      ";
		     ptrs[3]="   Iзар=     !А     ";
		     }
		else
		     {
		     ptrs[1]="   разряжается      ";
		     ptrs[3]="   Iразр=    !А     ";
		     }	
		ptrs[2]=		"   Uбат=     @В     ";
		
		if(NDB)ptrs[4]="    ДТ. неисправен  ";
		else ptrs[4]=	"   tбат =   $°C     ";
		ptrs[5]=		"   Заряд=    w%     ";
		ptrs[6]=		"   Cбат=     QА*ч   ";
		ptrs[7]=		"   Uэ1 =     [В     ";
		ptrs[8]=		"   Uэ2 =     ]В     ";
		ptrs[9]=		"   Uэ3 =     {В     ";
		ptrs[10]=		"   Uэ4 =     }В     ";
		ptrs[11]=		"   Uэ5 =     )В     ";
		if((AUSW_MAIN/100)==48)ptrs[11]=sm_exit;
		else ptrs[12]=sm_exit;
 
		bgnd_par("      БАТАРЕЯ       ",
		          ptrs[sub_ind+1],ptrs[sub_ind+2],ptrs[sub_ind+3]);
	     
	     
	     int2lcd(Ubat,'@',1);
	     int2lcd_mmm(Ibat,'!',2);
	     int2lcd(t_b,'$',0);
	     int2lcd(zar_percent,'w',0);
	     if(BAT_C_REAL==0x5555)sub_bgnd("------",'Q',-1);
	     else int2lcd(BAT_C_REAL,'Q',1);
		if(((AUSW_MAIN/100)==48)&&(sub_ind==8))lcd_buffer[60]=1;
	     else if(((AUSW_MAIN/100)==60)&&(sub_ind==9))lcd_buffer[60]=1;
		int2lcd(Ubat_e[0],'[',1);
		int2lcd(Ubat_e[1],']',1);
		int2lcd(Ubat_e[2],'{',1);
		int2lcd(Ubat_e[3],'}',1);
		int2lcd(Ubat_e[4],')',1);
		}
		
	} 

else if(ind==iSrc)
	{
	char src_off;
	const char* ptr;
	if(sub_ind1==0)src_off=OFFBP1;
	else src_off=OFFBP2;
	
	if((sub_ind1==1)&&(NUMIST==1))
		{
		ptr=    "        БПС2        ";
		ptrs[0]="    ОТСУТСТВУЕТ     ";
		ptrs[1]="  В СТРУКТУРЕ ИБЭП  ";
		ptrs[2]=sm_;
		}
	else
		{
		if(src_off) ptr=	"    БПС! ВЫКЛЮЧЕН   ";
		else ptr=			"    БПС! ВКЛЮЧЕН    ";
	
		if(St&0x01)ptrs[0]=" авария сети        ";
		else if(St_[sub_ind1]&(1<<4))ptrs[0]=" авария перегрев    ";
		else if(St_[sub_ind1]&(1<<3))ptrs[0]=" авария Uвых<Uмин   ";
		else if(St_[sub_ind1]&(1<<2))ptrs[0]=" авария Uвых>Uмакс  ";
		else if(!(St_[sub_ind1]&0x3c))ptrs[0]=" состояние - норма  "; 
		
		ptrs[1]=" Uист =    @В       "; 
		ptrs[2]=" Iист =    #А       ";
		if(ND[sub_ind1])ptrs[3]=" ДТ неисправен      ";
		else ptrs[3]=" tист =   $°C       ";
	
		ptrs[4]=sm_exit;
		}
		
	if((sub_ind-index_set)>0)index_set=sub_ind;
	else if(sub_ind<index_set)index_set=sub_ind;
	bgnd_par(ptr,ptrs[index_set],ptrs[index_set+1],ptrs[index_set+2]);	
	
	int2lcd(sub_ind1+1,'!',0);
	int2lcd(Us[sub_ind1],'@',1);
	int2lcd(Is[sub_ind1],'#',1);
	int2lcd(t_i[sub_ind1],'$',0); 
	if(sub_ind==2)lcd_buffer[60]=1;
	//char2lcdbyx(St_[sub_ind1],0,7);

     //int2lcdyx(adc_buff_[2],0,19,0);
     //int2lcdyx(adc_buff_[0],1,19,0);
     //int2lcdyx(adc_buff_[6],2,19,0);
     //int2lcdyx(adc_buff_[7],3,19,0);
	}
	
else if(ind==iLoad)
	{
     bgnd_par("      НАГРУЗКА      "
             ," Uнагр=       !В    "
             ," Iнагр=       @A    "
             ," Выход              ");	
	lcd_buffer[60]=1;
     int2lcd(Uload,'!',1);
     int2lcd(iload,'@',1);
       
	}
		
else if(ind==iNet)
	{
	if(St&0x01)
	     {
	     if(bFL2)ptrs[0]="   АВАРИЯ СЕТИ!!!  ";
	     else ptrs[0]=sm_;
	     }
	else ptrs[0]="       СЕТЬ         ";
//#if(UKU_VERSION==900)
     bgnd_par(ptrs[0]
             ," Uсети=       #В    "
             ," Fсети=       @Гц   "
             ," Выход              ");	
	lcd_buffer[60]=1;
     int2lcd(Unet,'#',0);
     int2lcd(fnet,'@',1);
//#else
//     bgnd_par(ptrs[0]
//             ," Uсети=       #В    "
//             ," Выход              "
//             ,"                    ");	
//	lcd_buffer[40]=1;
//     int2lcd(Unet,'#',0);
//#endif          
	}

else if(ind==iSpc)
	{
	ptrs[0]=" Контроль емкости   ";
	ptrs[1]=" Выравнивающий заряд";
	ptrs[2]=" Авт.выравнив. заряд";
	ptrs[3]=sm_exit;
	
     if((sub_ind-index_set)>2)index_set=sub_ind-2;
	else if(sub_ind<index_set)index_set=sub_ind;
		
     bgnd_par("    СПЕЦФУНКЦИИ     ",ptrs[index_set],ptrs[index_set+1],ptrs[index_set+2]);	
	pointer_set(1);
	
	}

else if(ind==iKe)
	{
	if((spc_stat==spc_KE/*p1)||(spc_stat==spc_KEp2*/))ptrs[0]=" Включен            ";
	else ptrs[0]=" Выключен           ";
	bgnd_par("  КОНТРОЛЬ ЕМКОСТИ  ",
	         ptrs[0],
	         sm_exit,
	         sm_);	
	pointer_set(1);
	} 
else if (ind==iKednd)
	{
	ptrs[0]="  Контроль емкости  ";
	ptrs[1]="   невозможен!!!    ";
	if(sub_ind==0)
		{
		ptrs[0]="    Отсутствует     ";
		ptrs[1]="      батарея       ";
		}  
	else if(sub_ind==1)
		{
		ptrs[0]="       Авария       ";
		ptrs[1]="        сети        ";
		}	
	else if(sub_ind==2)
		{
		ptrs[0]="      Батарея       ";
		ptrs[1]="    разряжается     ";
		}	
	else if(sub_ind==3)
		{
		ptrs[0]="      Батарея       ";
		ptrs[1]="     заряжается     ";
		}	
						
	bgnd_par("  Контроль емкости  ",
	         "   невозможен!!!    ",
	         ptrs[0],
	         ptrs[1]);	
	}	
else if(ind==iVz)
	{
	if(spc_stat==spc_VZ)ptrs[0]=" Включен            ";
	else ptrs[0]=" Выключен           ";
	bgnd_par(" ВЫРАВНИВАЮЩИЙ ЗАРЯД",
	         " длительность -  !ч.",
	         ptrs[0],
	         sm_exit);	
	pointer_set(1);
	int2lcd(VZ_HR,'!',0);
	} 

else if(ind==iAvz)
	{
		bgnd_par(" ВЫРАВНИВАЮЩИЙ ЗАРЯД",
		" ВЫРАВНИВАЮЩИЙ ЗАРЯД",
		" ВЫРАВНИВАЮЩИЙ ЗАРЯД",
		" ВЫРАВНИВАЮЩИЙ ЗАРЯД");
	simax=4;
	if(AVZ==AVZ_1) ptrs[0]=" раз в месяц        ";
	else if(AVZ==AVZ_2) ptrs[0]=" раз в 2 месяца     ";
	else if(AVZ==AVZ_3) ptrs[0]=" раз в 3 месяца     "; 
	else if(AVZ==AVZ_6) ptrs[0]=" раз в полгода      "; 
	else if(AVZ==AVZ_12) ptrs[0]=" раз в год          ";
	else
		{
		ptrs[0]=" выключен           ";
		simax=1;
		}
	if(simax==4)
		{
		ptrs[1]=" длительность   (ч. ";
		ptrs[2]=" очередное включение";
	 	ptrs[3]=" 0%:0^:0& 0</>  /0{ ";
	 	ptrs[4]=sm_exit;
	 	}
	else ptrs[1]=sm_exit;
	 
	bgnd_par("   АВТОМАТИЧЕСКИЙ   ",
						 "ВЫРАВНИВАЮЩИЙ ЗАРЯД ",
						 ptrs[index_set],ptrs[index_set+1]);	
						 
	
	if(sub_ind<index_set) index_set=sub_ind;
	else if((sub_ind-index_set)>1) index_set=sub_ind-1;
	if(sub_ind==2) index_set=2;
	
	pointer_set(2);
	
	int2lcd(HOUR_AVZ,'%',0);
	int2lcd(MIN_AVZ,'^',0);	
	int2lcd(SEC_AVZ,'&',0);
	int2lcd(DATE_AVZ,'<',0);
	
	
	sub_bgnd(sm_mont[MONTH_AVZ],'>',0);
	int2lcd(YEAR_AVZ,'{',0);
    
	int2lcd(AVZ_TIME,'(',0);
 	}
	
else if(ind==iLog)
	{
	av_j_si_max=lc640_read_int(CNT_EVENT_LOG);
	if(av_j_si_max>64)av_j_si_max=0;

	if(av_j_si_max==0)
		{
		bgnd_par("   ЖУРНАЛ СОБЫТИЙ   "," Журнал пуст        ",sm_exit,sm_);
		//lcd_buffer[33]=1;
		//sub_ind=0;
		index_set=0;
		}       
		
	else if(av_j_si_max==1)
		{
		bgnd_par("   ЖУРНАЛ СОБЫТИЙ   "," (                  ",sm_exit," Очистить журнал    ");
		//if(sub_ind==0)lcd_buffer[16]=1;
		//else if(sub_ind==1)lcd_buffer[33]=1;
		//else if(sub_ind==2)lcd_buffer[50]=1;		
		index_set=0;
		}

	else if(av_j_si_max==2)
		{
		if(sub_ind<index_set) index_set=sub_ind;
		else if((sub_ind-index_set)>2) index_set=sub_ind-2;		
		if(index_set==0) bgnd_par("   ЖУРНАЛ СОБЫТИЙ   "," (                  "," [                  ",sm_exit);
		else if(index_set==1) bgnd_par("   ЖУРНАЛ СОБЫТИЙ   "," (                  ",sm_exit," Очистить журнал    ");
		
		//if((sub_ind-index_set)==0) lcd_buffer[16]=1; 
		//else if((sub_ind-index_set)==1) lcd_buffer[33]=1;
		//else if((sub_ind-index_set)==2) lcd_buffer[50]=1;
		}
		
	else if(av_j_si_max>2)
		{
		if(sub_ind<index_set) index_set=sub_ind;
		else if((sub_ind-index_set)>2) index_set=sub_ind-2;  
		if(index_set==(av_j_si_max-1)) bgnd_par("   ЖУРНАЛ СОБЫТИЙ   "," (                  ",sm_exit," Очистить журнал    ");
		else if(index_set==(av_j_si_max-2)) bgnd_par("   ЖУРНАЛ СОБЫТИЙ   "," (                  "," [                  ",sm_exit);
		else bgnd_par("   ЖУРНАЛ СОБЫТИЙ   "," (                  "," [                  "," {                  ");
		
		//if((sub_ind-index_set)==0) lcd_buffer[16]=1; 
		//else if((sub_ind-index_set)==1) lcd_buffer[33]=1;
		//else if((sub_ind-index_set)==2) lcd_buffer[50]=1;

		}
	pointer_set(1);
     event2ind(index_set,'(');
     event2ind(index_set+1,'[');	
     event2ind(index_set+2,'{');	  
     
	}



else if(ind==iLog_)
	{	
	unsigned short tempUI;
//	unsigned long tempUL;
	char av_head[4],av_data_on[8],av_data_off[8],av_data[4];
	short av_head_int[2];
	
	bgnd_par(sm_,sm_,sm_,sm_);
	tempUI=lc640_read_int(PTR_EVENT_LOG);
	tempUI=ptr_carry(tempUI,64,-1*((signed)sub_ind1));
	tempUI*=32;
	tempUI+=EVENT_LOG;
     
     lc640_read_long_ptr(tempUI,av_head);
     lc640_read_long_ptr(tempUI+4,(char*)av_head_int);
     lc640_read_long_ptr(tempUI+8,av_data_on);
     lc640_read_long_ptr(tempUI+12,&(av_data_on[4])); 
     lc640_read_long_ptr(tempUI+16,av_data_off);
     lc640_read_long_ptr(tempUI+20,&(av_data_off[4]));      
	lc640_read_long_ptr(tempUI+24,av_data);  
	
	if((av_head[0]=='U')&&(av_head[2]=='R'))
		{
		bgnd_par(	"    Перезагрузка    ",
				"   или включение    ",
				"        ИБЭП        ",
				"  0%(  0^ 0@:0#:0$  ");
				
				  	
		int2lcd(av_data_on[4],'@',0);
		int2lcd(av_data_on[5],'#',0);
		int2lcd(av_data_on[6],'$',0);
		int2lcd(av_data_on[2],'%',0);
		int2lcd(av_data_on[0],'^',0); 
		if(!((av_data_on[1]>=1)&&(av_data_on[1]<=12)))av_data_on[1]=1;
		sub_bgnd(sm_mont[av_data_on[1]],'(',0);
		
		av_j_si_max=0;
		
		}

	else if((av_head[0]=='P')&&(av_head[2]=='A'))
		{  
		ptrs[0]="   Авария сети!!!   ";
		ptrs[1]="  0%(  0^ 0@:0#:0$  ";
		if((av_data_off[0]=='A')&&(av_data_off[1]=='A'))
			{
			ptrs[2]="    не устранена    ";
			ptrs[3]="     Uсети=  +В     ";
			bgnd_par(ptrs[0],ptrs[1],ptrs[2],ptrs[3]);
			int2lcd(Unet,'+',0);
			}
		else 
			{
			gran_char(&index_set,0,1);
			ptrs[2]="      устранена     ";
			ptrs[3]="  0[]  0< 0>:0=:0)  ";
			ptrs[4]="     Uмин=  +В      ";
			bgnd_par(ptrs[index_set],ptrs[1+index_set],ptrs[2+index_set],ptrs[3+index_set]);
			int2lcd(av_data_off[4],'>',0);
			int2lcd(av_data_off[5],'=',0);
			int2lcd(av_data_off[6],')',0);
			int2lcd(av_data_off[2],'[',0);
			int2lcd(av_data_off[0],'<',0); 
			if(!((av_data_off[1]>=1)&&(av_data_off[1]<=12)))av_data_off[1]=1;
			sub_bgnd(sm_mont[av_data_off[1]],']',0);
			
			int2lcd(av_data[0]+(av_data[1]*256),'+',0);			
			}	
		
		int2lcd(av_data_on[4],'@',0);
		int2lcd(av_data_on[5],'#',0);
		int2lcd(av_data_on[6],'$',0);
		int2lcd(av_data_on[2],'%',0);
		int2lcd(av_data_on[0],'^',0); 
		if(!((av_data_on[1]>=1)&&(av_data_on[1]<=12)))av_data_on[1]=1;
		sub_bgnd(sm_mont[av_data_on[1]],'(',0);
		
		av_j_si_max=1;
		}

	else if((av_head[0]=='B')&&(av_head[2]=='C'))
		{  
		ptrs[0]="       Авария       ";
		ptrs[1]="      батареи       ";
		ptrs[2]="  0%(  0^ 0@:0#:0$  ";
		if((av_data_off[0]=='A')&&(av_data_off[1]=='A'))
			{
			ptrs[3]="    не устранена    ";
			bgnd_par(ptrs[0],ptrs[1],ptrs[2],ptrs[3]);
			}
		else 
			{
			gran_char(&index_set,0,1);
			ptrs[3]="      устранена     ";
			ptrs[4]="  0[]  0< 0>:0=:0)  ";
			bgnd_par(ptrs[index_set],ptrs[1+index_set],ptrs[2+index_set],ptrs[3+index_set]);
			int2lcd(av_data_off[4],'>',0);
			int2lcd(av_data_off[5],'=',0);
			int2lcd(av_data_off[6],')',0);
			int2lcd(av_data_off[2],'[',0);
			int2lcd(av_data_off[0],'<',0); 
			if(!((av_data_off[1]>=1)&&(av_data_off[1]<=12)))av_data_off[1]=1;
			sub_bgnd(sm_mont[av_data_off[1]],']',0);
			
						
			}	
		int2lcd(av_head[1]+1,'+',0);
		int2lcd(av_data_on[4],'@',0);
		int2lcd(av_data_on[5],'#',0);
		int2lcd(av_data_on[6],'$',0);
		int2lcd(av_data_on[2],'%',0);
		int2lcd(av_data_on[0],'^',0); 
		if(!((av_data_on[1]>=1)&&(av_data_on[1]<=12)))av_data_on[1]=1;
		sub_bgnd(sm_mont[av_data_on[1]],'(',0);
		
		av_j_si_max=1;
		}

	else if((av_head[0]=='B')&&(av_head[2]=='S'))
		{  
		ptrs[0]="       Авария       ";
		ptrs[1]=" асимметрия батареи ";
		ptrs[2]="  0%(  0^ 0@:0#:0$  ";
		if((av_data_off[0]=='A')&&(av_data_off[1]=='A'))
			{
			ptrs[3]="    не устранена    ";
			bgnd_par(ptrs[0],ptrs[1],ptrs[2],ptrs[3]);
			}
		else 
			{
			gran_char(&index_set,0,1);
			ptrs[3]="      устранена     ";
			ptrs[4]="  0[]  0< 0>:0=:0)  ";
			bgnd_par(ptrs[index_set],ptrs[1+index_set],ptrs[2+index_set],ptrs[3+index_set]);
			int2lcd(av_data_off[4],'>',0);
			int2lcd(av_data_off[5],'=',0);
			int2lcd(av_data_off[6],')',0);
			int2lcd(av_data_off[2],'[',0);
			int2lcd(av_data_off[0],'<',0); 
			if(!((av_data_off[1]>=1)&&(av_data_off[1]<=12)))av_data_off[1]=1;
			sub_bgnd(sm_mont[av_data_off[1]],']',0);
			
						
			}	
		int2lcd(av_head[1]+1,'+',0);
		int2lcd(av_data_on[4],'@',0);
		int2lcd(av_data_on[5],'#',0);
		int2lcd(av_data_on[6],'$',0);
		int2lcd(av_data_on[2],'%',0);
		int2lcd(av_data_on[0],'^',0); 
		if(!((av_data_on[1]>=1)&&(av_data_on[1]<=12)))av_data_on[1]=1;
		sub_bgnd(sm_mont[av_data_on[1]],'(',0);
		
		av_j_si_max=1;
		}

	else if((av_head[0]=='B')&&(av_head[2]=='Z'))
		{  
		ptrs[0]="   Выравнивающий    ";
		ptrs[1]="       заряд        ";
		ptrs[2]=" 0%/(  /0^ 0@:0#:0$ ";
		if((av_data_off[0]=='A')&&(av_data_off[1]=='A'))
			{
			ptrs[3]="    не завершен     ";
			bgnd_par(ptrs[0],ptrs[1],ptrs[2],ptrs[3]);
			}
		else 
			{
			gran_char(&index_set,0,1);
			ptrs[3]="      завершен      ";
			ptrs[4]=" 0[/]  /0< 0>:0=:0) ";
			bgnd_par(ptrs[index_set],ptrs[1+index_set],ptrs[2+index_set],ptrs[3+index_set]);
			int2lcd(av_data_off[4],'>',0);
			int2lcd(av_data_off[5],'=',0);
			int2lcd(av_data_off[6],')',0);
			int2lcd(av_data_off[2],'[',0);
			int2lcd(av_data_off[0],'<',0); 
			if(!((av_data_off[1]>=1)&&(av_data_off[1]<=12)))av_data_off[1]=1;
			sub_bgnd(sm_mont[av_data_off[1]],']',0);
			
						
			}	
		int2lcd(av_head[1]+1,'+',0);
		int2lcd(av_data_on[4],'@',0);
		int2lcd(av_data_on[5],'#',0);
		int2lcd(av_data_on[6],'$',0);
		int2lcd(av_data_on[2],'%',0);
		int2lcd(av_data_on[0],'^',0); 
		if(!((av_data_on[1]>=1)&&(av_data_on[1]<=12)))av_data_on[1]=1;
		sub_bgnd(sm_mont[av_data_on[1]],'(',0);
		
		av_j_si_max=1;
		}



	else if((av_head[0]=='B')&&(av_head[2]=='W'))
		{  
		ptrs[0]="       Разряд       ";
		ptrs[1]="       батареи      ";
		ptrs[2]=" Начало             ";
		ptrs[3]=" 0%/(  /0^ 0@:0#:0$ ";
		ptrs[4]="          Uбат=  <В ";
		ptrs[5]=" Конец              ";
		ptrs[6]=" 0q/w  /0r 0i:0l:0s ";
		ptrs[7]="          Uбат=  >В ";
		ptrs[8]="    Отдано    zа*ч. ";
		
		bgnd_par(ptrs[index_set],ptrs[1+index_set],ptrs[2+index_set],ptrs[3+index_set]);
		
		int2lcd(av_head[1]+1,'!',0);

		int2lcd(av_data_on[4],'@',0);
		int2lcd(av_data_on[5],'#',0);
		int2lcd(av_data_on[6],'$',0);
		int2lcd(av_data_on[2],'%',0);
		int2lcd(av_data_on[0],'^',0); 
		if(!((av_data_on[1]>=1)&&(av_data_on[1]<=12)))av_data_on[1]=1;
		sub_bgnd(sm_mont[av_data_on[1]],'(',0);
				
		int2lcd(av_data_off[4],'i',0);
		int2lcd(av_data_off[5],'l',0);
		int2lcd(av_data_off[6],'s',0);
		int2lcd(av_data_off[2],'q',0);
		int2lcd(av_data_off[0],'r',0); 
		if(!((av_data_off[1]>=1)&&(av_data_off[1]<=12)))av_data_off[1]=1;
		sub_bgnd(sm_mont[av_data_off[1]],'w',0);
		
		
		int2lcd(av_head_int[0],'z',1);
		int2lcd(av_data_on[3]+(av_data_on[7]*256),'<',1);
		int2lcd(av_head_int[1],'>',1);	
		av_j_si_max=5;				

		
		}

	else if((av_head[0]=='B')&&(av_head[2]=='K'))
		{  
		ptrs[0]="  Контроль емкости  ";
		ptrs[1]="       батареи      ";
		ptrs[2]=" Начало             ";
		ptrs[3]=" 0%/(  /0^ 0@:0#:0$ ";
		ptrs[4]="         Uбат=  <В  ";
		ptrs[5]=" Конец              ";
		ptrs[6]=" 0q/w  /0r 0i:0l:0s ";
		ptrs[7]="         Uбат=  >В  ";
		ptrs[8]=" Ёмкость     zа*ч.  ";
		
		bgnd_par(ptrs[index_set],ptrs[1+index_set],ptrs[2+index_set],ptrs[3+index_set]);
		
		int2lcd(av_head[1]+1,'!',0);

		int2lcd(av_data_on[4],'@',0);
		int2lcd(av_data_on[5],'#',0);
		int2lcd(av_data_on[6],'$',0);
		int2lcd(av_data_on[2],'%',0);
		int2lcd(av_data_on[0],'^',0); 
		if(!((av_data_on[1]>=1)&&(av_data_on[1]<=12)))av_data_on[1]=1;
		sub_bgnd(sm_mont[av_data_on[1]],'(',0);
				
		int2lcd(av_data_off[4],'i',0);
		int2lcd(av_data_off[5],'l',0);
		int2lcd(av_data_off[6],'s',0);
		int2lcd(av_data_off[2],'q',0);
		int2lcd(av_data_off[0],'r',0); 
		if(!((av_data_off[1]>=1)&&(av_data_off[1]<=12)))av_data_off[1]=1;
		sub_bgnd(sm_mont[av_data_off[1]],'w',0);
		
		
		int2lcd(av_head_int[0],'z',1);
		int2lcd(av_data_on[3]+(av_data_on[7]*256),'<',1);
		int2lcd(av_head_int[1],'>',1);	
		av_j_si_max=5;				

		
		}



	else if(av_head[0]=='S')
		{  
		ptrs[0]="   Авария БПС N+    ";
		
		if(av_head[2]=='L')
			{
			ptrs[1]="     отключился     ";
			}
		else if(av_head[2]=='T')
			{
			ptrs[1]="      перегрев      ";
			}		
		else if(av_head[2]=='U')
			{
			ptrs[1]="   завышено Uвых.   ";
			}		
		else if(av_head[2]=='u')
			{
			ptrs[1]="   занижено Uвых.   ";
			}								
		
		
		ptrs[2]="  0%(  0^ 0@:0#:0$  ";
		if((av_data_off[0]=='A')&&(av_data_off[1]=='A'))
			{
			ptrs[3]="    не устранена    ";
			bgnd_par(ptrs[0],ptrs[1],ptrs[2],ptrs[3]);
			}
		else 
			{
			gran_char(&index_set,0,1);
			ptrs[3]="      устранена     ";
			ptrs[4]="  0[]  0< 0>:0=:0)  ";
			bgnd_par(ptrs[index_set],ptrs[1+index_set],ptrs[2+index_set],ptrs[3+index_set]);
			int2lcd(av_data_off[4],'>',0);
			int2lcd(av_data_off[5],'=',0);
			int2lcd(av_data_off[6],')',0);
			int2lcd(av_data_off[2],'[',0);
			int2lcd(av_data_off[0],'<',0); 
			if(!((av_data_off[1]>=1)&&(av_data_off[1]<=12)))av_data_off[1]=1;
			sub_bgnd(sm_mont[av_data_off[1]],']',0);
			
						
			}	
		int2lcd(av_head[1]+1,'+',0);
		int2lcd(av_data_on[4],'@',0);
		int2lcd(av_data_on[5],'#',0);
		int2lcd(av_data_on[6],'$',0);
		int2lcd(av_data_on[2],'%',0);
		int2lcd(av_data_on[0],'^',0); 
		if(!((av_data_on[1]>=1)&&(av_data_on[1]<=12)))av_data_on[1]=1;
		sub_bgnd(sm_mont[av_data_on[1]],'(',0);
		
		}

	else if((av_head[0]=='F'))
		{  
		ptrs[0]="       Авария       ";
		ptrs[1]=" предохранителя №!  ";
		ptrs[2]="  0%(  0^ 0@:0#:0$  ";
		ptrs[3]="                    ";
	
		bgnd_par(ptrs[0],ptrs[1],ptrs[2],ptrs[3]);

		int2lcd(av_head[1]+1,'!',0);
		int2lcd(av_data_on[4],'@',0);
		int2lcd(av_data_on[5],'#',0);
		int2lcd(av_data_on[6],'$',0);
		int2lcd(av_data_on[2],'%',0);
		int2lcd(av_data_on[0],'^',0); 
		if(!((av_data_on[1]>=1)&&(av_data_on[1]<=12)))av_data_on[1]=1;
		sub_bgnd(sm_mont[av_data_on[1]],'(',0);
		
		av_j_si_max=1;
		}
	
	}
	
	
				
else if(ind==iAusw)
	{
	if(AUSW_MAIN==6024)		ptrs[0]="   ИБЭП220/60-24A   ";
	else if(AUSW_MAIN==6012)	ptrs[0]="   ИБЭП220/60-12A   ";
	else if(AUSW_MAIN==4824)	ptrs[0]="   ИБЭП220/48-24A   ";
	else if(AUSW_MAIN==4812)	ptrs[0]="   ИБЭП220/48-12A   ";
	else if(AUSW_MAIN==6010)	ptrs[0]="   ИБЭП220/60-10A   ";
	else if(AUSW_MAIN==6005)	ptrs[0]="   ИБЭП220/60-5A    ";
	else if(AUSW_MAIN==4810)	ptrs[0]="   ИБЭП220/48-10A   ";
	else if(AUSW_MAIN==4805)	ptrs[0]="   ИБЭП220/48-5A    ";
	else if(AUSW_MAIN==2424)	ptrs[0]="   ИБЭП220/24-24A   ";
	else if(AUSW_MAIN==2412)	ptrs[0]="   ИБЭП220/24-12A   ";
	else if(AUSW_MAIN==4840)	ptrs[0]="   ИБЭП220/48-40A   ";
	else if(AUSW_MAIN==6040)	ptrs[0]="   ИБЭП220/60-40A   ";
	else if(AUSW_MAIN==4820)	ptrs[0]="   ИБЭП220/48-20A   ";
	else if(AUSW_MAIN==6020)	ptrs[0]="   ИБЭП220/60-20A   ";
	else if(AUSW_MAIN==2450)	ptrs[0]="   ИБЭП220/24-50A   ";
	else if(AUSW_MAIN==2425)	ptrs[0]="   ИБЭП220/24-25A   ";	
	else if(AUSW_MAIN==2424)	ptrs[0]="   ИБЭП220/24-24A   ";
	else if(AUSW_MAIN==2412)	ptrs[0]="   ИБЭП220/24-12A   ";

	else ptrs[0]="   Тип неизвестен   ";
	
	ptrs[1]="Дата изгот. 0!/0@/0#";
	
	ptrs[2]="шасси     S/N00000? ";
	
	ptrs[3]="УКУ202.+( S/N00000) ";
	
	ptrs[4]="ПО        v    [    ";
	
	ptrs[5]="БПС1 -    S/N00000< ";
	
	ptrs[6]="БПС2 -    S/N00000> ";
	
	ptrs[7]="RS232      v   $    ";

	ptrs[8]="CAN PDH -  v   %    ";
	
	ptrs[9]="CAN SDH -  v   ^    ";
	
	ptrs[10]="Ethernet-  v   &    ";

	ptrs[11]=" Выход              ";		
	
	
	bgnd_par(ptrs[index_set],ptrs[index_set+1],ptrs[index_set+2],ptrs[index_set+3]);	
	if(index_set==8)lcd_buffer[60]=1;
	/*long2lcdyx_mmm(AUSW_BPS2_NUMBER,3,9,0);
	long2lcdyx_mmm(lc640_read_long(EE_AUSW_BPS2_NUMBER),3,19,0);*/
	
	if(AUSW_DAY<32)int2lcd(AUSW_DAY,'!',0);
	else sub_bgnd("**",'!',-1);
	if(AUSW_MONTH<13)int2lcd(AUSW_MONTH,'@',0);
	else sub_bgnd("**",'@',-1);
	if(AUSW_YEAR<100)int2lcd(AUSW_YEAR,'#',0);
	else sub_bgnd("**",'#',-1);	

	if(AUSW_MAIN_NUMBER>=1000000)sub_bgnd("******",'?',-5);
	else long2lcd_mmm(AUSW_MAIN_NUMBER,'?',0);

	/*if(AUSW_UKU>=1000) sub_bgnd("***",'+',-2);
	else int2lcd(AUSW_UKU,'+',0);

	if(AUSW_UKU_SUB>=100) sub_bgnd("**",'(',-1);
	else int2lcd(AUSW_UKU_SUB,'(',0);*/
	int2lcd(UKU_VERSION/100,'+',0);
	int2lcd(HARDVARE_VERSION,'(',0);
	
	if(AUSW_UKU_NUMBER>=1000000)sub_bgnd("******",')',-5);
	else long2lcd_mmm(AUSW_UKU_NUMBER,')',0);

	if(AUSW_UKU_NUMBER>=1000000)sub_bgnd("******",')',-5);
	else long2lcd_mmm(AUSW_UKU_NUMBER,')',0);
	
	if(AUSW_BPS1_NUMBER>=1000000)sub_bgnd("******",'<',-5);
	else long2lcd_mmm(AUSW_BPS1_NUMBER,'<',0);	
	
	if(AUSW_BPS2_NUMBER>=1000000)sub_bgnd("******",'>',-5);
	else long2lcd_mmm(AUSW_BPS2_NUMBER,'>',0);	

	#ifdef SOFT 
	int2lcd(SOFT,'[',2);
	#else 
	sub_bgnd("---------",'[',-4);
	#endif

	#ifdef RS232_VERSION 
	int2lcd(RS232_VERSION,'$',2);
	#else 
	sub_bgnd("---------",'$',-4);
	#endif
	
	#ifdef CAN_PDH_VERSION 
	int2lcd(CAN_PDH_VERSION,'%',2);
	#else 
	sub_bgnd("---------",'%',-4);
	#endif	
	//else if((AUSW_RS232<0)||(AUSW_RS232>=1000))sub_bgnd("не установлен",'$',-5);
	//else 

	//if(AUSW_PDH==0)sub_bgnd("отсутствует",'%',-5);
//	else if((AUSW_PDH<0)||(AUSW_PDH>=1000))sub_bgnd("не установлен",'%',-5);
	//else int2lcd(AUSW_PDH,'%',2);

	#ifdef CAN_SDH_VERSION 
	int2lcd(CAN_SDH_VERSION,'^',2);
	#else 
	sub_bgnd("---------",'^',-4);
	#endif	
	
	//if(AUSW_SDH==0)sub_bgnd("отсутствует",'%',-5);
	//else if((AUSW_SDH<0)||(AUSW_SDH>=1000))sub_bgnd("не установлен",'%',-5);
	//else int2lcd(AUSW_SDH,'%',2);

	#ifdef ETH_VERSION 
	int2lcd(ETH_VERSION,'&',2);
	#else 
	sub_bgnd("---------",'&',-4);
	#endif	
	
	fl_simv(0,0,0);
	
	}

else if(ind==iAusw_set)
	{
	if(AUSW_MAIN==6024)		ptrs[0]="   ИБЭП220/60-24A   ";
	else if(AUSW_MAIN==6012)	ptrs[0]="   ИБЭП220/60-12A   ";
	else if(AUSW_MAIN==4824)	ptrs[0]="   ИБЭП220/48-24A   ";
	else if(AUSW_MAIN==4812)	ptrs[0]="   ИБЭП220/48-12A   ";
	else if(AUSW_MAIN==6010)	ptrs[0]="   ИБЭП220/60-10A   ";
	else if(AUSW_MAIN==6005)	ptrs[0]="   ИБЭП220/60-5A    ";
	else if(AUSW_MAIN==4810)	ptrs[0]="   ИБЭП220/48-10A   ";
	else if(AUSW_MAIN==4805)	ptrs[0]="   ИБЭП220/48-5A    ";
	else if(AUSW_MAIN==2424)	ptrs[0]="   ИБЭП220/24-24A   ";
	else if(AUSW_MAIN==2412)	ptrs[0]="   ИБЭП220/24-12A   ";
	else if(AUSW_MAIN==4840)	ptrs[0]="   ИБЭП220/48-40A   ";
	else if(AUSW_MAIN==6030)	ptrs[0]="   ИБЭП220/60-30A   ";
	else if(AUSW_MAIN==4820)	ptrs[0]="   ИБЭП220/48-20A   ";
	else if(AUSW_MAIN==6015)	ptrs[0]="   ИБЭП220/60-15A   ";
	else if(AUSW_MAIN==2450)	ptrs[0]="   ИБЭП220/24-50A   ";
	else if(AUSW_MAIN==2425)	ptrs[0]="   ИБЭП220/24-25A   ";
		
	else ptrs[0]="   Тип неизвестен   ";
	
	ptrs[1]="Дата изгот. 0!/0@/0#";
	
	ptrs[2]="шасси     S/N00000? ";
	
	ptrs[3]="УКУ00+.0( S/N00000) ";
	
	ptrs[4]="БПС1 -    S/N00000< ";
	
	ptrs[5]="БПС2 -    S/N00000> ";
	
	ptrs[6]=" Выход              ";		
	
	
	bgnd_par(ptrs[index_set],ptrs[index_set+1],ptrs[index_set+2],ptrs[index_set+3]);	
	
	/*long2lcdyx_mmm(AUSW_BPS2_NUMBER,3,9,0);
	long2lcdyx_mmm(lc640_read_long(EE_AUSW_BPS2_NUMBER),3,19,0);*/

	if(AUSW_DAY<32)int2lcd(AUSW_DAY,'!',0);
	else sub_bgnd("**",'!',-1);
	if(AUSW_MONTH<13)int2lcd(AUSW_MONTH,'@',0);
	else sub_bgnd("**",'@',-1);
	if(AUSW_YEAR<100)int2lcd(AUSW_YEAR,'#',0);
	else sub_bgnd("**",'#',-1);	
	
	if(AUSW_MAIN_NUMBER>=1000000)sub_bgnd("******",'?',-5);
	else long2lcd_mmm(AUSW_MAIN_NUMBER,'?',0);
	
	/*if(AUSW_UKU>=1000) sub_bgnd("***",'+',-2);
	else int2lcd(AUSW_UKU,'+',0);

	if(AUSW_UKU_SUB>=100) sub_bgnd("**",'(',-1);
	else int2lcd(AUSW_UKU_SUB,'(',0);*/
	
	int2lcd(UKU_VERSION,'+',0);
	int2lcd(HARDVARE_VERSION,'(',0);	

	if(AUSW_UKU_NUMBER>=1000000)sub_bgnd("******",')',-5);
	else long2lcd_mmm(AUSW_UKU_NUMBER,')',0);

	if(AUSW_BPS1_NUMBER>=1000000)sub_bgnd("******",'<',-5);
	else long2lcd_mmm(AUSW_BPS1_NUMBER,'<',0);	

	if(AUSW_BPS2_NUMBER>=1000000)sub_bgnd("******",'>',-5);
	else long2lcd_mmm(AUSW_BPS2_NUMBER,'>',0);
	
				
	if((index_set==3)&&(sub_ind==9))lcd_buffer[60]=1;

	
	if((sub_ind==0)&&(!index_set)) fl_simv(0,0,20);	
	else if((sub_ind==1)&&(index_set<2))fl_simv(1-index_set,12,2);
	else if((sub_ind==2)&&(index_set<2))fl_simv(1-index_set,15,2);
	else if((sub_ind==3)&&(index_set<2))fl_simv(1-index_set,18,2);
	
	else if((sub_ind==4)&&(index_set<3)) fl_simv(2-index_set,10,10);	
	else if((sub_ind==5)&&(index_set<4)) fl_simv(3-index_set,3,6);	
	else if((sub_ind==6)&&(index_set<4)) fl_simv(3-index_set,10,10);	

	else if((sub_ind==7)/*&&(index_set<4)*/) fl_simv(4-index_set,10,10);	
	else if((sub_ind==8)/*&&(index_set<4)*/) fl_simv(5-index_set,10,10);	
	else fl_simv(0,0,0);	
	/*
	
	
	ptrs[1]="Дата изгот. - неизв.";
	if((AUSW_YEAR<=99)||(AUSW_MONTH<=12)||(AUSW_DAY<=31))
		{
		

		}	
	
	
	ptrs[2]="шасси S/N - неизв.  ";
	ptrs[2]="шасси S/N - неизв.  ";
	if(AUSW_MAIN_NUMBER<60000)
		{
		ptrs[2]="шасси S/N - 00000?  ";
		
		}	

	ptrs[3]="УКУ--+.-( S/N неуст)";
	if(AUSW_UKU>1000) sub_bgnd("-.--",'(',-3);
	else 
		{
	
		}
	if(AUSW_UKU_SUB>99) sub_bgnd("-",'(',0);
	else 
		{
	
		}		
	if(AUSW_UKU_NUMBER>30000)sub_bgnd("ст.",')',-2);
	else
		{
		sub_bgnd("00000)",')',-5);
		//int2lcd((signed short)AUSW_UKU_NUMBER,')',0);
		}

	ptrs[4]="БПС1 - не установлен";
	if(AUSW_BPS1<60000)
		{
		ptrs[4]="БПС1 - S/N0000<     ";
		
		}
		
	ptrs[5]="БПС2 - не установлен";	
	if(AUSW_BPS2<60000)
		{
		ptrs[5]="БПС2 - S/N0000>     ";
		
		}	

	if((AUSW_RS232==0)||(AUSW_RS232>=1000))
		{
		ptrs[6]="RS232   отсутствует ";
		}
	else ptrs[6]="RS232 - версия     [";
	
	if((AUSW_PDH==0)||(AUSW_PDH>=1000))
		{
		ptrs[7]="CAN PDH отсутствует ";
		}
	else ptrs[7]="CAN PDH - версия   ]";
	
	if((AUSW_SDH==0)||(AUSW_SDH>=1000))
		{
		ptrs[8]="CAN SDH отсутствует ";
		}
	else ptrs[8]="CAN SDH - версия   )";		
	
	
	int2lcd((unsigned short)AUSW_MAIN_NUMBER,'?',0);
	if(AUSW_DAY<32)int2lcd(AUSW_DAY,'!',0);
	else sub_bgnd("**",'!',-1);
	if(AUSW_MONTH<13)int2lcd(AUSW_MONTH,'@',0);
	else sub_bgnd("**",'@',-1);
	if(AUSW_YEAR<100)int2lcd(AUSW_YEAR,'#',0);
	else sub_bgnd("**",'#',-1);
	int2lcd((unsigned short)AUSW_BPS1,'<',0);
	int2lcd((unsigned short)AUSW_BPS2,'>',0);
	int2lcd(AUSW_UKU,'+',0);
	int2lcd(AUSW_UKU_SUB,'(',0);
	if(AUSW_UKU_NUMBER==0xffff)
		{
		sub_bgnd(" ",')',0);
		}
	else
		{
		sub_bgnd("     )",')',-5);
		int2lcd((unsigned short)AUSW_UKU_NUMBER,')',0);
		}
	
	//int2lcdyx(AUSW_UKU,0,19,0);
	
	if(ind==iAusw_set)
		{
		if((sub_ind==0)&&(!index_set)) fl_simv(0,0,20);
		
		else if((sub_ind==1)&&(index_set<2))
			{
			if((AUSW_YEAR>99)&&(AUSW_MONTH>12)&&(AUSW_DAY>31)) fl_simv(1-index_set,14,6);
			else fl_simv(1,12,2);
			}
		else if((sub_ind==2)&&(index_set<2))
			{
			if((AUSW_YEAR>99)&&(AUSW_MONTH>12)&&(AUSW_DAY>31)) fl_simv(1-index_set,14,6);
			else fl_simv(1,15,2);
			}	
		else if((sub_ind==3)&&(index_set<2))
			{
			if((AUSW_YEAR>99)&&(AUSW_MONTH>12)&&(AUSW_DAY>31)) fl_simv(1-index_set,14,6);
			else fl_simv(1,18,2);
			}	
		else if((sub_ind==4)&&(index_set<3)) fl_simv(2-index_set,10,10);	
		else if((sub_ind==5)&&(index_set<4)) fl_simv(3-index_set,3,6);	
		else if((sub_ind==6)&&(index_set<4)) fl_simv(3-index_set,14,6);				
		}*/
	}



else if(ind==iSet)
	{
     ptrs[0]=" Стандартные        ";
	ptrs[1]=" Время и дата       ";
     ptrs[2]=" Структура          ";
     ptrs[3]=" Мнемоника         y";
     ptrs[4]=" Основной источн. N<";
	ptrs[5]=" Зв.сигн.   (       ";
	ptrs[6]=" Отключение сигнала ";
	ptrs[7]="  аварии    )       ";
	ptrs[8]=" АПВ источников     ";
	ptrs[9]=" T проверки   цепи  ";
     ptrs[10]=" батареи     qмин.  ";
     ptrs[11]=" Umax=       !В     ";
     ptrs[12]=" Uб0°=       @В     ";
     ptrs[13]=" Uб20°=      #В     ";
     ptrs[14]=" Uсигн=      ^В     ";
     ptrs[15]=" Umin.сети=  &В     ";
	ptrs[16]=" U0б=        >В     ";
	ptrs[17]=" Iбк.=       jА     ";
     ptrs[18]=" Iз.мах.=    JА     ";
     ptrs[19]=" Imax=       ]A     ";
     ptrs[20]=" Kimax=      {      ";
     ptrs[21]=" Kвыр.зар.=    [    ";
     ptrs[22]=" Tз.вкл.а.с. !с     ";
	ptrs[23]=" tmax=       $°C    ";
	ptrs[24]=" Ethernet           ";
     ptrs[25]=" Внешние датчики    ";
     ptrs[26]=" Поэлементный к-ль. ";
     ptrs[27]=" батареи         Q% ";
	ptrs[28]=" Аварийные реле     ";
     ptrs[29]=sm_exit; 
     ptrs[30]=" Калибровки         "; 
     ptrs[31]=" Тест               ";        
	
	if((sub_ind-index_set)>2)index_set=sub_ind-2;
	else if(sub_ind<index_set)index_set=sub_ind;
	bgnd_par("     УСТАНОВКИ      ",
			ptrs[index_set],ptrs[index_set+1],ptrs[index_set+2]);

	pointer_set(1);	
	
	if(index_set<17)
	     {
	     int2lcd(MAIN_BPS+1,'<',0);
	     if(ZV_ON)sub_bgnd(smon,'(',0);
	     else sub_bgnd(smoff,'(',0);
	     if(AV_OFF_AVT)sub_bgnd("автом.",')',0);
	     else sub_bgnd("ручн.",')',0);
	     if(MNEMO_ON==mnON)
	     	{
	     	sub_bgnd("через yс.",'y',-8);
	     	int2lcd(MNEMO_TIME,'y',0);
	     	}
	     else sub_bgnd("выкл.",'y',-4);
	     int2lcd(UMAX,'!',1);
	     int2lcd(UB0,'@',1);
	     int2lcd(UB20,'#',1);
	     int2lcd(USIGN,'^',1);
	     int2lcd(UMN,'&',0);
	     int2lcd(U0B,'>',1);
	     } 
	int2lcd(TMAX,'$',0);
	int2lcd(IKB,'j',2);
	int2lcd(KVZ,'[',3);
	int2lcd(IMAX,']',1);
	int2lcd(KIMAX,'{',1);
	int2lcd(IZMAX,'J',1); 
	int2lcd(TZAS,'!',0);
	int2lcd(TBAT,'q',0); 
     if(UBM_AV)
          {
          int2lcd(UBM_AV,'Q',0);
          } 
     else sub_bgnd("ВЫКЛ.",'Q',-2);
	
	     
	}

else if (ind==iDef)
	{ 
	ptrs[0]=" ИБЭП220/60-12А-1/2 ";
	ptrs[1]=" ИБЭП220/60-12А-2/2 ";
	ptrs[2]=" ИБЭП220/48-12А-1/2 ";
	ptrs[3]=" ИБЭП220/48-12А-2/2 ";
	ptrs[4]=sm_exit;

	if(bFL5)ptrs[default_temp]=sm_;
	
	if(sub_ind<index_set) index_set=sub_ind;
	else if((sub_ind-index_set)>2) index_set=sub_ind-2;
     bgnd_par("СТАНДАРТНЫЕ УСТ.-КИ ",ptrs[index_set],ptrs[index_set+1],ptrs[index_set+2]);
	
	pointer_set(1);
	
  	} 	
	        



else if(ind==iSet_T)
	{
	static char phase_cnt;
	if(++phase_cnt>=15)
	     {
	     phase_cnt=0;
	     if(++phase>=3)phase=0;
	     }
	ptrs[0]=" 0%:0^:0& 0</>  /0{ ";
	ptrs[1]=sm_;
	if(phase==0)ptrs[2]="    љ њ - выбор     ";
     if(phase==1)ptrs[2]="   ї ќ - установка  ";
     if(phase==2)ptrs[2]="     ¤  - выход     ";
	
	bgnd_par(" УСТАНОВКА  ВРЕМЕНИ ",ptrs[0],ptrs[1],ptrs[2]);
     if(sub_ind==0)lcd_buffer[42]='^';
     else if(sub_ind==1)lcd_buffer[45]='^';
     else if(sub_ind==2)lcd_buffer[48]='^';
     else if(sub_ind==3)lcd_buffer[51]='^';
     else if(sub_ind==4)lcd_buffer[54]='^';
     else if(sub_ind==5)lcd_buffer[58]='^';
  
 	int2lcd(LPC_RTC->SEC,'&',0);
 	int2lcd(LPC_RTC->MIN,'^',0);
 	int2lcd(LPC_RTC->HOUR,'%',0);
 	
 	int2lcd(LPC_RTC->DOM,'<',0);
 	sub_bgnd(sm_mont[LPC_RTC->MONTH],'>',0);
 	int2lcd(LPC_RTC->YEAR,'{',0);
 	if(bFL2)
 	     {
 	     lcd_buffer[find(':')]=' ';
 	     lcd_buffer[find(':')]=' ';
 	     }  

	//int2lcdyx('ќ',3,19,0);
	}  

else if (ind==iApv)
	{ 
	ptrs[0]=" АПВ 1й уровень !   ";
	if(apv_on1!=apvON)
	     {
	     ptrs[1]=" Выход              ";
	     ptrs[2]=sm_;
	     ptrs[3]=sm_;
	     ptrs[4]=sm_;
	     simax=1;
	     }
	else
	     {
	     if(apv_on2!=apvON)
	          {
	          ptrs[1]=" АПВ 2й уровень @   ";
	          ptrs[2]=" Выход              ";
	          ptrs[3]=sm_;
	          ptrs[4]=sm_;
	          simax=2;
	          }
	     else 
	          {
               ptrs[1]=" АПВ 2й уровень @   ";
	          ptrs[2]=" Период АПВ2     #ч.";
	          ptrs[3]=" Выход              ";
	          ptrs[4]=sm_;
	          simax=3;	          
	          }     
	     }     
	
	if(sub_ind<index_set) index_set=sub_ind;
	else if((sub_ind-index_set)>2) index_set=sub_ind-2;	
     bgnd_par("   АПВ ИСТОЧНИКОВ   ",ptrs[index_set],ptrs[index_set+1],ptrs[index_set+2]);
	
	pointer_set(1);
	if(apv_on1==apvON)sub_bgnd(smon,'!',0);
	else sub_bgnd(smoff,'!',0);
	
	if(apv_on2==apvON)
	     {
	     sub_bgnd(smon,'@',0);
	     int2lcd(apv_on2_time,'#',0);
	     }
	else sub_bgnd(smoff,'@',0);	
     
 //    int2lcdyx(lc640_read_int(EE_APV_ON1),2,19,0);
 //    int2lcdyx(apv_on1,3,19,0);
  	} 

else if (ind==iTst_full)
	{
	ptrs[0]=						" Реле ав.№1        !";
     //ptrs[1]=						" сети              !";
     ptrs[1]=						" Реле ав.№2        @";
     //ptrs[3]=						" батарей           @";
     ptrs[2]=						" Реле ав.№3        #";
     //ptrs[5]=						" БПСов             #";
	ptrs[3]=						" Реле самокалибровки";
	ptrs[4]=						" Реле батареи      (";
	ptrs[5]=" БПС1               ";
	ptrs[6]=" БПС2               ";
	ptrs[7]=" БПС1+БПС2          ";
	ptrs[8]=sm_exit;
	
	if(sub_ind<index_set) index_set=sub_ind;
	else if((sub_ind-index_set)>2) index_set=sub_ind-2;
     
	bgnd_par("        ТЕСТ        ",
     		ptrs[index_set],
     		ptrs[index_set+1],
     		ptrs[index_set+2]);
	
	pointer_set(1);
 
	if(tst_state[0]==tstOFF)sub_bgnd(smoff,'!',-3);
	else if(tst_state[0]==tstON)sub_bgnd(smon,'!',-3);
	else sub_bgnd("РАБОЧ.",'!',-5);

	if(tst_state[1]==tstOFF)sub_bgnd(smoff,'@',-3);
	else if(tst_state[1]==tstON)sub_bgnd(smon,'@',-3);
	else sub_bgnd("РАБОЧ.",'@',-5);

	if(tst_state[2]==tstOFF)sub_bgnd(smoff,'#',-3);
	else if(tst_state[2]==tstON)sub_bgnd(smon,'#',-3);
	else sub_bgnd("РАБОЧ.",'#',-5);

	if(tst_state[3]==tstOFF)sub_bgnd(smoff,'(',-3);
	else if(tst_state[3]==tstON)sub_bgnd(smon,'(',-3);
	else sub_bgnd("РАБОЧ.",'(',-5);		

 /*   	if(tst_state[1]==tstOFF)sub_bgnd(smoff,')',0);
	else if(tst_state[1]==tstON)sub_bgnd(smon,')',0);*/
    /* int2lcdyx(tst_state[0],0,19,0);
     int2lcdyx(tst_state[1],0,16,0);
     int2lcdyx(tst_state[2],0,13,0);
     int2lcdyx(tst_state[3],0,10,0);
     int2lcdyx(tst_state[4],0,7,0);*/
	} 

else if (ind==iTst)
	{
	ptrs[0]=" Реле авария    (   ";
	ptrs[1]=" Реле самокалибровки";
	ptrs[2]=" БПС1               ";
	ptrs[3]=" БПС2               ";
	ptrs[4]=" БПС1+БПС2          ";
	ptrs[5]=sm_exit;
	
	if(sub_ind<index_set) index_set=sub_ind;
	else if((sub_ind-index_set)>2) index_set=sub_ind-2;
     bgnd_par("        ТЕСТ        ",
     		ptrs[index_set],
     		ptrs[index_set+1],
     		ptrs[index_set+2]);
	
	pointer_set(1);
 
	if(tst_state[0]==tstOFF)sub_bgnd(smoff,'(',0);
	else if(tst_state[0]==tstON)sub_bgnd(smon,'(',0);	

 /*   	if(tst_state[1]==tstOFF)sub_bgnd(smoff,')',0);
	else if(tst_state[1]==tstON)sub_bgnd(smon,')',0);*/
    /* int2lcdyx(tst_state[0],0,19,0);
     int2lcdyx(tst_state[1],0,16,0);
     int2lcdyx(tst_state[2],0,13,0);
     int2lcdyx(tst_state[3],0,10,0);
     int2lcdyx(tst_state[4],0,7,0);*/

	MSG_IND2OUT_EN_BAT=1;
	} 

else if (ind==iTst_klbr)
	{ 
	
	if(sub_ind<index_set) index_set=sub_ind;
	else if((sub_ind-index_set)>2) index_set=sub_ind-2;
     bgnd_par(" ТЕСТ САМОКАЛИБРОВКА",
     	    " Реле         <     ",
     	    " Iбат=           :A ",
     	    sm_exit);
	
	pointer_set(1);
 
    	if(tst_state[4]==tstOFF)sub_bgnd(smoff,'<',0);
	else if(tst_state[4]==tstON)sub_bgnd(smon,'<',0);
	else if(tst_state[4]==tstWRK)sub_bgnd("РАБОЧ.",'<',0);

     int2lcd_mmm(Ibat,':',2);

     /*int2lcdyx(tst_state[0],0,19,0);
     int2lcdyx(tst_state[1],0,16,0);
     int2lcdyx(tst_state[2],0,13,0);
     int2lcdyx(tst_state[3],0,10,0);
     int2lcdyx(tst_state[4],0,7,0);*/				
 	} 

else if (ind==iTst_BPS1)
	{ 
	
	if(sub_ind<index_set) index_set=sub_ind;
	else if((sub_ind-index_set)>2) index_set=sub_ind-2;
     bgnd_par("     ТЕСТ БПС1      ",
     	    " Uр=@  $     шим=  #",
     	    " U1=    <В I1=    >А",
     	    sm_exit);
	
	pointer_set(1);
 
     if(tst_state[5]==tstOFF)
     	{
     	sub_bgnd("Umin",'@',0);   
     	MSG_IND2PWM_SRC1=30;
     	}
	else if(tst_state[5]==tstON)
		{
		sub_bgnd("Umax",'@',0);
		MSG_IND2PWM_SRC1=970;
		}
	else if(tst_state[5]==tstU)
		{
		sub_bgnd("U20",'@',0);
		MSG_IND2PWM_SRC1=0x5555;
		}
     sub_bgnd("(   $)",'$',0);
     
     int2lcd(Us[0],'<',1);
     int2lcd(Is[0],'>',1);	
     int2lcd(cntrl_stat1,'#',0);		
     int2lcd(u_necc,'$',1);	
	
	MSG_IND2OUT_DIS_BAT=1;
	
/*	int2lcdyx(MSG_IND2PWM_SRC1,0,3,0);
	int2lcdyx(Uload,0,7,0);
	int2lcdyx(u_necc,0,11,0);
	int2lcdyx(Ubat,0,15,0);
	int2lcdyx(Us[1],0,19,0);	*/
 	} 



else if (ind==iTst_BPS2)
	{ 
	
	if(sub_ind<index_set) index_set=sub_ind;
	else if((sub_ind-index_set)>2) index_set=sub_ind-2;
     bgnd_par("     ТЕСТ БПС2      ",
     	    " Uр=@  $     шим=  #",
     	    " U2=    [В I2=    ]А",
     	    sm_exit);
	
	pointer_set(1);
 
     if(tst_state[6]==tstOFF)
     	{
     	sub_bgnd("Umin",'@',0);   
     	MSG_IND2PWM_SRC2=30;
     	}
	else if(tst_state[6]==tstON)
		{
		sub_bgnd("Umax",'@',0);
		MSG_IND2PWM_SRC2=970;
		}
	else if(tst_state[6]==tstU)
		{
		sub_bgnd("U20",'@',0);
		MSG_IND2PWM_SRC2=0x5555;
		}
	sub_bgnd("(   $)",'$',0);
	
     int2lcd(Us[1],'[',1);
     int2lcd(Is[1],']',1);
     int2lcd(cntrl_stat2,'#',0);	
     int2lcd(u_necc,'$',1);
	
	MSG_IND2OUT_DIS_BAT=1;			
 	}

/*
const char sm360[]	={"        ТЕСТ        "};
const char sm361[]	={" Реле авария    (   "};
const char sm362[]	={" Реле самокалибровки"};
const char sm363[]	={" БПС1               "};
const char sm364[]	={" БПС2               "};
const char sm365[]	={" БПС1+БПС2          "};
const char sm367[]	={" ТЕСТ САМОКАЛИБРОВКА"};
const char sm368[]	={" Реле         <     "};
const char sm369[]	={" Iбат=           :A "};
const char sm370[]	={"     ТЕСТ БПС1      "};
const char sm371[]	={" Uр=@  $     шим=  #"};
const char sm372[]	={" U1=    <В I1=    >А"};
const char sm373[]	={"     ТЕСТ БПС2      "};
const char sm374[]	={" Uр=@  $     шим=  #"};
const char sm375[]	={" U2=    [В I2=    ]А"};
const char sm376[]	={"   ТЕСТ БПС1+БПС2   "};
const char sm377[]	={" Uр=@  $       #   !"};
const char smumax[]	={"Umax"}; 
const char smumin[]	={"Umin"};
const char smu20[]	={"U20"}; 
const char smu20_[]	={"(   $)"};	
*/
else if (ind==iTst_BPS12)
	{ 
	ptrs[0]=" Uр=@  $       #   !";
	ptrs[1]=" U1=    <В I1=    >А";
	ptrs[2]=" U2=    [В I2=    ]А";
	ptrs[3]=sm_exit;
	
	if(sub_ind<index_set) index_set=sub_ind;
	else if((sub_ind-index_set)>2) index_set=sub_ind-2;
     bgnd_par("   ТЕСТ БПС1+БПС2   ",
     	    ptrs[index_set],
     	    ptrs[index_set+1],
     	    ptrs[index_set+2]);
	
	pointer_set(1);
 

     if(tst_state[7]==tstOFF)
     	{
     	sub_bgnd("Umin",'@',0);   
     	MSG_IND2PWM_SRC1=30;
     	MSG_IND2PWM_SRC2=30;
     	}
	else if(tst_state[7]==tstON)
		{
		sub_bgnd("Umax",'@',0);
     	MSG_IND2PWM_SRC1=970;
     	MSG_IND2PWM_SRC2=970;
     	}		
	else if(tst_state[7]==tstU)
		{
		sub_bgnd("U20",'@',0);
		MSG_IND2PWM_SRC1=0x5555;
		MSG_IND2PWM_SRC2=0x5555;
		} 
	sub_bgnd("(   $)",'$',0);
	
     int2lcd(Us[0],'<',1);
     int2lcd(Is[0],'>',1);	
     int2lcd(cntrl_stat1,'#',0);
     int2lcd(Us[1],'[',1);
     int2lcd(Is[1],']',1);
     int2lcd(cntrl_stat2,'!',0);
     int2lcd(u_necc,'$',1);

	MSG_IND2OUT_DIS_BAT=1;
 	}  	
 	
else if(ind==iStr)
	{
	ptrs[0]=" Батарей           @";
	ptrs[1]=" Источников        !";
	ptrs[2]=" Датчиков темпер.  #";
	ptrs[3]=" Сухих контактов   $";
	ptrs[4]=sm_exit;
	
	if(sub_ind<index_set) index_set=sub_ind;
	else if((sub_ind-index_set)>2) index_set=sub_ind-2;
	bgnd_par("      СТРУКТУРА     ",ptrs[index_set],ptrs[index_set+1],ptrs[index_set+2]);
	
	pointer_set(1);
		
	int2lcd(NUMIST,'!',0); 
	int2lcd(NUMBAT,'@',0);
	int2lcd(NUMDT,'#',0);
	int2lcd(NUMSK,'$',0);
	}    

else if (ind==iExt_set)
	{ 
	ptrs[0]=			" Датчик темпер. N1  ";
	ptrs[0+NUMDT]=		" Сухой контакт N1   ";
	ptrs[1+NUMDT]=		" Сухой контакт N2   ";
	ptrs[2+NUMDT]=		" Сухой контакт N3   ";
	ptrs[3+NUMDT]=		" Сухой контакт N4   ";
	ptrs[0+NUMDT+NUMSK]=sm_exit;
	ptrs[1+NUMDT+NUMSK]="                    ";
	ptrs[2+NUMDT+NUMSK]="                    ";
	
	if(sub_ind<index_set) index_set=sub_ind;
	else if((sub_ind-index_set)>2) index_set=sub_ind-2;
     bgnd_par("  ВНЕШНИЕ ДАТЧИКИ   ",ptrs[index_set],ptrs[index_set+1],ptrs[index_set+2]);
	
	pointer_set(1);
	}
	
else if (ind==iExt_dt)
	{ 
	ptrs[0]=" температура     @°C";
	ptrs[1]=" tmax            #°C";
	ptrs[2]=" tmin            $°C";
	ptrs[3]=" Реле            [  ";
	ptrs[4]=" Звук            ]  ";
	ptrs[5]=" Дисплей         (  ";
	ptrs[6]=" SNMP TRAP       )  ";
	ptrs[7]=sm_exit;
	
	if(sub_ind<index_set) index_set=sub_ind;
	else if((sub_ind-index_set)>1) index_set=sub_ind-1;
     bgnd_par("  ВНЕШНИЙ ДАТЧИК    ","   ТЕМПЕРАТУРЫ N!   ",ptrs[index_set],ptrs[index_set+1]);
	
	pointer_set(2);
	int2lcd(sub_ind1+1,'!',0);
	int2lcd_mmm(tout[sub_ind1],'@',0);
	if(!TMAX_EXT_EN[sub_ind1])int2lcd_mmm(TMAX_EXT[sub_ind1],'#',0);
	else sub_bgnd("выкл.",'#',-2);
	if(!TMIN_EXT_EN[sub_ind1])int2lcd_mmm(TMIN_EXT[sub_ind1],'$',0);
	else sub_bgnd("выкл.",'$',-2);
	if(!T_EXT_REL_EN[sub_ind1])sub_bgnd("вкл.",'[',-2);
	else sub_bgnd("выкл.",'[',-2);
	if(!T_EXT_ZVUK_EN[sub_ind1])sub_bgnd("вкл.",']',-2);
	else sub_bgnd("выкл.",']',-2);
	if(!T_EXT_LCD_EN[sub_ind1])sub_bgnd("вкл.",'(',-2);
	else sub_bgnd("выкл.",'(',-2);
	if(!T_EXT_SNMP_TRAP_EN[sub_ind1])sub_bgnd("вкл.",')',-2);
	else sub_bgnd("выкл.",')',-2);	
	
	//int2lcdyx(sub_ind,0,1,0);	
	//int2lcdyx(index_set,0,3,0);
	}	
else if (ind==iExt_sk)
	{ 
	ptrs[0]=" текущ.сост. @      ";
	ptrs[1]=" аварийное состояние";
	ptrs[2]="         - #        ";
	ptrs[3]=" Реле            [  ";
	ptrs[4]=" Звук            ]  ";
	ptrs[5]=" Дисплей         (  ";
	ptrs[6]=" SNMP TRAP       )  ";
	ptrs[7]=sm_exit;
	
	if(sub_ind<index_set) index_set=sub_ind;
	else if((sub_ind-index_set)>2) index_set=sub_ind-2;
     bgnd_par("  СУХОЙ КОНТАКТ N!  ",ptrs[index_set],ptrs[index_set+1],ptrs[index_set+2]);
	
	pointer_set(1);
	int2lcd(sub_ind1+1,'!',0);
	if(in_stat_out[sub_ind1]==255)sub_bgnd("замкнут",'@',0);
	else sub_bgnd("незамк.",'@',0);
	if(!SK_SIGN[sub_ind1])sub_bgnd("замкнут",'#',0);
	else sub_bgnd("незамк.",'#',0);
	if(!TMIN_EXT_EN[sub_ind1])int2lcd_mmm(TMIN_EXT[sub_ind1],'$',0);
	else sub_bgnd("выкл.",'$',-6);
	if(!SK_REL_EN[sub_ind1])sub_bgnd("вкл.",'[',-2);
	else sub_bgnd("выкл.",'[',-2);
	if(!SK_ZVUK_EN[sub_ind1])sub_bgnd("вкл.",']',-2);
	else sub_bgnd("выкл.",']',-2);
	if(!SK_LCD_EN[sub_ind1])sub_bgnd("вкл.",'(',-2);
	else sub_bgnd("выкл.",'(',-2);
	if(!SK_SNMP_TRAP_EN[sub_ind1])sub_bgnd("вкл.",')',-2);
	else sub_bgnd("выкл.",')',-2);	
	
	//int2lcdyx(sub_ind,0,1,0);	
	//int2lcdyx(index_set,0,3,0);
	}
	
else if (ind==iLan_set)
	{
	char sss[10]="abcdef";
	char i,i_flag;
	 
	ptrs[0]=	" Ethernet         ! ";
	ptrs[1]=	" DHCPклиент       @ ";
	ptrs[2]=	" IPадрес            ";
	ptrs[3]=	"  000.000.000.00#   ";
	ptrs[4]=	" Маска подсети      ";
	ptrs[5]=	"  000.000.000.00$   ";
	ptrs[6]=	" Шлюз               ";
	ptrs[7]=	"  000.000.000.00)   ";
	ptrs[8]=	" Порт.чтения       [";
	ptrs[9]=	" Порт.записи       ]";
	ptrs[10]=	" Community <        ";
	ptrs[11]=	" Адресат для TRAP N1";
	ptrs[12]=	"  000.000.000.00%   ";
	ptrs[13]=	" Адресат для TRAP N2";
	ptrs[14]=	"  000.000.000.00^   ";
	ptrs[15]=	" Адресат для TRAP N3";
	ptrs[16]=	"  000.000.000.00&   ";
	ptrs[17]=	" Адресат для TRAP N4";
	ptrs[18]=	"  000.000.000.00*   ";
	ptrs[19]=	" Адресат для TRAP N5";
	ptrs[20]=	"  000.000.000.00(   ";
	ptrs[21]=	" Выход              ";

	
	if(!ETH_IS_ON)
		{
		ptrs[1]=" Выход              ";
		ptrs[2]="                    ";
		ptrs[3]="                    ";
		}

	
	if(sub_ind<index_set) index_set=sub_ind;
	else if((sub_ind-index_set)>2) index_set=sub_ind-2;

     bgnd_par(	" УСТАНОВКИ Ethernet ",
			ptrs[index_set],
			ptrs[index_set+1],
			ptrs[index_set+2]);
	
	pointer_set(1);
     if(ETH_IS_ON)
     	{
     	sub_bgnd("ВКЛ.",'!',-3);   
     	}
     else 
     	{
     	sub_bgnd("ВЫКЛ.",'!',-4);   
     	}

     if(ETH_DHCP_ON)
     	{
     	sub_bgnd("ВКЛ.",'@',-3);   
     	}
     else 
     	{
     	sub_bgnd("ВЫКЛ.",'@',-4);   
     	}
		  
	if(sub_ind==2)	ip2lcd(ETH_IP_1,ETH_IP_2,ETH_IP_3,ETH_IP_4,'#',(sub_ind1+1));
	else ip2lcd(ETH_IP_1,ETH_IP_2,ETH_IP_3,ETH_IP_4,'#',0);
	if(sub_ind==4)	ip2lcd(ETH_MASK_1,ETH_MASK_2,ETH_MASK_3,ETH_MASK_4,'$',(sub_ind1+1));
	else ip2lcd(ETH_MASK_1,ETH_MASK_2,ETH_MASK_3,ETH_MASK_4,'$',0);
	if(sub_ind==6)	ip2lcd(ETH_GW_1,ETH_GW_2,ETH_GW_3,ETH_GW_4,')',(sub_ind1+1));
	else ip2lcd(ETH_GW_1,ETH_GW_2,ETH_GW_3,ETH_GW_4,')',0);

	int2lcd(ETH_SNMP_PORT_READ,'[',0);
	int2lcd(ETH_SNMP_PORT_WRITE,']',0);

	if( (ETH_TRAP1_IP_1==255) && (ETH_TRAP1_IP_2==255) && (ETH_TRAP1_IP_3==255) && (ETH_TRAP1_IP_4==255) ) sub_bgnd("    неактивен    ",'%',-14);
	else
		{
		if(sub_ind==11)	ip2lcd(ETH_TRAP1_IP_1,ETH_TRAP1_IP_2,ETH_TRAP1_IP_3,ETH_TRAP1_IP_4,'%',(sub_ind1+1));
		else ip2lcd(ETH_TRAP1_IP_1,ETH_TRAP1_IP_2,ETH_TRAP1_IP_3,ETH_TRAP1_IP_4,'%',0);
		}

	if( (ETH_TRAP2_IP_1==255) && (ETH_TRAP2_IP_2==255) && (ETH_TRAP2_IP_3==255) && (ETH_TRAP2_IP_4==255) ) sub_bgnd("    неактивен    ",'^',-14);
	else
		{
		if(sub_ind==13)	ip2lcd(ETH_TRAP2_IP_1,ETH_TRAP2_IP_2,ETH_TRAP2_IP_3,ETH_TRAP2_IP_4,'^',(sub_ind1+1));
		else ip2lcd(ETH_TRAP2_IP_1,ETH_TRAP2_IP_2,ETH_TRAP2_IP_3,ETH_TRAP2_IP_4,'^',0);
		}

	if( (ETH_TRAP3_IP_1==255) && (ETH_TRAP3_IP_2==255) && (ETH_TRAP3_IP_3==255) && (ETH_TRAP3_IP_4==255) ) sub_bgnd("    неактивен    ",'&',-14);
	else
		{
		if(sub_ind==15)	ip2lcd(ETH_TRAP3_IP_1,ETH_TRAP3_IP_2,ETH_TRAP3_IP_3,ETH_TRAP3_IP_4,'&',(sub_ind1+1));
		else ip2lcd(ETH_TRAP3_IP_1,ETH_TRAP3_IP_2,ETH_TRAP3_IP_3,ETH_TRAP3_IP_4,'&',0);
		}

	if( (ETH_TRAP4_IP_1==255) && (ETH_TRAP4_IP_2==255) && (ETH_TRAP4_IP_3==255) && (ETH_TRAP4_IP_4==255) ) sub_bgnd("    неактивен    ",'*',-14);
	else
		{
		if(sub_ind==17)	ip2lcd(ETH_TRAP4_IP_1,ETH_TRAP4_IP_2,ETH_TRAP4_IP_3,ETH_TRAP4_IP_4,'*',(sub_ind1+1));
		else ip2lcd(ETH_TRAP4_IP_1,ETH_TRAP4_IP_2,ETH_TRAP4_IP_3,ETH_TRAP4_IP_4,'*',0);
		}

	if( (ETH_TRAP5_IP_1==255) && (ETH_TRAP5_IP_2==255) && (ETH_TRAP5_IP_3==255) && (ETH_TRAP5_IP_4==255) ) sub_bgnd("    неактивен    ",'(',-14);
	else
		{
		if(sub_ind==19)	ip2lcd(ETH_TRAP5_IP_1,ETH_TRAP5_IP_2,ETH_TRAP5_IP_3,ETH_TRAP5_IP_4,'(',(sub_ind1+1));
		else ip2lcd(ETH_TRAP5_IP_1,ETH_TRAP5_IP_2,ETH_TRAP5_IP_3,ETH_TRAP5_IP_4,'(',0);
		}

/*	if((sub_ind==2)&&(sub_ind1==0)&&(bFL2))
		{
		sub_bgnd("   ",'#',-2);
		}
	else int2lcd(ETH_IP_1,'#',0);

	if((sub_ind==2)&&(sub_ind1==1)&&(bFL2))
		{
		sub_bgnd("   ",'$',-2);
		}
	else int2lcd(ETH_IP_2,'$',0);

	if((sub_ind==2)&&(sub_ind1==2)&&(bFL2))
		{
		sub_bgnd("   ",'%',-2);
		}
	else int2lcd(ETH_IP_3,'%',0);

	if((sub_ind==2)&&(sub_ind1==3)&&(bFL2))
		{
		sub_bgnd("   ",'^',-2);
		}
	else int2lcd(ETH_IP_4,'^',0);*/


	//int2lcdyx(sub_ind,0,1,0);	
	//int2lcdyx(index_set,0,3,0);
	//int2lcdyx(sub_ind1,0,5,0);
	//for(i=0;(i<9)&&(snmp_community[i]))

	for(i=0;i<9;i++)
		{
		sss[i]=snmp_community[i];
		}
	sss[9]=0;		

	if(sub_ind==10)community2lcd(sss,'<',sub_ind1,1);
	else community2lcd(sss,'<',sub_ind1,0);
	
	//int2lcdyx(snmp_community[0],0,4,0);
	//int2lcdyx(snmp_community[11],0,9,0);
	//int2lcdyx(snmp_community[2],0,14,0);
	//int2lcdyx(snmp_community[sub_ind1],0,19,0);	
	}

else if(ind==iSet_rav)
	{
	ptrs[0]=" Реле №1            ";
	ptrs[1]=" Реле №2            ";
	ptrs[2]=" Реле №3            ";
	ptrs[3]=" Заводские установки";
	if((sub_ind1==30)&&(bFL2))
	ptrs[3]="                    ";
	ptrs[4]=sm_exit;
	
	if(sub_ind<index_set) index_set=sub_ind;
	else if((sub_ind-index_set)>2) index_set=sub_ind-2;
	bgnd_par(		"   АВАРИЙНЫЕ РЕЛЕ   ",
				ptrs[index_set],
				ptrs[index_set+1],
				ptrs[index_set+2]);
	
	pointer_set(1);
		
	}   


else if(ind==iSet_rav_)
	{
	ptrs[0]=" Авария сети       @";
	ptrs[1]=" Авария батареи    #";
	ptrs[2]=" Авария БПС        $";
	ptrs[3]=" Авария предо-      ";
	ptrs[4]=" хранителей        &";
	ptrs[5]=" Авария сухих       ";
	ptrs[6]=" контактов         %";
	ptrs[7]=" Авария внешней     ";
	ptrs[8]=" температуры       ^";
	ptrs[9]=sm_exit;
	
	if(sub_ind<index_set) index_set=sub_ind;
	else if((sub_ind-index_set)>2) index_set=sub_ind-2;
	bgnd_par(		"   РЕЛЕ АВАРИИ №!   ",
				ptrs[index_set],
				ptrs[index_set+1],
				ptrs[index_set+2]);
	
	pointer_set(1);

	int2lcd(sub_ind1+1,'!',0);
	if(REL_AV[sub_ind1]&0x0001)sub_bgnd(smon,'@',-3);
	else sub_bgnd(smoff,'@',-3);
	if(REL_AV[sub_ind1]&0x0002)sub_bgnd(smon,'#',-3);
	else sub_bgnd(smoff,'#',-3);	
	if(REL_AV[sub_ind1]&0x0004)sub_bgnd(smon,'$',-3);
	else sub_bgnd(smoff,'$',-3);
	if(REL_AV[sub_ind1]&0x0008)sub_bgnd(smon,'&',-3);
	else sub_bgnd(smoff,'&',-3);
	if(REL_AV[sub_ind1]&0x0010)sub_bgnd(smon,'%',-3);
	else sub_bgnd(smoff,'%',-3);
	if(REL_AV[sub_ind1]&0x0020)sub_bgnd(smon,'^',-3);
	else sub_bgnd(smoff,'^',-3);				
	}  
				
else if(ind==iK_pdp)
	{
/*	
const char sm500[]	={" Включите АВ  СЕТЬ  "};
const char sm501[]	={"  БАТАРЕЯ НАГРУЗКА  "};
const char sm502[]	={"   Установите ток   "};
const char sm503[]	={"   нагрузки 4-10А   "};*/
 	
 	bgnd_par(	"Включите авт-ты СЕТЬ",
 			"  БАТАРЕЯ НАГРУЗКА  ",
 			"   Установите ток   ",
 			#if(UKU_VERSION==900)
 			"   нагрузки 4-10А   "
 			#endif
 			#if(UKU_VERSION==300)
 			"   нагрузки 3-4А    "
 			#endif
 			);
     }    

else if(ind==iK)
	{
	ptrs[0]=" Сеть               ";
     ptrs[1]=" Батарея            ";
	ptrs[2]=" БПС1               ";
     ptrs[3]=" БПС2               ";
     ptrs[4]=" Внешние датчики    ";
     ptrs[5]=sm_exit;
	
	if((sub_ind-index_set)>2)index_set=sub_ind-2;
	else if(sub_ind<index_set)index_set=sub_ind;
	bgnd_par("     КАЛИБРОВКА     ",
			ptrs[index_set],
			ptrs[index_set+1],
			ptrs[index_set+2]);

	pointer_set(1);	 
	}    

else if(ind==iK_bat)
	{
	//static char ind_cnt;
	ptrs[0]=" Uбат =     @В      ";
	ptrs[1]=" откалибруйте Uбат  ";
	ptrs[2]="  нажатием љ или њ  ";
     ptrs[3]=" Iбат =     #А      ";
     if(phase==0)
          {
          ptrs[4]="   нажмите ¤ для    ";
          ptrs[5]="калибровки нуля Iбат";
          }
     else          
          {
          ptrs[4]=" откалибруйте Iбат  ";
          ptrs[5]="  нажатием љ или њ  ";
          }
     ptrs[6]=	" tбат =    $°C      ";
     ptrs[7]=	" откалибруйте tбат  ";
     ptrs[8]=	"  нажатием љ или њ  ";
	ptrs[9]=	" U'+'э1        [В   ";
	ptrs[10]=	" U'+'э2        ]В   ";
	ptrs[11]=	" U'+'э3        {В   ";
	ptrs[12]=	" U'+'э4        }В   ";
	ptrs[13]=sm_exit;
	ptrs[14]=sm_;
	ptrs[15]=sm_;
	if((AUSW_MAIN/100)==48)
		{
		ptrs[12]=sm_exit;
		ptrs[13]=sm_;
		ptrs[14]=sm_;
		}
     
     
	/*if(sub_ind==0)ptrs[4]="  откалибруйте Uбат ";
	else if(sub_ind==1)
	     {
	     ind_cnt++;
	     if(ind_cnt<15)ptrs[4]         ="Для калибровки нуля ";
          else if(ind_cnt<30)ptrs[4]    ="нажмите 0, или < >  ";
          else if(ind_cnt<45)ptrs[4]    ="для калибровки Iбат ";
          if(ind_cnt>=45)ind_cnt=0;
	     }
	else if(sub_ind==2)ptrs[4]="  откалибруйте tбат ";*/     
	//if((sub_ind-index_set)>2)index_set=sub_ind-2;
	//else if(sub_ind<index_set)index_set=sub_ind;
	if((sub_ind==0)||(sub_ind==1)||(sub_ind==2))index_set=0;
	else if((sub_ind==3)||(sub_ind==4)||(sub_ind==5))index_set=3;
	else if((sub_ind==6)||(sub_ind==7)||(sub_ind==8))index_set=6;
	else 
		{
		if((sub_ind-index_set)>2)index_set=sub_ind-2;
		else if(sub_ind<index_set)index_set=sub_ind;
		}
	bgnd_par(" КАЛИБРОВКА БАТАРЕИ ",ptrs[index_set],ptrs[index_set+1],ptrs[index_set+2]);

	pointer_set(1);	
	int2lcd(Ubat,'@',1);
	int2lcd_m(Ibat,'#',2);
	int2lcd(Ubat_part[0],'[',1);
	int2lcd(Ubat_part[1],']',1);
	int2lcd(Ubat_part[2],'{',1);
	int2lcd(Ubat_part[3],'}',1);
	if(NDB&&bFL2)sub_bgnd("неисправен",'$',-5);
	else int2lcd_mmm(t_b,'$',0);
	
	//int2lcdyx(adc_buff_[5],0,4,0);
	//int2lcdyx(adc_buff_[8],0,19,0); 
	//int2lcdyx(Kubat,0,14,0);
	//int2lcdyx(Ktbat,0,14,0);	
	//int2lcdyx(Kibat1,0,19,0);
	
	/*int2lcdyx(cnt_src[0],2,19,0);
	int2lcdyx(cnt_src[1],3,19,0);*/
	
	if((sub_ind==3)&&(phase==0))MSG_IND2OUT_EN_RELSAM=1;
     MSG_IND2OUT_DIS_SRC1=1;
     MSG_IND2OUT_DIS_SRC2=1;
     //if((sub_ind==1)&&(phase==0))MSG_IND2OUT_EN_RELSAM=1;
	}  	

else if(ind==iK_src)
	{
	//if((phase==1)&&(Ibat>-10)&&(Ibat<10))phase=2;
	ptrs[0]=" Iист =    #А       ";
     if((phase==0)/*||(phase==1)*/)
          {
          ptrs[1]="   нажмите ¤ для    ";
          ptrs[2]="калибровки нуля Iист";
          }
/*     else if(phase==1)
          {
          ptrs[1]="     отключите      ";
          ptrs[2]="    АВ   БАТАРЕЯ    ";
          }  */   
     else          
          {
          ptrs[1]=" откалибруйте Iист  ";
          ptrs[2]="  нажатием љ или њ  ";
          }	
     ptrs[3]=" Uист =    @В       ";
     ptrs[4]=" откалибруйте Uист  ";
     ptrs[5]="  нажатием љ или њ  "; 
     ptrs[6]=" tист =   $°C       ";    
     if(ND[sub_ind1])
          {
          ptrs[7]=sm_;
          if(bFL5)ptrs[7]=" датчик неисправен  ";
          ptrs[8]=sm_;
          }
     else         
          {
          ptrs[7]=" откалибруйте tист  ";
          ptrs[8]="  нажатием љ или њ  ";
          }	
     ptrs[9]=sm_exit;
     ptrs[10]=sm_;
     ptrs[11]=sm_;

     if((sub_ind==0)||(sub_ind==1)||(sub_ind==2))index_set=0;
	else if((sub_ind==3)||(sub_ind==4)||(sub_ind==5))index_set=3;
	else if((sub_ind==6)||(sub_ind==7)||(sub_ind==8))index_set=6;
	else index_set=9;
	
	bgnd_par("  КАЛИБРОВКА БПС!   ",ptrs[index_set],ptrs[index_set+1],ptrs[index_set+2]);

	pointer_set(1);	
	int2lcd(sub_ind1+1,'!',0);
	int2lcd(Us[sub_ind1],'@',1);
	int2lcd(Is[sub_ind1],'#',1);
	int2lcd(t_i[sub_ind1],'$',0); 
	//int2lcdyx(adc_buff_[sub_ind1],2,5,0);
	//int2lcdyx(phase,0,0,0);
	//int2lcdyx(MSG_IND2OUT_DIS_SRC1,0,3,0);
	//int2lcdyx(/*MSG_IND2OUT_EN_SRC1*/Kisrc[0],0,5,0);
	//int2lcdyx(MSG_IND2OUT_DIS_SRC2,0,7,0);
	//int2lcdyx(/*MSG_IND2OUT_EN_SRC2*/Kisrc[1],0,9,0); 
	if(sub_ind1==0)
	     {
	     if(sub_ind==0)
			{
			if(phase==0)
	          	{
	          	MSG_IND2OUT_DIS_SRC1=1;
	          	MSG_IND2OUT_EN_SRC2=1;
	          	}
			else           
	          	{
	          	MSG_IND2OUT_EN_SRC1=1;
	          	MSG_IND2OUT_DIS_SRC2=1;
	          	}
			}
		else
			{
			MSG_IND2OUT_EN_SRC1=1;
	          MSG_IND2OUT_DIS_SRC2=1;
			}
	     }
	 else if(sub_ind1==1)
	     {
	     if(sub_ind==0)
			{
			if(phase==0)
	          	{
	          	MSG_IND2OUT_DIS_SRC2=1;
	          	MSG_IND2OUT_EN_SRC1=1;
	          	}
			else           
	          	{
	          	MSG_IND2OUT_EN_SRC2=1;
	          	MSG_IND2OUT_DIS_SRC1=1;
	          	}
			}
		else
			{
			MSG_IND2OUT_EN_SRC2=1;
	          MSG_IND2OUT_DIS_SRC1=1;
			}
	     }
	MSG_IND2PWM_SRC1=900;
	MSG_IND2PWM_SRC2=900;
	MSG_IND2OUT_DIS_BAT=1;         
/*int2lcdyx(sub_ind1,0,0,0);
int2lcdyx(sub_ind,0,1,0);
int2lcdyx(phase,0,2,0);
int2lcdyx(MSG_IND2OUT_DIS_SRC1,0,3,0);
int2lcdyx(MSG_IND2OUT_DIS_SRC2,0,4,0);  
int2lcdyx(MSG_IND2OUT_EN_SRC1,0,5,0);
int2lcdyx(MSG_IND2OUT_EN_SRC2,0,6,0); */

//int2lcdyx(cntrl_stat1,0,19,0); 
//int2lcdyx(cntrl_stat2,0,15,0); 
/*int2lcdyx(adc_buff_[3],2,19,0); */
	//int2lcdyx(adc_buff_[1],0,3,0);
	//int2lcdyx(adc_buff_[3],0,8,0);
	//int2lcdyx(cntrl_stat1,0,13,0);

/*	int2lcdyx(MSG_IND2OUT_DIS_SRC1,0,10,0);
	int2lcdyx(MSG_IND2OUT_DIS_SRC1_OLD,0,11,0);
	int2lcdyx(MSG_IND2OUT_DIS_SRC2,0,12,0);
	int2lcdyx(MSG_IND2OUT_DIS_SRC2_OLD,0,13,0);*/


	//int2lcdyx(cntrl_stat1,0,19,0);  
	}  
	
else if(ind==iK_net)
	{
	ptrs[0]=" U =     @В         ";
     ptrs[1]=sm_exit;
	ptrs[2]=sm_;
	
	if((sub_ind-index_set)>2)index_set=sub_ind-2;
	else if(sub_ind<index_set)index_set=sub_ind;
	bgnd_par("   КАЛИБРОВКА СЕТИ  ",ptrs[index_set],ptrs[index_set+1],ptrs[index_set+2]);

	pointer_set(1);	
	int2lcd(Unet,'@',0);

	//int2lcdyx(unet_buff_,0,4,0);
	//int2lcdyx(Kunet,0,14,0);
     }
     
else if(ind==iK_load)
	{
	ptrs[0]=" Uбат =     @В      ";
     ptrs[1]=" Iбат =     #А      ";
     ptrs[2]=" tбат =    $°C      ";
     ptrs[3]=sm_exit;
	
	if((sub_ind-index_set)>2)index_set=sub_ind-2;
	else if(sub_ind<index_set)index_set=sub_ind;
	bgnd_par(" КАЛИБРОВКА БАТАРЕИ ",ptrs[index_set],ptrs[index_set+1],ptrs[index_set+2]);

	pointer_set(1);	
	int2lcd(Ubat,'@',1);
	int2lcd_m(Ibat,'#',2);
	int2lcd(t_b,'$',0); 
	} 

else if(ind==iK_t_out)
	{
     ptrs[0]=" tвнешн1=      !°С  ";
     ptrs[NUMDT]=sm_exit;
	ptrs[NUMDT+1]="                    ";
	ptrs[NUMDT+2]="                    ";

	if((sub_ind-index_set)>1)index_set=sub_ind-1;
	else if(sub_ind<index_set)index_set=sub_ind;
	bgnd_par(" КАЛИБРОВКА ВНЕШНИХ ","      ДАТЧИКОВ      ",ptrs[index_set],ptrs[index_set+1]);

	pointer_set(2);	
	if(ND_out[0]&&bFL2)sub_bgnd("неисправен",'!',-5);
	else int2lcd_mmm(tout[0],'!',0);
/*	if(ND_out[1]&&bFL2)sub_bgnd("неисправен",'@',-5);
	else int2lcd_mmm(tout[1],'@',0);
	if(ND_out[2]&&bFL2)sub_bgnd("неисправен",'#',-5);
	else int2lcd_mmm(tout[2],'#',0);  */
	
	//int2lcdyx(adc_buff_out_[0],0,4,0);
	//int2lcdyx(adc_buff_out_[1],0,10,0);
	//int2lcdyx(adc_buff_out_[2],0,17,0);
	} 
	
else if(ind==iJ_bat)
	{
	if(BAT_IS_ON==bisON)ptrs[0]=" Введена  0!/@  /0# ";
	else ptrs[0]=" Выведена 0!/@  /0# ";
     ptrs[1]=" Номин.емк.     $A*ч";
     ptrs[2]=" Наработка      %ч. ";
     ptrs[3]=" Контроль емкости   ";
     ptrs[4]=" Выравнивающий заряд";
     ptrs[5]=" Разряды            ";
     ptrs[6]=sm_exit;	
	if((sub_ind-index_set)>2)index_set=sub_ind-2;
	else if(sub_ind<index_set)index_set=sub_ind;
	bgnd_par(" БАТАРЕЙНЫЙ ЖУРНАЛ  ",ptrs[index_set],ptrs[index_set+1],ptrs[index_set+2]);
	pointer_set(1);	

	int2lcd(BAT_DAY_OF_ON,'!',0);
	sub_bgnd(sm_mont[BAT_MONTH_OF_ON],'@',0);
	int2lcd(BAT_YEAR_OF_ON,'#',0); 
	
	
	/*if(BAT_C_IZM!=0x5555)int2lcd(BAT_C_IZM,'$',0);
	else*/
	int2lcd(BAT_C_NOM,'$',0);
	int2lcd(BAT_RESURS,'%',0);
	
	/*int2lcd(sec__,'&',0);
 	int2lcd(min__,'^',0);
 	int2lcd(hour__,'%',0);
 	
 	int2lcd(RTC_DOM,'<',0);
 	sub_bgnd(sm_month[month__],'>');
 	int2lcd(lc640_read_int(KE_CNT),'{',0);*/
	//int2lcdyx(lc640_read_int(KE_CNT),3,8,0);
	//int2lcdyx(lc640_read_int(KE_PTR),3,14,0);
	}
else if(ind==iPrl_bat_in_out)
	{
	if(BAT_IS_ON==bisON)ptrs[0]="Для выведения бат.-и";
	else  ptrs[0]="Для введения батареи";
	bgnd_par(ptrs[0],"  наберите пароль   ",sm_,sm_);
	
     int2lcdyx(parol[0],2,8,0);
     int2lcdyx(parol[1],2,9,0);
     int2lcdyx(parol[2],2,10,0);
     lcd_buffer[68+sub_ind]='¤';	
   //  int2lcdyx(sub_ind,2,18,0); 
    //  int2lcdyx(but,3,18,0);                
	}		   

else if(ind==iJ_bat_ke_sel)
	{
	if(av_j_si_max==0)
		{
		bgnd_par(	"      БАТАРЕЯ       ",
				"  КОНТРОЛИ ЕМКОСТИ  ",
		         	" Журнал пуст        ",
		         	" Выход              ");
		lcd_buffer[60]=1;
		sub_ind=0;
		index_set=0;
		}
	else if(av_j_si_max==1)
		{
		bgnd_par(	"      БАТАРЕЯ       ",
				"  КОНТРОЛИ ЕМКОСТИ  ",
				" (                  ",
				" Выход              ");
		pointer_set(2);
		}	
	else
		{
		if(sub_ind<index_set) index_set=sub_ind;
		else if((sub_ind-index_set)>1) index_set=sub_ind-1;
		if(index_set==(av_j_si_max-1)) 
		
		bgnd_par(	"      БАТАРЕЯ       ",
				"  КОНТРОЛИ ЕМКОСТИ  ",
				" (                  ",
				" Выход              ");

		else 
		bgnd_par(	"      БАТАРЕЯ       ",
				"  КОНТРОЛИ ЕМКОСТИ  ",
				" (                  ",
				" [                  ");

		pointer_set(2);
		}
		
 	event_data2ind(content[index_set],'(');
 	event_data2ind(content[index_set+1],'[');
     }

else if(ind==iJ_bat_ke)
	{
	unsigned short tempUI,tempUI_;
//	unsigned long tempUL;
	char av_head[4],av_data_on[8],av_data_off[8],av_data[4];
	short av_head_int[2];
	
	bgnd_par(sm_,sm_,sm_,sm_);

     tempUI=(unsigned)sub_ind2;
	tempUI*=32;
	tempUI+=EVENT_LOG;
     
     lc640_read_long_ptr(tempUI,av_head);
     lc640_read_long_ptr(tempUI+4,(char*)av_head_int);
     lc640_read_long_ptr(tempUI+8,av_data_on);
     lc640_read_long_ptr(tempUI+12,&(av_data_on[4])); 
     lc640_read_long_ptr(tempUI+16,av_data_off);
     lc640_read_long_ptr(tempUI+20,&(av_data_off[4]));      
	lc640_read_long_ptr(tempUI+24,av_data);  
	
	
	ptrs[0]=" Начало             ";
	ptrs[1]=" 0%/(  /0^ 0@:0#:0$ ";

	if((av_data_off[0]=='A')&&(av_data_off[1]=='A'))
			{
			ptrs[2]=" Не завершен        ";
			ptrs[3]="			   	    ";
			av_j_si_max_=0;                    
			}
	else
		{
		ptrs[2]=" Окончание          ";
		ptrs[3]=" 0q/w  /0r 0i:0l:0s ";
		ptrs[4]=" Ёмкость     zа*ч.  ";
		av_j_si_max_=2;
		}


	bgnd_par(	"  КОНТРОЛЬ ЕМКОСТИ  ",
			ptrs[index_set],
			ptrs[index_set+1],
			ptrs[index_set+2]);

		
	int2lcd(av_data_on[4],'@',0);
	int2lcd(av_data_on[5],'#',0);
	int2lcd(av_data_on[6],'$',0);
	int2lcd(av_data_on[2],'%',0);
	int2lcd(av_data_on[0],'^',0); 
	if(!((av_data_on[1]>=1)&&(av_data_on[1]<=12)))av_data_on[1]=1;
	sub_bgnd(sm_mont[av_data_on[1]],'(',0);
		
	int2lcd(av_data_off[4],'i',0);
	int2lcd(av_data_off[5],'l',0);
	int2lcd(av_data_off[6],'s',0);
	int2lcd(av_data_off[2],'q',0);
	int2lcd(av_data_off[0],'r',0); 
	if(!((av_data_off[1]>=1)&&(av_data_off[1]<=12)))av_data_off[1]=1;
	sub_bgnd(sm_mont[av_data_off[1]],'w',0);

	//av_j_si_max=2;				
	//int2lcdyx(av_j_si_max,0,3,0);
	//int2lcdyx(index_set,0,9,0);
 /*	
	int2lcdxy(sub_ind,0x60,0);
	
	int2lcdxy(sub_ind2,0xc0,0);*/	

	int2lcd(av_head_int[0],'z',1);
	}

else if(ind==iJ_bat_vz_sel)
	{
	if(av_j_si_max==0)
		{
		bgnd_par(	"      БАТАРЕЯ       ",
				"ВЫРАВНИВАЮЩ. ЗАЗРЯДЫ",
		         	" Журнал пуст        ",
		         	" Выход              ");
		lcd_buffer[60]=1;
		sub_ind=0;
		index_set=0;
		} 
	else if(av_j_si_max==1)
		{
		bgnd_par(	"      БАТАРЕЯ       ",
				"ВЫРАВНИВАЮЩ. ЗАЗРЯДЫ",
				" (                  ",
				" Выход              ");
		pointer_set(2);		
		index_set=0;
		}	
	else
		{
		if(sub_ind<index_set) index_set=sub_ind;
		else if((sub_ind-index_set)>1) index_set=sub_ind-1;
		if(index_set==(av_j_si_max-1)) 
		bgnd_par(	"      БАТАРЕЯ       ",
				"ВЫРАВНИВАЮЩ. ЗАЗРЯДЫ",
				" (                  ",
				" Выход              ");

		else 
		bgnd_par(	"      БАТАРЕЯ       ",
				"ВЫРАВНИВАЮЩ. ЗАЗРЯДЫ",
				" (                  ",
				" [                  ");
				
		pointer_set(2);

		}

 	event_data2ind(content[index_set],'(');
 	event_data2ind(content[index_set+1],'[');

	}

else if(ind==iJ_bat_vz)
	{
	unsigned short tempUI;
//	unsigned long tempUL;
	char av_head[4],av_data_on[8],av_data_off[8],av_data[4];
	short av_head_int[2];
	
	bgnd_par(sm_,sm_,sm_,sm_);
   //	tempUI=lc640_read_int(PTR_EVENT_LOG);
   //	tempUI=ptr_carry(tempUI,64,-1*((signed)sub_ind2)); 
     tempUI=(unsigned)sub_ind2;
	tempUI*=32;
	tempUI+=EVENT_LOG;
     
     lc640_read_long_ptr(tempUI,av_head);
     lc640_read_long_ptr(tempUI+4,(char*)av_head_int);
     lc640_read_long_ptr(tempUI+8,av_data_on);
     lc640_read_long_ptr(tempUI+12,&(av_data_on[4])); 
     lc640_read_long_ptr(tempUI+16,av_data_off);
     lc640_read_long_ptr(tempUI+20,&(av_data_off[4]));      
	lc640_read_long_ptr(tempUI+24,av_data);  
	
	ptrs[0]=" Начало             ";
	ptrs[1]=" 0%/(  /0^ 0@:0#:0$ ";

	if((av_data_off[0]=='A')&&(av_data_off[1]=='A'))
			{
			ptrs[2]=" Не завершен        ";
			ptrs[3]="			   	    ";
			av_j_si_max_=0;                    
			}
	else
		{
		ptrs[2]=" Окончание          ";
		ptrs[3]=" 0q/w  /0r 0i:0l:0s ";
		av_j_si_max_=2;
		}


	bgnd_par(	"ВЫРАВНИВАЮЩИЙ ЗАЗРЯД",
			ptrs[index_set],
			ptrs[index_set+1],
			ptrs[index_set+2]);

		
	int2lcd(av_data_on[4],'@',0);
	int2lcd(av_data_on[5],'#',0);
	int2lcd(av_data_on[6],'$',0);
	int2lcd(av_data_on[2],'%',0);
	int2lcd(av_data_on[0],'^',0); 
	if(!((av_data_on[1]>=1)&&(av_data_on[1]<=12)))av_data_on[1]=1;
	sub_bgnd(sm_mont[av_data_on[1]],'(',0);
		
	int2lcd(av_data_off[4],'i',0);
	int2lcd(av_data_off[5],'l',0);
	int2lcd(av_data_off[6],'s',0);
	int2lcd(av_data_off[2],'q',0);
	int2lcd(av_data_off[0],'r',0); 
	if(!((av_data_off[1]>=1)&&(av_data_off[1]<=12)))av_data_off[1]=1;
	sub_bgnd(sm_mont[av_data_off[1]],'w',0);

	}	

else if(ind==iJ_bat_wrk_sel)
	{
	if(av_j_si_max==0)
		{
		bgnd_par(	"      БАТАРЕЯ       ",
				"      РАЗРЯДЫ       ",
		         	" Журнал пуст        ",
		         	" Выход              ");
		lcd_buffer[60]=1;
		sub_ind=0;
		index_set=0;
		} 
	else if(av_j_si_max==1)
		{
		bgnd_par(	"      БАТАРЕЯ       ",
				"      РАЗРЯДЫ       ",
				" (                  ",
				" Выход              ");
		pointer_set(2);		
		index_set=0;
		}	
	else
		{
		if(sub_ind<index_set) index_set=sub_ind;
		else if((sub_ind-index_set)>1) index_set=sub_ind-1;
		if(index_set==(av_j_si_max-1)) 
		bgnd_par(	"      БАТАРЕЯ       ",
				"      РАЗРЯДЫ       ",
				" (                  ",
				" Выход              ");

		else 
		bgnd_par(	"      БАТАРЕЯ       ",
				"      РАЗРЯДЫ       ",
				" (                  ",
				" [                  ");
				
		pointer_set(2);

		}

 	event_data2ind(content[index_set],'(');
 	event_data2ind(content[index_set+1],'[');
	}


else if(ind==iJ_bat_wrk)
	{
	unsigned short tempUI,tempUI_;
	unsigned long tempUL;
	char av_head[4],av_data_on[8],av_data_off[8],av_data[4];
	short av_head_int[2];
	
	bgnd_par(sm_,sm_,sm_,sm_);
   //	tempUI=lc640_read_int(PTR_EVENT_LOG);
   //	tempUI=ptr_carry(tempUI,64,-1*((signed)sub_ind2)); 
     tempUI=(unsigned)sub_ind2;
	tempUI*=32;
	tempUI+=EVENT_LOG;
     
     lc640_read_long_ptr(tempUI,av_head);
     lc640_read_long_ptr(tempUI+4,(char*)av_head_int);
     lc640_read_long_ptr(tempUI+8,av_data_on);
     lc640_read_long_ptr(tempUI+12,&(av_data_on[4])); 
     lc640_read_long_ptr(tempUI+16,av_data_off);
     lc640_read_long_ptr(tempUI+20,&(av_data_off[4]));      
	lc640_read_long_ptr(tempUI+24,av_data);  
	
	

	ptrs[0]=" Начало             ";
	ptrs[1]=" 0%/(  /0^ 0@:0#:0$ ";
	ptrs[2]="          Uбат=  <В ";
	ptrs[3]=" Окончание          ";
	ptrs[4]=" 0q/w  /0r 0i:0l:0s ";
	ptrs[5]="          Uбат=  >В ";
	ptrs[6]=" Отдано        zА*ч ";

	bgnd_par(	"      РАЗРЯД        ",
			ptrs[index_set],
			ptrs[index_set+1],
			ptrs[index_set+2]);

		
	int2lcd(av_data_on[4],'@',0);
	int2lcd(av_data_on[5],'#',0);
	int2lcd(av_data_on[6],'$',0);
	int2lcd(av_data_on[2],'%',0);
	int2lcd(av_data_on[0],'^',0); 
	if(!((av_data_on[1]>=1)&&(av_data_on[1]<=12)))av_data_on[1]=1;
	sub_bgnd(sm_mont[av_data_on[1]],'(',0);
		
	int2lcd(av_data_off[4],'i',0);
	int2lcd(av_data_off[5],'l',0);
	int2lcd(av_data_off[6],'s',0);
	int2lcd(av_data_off[2],'q',0);
	int2lcd(av_data_off[0],'r',0); 
	if(!((av_data_off[1]>=1)&&(av_data_off[1]<=12)))av_data_off[1]=1;
	sub_bgnd(sm_mont[av_data_off[1]],'w',0);
	
	
	int2lcd(av_head_int[0],'z',1);
	int2lcd(av_data_on[3]+(av_data_on[7]*256),'<',1);
	int2lcd(av_head_int[1],'>',1);	
	

	}

	
else if((ind==iSet_prl)||(ind==iK_prl)||(ind==iSpc_prl_vz)
	||(ind==iSpc_prl_ke)||(ind==iAusw_prl)||(ind==iPrltst))
	{
	bgnd_par("  Введите  пароль   ",sm_,sm_,sm_);
	int2lcdyx(parol[0],1,8,0);
     int2lcdyx(parol[1],1,9,0);
     int2lcdyx(parol[2],1,10,0);
     //lcd_buffer[48+sub_ind]='†';	
     lcd_buffer[48+sub_ind]='¤';
	}	
	
			
//int2lcdyx(u_necc,0,19,0);
//int2lcdyx(cntrl_stat1,0,3,0);
//int2lcdyx(cntrl_stat2,0,7,0);
//int2lcdyx(u_necc,0,19,0);
//int2lcdyx(ind,1,19,0);
//int2lcdyx(sub_ind,2,19,0);
//int2lcdyx(index_set,3,19,0);
//int2lcdyx(Kisrc[0],1,19,0);
//int2lcdyx(Kisrc[1],2,19,0);
//int2lcdyx(retcntsec,7,16,0); 
//int2lcdyx(plazma[0],1,20,0);
//int2lcdyx(plazma[1],2,20,0);

else if(ind==iDnd)
     {
     bgnd_par("      Пароль        "
             ,"    неверный !!!    "
             ,"                    "
             ,"                    ");	
     }
else if(ind==iPdp1)
     {
     bgnd_par("    ВНИМАНИЕ !!!    "
             ,"  Введение батареи  "
             ,"     сотрет все     "
             ,"  предыдущие данные ");	
     }  
//int2lcdyx(ptr_ind,0,0,0);

if((bFL2)&&(fl_simv_len))
	{
	for(i=fl_simv_num;i<fl_simv_num+fl_simv_len;i++)
		{
		lcd_buffer[i]=' ';
		}
	}
/*
if(bFL5)
	{
	for(i=4;i<12;i++)
		{
		lcd_buffer[i]=' ';
		}
	}	*/

/*	int2lcdyx(GET_REG(LPC_GPIO0->FIOPIN,8,1),0,0,0);
	int2lcdyx(GET_REG(LPC_GPIO0->FIOPIN,9,1),0,2,0);
		
	int2lcdyx(GET_REG(LPC_GPIO0->FIOSET,8,1),0,4,0);
	int2lcdyx(GET_REG(LPC_GPIO0->FIOSET,9,1),0,6,0);

	int2lcdyx(bp_tumbler[0],0,8,0);
	int2lcdyx(bp_tumbler[1],0,10,0);
	
	char2lcdhyx(St,0,19);
	char2lcdhyx(St_[0],0,17);
	char2lcdhyx(St_[1],0,15);*/	

//char2lcdhyx(MAIN_BPS,0,19);
		   
} 

//-----------------------------------------------
#define BUT_ON 4
#define BUT_ONL 20 
#define LongPush 50
#define	butOFF 31
#define butU   23
#define butU_  butU+LongPush
#define butD   30
#define butD_  butD+LongPush
#define butL   29
#define butL_  butL+LongPush
#define butR   27
#define butR_  butR+LongPush
#define butE   15
#define butE_  butE+LongPush
#define butUD  22
#define butUD_ butUD+LongPush
#define butLR  25
#define butLR_ butLR+LongPush
#define butEL  13
#define butEL_ butEL+LongPush

void but_an(void)
{
signed short temp_SS;
signed int deep,i,cap,ptr;
char av_head[4];

if(!n_but)goto but_an_end;

if(main_cnt>3)mnemo_cnt=MNEMO_TIME;
av_beep=0x0000;
av_rele=0x0000;
fuse_av_stat[0]=0;
fuse_av_stat[1]=0;
fuse_av_stat[2]=0;

 if(but==butUD)	 
     {
     if(ind!=iDeb)
          {
          b[ptr_ind++]=a;
          ind=iDeb;
          sub_ind=0;
          }
     else 
          {
          a=b[--ptr_ind];
          }     
     }
else if(ind==iDeb)
	{
	if(but==butR)
		{
		sub_ind++;
		index_set=0;
		gran_ring_char(&sub_ind,0,4);
		}
	else if(but==butL)
		{
		sub_ind--;
		index_set=0;
		gran_ring_char(&sub_ind,0,4);
		}
	else if(but==butU)
	     {
	     index_set--;
	     gran_char(&index_set,0,4);
	     //lc640_write_int(ptr_ki_src[0],lc640_read_int(ptr_ki_src[0])+10);
	     }	
     else if(but==butD)
	     {
	     index_set++;
	     gran_char(&index_set,0,4); 
	     //lc640_write_int(ptr_ki_src[0],lc640_read_int(ptr_ki_src[0])-10);
	     }	
     else if(but==butE)
         	{
          a=b[--ptr_ind];
          }            				
	}

else if(ind==iMn)
	{
	if(but==butD)
		{
		sub_ind++;
		gran_char(&sub_ind,0,11);
		//suz_temp=1;
		//snmp_trap_send("ABCDEFGHIJKLMN",15,0,0);
		//snmp_trap_send("Main power is on",2,2,2);
		//reload_hndl();
		}
	else if(but==butU)
		{
		sub_ind--;
		gran_char(&sub_ind,0,11);
		}	
	else if(but==butE_)
		{
		avar_bat_as_hndl();
		}
	else if(but==butL)
		{
		ind=iMn;
		sub_ind=0;
		}		
	else if(sub_ind==1)
		{
		if(but==butE)
		     {
		     tree_up(iBat,0,0,0);
		     ret(1000);
		     }
		}
     else if(sub_ind==2)
		{
		if(but==butE)
		     {
		     tree_up(iSrc,0,0,0);
		     ret(1000);
		     }
		}
     else if(sub_ind==3)
		{
		if(but==butE)
		     {
		     tree_up(iSrc,0,0,1);
		     ret(1000);
		     }
		}		

     else if(sub_ind==4)
		{
		if(but==butE)
		     {
		     tree_up(iLoad,0,0,0);
		     ret(1000);
		     }
		}		

     else if(sub_ind==5)
		{
		if(but==butE)
		     {
		     tree_up(iNet,0,0,0);
		     ret(1000);
		     }
		}	

     else if(sub_ind==6)
		{
		if(but==butE)
		     {
		     tree_up(iSpc,0,0,0);
		     ret(1000);
		     }
		}	

     else if(sub_ind==7)
		{
		if(but==butE)
		     {
		     tree_up(iLog,0,0,0);
		     ret(1000);
		     }
		}	

     else if(sub_ind==8)
		{
		if(but==butE)
		     {
			tree_up(iJ_bat,0,0,0);
		     ret(1000);
			}
		}	

 /*   	else if(sub_ind==9)
		{
		if(but==butE)
		     {
			tree_up(iAusw,0,0,0);
		     ret(1000);
			}
		}*/	

     else if(sub_ind==9)
		{
		if(but==butE)
		     {
			tree_up(iSet_prl,0,0,0);
		     ret(50);
		     parol_init();
			}
		}	
     else if(sub_ind==10)
		{
		if(but==butE)
		     {
			St_[0]&=0xe3;
			St_[1]&=0xe3;
			}
		}		
     else if(sub_ind==11)
		{
		if(but==butE)
		     {
			sub_ind=0;
			}
		}			
	}

else if(ind==iBat)
	{
	char si_max;

	if((AUSW_MAIN/100)==48)si_max=8;
	else si_max=9;
	
	ret(1000);
	if(but==butD)
		{
		sub_ind++;
		gran_char(&sub_ind,0,si_max);
		}
	else if(but==butU)
		{
		sub_ind--;
		gran_char(&sub_ind,0,si_max);
		}
	else if((but==butL)||((sub_ind==si_max)&&(but==butE)))
	     {
	     tree_down(0,0);
	     ret(0);
	     }
	else if(but==butD_)
		{
		sub_ind=si_max;
		}		     
     }

else if(ind==iSrc)
	{
	ret(1000);
	if((but==butD)&&!((sub_ind1==1)&&(NUMIST==1)))
		{
		sub_ind++;
		gran_char(&sub_ind,0,2);
		}
	else if((but==butU)&&!((sub_ind1==1)&&(NUMIST==1)))
		{
		sub_ind--;
		gran_char(&sub_ind,0,2);
		}
	else if((but==butL)||((sub_ind==2)&&(but==butE)))
	     {
	     //a=b[--ptr_ind];
	     tree_down(0,0);
	     ret(0);	     
	     }
	else if(but==butD_)
		{
		sub_ind=2;
		}		     
     }

else if(ind==iLoad)
	{
     if((but==butE)||(but==butL))
	     {
	     tree_down(0,0);
	     ret(0);
	     }
     }  
          
else if(ind==iNet)
	{
     if((but==butE)||(but==butL))
	     {
	     tree_down(0,0);
	     ret(0);
	     }
     }

else if(ind==iSpc)
	{
	ret(1000);
	if(but==butD)
		{
		sub_ind++;
		gran_char(&sub_ind,0,3);
		}
	else if(but==butU)
		{
		sub_ind--;
		gran_char(&sub_ind,0,3);
		}
	else if(but==butD_)
		{
		sub_ind=3;
		}				
	else if(sub_ind==0)
	     {
	     if(but==butE)
	     	{
	     	tree_up(iSpc_prl_ke,0,0,0);
	     	ret(50);
	     	parol_init();
	     	}
	     }		
	else if(sub_ind==1)
		{
		if(but==butE)
	     	{
	     	tree_up(iSpc_prl_vz,0,0,0);
	     	ret(50);
	     	parol_init();
	     	}
	     }
	else if(sub_ind==2)
		{
		if(but==butE)
	     	{
	     	tree_up(iAvz,0,0,0);
			ret(1000);
	     	}
	     }	     
	else if((but==butL)||((sub_ind==3)&&(but==butE)))
	     {
	     tree_down(0,0);
	     ret(0);
	     }
     }

else if(ind==iKe)
	{
	ret(1000);
	if(but==butU)
		{
		sub_ind--;
		gran_char(&sub_ind,0,1);
		}
	else if(but==butD)
		{ 
		sub_ind++;
		gran_char(&sub_ind,0,1);
		}
	else if(but==butD_)
		{
		sub_ind=1;
		}				
	else if(but==butE)
		{
		if(sub_ind==0)
			{
			if(spc_stat==spc_KE/*p1*/) spc_stat=spc_OFF;
			else if (ke_start()==1)
				{
				tree_up(iKednd,0,0,0);
				ret(15);
				}
			else if (ke_start()==2)
				{
				tree_up(iKednd,1,0,0);
				ret(15);				
				}
			else if (ke_start()==3)
				{
				tree_up(iKednd,2,0,0);
				ret(15);				
				}	
			else if (ke_start()==4)
				{
				tree_up(iKednd,3,0,0);
				ret(15);				
				}										
			} 	             
		else if(sub_ind==1)
			{
			//a=b[--ptr_ind];	
			tree_down(-1,0);
			ret(0);			
			} 	
		}
	else if(but==butL)	
	 	{
		tree_down(-1,0);
		ret(0);	
	 	} 
	}
else if(ind==iVz)
	{
	ret(1000);
	if(but==butD)
		{
		sub_ind++;
		gran_char(&sub_ind,0,4);
		}
	else if(but==butU)
		{
		sub_ind--;
		gran_char(&sub_ind,0,4);
		}
	else if(but==butD_)
		{
		sub_ind=4;
		}				
	else if(sub_ind==0)
		{
		speed=1;
		if((but==butR)||(but==butR_))VZ_HR++;
		else if((but==butL)||(but==butL_))VZ_HR--;
		gran(&VZ_HR,1,24);
		lc640_write_int(EE_VZ_HR,VZ_HR);
		}
	else if(sub_ind==1)
		{
		if(spc_stat==spc_VZ)
			{
			spc_stat=spc_OFF;
			vz_mem_hndl(0);
			}
		else if(!(St&0x03)&&(spc_stat==spc_OFF))
			{
			spc_stat=spc_VZ;
			cnt_vz_sec=0L;
			cnt_vz_sec_=(SEC_IN_HOUR*VZ_HR);
			vz_mem_hndl(1);
			}
		} 	  		
	else if(sub_ind==2)
		{
		if(but==butE)
			{
			tree_down(-1,0);
			ret(0);
			}
		}			
	} 

else if(ind==iAvz)
	{
	ret(1000);
	if(but==butD)
		{
		sub_ind++;
		gran_char(&sub_ind,0,simax);
		if(sub_ind==3)sub_ind=4;
		}
	else if(but==butU)
		{
		sub_ind--;
		gran_char(&sub_ind,0,simax);
		if(sub_ind==3)sub_ind=2;
		}
	else if(but==butD_)
		{
		sub_ind=simax;
		}				
	else if(sub_ind==0)
		{
		if(but==butL)
			{
			if(AVZ==AVZ_1)AVZ=AVZ_OFF;
			else if(AVZ==AVZ_2)AVZ=AVZ_1;
			else if(AVZ==AVZ_3)AVZ=AVZ_2; 
			else if(AVZ==AVZ_6)AVZ=AVZ_3;
			else if(AVZ==AVZ_12)AVZ=AVZ_6;			
			else AVZ=AVZ_12;
			lc640_write_int(EE_AVZ,AVZ);
			}
		else if((but==butR)||(but==butE))
			{
			if(AVZ==AVZ_1)AVZ=AVZ_2;
			else if(AVZ==AVZ_2)AVZ=AVZ_3;
			else if(AVZ==AVZ_3)AVZ=AVZ_6; 
			else if(AVZ==AVZ_6)AVZ=AVZ_12;
			else if(AVZ==AVZ_12)AVZ=AVZ_OFF;			
			else AVZ=AVZ_1;
			lc640_write_int(EE_AVZ,AVZ);
			} 
		if(AVZ!=AVZ_OFF)next_date_hndl(AVZ);		
		}       	
	else if(sub_ind==1)
		{
		if((but==butE)&&(simax==1))
			{
			tree_down(0,0);
			}
		else if(simax==4)
			{
			if((but==butL)||(but==butL_))
				{
				speed=1;
				AVZ_TIME--;
				gran(&AVZ_TIME,1,24);
				lc640_write_int(EE_AVZ_TIME,AVZ_TIME);
				}
			else if((but==butR)||(but==butR_))
				{
				speed=1;
				AVZ_TIME++;
				gran(&AVZ_TIME,1,24);
				lc640_write_int(EE_AVZ_TIME,AVZ_TIME);
				}				
			}
		}		
		
	else if(sub_ind==4)
		{
		if((but==butE)&&(simax==4))
			{
			tree_down(0,0);
			ret(0);
			}
		}			
	}
	     
else if(ind==iLog)
	{
	ret_ind_sec(0,0);
	ret_ind(0,0,0);
	if (but==butU)
		{
		sub_ind--;
		gran_char(&sub_ind,0,av_j_si_max+1);
		}
	else if (but==butD)
		{
		sub_ind++;
		gran_char(&sub_ind,0,av_j_si_max+1);
          
		}  

	else if (but==butD_)
		{
		sub_ind=av_j_si_max;
		}  
		
	else if(but==butE)
		{  
		if(sub_ind==av_j_si_max+1)
			{
			lc640_write(CNT_EVENT_LOG,0);
			lc640_write(PTR_EVENT_LOG,0);
	          tree_down(0,0);
	          ret(0);				
			}
					
		else if(sub_ind==av_j_si_max)
			{
	          tree_down(0,0);
	          ret(0);			}
			
		else 
			{
			//ind=iLog_;
			//sub_ind1=sub_ind;
			//index_set=0;
			//sub_ind=0;
			tree_up(iLog_,0,0,sub_ind);
			}	
			
		} 

	else if(but==butR)
		{
	    //	avar_bat_hndl(0,1);	
	    	//avar_bps_hndl(0,1,2);
		ke_mem_hndl(2000);
		}
	else if(but==butR_)
		{
	    	//avar_bat_hndl(0,0);
		//avar_bps_hndl(0,0,2);	
		}		
	else if(but==butL)
		{
	    	//avar_s_hndl(1,0,1);
		//avar_bps_hndl(1,1,1);
		//vz_mem_hndl(0);	
		}
				
	else if(but==butL_)
		{           
		//avar_bps_hndl(1,0,1);	
		}	 		
		
	}

else if(ind==iLog_)
	{          
	if(but==butU)
		{
		index_set--;
		gran_char(&index_set,0,av_j_si_max);
		}
	else if(but==butD)
		{
		if(index_set==av_j_si_max)tree_down(0,0);
		else 
			{
			index_set++;
			gran_char(&index_set,0,av_j_si_max);
			}
		}
	else 
		{
		//ind=iLog;
		//sub_ind=sub_ind1;
		tree_down(0,0);
		}		
	}	

else if(ind==iAusw)
	{
	if(but==butD)
		{
		index_set++;
		gran_char(&index_set,0,8);
		}
	else if(but==butU)
		{
		index_set--;
		gran_char(&index_set,0,8);
		}
	else if(but==butD_)
		{
		index_set=8;
		}				
	else if(but==butE_)
		{
		tree_up(iAusw_prl,0,0,0);
		ret(50);
		parol_init();
		}	
     else if(index_set==8)
	     {
	     if(but==butE)
	          {
	          tree_down(0,0);
	          ret(0);
	          }
	     }		
    else  if(but==butL)
	     {
          tree_down(0,0);
          ret(0);
	     }			     	
	}

else if(ind==iAusw_set)
	{
	if(but==butD)
		{
		sub_ind++;
		if(sub_ind==5)sub_ind++;
		if(sub_ind==6)sub_ind++;
		gran_char(&sub_ind,0,9);
		if((sub_ind>6)&&(index_set<1))index_set=1;
		if((sub_ind>7)&&(index_set<2))index_set=2;
		if((sub_ind>8)&&(index_set<3))index_set=3;
		}
	else if(but==butU)
		{
		sub_ind--;
		if(sub_ind==6)sub_ind--;
		if(sub_ind==5)sub_ind--;
		gran_char(&sub_ind,0,9);
		if((sub_ind<5)&&(index_set>2))index_set=2;
		if((sub_ind<4)&&(index_set>1))index_set=1;
		if((sub_ind<1)&&(index_set))index_set=0;
		}
	else if(but==butD_)
		{
		sub_ind=9;
		}			
	else if(but==butE_)
		{
		fl_simv(0,0,0);
		tree_down(-1,0);
		ret(0);
		}
			
     else if(sub_ind==0)
	     {

#if(UKU_VERSION==900)

#ifdef _24_


	     if((but==butR)||(but==butR_))
	     	{
			if(AUSW_MAIN==2450)AUSW_MAIN=2425;
	          else AUSW_MAIN=2450;
	          lc640_write_int(EE_AUSW_MAIN,AUSW_MAIN);
	          }
	     else if((but==butL)||(but==butL_))
	          {
			if(AUSW_MAIN==2450)AUSW_MAIN=2425;
	          else AUSW_MAIN=2450;
	          lc640_write_int(EE_AUSW_MAIN,AUSW_MAIN);
	          }

#else


	     if((but==butR)||(but==butR_))
	     	{
			if(AUSW_MAIN==6024)AUSW_MAIN=4824;
               else if(AUSW_MAIN==4824)AUSW_MAIN=6012;
               else if(AUSW_MAIN==6012)AUSW_MAIN=4812;
               else if(AUSW_MAIN==4812)AUSW_MAIN=6040;
               else if(AUSW_MAIN==6040)AUSW_MAIN=4840;
               else if(AUSW_MAIN==4840)AUSW_MAIN=6020;
               else if(AUSW_MAIN==6020)AUSW_MAIN=4820;
               else AUSW_MAIN=6024;
	          lc640_write_int(EE_AUSW_MAIN,AUSW_MAIN);
	          }
	     else if((but==butL)||(but==butL_))
	          {
			if(AUSW_MAIN==6024)AUSW_MAIN=4820;
               else if(AUSW_MAIN==4824)AUSW_MAIN=6024;
               else if(AUSW_MAIN==6012)AUSW_MAIN=4824;
               else if(AUSW_MAIN==4812)AUSW_MAIN=6012;
               else if(AUSW_MAIN==6040)AUSW_MAIN=4812;
               else if(AUSW_MAIN==4840)AUSW_MAIN=6040;
               else if(AUSW_MAIN==6020)AUSW_MAIN=4840;
               else AUSW_MAIN=6020;
	          lc640_write_int(EE_AUSW_MAIN,AUSW_MAIN);
	          }

#endif				
#endif 

#if(UKU_VERSION==300)
#ifdef _24_

	     if((but==butR)||(but==butR_))
	     	{
			if(AUSW_MAIN==2424)AUSW_MAIN=2412;
	          else AUSW_MAIN=2424;
	          lc640_write_int(EE_AUSW_MAIN,AUSW_MAIN);
	          }
	     else if((but==butL)||(but==butL_))
	          {
			if(AUSW_MAIN==2424)AUSW_MAIN=2412;
	          else AUSW_MAIN=2424;
	          lc640_write_int(EE_AUSW_MAIN,AUSW_MAIN);
	          }

#else

	     if((but==butR)||(but==butR_))
	     	{
			if(AUSW_MAIN==6010)AUSW_MAIN=4810;
               else if(AUSW_MAIN==4810)AUSW_MAIN=6005;
               else if(AUSW_MAIN==6005)AUSW_MAIN=4805;
               else AUSW_MAIN=6010;
	          lc640_write_int(EE_AUSW_MAIN,AUSW_MAIN);
	          }
	     else if((but==butL)||(but==butL_))
	          {
			if(AUSW_MAIN==6010)AUSW_MAIN=4805;
               else if(AUSW_MAIN==4810)AUSW_MAIN=6010;
               else if(AUSW_MAIN==6005)AUSW_MAIN=4810;
               else AUSW_MAIN=6005;
	          lc640_write_int(EE_AUSW_MAIN,AUSW_MAIN);
	          }


#endif				
#endif    	     	

		}

	          	          
	    	
	     
     else if(sub_ind==1)
	     {
	     speed=1;
	     if(but==butR)
	          {
	          AUSW_DAY++;
	          }
	     else if(but==butR_)
	          {
	          AUSW_DAY+=2;
		     }
	     else if(but==butL)
	          {
	          AUSW_DAY--;
		     }
	     else if(but==butL_)
	          {
	          AUSW_DAY-=2;
		     }
		else if(but==butLR)
	          {
	          lc640_write_int(EE_AUSW_DAY,LPC_RTC->DOM);
	          lc640_write_int(EE_AUSW_MONTH,LPC_RTC->MONTH);
	          lc640_write_int(EE_AUSW_YEAR,LPC_RTC->YEAR);
	          sub_ind=4;
		     }     
		gran(&AUSW_DAY,1,31);     		     		     
		lc640_write_int(EE_AUSW_DAY,AUSW_DAY); 	
	     }
	     	
     else if(sub_ind==2)
	     {
	     speed=1;
	     if(but==butR)
	          {
	          AUSW_MONTH++;
	          }
	     else if(but==butR_)
	          {
	          AUSW_MONTH+=2;
		     }
	     else if(but==butL)
	          {
	          AUSW_MONTH--;
		     }
	     else if(but==butL_)
	          {
	          AUSW_MONTH-=2;
		     }
		else if(but==butLR)
	          {
	          lc640_write_int(EE_AUSW_DAY,LPC_RTC->DOM);
	          lc640_write_int(EE_AUSW_MONTH,LPC_RTC->MONTH);
	          lc640_write_int(EE_AUSW_YEAR,LPC_RTC->YEAR);
	          sub_ind=4;
		     }		     
		gran(&AUSW_MONTH,1,12);     		     		     
		lc640_write_int(EE_AUSW_MONTH,AUSW_MONTH);
	     }		
	     
     else if(sub_ind==3)
	     {
	     speed=1;
	     if(but==butR)
	          {
	          AUSW_YEAR++;
	          }
	     else if(but==butR_)
	          {
	          AUSW_YEAR+=5;
		     }
	     else if(but==butL)
	          {
	          AUSW_YEAR--;
		     }
	     else if(but==butL_)
	          {
	          AUSW_YEAR-=5;
		     }
		else if(but==butLR)
	          {
	          lc640_write_int(EE_AUSW_DAY,LPC_RTC->DOM);
	          lc640_write_int(EE_AUSW_MONTH,LPC_RTC->MONTH);
	          lc640_write_int(EE_AUSW_YEAR,LPC_RTC->YEAR);
	          sub_ind=4;
		     }		     
		gran(&AUSW_YEAR,1,12);     		     		     
		lc640_write_int(EE_AUSW_YEAR,AUSW_YEAR);
	     }		          			     		
	     
     else if(sub_ind==4)
	     {
	     speed=1;
	     if(but==butR)
	          {
	          AUSW_MAIN_NUMBER++;
	          }
	     else if(but==butR_)
	          {
	          AUSW_MAIN_NUMBER+=10;
		     }
	     else if(but==butL)
	          {
	          AUSW_MAIN_NUMBER--;
		     }
	     else if(but==butL_)
	          {
	          AUSW_MAIN_NUMBER-=10;
		     }
		gran_long(&AUSW_MAIN_NUMBER,11000,999999L);     		     		     
		lc640_write_long(EE_AUSW_MAIN_NUMBER,AUSW_MAIN_NUMBER);
		lc640_write_long(EE_AUSW_UKU_NUMBER,AUSW_MAIN_NUMBER);
	     }				     		

    else if(sub_ind==6)
	     {
     	speed=1;
	     if(but==butR)
	          {
	          AUSW_UKU_NUMBER++;
	          }
	     else if(but==butR_)
	          {
	          AUSW_UKU_NUMBER+=10;
		     }
	     else if(but==butL)
	          {
	          AUSW_UKU_NUMBER--;
		     }
	     else if(but==butL_)
	          {
	          AUSW_UKU_NUMBER-=10;
		     }
		gran_long(&AUSW_UKU_NUMBER,11000,999999);     		     		     
		lc640_write_long(EE_AUSW_UKU_NUMBER,AUSW_UKU_NUMBER);
	     }
	     
     else if(sub_ind==7)
	     {
     	speed=1;
	     if(but==butR)
	          {
	          AUSW_BPS1_NUMBER++;
	          }
	     else if(but==butR_)
	          {
	          AUSW_BPS1_NUMBER+=10;
		     }
	     else if(but==butL)
	          {
	          AUSW_BPS1_NUMBER--;
		     }
	     else if(but==butL_)
	          {
	          AUSW_BPS1_NUMBER-=10;
		     }
		gran_long(&AUSW_BPS1_NUMBER,31000,999999);     		     		     
		lc640_write_long(EE_AUSW_BPS1_NUMBER,AUSW_BPS1_NUMBER);
	     }	     
	     
     else if(sub_ind==8)
	     {
     	speed=1;
	     if(but==butR)
	          {
	          AUSW_BPS2_NUMBER++;
	          }
	     else if(but==butR_)
	          {
	          AUSW_BPS2_NUMBER+=10;
		     }
	     else if(but==butL)
	          {
	          AUSW_BPS2_NUMBER--;
		     }
	     else if(but==butL_)
	          {
	          AUSW_BPS2_NUMBER-=10;
		     }
		gran_long(&AUSW_BPS2_NUMBER,31000,999999);     		     		     
		lc640_write_long(EE_AUSW_BPS2_NUMBER,AUSW_BPS2_NUMBER);
	     }	     	     
	     
     else if(sub_ind==90)
	     {
     	speed=1;
	     if(but==butR)
	          {
	          AUSW_RS232++;
	          }
	     else if(but==butR_)
	          {
	          AUSW_RS232+=10;
		     }
	     else if(but==butL)
	          {
	          AUSW_RS232--;
		     }
	     else if(but==butL_)
	          {
	          AUSW_RS232-=10;
		     }
		gran(&AUSW_RS232,0,1000);     		     		     
		lc640_write_long(EE_AUSW_BPS2_NUMBER,AUSW_BPS2_NUMBER);
	     }	
	else if(sub_ind==9)
	     {
	     if(but==butE)
	     	{
	     	tree_down(-1,0);
	     	ret(0);
	     	}
	     }    	     	     
	}


else if(ind==iSet)
	{

/*     ptrs[0]=" Стандартные        ";
	ptrs[1]=" Время и дата       ";
     ptrs[2]=" Структура          ";
     ptrs[3]=" Основной источн. N<";
	ptrs[4]=" Зв.сигн.   (       ";
	ptrs[5]=" Отключение сигнала ";
	ptrs[6]="  аварии    )       ";
	ptrs[7]=" АПВ источников     ";
     ptrs[8]=" Umax=       !В     ";
     ptrs[9]=" Uб0°=       @В     ";
     ptrs[10]=" Uб20°=      #В     ";
     ptrs[11]=" Uсигн=      ^В     ";
     ptrs[12]=" Umin.сети=  &В     ";
	ptrs[13]=" U0б=        >В     ";
	ptrs[14]=" Iбк.=       )А     ";
     ptrs[15]=" Iз.мах.=    <А     ";
     ptrs[16]=" Imax=       ]A     ";
     ptrs[17]=" Kimax=      {      ";
     ptrs[18]=" Kвыр.зар.=    [    ";
     ptrs[19]=" Tз.вкл.а.с. !с     ";
	ptrs[20]=" tmax=       $°C    ";
     ptrs[21]=" Внешние датчики    ";
     ptrs[22]=sm_exit; 
     ptrs[23]=" Калибровки         "; 
     ptrs[24]=" Тест               ";     */	
	ret(1000);
	if(but==butD)
		{
		sub_ind++;
		if(sub_ind==6)index_set=5;
		if(sub_ind==7)sub_ind=8;
		if(sub_ind==9)index_set=8;
		if(sub_ind==10)sub_ind=11;
		if(sub_ind==26)index_set=25;
		if(sub_ind==27)sub_ind=28;
		
		gran_char(&sub_ind,0,31);
		}
	else if(but==butU)
		{
		sub_ind--;
		if(sub_ind==7)sub_ind=6;
		if(sub_ind==10)sub_ind=9;
		if(sub_ind==27)sub_ind=26;
		gran_char(&sub_ind,0,31);
		}
	else if(but==butD_)
		{
		sub_ind=29;
		}
		
	else if(sub_ind==0)
	     {
	     if(but==butE)
	          {
	          tree_up(iDef,0,0,0);
	          ret(1000);
	          default_temp=10;
	          }
	     }	
	
     else if(sub_ind==1)
		{
		if(but==butE)
		     {
		     tree_up(iSet_T,0,0,0);
		     ret(1000);
		     phase=0;
		     }
		}	
					 
     else if(sub_ind==2)
		{
		if(but==butE)
		     {
		     tree_up(iStr,0,0,0);
		     ret(1000);
		     index_set=0;
		     }
		}	
	
	else if(sub_ind==3)
	     {
	     if(but==butR)MNEMO_TIME++;
	     else if(but==butR_)MNEMO_TIME+=10;
	     else if(but==butL)MNEMO_TIME--;
	     else if(but==butL_)MNEMO_TIME-=10;
	    /* else if(but==butE)
	     	{
	     	if(MNEMO_ON==mnON)lc640_write_int(EE_MNEMO_ON,mnOFF);
	     	else lc640_write_int(EE_MNEMO_ON,mnON);
	     	}*/
	     if(((MNEMO_TIME<5)||(MNEMO_TIME>60))&&(MNEMO_ON!=mnOFF))lc640_write_int(EE_MNEMO_ON,mnOFF);	
	     if(((MNEMO_TIME>=5)&&(MNEMO_TIME<=60))&&(MNEMO_ON!=mnON))lc640_write_int(EE_MNEMO_ON,mnON);
	     gran(&MNEMO_TIME,4,61);
	     lc640_write_int(EE_MNEMO_TIME,MNEMO_TIME);
	     speed=1;
	     }
				     		
	else if(sub_ind==4)
	     {
	     if(MAIN_BPS==0)
	     	{
	     	lc640_write_int(EE_MAIN_BPS,1);
	     	cnt_src[0]=10;
	     	}
	     else
	     	{
	     	lc640_write_int(EE_MAIN_BPS,0);
	     	cnt_src[1]=10;
	     	} 
	     }	
	
	
	     
	else if(sub_ind==5)
	     {
		if(ZV_ON)ZV_ON=0;
		else ZV_ON=1;
	     lc640_write_int(EE_ZV_ON,ZV_ON);
	     speed=1;
	     }	
	
	else if(sub_ind==6)
	     {
		if(AV_OFF_AVT)AV_OFF_AVT=0;
		else AV_OFF_AVT=1;
	     lc640_write_int(EE_AV_OFF_AVT,AV_OFF_AVT);
	     speed=1;
	     }	

	else if(sub_ind==8)
	     {
	     if(but==butE)
	          {
	          /*b[ptr_ind++]=a;
	          ind=iApv;
	          sub_ind=0;*/
	          tree_up(iApv,0,0,0);
	          ret(1000);
	          }
	     }	

	else if(sub_ind==9)
	     {
	     if(but==butR)TBAT++;
	     else if(but==butR_)TBAT+=10;
	     else if(but==butL)TBAT--;
	     else if(but==butL_)TBAT-=10;
	     gran(&TBAT,5,60);
	     lc640_write_int(EE_TBAT,TBAT);
	     speed=1;
	     }	
	                    	     	
	else if(sub_ind==11)
	     {
	     if(but==butR)UMAX++;
	     else if(but==butR_)UMAX+=10;
	     else if(but==butL)UMAX--;
	     else if(but==butL_)UMAX-=10;
	     gran(&UMAX,10,1000);
	     lc640_write_int(EE_UMAX,UMAX);
	     speed=1;
	     }
	     
	else if(sub_ind==12)
	     {
	     if(but==butR)UB0++;
	     else if(but==butR_)UB0+=10;
	     else if(but==butL)UB0--;
	     else if(but==butL_)UB0-=10;
	     gran(&UB0,10,1000);
	     lc640_write_int(EE_UB0,UB0);
	     speed=1;
	     }
	     
	else if(sub_ind==13)
	     {
	     if(but==butR)UB20++;
	     else if(but==butR_)UB20+=10;
	     else if(but==butL)UB20--;
	     else if(but==butL_)UB20-=10;
	     gran(&UB20,10,1000);
	     lc640_write_int(EE_UB20,UB20);
	     speed=1;
	     }	

	else if(sub_ind==14)
	     {
	     if(but==butR)USIGN++;
	     else if(but==butR_)USIGN=((USIGN/10)+1)*10;
	     else if(but==butL)USIGN--;
	     else if(but==butL_)USIGN=((USIGN/10)-1)*10;
	     gran(&USIGN,10,1000);
	     lc640_write_int(EE_USIGN,USIGN);
	     speed=1;
	     }	
	else if(sub_ind==15)
	     {
	     if(but==butR)UMN++;
	     else if(but==butR_)UMN+=10;
	     else if(but==butL)UMN--;
	     else if(but==butL_)UMN-=10;
	     gran(&UMN,1,220);
	     lc640_write_int(EE_UMN,UMN);
	     speed=1;
	     }	

	else if(sub_ind==16)
	     {
	     if(but==butR)U0B++;
	     else if(but==butR_)U0B+=10;
	     else if(but==butL)U0B--;
	     else if(but==butL_)U0B-=10;
	     gran(&U0B,10,1000);
	     lc640_write_int(EE_U0B,U0B);
	     speed=1;
	     }	
	     
	else if(sub_ind==17)
	     {
	     if(but==butR)IKB++;
	     else if(but==butR_)IKB+=10;
	     else if(but==butL)IKB--;
	     else if(but==butL_)IKB-=10;
	     gran(&IKB,1,500);
	     lc640_write_int(EE_IKB,IKB);
	     speed=1;
	     }		
            
	else if(sub_ind==18)
	     {
	     if(but==butR)IZMAX++;
	     else if(but==butR_)IZMAX+=10;
	     else if(but==butL)IZMAX--;
	     else if(but==butL_)IZMAX-=10;
	     gran(&IZMAX,10,200);
	     lc640_write_int(EE_IZMAX,IZMAX);
	     speed=1;
	     }   

	else if(sub_ind==19)
	     {
	     if(but==butR)IMAX++;
	     else if(but==butR_)IMAX+=10;
	     else if(but==butL)IMAX--;
	     else if(but==butL_)IMAX-=10;
	     gran(&IMAX,1,500);
	     lc640_write_int(EE_IMAX,IMAX);
	     speed=1;
	     }		
	     
	else if(sub_ind==20)
	     {
	     if(but==butR)KIMAX++;
	     else if(but==butR_)KIMAX+=10;
	     else if(but==butL)KIMAX--;
	     else if(but==butL_)KIMAX-=10;
	     gran(&KIMAX,5,10);
	     lc640_write_int(EE_KIMAX,KIMAX);
	     speed=1;
	     }
	
	else if(sub_ind==21)
	     {
	     if ((but==butR)||(but==butR_))KVZ+=5;
		if ((but==butL)||(but==butL_))KVZ-=5;
		gran(&KVZ,1005,1030); 	          
		lc640_write_int(EE_KVZ,KVZ);
	     speed=1;
	     }
	     
	else if(sub_ind==22)
		{
		if ((but==butR)||(but==butR_))TZAS++;
		if ((but==butL)||(but==butL_))TZAS--;
		gran(&TZAS,0,3);
		lc640_write_int(EE_TZAS,TZAS);
		speed=1; 
		}	
			       	        
	else if(sub_ind==23)
	     {
	     if(but==butR)TMAX++;
	     else if(but==butR_)TMAX+=10;
	     else if(but==butL)TMAX--;
	     else if(but==butL_)TMAX-=10;
	     gran(&TMAX,10,100);
	     lc640_write_int(EE_TMAX,TMAX);
	     speed=1;
	     }	

    else if(sub_ind==24)
		{
		if(but==butE)
		     {
		     tree_up(iLan_set,0,0,0);
		     ret(1000);
		     }
		}		
		     	     	     		     	     
    else if(sub_ind==25)
		{
		if(but==butE)
		     {
		     tree_up(iExt_set,0,0,0);
		     ret(1000);
		     }
		}
	else if(sub_ind==26)
	     {
	     if(but==butR)UBM_AV++;
	     else if(but==butR_)UBM_AV++;
	     else if(but==butL)UBM_AV--;
	     else if(but==butL_)UBM_AV--;
	     gran(&UBM_AV,0,20);
	     lc640_write_int(EE_UBM_AV,UBM_AV);
	     speed=1;
	     }		

    else if(sub_ind==28)
		{
		if(but==butE)
		     {
		     tree_up(iSet_rav,0,0,0);
		     ret(1000);
		     }
		}
				
     else if(sub_ind==29)
		{
		if(but==butE)
		     {
		     tree_down(-1,0);
		     ret(0);
		     }
		}		
	else if(sub_ind==30)
		{
		if(but==butE)
		     {		
			tree_up(iK_prl,0,0,0);
			parol_init();
			ret(50);
			}						
		}			
	else if(sub_ind==31)
		{
		if(but==butE)
		     {
		     tree_up(iPrltst,0,0,0);
		     parol_init();
		     }
		}			
     }		
  
else if(ind==iDef)
	{
	#define SIMAXIDEF 4
	
	ret(1000);
	if(but==butD)
		{
		sub_ind++;
		gran_char(&sub_ind,0,SIMAXIDEF);
		}
	else if(but==butU)
		{
		sub_ind--;
		gran_char(&sub_ind,0,SIMAXIDEF);
		}
	else if(but==butD_)
		{
		sub_ind=SIMAXIDEF;
		}
	
	else if(but==butE)
		{
		if(sub_ind==0)
			{
			def_set(750,705,681,550,30,600,1);
			lc640_write_int(EE_AUSW_MAIN,6012);

			}
		else if(sub_ind==1)
			{
			def_set(750,705,681,550,30,600,2);
			lc640_write_int(EE_AUSW_MAIN,6006);

			}
		else if(sub_ind==2)
			{
			def_set(600,564,545,440,30,480,1);
			lc640_write_int(EE_AUSW_MAIN,4812);
			}				
		else if(sub_ind==3)
			{
			def_set(600,564,545,440,30,480,2);
			lc640_write_int(EE_AUSW_MAIN,4806);
			}
										
		else if(sub_ind==SIMAXIDEF)
			{
			tree_down(0,0);
			ret(0);
			}
		default_temp=sub_ind;	
		}
	}
	     
else if(ind==iStr)
	{
	ret(1000);
	if(but==butD)
		{
		sub_ind++;
		gran_char(&sub_ind,0,4);
		}
	else if(but==butU)
		{
		sub_ind--;
		gran_char(&sub_ind,0,4);
		}
	else if(but==butD_)
		{
		sub_ind=4;
		}				
	else if(sub_ind==0)
	     {
	     if(NUMBAT==0)lc640_write_int(EE_NUMBAT,1);
	     else lc640_write_int(EE_NUMBAT,0);
	     }	
     else if(sub_ind==1)
	     {
          
	     if(NUMIST==1)
               {
               lc640_write_int(EE_NUMIST,2);
               St_[0]&=0x00;
               St_[1]&=0x00;
               }
	     else 
               {
               lc640_write_int(EE_NUMIST,1);
               lc640_write_int(EE_MAIN_BPS,0);
               }
          }	
     else if(sub_ind==2)
	     {
	     if((but==butR)||(but==butR_))
	     	{
	     	NUMDT++;
	     	gran(&NUMDT,0,1);
	     	lc640_write_int(EE_NUMDT,NUMDT);
	     	}
	     
	     else if((but==butL)||(but==butL_))
	     	{
	     	NUMDT--;
	     	gran(&NUMDT,0,1);
	     	lc640_write_int(EE_NUMDT,NUMDT);
	     	}
          }	
     else if(sub_ind==3)
	     {
	     if((but==butR)||(but==butR_))
	     	{
	     	NUMSK++;
	     	gran(&NUMSK,0,4);
	     	lc640_write_int(EE_NUMSK,NUMSK);
	     	}
	     
	     else if((but==butL)||(but==butL_))
	     	{
	     	NUMSK--;
	     	gran(&NUMSK,0,4);
	     	lc640_write_int(EE_NUMSK,NUMSK);
	     	}
          }	                 
    else if(sub_ind==4)
	     {
	     if(but==butE)
	          {
			tree_down(0,0);
	          }
          }	          
	}     


else if (ind==iExt_set)
	{
	char si_max;
	si_max=NUMDT+NUMSK;
	ret(1000);
	if(but==butD)
		{
		sub_ind++;
		gran_char(&sub_ind,0,si_max);
		}
	else if(but==butU)
		{
		sub_ind--;
		gran_char(&sub_ind,0,si_max);
		}
	else if(but==butD_)
		{
		sub_ind=si_max;
		}				
	else if((sub_ind==0)&&(NUMDT))
	     {
	     if(but==butE)
	     	{
	     	tree_up(iExt_dt,1,0,0);
	     	}
	     }
	else if((sub_ind>=NUMDT)&&(sub_ind<si_max))
		{
		tree_up(iExt_sk,1,0,sub_ind-NUMDT);
		}		
    else if(sub_ind==si_max)
	     {
	     if(but==butE)
	          {
	          tree_down(0,0);
	          }
          }	          	
	}
	
else if (ind==iExt_dt)
	{
	ret(1000);
	if(but==butD)
		{
		sub_ind++;
		gran_char(&sub_ind,1,7);
		}
	else if(but==butU)
		{
		if(sub_ind==1)index_set=0;
		else sub_ind--;
		gran_char(&sub_ind,1,7);
		}	
	else if(but==butD_)
		{
		sub_ind=7;
		}			
		
	else if(sub_ind==1) 
		{
		if(but==butE)
			{
			if(!TMAX_EXT_EN[sub_ind1])lc640_write_int(ADR_TMAX_EXT_EN[sub_ind1],0xffff);
			else lc640_write_int(ADR_TMAX_EXT_EN[sub_ind1],0);
			}
		else if((but==butR)||(but==butR_))
			{
			TMAX_EXT[sub_ind1]++;
			}	
		else if((but==butL)||(but==butL_))
			{
			TMAX_EXT[sub_ind1]--;
			}	
		gran(&TMAX_EXT[sub_ind1],-50,100);
		if(lc640_read_int(ADR_TMAX_EXT[sub_ind1])!=TMAX_EXT[sub_ind1]) lc640_write_int(ADR_TMAX_EXT[sub_ind1],TMAX_EXT[sub_ind1]);			
		speed=1;
		}
	else if(sub_ind==2) 
		{
		if(but==butE)
			{
			if(!TMIN_EXT_EN[sub_ind1])lc640_write_int(ADR_TMIN_EXT_EN[sub_ind1],0xffff);
			else lc640_write_int(ADR_TMIN_EXT_EN[sub_ind1],0);
			}
		else if((but==butR)||(but==butR_))
			{
			TMIN_EXT[sub_ind1]++;
			}	
		else if((but==butL)||(but==butL_))
			{
			TMIN_EXT[sub_ind1]--;
			}	
		gran(&TMIN_EXT[sub_ind1],-50,100);
		if(lc640_read_int(ADR_TMIN_EXT[sub_ind1])!=TMIN_EXT[sub_ind1]) lc640_write_int(ADR_TMIN_EXT[sub_ind1],TMIN_EXT[sub_ind1]);			
		speed=1;
		}		
	else if(sub_ind==3) 
		{
		if(but==butE)
			{
			if(!T_EXT_REL_EN[sub_ind1])lc640_write_int(ADR_T_EXT_REL_EN[sub_ind1],0xffff);
			else lc640_write_int(ADR_T_EXT_REL_EN[sub_ind1],0);
			}
		}	
	else if(sub_ind==4) 
		{
		if(but==butE)
			{
			if(!T_EXT_ZVUK_EN[sub_ind1])lc640_write_int(ADR_T_EXT_ZVUK_EN[sub_ind1],0xffff);
			else lc640_write_int(ADR_T_EXT_ZVUK_EN[sub_ind1],0);
			}
		}	
	else if(sub_ind==5) 
		{
		if(but==butE)
			{
			if(!T_EXT_LCD_EN[sub_ind1])lc640_write_int(ADR_T_EXT_LCD_EN[sub_ind1],0xffff);
			else lc640_write_int(ADR_T_EXT_LCD_EN[sub_ind1],0);
			}
		}	
	else if(sub_ind==6) 
		{
		if(but==butE)
			{
			if(!T_EXT_SNMP_TRAP_EN[sub_ind1])lc640_write_int(ADR_T_EXT_SNMP_TRAP_EN[sub_ind1],0xffff);
			else lc640_write_int(ADR_T_EXT_SNMP_TRAP_EN[sub_ind1],0);
			}
		}	
	else if(sub_ind==7) 
		{
		if(but==butE)
			{
			tree_down(0,0);
			//a=b[--ptr_ind];
			}
		}												
	}	

else if (ind==iExt_sk)
	{
	ret(1000);
	if(but==butD)
		{
		sub_ind++;
		if(sub_ind==2)sub_ind=3;
		gran_char(&sub_ind,1,7);
		
		}
	else if(but==butU)
		{
		if(sub_ind==1)index_set=0;
		else sub_ind--;
		if(sub_ind==2)sub_ind=1;
		gran_char(&sub_ind,1,7);
		}	
	else if(but==butD_)
		{
		sub_ind=7;
		}			
	else if(sub_ind==1) 
		{
		if(but==butE)
			{
			if(!SK_SIGN[sub_ind1])lc640_write_int(ADR_SK_SIGN[sub_ind1],0xffff);
			else lc640_write_int(ADR_SK_SIGN[sub_ind1],0);
			}
		}
	else if(sub_ind==3) 
		{
		if(but==butE)
			{
			if(!SK_REL_EN[sub_ind1])lc640_write_int(ADR_SK_REL_EN[sub_ind1],0xffff);
			else lc640_write_int(ADR_SK_REL_EN[sub_ind1],0);
			}
		}	
	else if(sub_ind==4) 
		{
		if(but==butE)
			{
			if(!SK_ZVUK_EN[sub_ind1])lc640_write_int(ADR_SK_ZVUK_EN[sub_ind1],0xffff);
			else lc640_write_int(ADR_SK_ZVUK_EN[sub_ind1],0);
			}
		}	
	else if(sub_ind==5) 
		{
		if(but==butE)
			{
			if(!SK_LCD_EN[sub_ind1])lc640_write_int(ADR_SK_LCD_EN[sub_ind1],0xffff);
			else lc640_write_int(ADR_SK_LCD_EN[sub_ind1],0);
			}
		}	
	else if(sub_ind==6) 
		{
		if(but==butE)
			{
			if(!SK_SNMP_TRAP_EN[sub_ind1])lc640_write_int(ADR_SK_SNMP_TRAP_EN[sub_ind1],0xffff);
			else lc640_write_int(ADR_SK_SNMP_TRAP_EN[sub_ind1],0);
			}
		}	
	else if(sub_ind==7) 
		{
		if(but==butE)
			{
			//a=b[--ptr_ind];
			tree_down(0,0);
			}
		}												
	}	
			
else if(ind==iApv)
     {
     ret(1000);
	if(but==butD)
		{
		sub_ind++;
		gran_char(&sub_ind,0,simax);
		}
	else if(but==butU)
		{
		sub_ind--;
		gran_char(&sub_ind,0,simax);
		}
	else if(but==butD_)
		{
		sub_ind=simax;
		}			
	else if(but==butE)
	     {
	     if(sub_ind==simax)
	          {
	          //a=b[--ptr_ind];
	          tree_down(0,0);
	          }
	     else if(sub_ind==0)   
	          {
	          if(apv_on1==apvON)lc640_write_int(EE_APV_ON1,apvOFF);
	          else lc640_write_int(EE_APV_ON1,apvON);
	          }
          else if((sub_ind==1)&&(apv_on1==apvON))   
	          {
	          if(apv_on2==apvON)lc640_write_int(EE_APV_ON2,apvOFF);
	          else lc640_write_int(EE_APV_ON2,apvON);
	          }	 
          }
     
     else if((sub_ind==2)&&(apv_on2==apvON))   
          {
	     if((but==butR)||(but==butR_))
	          {
	          signed short tempSS;
	          tempSS=apv_on2_time;
	          tempSS++;
	          gran/*_ring*/(&tempSS,1,24);
	          lc640_write_int(EE_APV_ON2_TIME,tempSS);
	          }
          else if((but==butL)||(but==butL_))
	          {
	          signed short tempSS;
	          tempSS=apv_on2_time;
	          tempSS--;
	          gran/*_ring*/(&tempSS,1,24);
	          lc640_write_int(EE_APV_ON2_TIME,tempSS);
	          }	          
	     speed=1;
	     }		
     } 


else if(ind==iSet_T)
	{
	signed char temp;
	if(but==butR)
		{
		sub_ind++;
		gran_char(&sub_ind,0,5);
		}
	else if(but==butL)
		{
		sub_ind--;
		gran_char(&sub_ind,0,5);
		}
	else if(but==butE)
		{
		a=b[--ptr_ind];
		}	
	else if(sub_ind==0)
	     {
	     temp=LPC_RTC->HOUR;
	     if((but==butU)||(but==butU_))
	          {
	          temp++;
	          gran_ring_char(&temp,0,23);
			LPC_RTC->HOUR=temp;
	          }
          else if((but==butD)||(but==butD_))
	          {
	          temp--;
	          gran_ring_char(&temp,0,23);
			LPC_RTC->HOUR=temp;
	          }	
	     speed=1;               
	     }
     else if(sub_ind==1)
	     {
	     temp=LPC_RTC->MIN;
	     if((but==butU)||(but==butU_))
	          {
	          temp++;
	          gran_ring_char(&temp,0,59);
			LPC_RTC->MIN=temp;
	          }
          else if((but==butD)||(but==butD_))
	          {
	          temp--;
	          gran_ring_char(&temp,0,59);
			LPC_RTC->MIN=temp;
	          }	
	     speed=1;               
	     }
     else if(sub_ind==2)
	     {
	     if((but==butU)||(but==butU_))
	          {
	          LPC_RTC->SEC=0;
	          }
          else if((but==butD)||(but==butD_))
	          {
	          LPC_RTC->SEC=0;
	          }	
	     speed=1;               
	     }

     else if(sub_ind==3)
	     {
	     temp=LPC_RTC->DOM;
	     if((but==butU)||(but==butU_))
	          {
	          temp++;
	          gran_ring_char(&temp,1,31);
	          LPC_RTC->DOM=temp;
	          }
          else if((but==butD)||(but==butD_))
	          {
	          temp--;
	          gran_ring_char(&temp,1,31);
	          LPC_RTC->DOM=temp;
	          }	
	     speed=1;               
	     }
     else if(sub_ind==4)
	     {
	     temp=LPC_RTC->MONTH;
	     if((but==butU)||(but==butU_))
	          {
	          temp++;
	          gran_ring_char(&temp,1,12);
	          LPC_RTC->MONTH=temp;
	          }
          else if((but==butD)||(but==butD_))
	          {
	          temp--;
	          gran_ring_char(&temp,1,12);
	          LPC_RTC->MONTH=temp;
	          }	
	     speed=1;               
	     }	  
     else if(sub_ind==5)
	     {
	     temp=LPC_RTC->YEAR;
	     if((but==butU)||(but==butU_))
	          {
	          temp++;
	          gran_ring_char(&temp,0,99);
	          LPC_RTC->YEAR=temp;
	          }
          else if((but==butD)||(but==butD_))
	          {
	          temp--;
	          gran_ring_char(&temp,0,99);
	          LPC_RTC->YEAR=temp;
	          }	
	     speed=1;               
	     }		        
	}
	
else if (ind==iTst_full)
	{
	ret(1000);
	if(but==butU)
     	{
    		sub_ind--;
/*		if(sub_ind==1)
			{
			sub_ind=0;
			index_set=0;
			}
		if(sub_ind==3)
			{
			sub_ind=2;
			index_set=2;
			}
		if(sub_ind==5)
			{
			sub_ind=4;
			index_set=4;
			}  */
/*		if(sub_ind==8)
			{
			sub_ind=9;
			index_set=6;
			} */
		tst_state[0]=tstWRK;
		tst_state[1]=tstWRK;
		tst_state[2]=tstWRK;
		tst_state[3]=tstWRK;
		tst_state[4]=tstWRK;
		tst_state[5]=tstOFF;
		tst_state[6]=tstOFF;
		tst_state[7]=tstOFF;

     	gran_char(&sub_ind,0,8);
     	}
 	else if(but==butD)
     	{
     	sub_ind++;
/*		if(sub_ind==1)
			{
			sub_ind=2;
			index_set=1;
			}
		if(sub_ind==3)
			{
			sub_ind=4;
			index_set=3;
			}
		if(sub_ind==5)
			{
			sub_ind=6;
			//index_set=5;
			}*/
/*		if(sub_ind==8)
			{
			sub_ind=9;
			index_set=9;
			}*/
		tst_state[0]=tstWRK;
		tst_state[1]=tstWRK;
		tst_state[2]=tstWRK;
		tst_state[3]=tstWRK;
		tst_state[4]=tstWRK;
		tst_state[5]=tstOFF;
		tst_state[6]=tstOFF;
		tst_state[7]=tstOFF;

     	gran_char(&sub_ind,0,8);
     	}
	else if(but==butD_)
		{
		sub_ind=8;
		}		     	
 	else if (sub_ind==0)
		{
		if((but==butE)||(but==butR))
			{
 			if(tst_state[0]==tstOFF)tst_state[0]=tstON;
			else if(tst_state[0]==tstON)tst_state[0]=tstWRK;
			else tst_state[0]=tstOFF;
   			}
		else if(but==butL)
			{
 			if(tst_state[0]==tstOFF)tst_state[0]=tstWRK;
			else if(tst_state[0]==tstON)tst_state[0]=tstOFF;
			else tst_state[0]=tstON;
   			}
		 }   
 	else if (sub_ind==1)
		{
		if((but==butE)||(but==butR))
			{
 			if(tst_state[1]==tstOFF)tst_state[1]=tstON;
			else if(tst_state[1]==tstON)tst_state[1]=tstWRK;
			else tst_state[1]=tstOFF;
   			}
		else if(but==butL)
			{
 			if(tst_state[1]==tstOFF)tst_state[1]=tstWRK;
			else if(tst_state[1]==tstON)tst_state[1]=tstOFF;
			else tst_state[1]=tstON;
   			}
		 }  
 	else if (sub_ind==2)
		{
		if((but==butE)||(but==butR))
			{
 			if(tst_state[2]==tstOFF)tst_state[2]=tstON;
			else if(tst_state[2]==tstON)tst_state[2]=tstWRK;
			else tst_state[2]=tstOFF;
   			}
		else if(but==butL)
			{
 			if(tst_state[2]==tstOFF)tst_state[2]=tstWRK;
			else if(tst_state[2]==tstON)tst_state[2]=tstOFF;
			else tst_state[2]=tstON;
   			}
		 }  
 	else if (sub_ind==3)
		{
		if (but==butE)
			{
			tree_up(iTst_klbr,0,0,0);
			ret(1000);
   			}
		 }
 	else if (sub_ind==4)
		{
		if((but==butE)||(but==butR))
			{
 			if(tst_state[3]==tstOFF)tst_state[3]=tstON;
			else if(tst_state[3]==tstON)tst_state[3]=tstWRK;
			else tst_state[3]=tstOFF;
   			}
		else if(but==butL)
			{
 			if(tst_state[3]==tstOFF)tst_state[3]=tstWRK;
			else if(tst_state[3]==tstON)tst_state[3]=tstOFF;
			else tst_state[3]=tstON;
   			}
		 }
    else if (sub_ind==5)
		{
		if (but==butE)
			{
			tree_up(iTst_BPS1,0,0,0);
			ret(1000);
			cnt_src[1]=10;
			tst_state[5]=tstOFF;
			tst_state[6]=tstOFF;
			tst_state[7]=tstOFF;
   			}
		}	

    else if (sub_ind==6)
		{
		if (but==butE)
			{
			tree_up(iTst_BPS2,0,0,0);
			ret(1000);
			cnt_src[1]=10;	
			tst_state[5]=tstOFF;
			tst_state[6]=tstOFF;
			tst_state[7]=tstOFF;
   			}
		}
		
    else if (sub_ind==7)
		{
		if (but==butE)
			{
			tree_up(iTst_BPS12,0,0,0);	
			ret(1000);
			tst_state[5]=tstOFF;
			tst_state[6]=tstOFF;
			tst_state[7]=tstOFF;
   			}
		}
		
    else if (sub_ind==8)
		{
		speed=1; 
		if(but==butE)
			{
             	tree_down(0,0);
             	ret(0);
               St&=0xf8;
           	St_[0]&=0x01;
			St_[1]&=0x01;
			avar_bat_hndl(0);
			avar_unet_hndl(0);
			avar_src_reset(0);
			avar_src_reset(1);
			tst_state[5]=tstOFF;
			tst_state[6]=tstOFF;
			tst_state[7]=tstOFF;
   			}
		}			
  
     	
     	
 		
	}
		     	
else if (ind==iTst)
	{
	ret(1000);
	if(but==butU)
     	{
    		sub_ind--;
    		gran_char(&sub_ind,0,5);
     	}
 	else if(but==butD)
     	{
     	sub_ind++;
     	gran_char(&sub_ind,0,5);
     	}
	else if(but==butD_)
		{
		sub_ind=5;
		}		     	
 	else if (sub_ind==0)
		{
		if(but==butE)
			{
 			if(tst_state[0]!=tstOFF)tst_state[0]=tstOFF;
 			else tst_state[0]=tstON;
   			}
		 }   
 	else if (sub_ind==1)
		{
		if (but==butE)
			{
			tree_up(iTst_klbr,0,0,0);
			ret(1000);
   			}
		 }
    else if (sub_ind==2)
		{
		if (but==butE)
			{
			tree_up(iTst_BPS1,0,0,0);
			ret(1000);
			cnt_src[1]=10;
   			}
		}	

    else if (sub_ind==3)
		{
		if (but==butE)
			{
			tree_up(iTst_BPS2,0,0,0);
			ret(1000);
			cnt_src[1]=10;	
   			}
		}
		
    else if (sub_ind==4)
		{
		if (but==butE)
			{
			tree_up(iTst_BPS12,0,0,0);	
			ret(1000);
   			}
		}
		
    else if (sub_ind==5)
		{
		speed=1; 
		if(but==butE)
			{
             	tree_down(0,0);
             	ret(0);
               St&=0xf8;
           	St_[0]&=0x01;
			St_[1]&=0x01;
			avar_bat_hndl(0);
			avar_unet_hndl(0);
			avar_src_reset(0);
			avar_src_reset(1);
   			}
		}			
  
     	
     	
 		
	}

else if (ind==iTst_klbr)
	{
	ret(1000);
	if(but==butU)
     	{
    		sub_ind=0;
       	}
 	else if(but==butD)
     	{
     	sub_ind=2;
     	}
	else if(but==butD_)
		{
		sub_ind=2;
		}		     	
 	else if (sub_ind==0)
		{
		if((but==butE)||(but==butR))
			{
 			if(tst_state[4]==tstOFF)tst_state[4]=tstON;
			else if(tst_state[4]==tstON)tst_state[4]=tstWRK;
			else tst_state[4]=tstOFF;
   			}
		else if(but==butL)
			{
 			if(tst_state[4]==tstOFF)tst_state[4]=tstWRK;
			else if(tst_state[4]==tstON)tst_state[4]=tstOFF;
			else tst_state[4]=tstON;
   			}
		}
 	else if (sub_ind==2)
		{
		if (but==butE)
			{
 			tree_down(0,0);
 			ret(0);
   			}
	     }	
	}	 

else if (ind==iTst_BPS1)
	{
	ret(1000);
	if(but==butU)
     	{
    		sub_ind=0;
       	}
 	else if(but==butD)
     	{
     	sub_ind=2;
     	}
     else if(but==butD_)
		{
		sub_ind=2;
		}			
 	else if (sub_ind==0)
		{
		if (but==butE)
			{
 			if(tst_state[5]==tstOFF)tst_state[5]=tstON;
 			else if(tst_state[5]==tstON)tst_state[5]=tstU;
 			else tst_state[5]=tstOFF;
   			}
		 }
 	else if (sub_ind==2)
		{
		if (but==butE)
			{
 			tree_down(0,0);
 			ret(0);
			MSG_IND2OUT_EN_BAT=1;
   			}
	     }	
	}	 

else if (ind==iTst_BPS2)
	{
	ret(1000);
	if(but==butU)
     	{
    		sub_ind=0;
       	}
 	else if(but==butD)
     	{
     	sub_ind=2;
     	}
	else if(but==butD_)
		{
		sub_ind=2;
		}		     	
 	else if (sub_ind==0)
		{
		if (but==butE)
			{
 			if(tst_state[6]==tstOFF)tst_state[6]=tstON;
 			else if(tst_state[6]==tstON)tst_state[6]=tstU;
 			else tst_state[6]=tstOFF;
   			}
		 }
 	else if (sub_ind==2)
		{
		if (but==butE)
			{
 			tree_down(0,0);
 			ret(0);
			MSG_IND2OUT_EN_BAT=1;
   			}
	     }	
	}


else if (ind==iTst_BPS12)
	{
	ret(1000);
	if(but==butU)
     	{
    		sub_ind=0;
       	}
 	else if(but==butD)
     	{
     	sub_ind=3;
     	}
	else if(but==butD_)
		{
		sub_ind=3;
		}		     	
 	else if(sub_ind==0)
		{
		if(but==butE)
			{
 			if(tst_state[7]==tstOFF)tst_state[7]=tstON;
 			else if(tst_state[7]==tstON)tst_state[7]=tstU;
 			else tst_state[7]=tstOFF;
   			}
		 }
 	else if(sub_ind==3)
		{
		if(but==butE)
			{
 			tree_down(0,0);
 			ret(0);
			MSG_IND2OUT_EN_BAT=1;
   			}
	     }	
	}	

else if (ind==iLan_set)
	{
	char si_max;
	ret(1000);

	si_max=1;
	if(ETH_IS_ON!=0)si_max=21;

	if(but==butD)
		{
		sub_ind++;

		if((sub_ind==2)&&(index_set==0))
			{
			index_set=1;
			sub_ind1=0;
			}
		if(sub_ind==3) 
			{
			sub_ind=4;
			index_set=3;
			sub_ind1=0;
			}
		if(sub_ind==5) 
			{
			sub_ind=6;
			index_set=5;
			sub_ind1=0;
			}
		if(sub_ind==7) 
			{
			sub_ind=8;
			//index_set=3;
			sub_ind1=0;
			}
		if(sub_ind==10) 
			{
			//sub_ind=6;
			//index_set=9;
			sub_ind1=0;
			}
		if(sub_ind==11) 
			{
			//sub_ind=6;
			index_set=10;
			sub_ind1=0;
			}
		if(sub_ind==12) 
			{
			sub_ind++;
			}
		if(sub_ind==13) 
			{
			//sub_ind=6;
			index_set=12;
			sub_ind1=0;
			}
		if(sub_ind==14) 
			{
			sub_ind++;
			}
		if(sub_ind==15) 
			{
			//sub_ind=6;
			index_set=14;
			sub_ind1=0;
			}
		if(sub_ind==16) 
			{
			sub_ind++;
			}
		if(sub_ind==17) 
			{
			//sub_ind=6;
			index_set=16;
			sub_ind1=0;
			}
		if(sub_ind==18) 
			{
			sub_ind++;
			}
		if(sub_ind==19) 
			{
			//sub_ind=6;
			index_set=18;
			sub_ind1=0;
			}
		if(sub_ind==20) 
			{
			sub_ind++;
			}
	/*	if((sub_ind==4)&&(index_set==2))
			{
			index_set=3;
			sub_ind1=0;
			}*/
		
		gran_char(&sub_ind,0,si_max);
		}
	else if(but==butU)
		{
		sub_ind--;
		gran_char(&sub_ind,0,si_max);
		if(sub_ind==20) 
			{
			sub_ind--;
			}		
		if(sub_ind==18) 
			{
			sub_ind--;
			}		
		if(sub_ind==16) 
			{
			sub_ind--;
			}
		if(sub_ind==14) 
			{
			sub_ind--;
			}
		if(sub_ind==12) 
			{
			sub_ind--;
			}
		if(sub_ind==7) 
			{
			sub_ind--;
			}
		if(sub_ind==5) 
			{
			sub_ind--;
			}
		if(sub_ind==3) 
			{
			sub_ind--;
			}
		}
	else if(but==butD_)
		{
		sub_ind=si_max;
		}

	else if(but==butLR_)
		{
		lc640_write_int(EE_ETH_IS_ON,1);
		lc640_write_int(EE_ETH_DHCP_ON,0);
		lc640_write_int(EE_ETH_IP_1,192);
		lc640_write_int(EE_ETH_IP_2,168);
		lc640_write_int(EE_ETH_IP_3,1);
		lc640_write_int(EE_ETH_IP_4,249);
		lc640_write_int(EE_ETH_MASK_1,255);
		lc640_write_int(EE_ETH_MASK_2,255);
		lc640_write_int(EE_ETH_MASK_3,255);
		lc640_write_int(EE_ETH_MASK_4,0);
		lc640_write_int(EE_ETH_GW_1,192);
		lc640_write_int(EE_ETH_GW_2,168);
		lc640_write_int(EE_ETH_GW_3,1);
		lc640_write_int(EE_ETH_GW_4,254);
		lc640_write_int(EE_ETH_SNMP_PORT_READ,161);
		lc640_write_int(EE_ETH_SNMP_PORT_WRITE,162);
		lc640_write_int(EE_COMMUNITY,'p');
		lc640_write_int(EE_COMMUNITY+2,'u');
		lc640_write_int(EE_COMMUNITY+4,'b');
		lc640_write_int(EE_COMMUNITY+6,'l');
		lc640_write_int(EE_COMMUNITY+8,'i');
		lc640_write_int(EE_COMMUNITY+10,'c');
		lc640_write_int(EE_COMMUNITY+12,' ');
		lc640_write_int(EE_COMMUNITY+14,0);
		lc640_write_int(EE_ETH_TRAP1_IP_1,255);
		lc640_write_int(EE_ETH_TRAP1_IP_2,255);
		lc640_write_int(EE_ETH_TRAP1_IP_3,255);
		lc640_write_int(EE_ETH_TRAP1_IP_4,255);
		lc640_write_int(EE_ETH_TRAP2_IP_1,255);
		lc640_write_int(EE_ETH_TRAP2_IP_2,255);
		lc640_write_int(EE_ETH_TRAP2_IP_3,255);
		lc640_write_int(EE_ETH_TRAP2_IP_4,255);
		lc640_write_int(EE_ETH_TRAP3_IP_1,255);
		lc640_write_int(EE_ETH_TRAP3_IP_2,255);
		lc640_write_int(EE_ETH_TRAP3_IP_3,255);
		lc640_write_int(EE_ETH_TRAP3_IP_4,255);
		lc640_write_int(EE_ETH_TRAP4_IP_1,255);
		lc640_write_int(EE_ETH_TRAP4_IP_2,255);
		lc640_write_int(EE_ETH_TRAP4_IP_3,255);
		lc640_write_int(EE_ETH_TRAP4_IP_4,255);
		lc640_write_int(EE_ETH_TRAP5_IP_1,255);
		lc640_write_int(EE_ETH_TRAP5_IP_2,255);
		lc640_write_int(EE_ETH_TRAP5_IP_3,255);
		lc640_write_int(EE_ETH_TRAP5_IP_4,255);
		}			
						
	else if(sub_ind==0)
	     {
	     if((but==butE)||(but==butL)||(but==butR))
	     	{
	     	if(ETH_IS_ON)lc640_write_int(EE_ETH_IS_ON,0);
			else lc640_write_int(EE_ETH_IS_ON,1);
	     	}
	     }	
     else if((sub_ind==1)&&(ETH_IS_ON))
	     {
		if((but==butE)||(but==butL)||(but==butR))
	     	{
	     	if(ETH_DHCP_ON)lc640_write_int(EE_ETH_DHCP_ON,0);
			else lc640_write_int(EE_ETH_DHCP_ON,1);
	     	}
		}	
     else if(sub_ind==2)
	     {
		if(but==butE_)
	     	{
	     	sub_ind1++;
			gran_ring_char(&sub_ind1,0,3);
	     	}
		else if(sub_ind1==0)
			{
			if((but==butR)||(but==butR_))
				{
				ETH_IP_1++;
				gran_ring(&ETH_IP_1,0,255);
				lc640_write_int(EE_ETH_IP_1,ETH_IP_1);
				}
			else if((but==butL)||(but==butL_))
				{
				ETH_IP_1--;
				gran_ring(&ETH_IP_1,0,255);
				lc640_write_int(EE_ETH_IP_1,ETH_IP_1);
				}
			speed=1;
			}
		else if(sub_ind1==1)
			{
			if((but==butR)||(but==butR_))
				{
				ETH_IP_2++;
				gran_ring(&ETH_IP_2,0,255);
				lc640_write_int(EE_ETH_IP_2,ETH_IP_2);
				}
			else if((but==butL)||(but==butL_))
				{
				ETH_IP_2--;
				gran_ring(&ETH_IP_2,0,255);
				lc640_write_int(EE_ETH_IP_2,ETH_IP_2);
				}
			speed=1;
			}
		else if(sub_ind1==2)
			{
			if((but==butR)||(but==butR_))
				{
				ETH_IP_3++;
				gran_ring(&ETH_IP_3,0,255);
				lc640_write_int(EE_ETH_IP_3,ETH_IP_3);
				}
			else if((but==butL)||(but==butL_))
				{
				ETH_IP_3--;
				gran_ring(&ETH_IP_3,0,255);
				lc640_write_int(EE_ETH_IP_3,ETH_IP_3);
				}
			speed=1;
			}
		else if(sub_ind1==3)
			{
			if((but==butR)||(but==butR_))
				{
				ETH_IP_4++;
				gran_ring(&ETH_IP_4,0,255);
				lc640_write_int(EE_ETH_IP_4,ETH_IP_4);
				}
			else if((but==butL)||(but==butL_))
				{
				ETH_IP_4--;
				gran_ring(&ETH_IP_4,0,255);
				lc640_write_int(EE_ETH_IP_4,ETH_IP_4);
				}
			speed=1;
			}

          }
     else if(sub_ind==4)
	     {
		if(but==butE_)
	     	{
	     	sub_ind1++;
			gran_ring_char(&sub_ind1,0,3);
	     	}
		else if(sub_ind1==0)
			{
			if((but==butR)||(but==butR_))
				{
				ETH_MASK_1++;
				gran_ring(&ETH_MASK_1,0,255);
				lc640_write_int(EE_ETH_MASK_1,ETH_MASK_1);
				}
			else if((but==butL)||(but==butL_))
				{
				ETH_MASK_1--;
				gran_ring(&ETH_MASK_1,0,255);
				lc640_write_int(EE_ETH_MASK_1,ETH_MASK_1);
				}
			speed=1;
			}
		else if(sub_ind1==1)
			{
			if((but==butR)||(but==butR_))
				{
				ETH_MASK_2++;
				gran_ring(&ETH_MASK_2,0,255);
				lc640_write_int(EE_ETH_MASK_2,ETH_MASK_2);
				}
			else if((but==butL)||(but==butL_))
				{
				ETH_MASK_2--;
				gran_ring(&ETH_MASK_2,0,255);
				lc640_write_int(EE_ETH_MASK_2,ETH_MASK_2);
				}
			speed=1;
			}
		else if(sub_ind1==2)
			{
			if((but==butR)||(but==butR_))
				{
				ETH_MASK_3++;
				gran_ring(&ETH_MASK_3,0,255);
				lc640_write_int(EE_ETH_MASK_3,ETH_MASK_3);
				}
			else if((but==butL)||(but==butL_))
				{
				ETH_MASK_3--;
				gran_ring(&ETH_MASK_3,0,255);
				lc640_write_int(EE_ETH_MASK_3,ETH_MASK_3);
				}
			speed=1;
			}
		else if(sub_ind1==3)
			{
			if((but==butR)||(but==butR_))
				{
				ETH_MASK_4++;
				gran_ring(&ETH_MASK_4,0,255);
				lc640_write_int(EE_ETH_MASK_4,ETH_MASK_4);
				}
			else if((but==butL)||(but==butL_))
				{
				ETH_MASK_4--;
				gran_ring(&ETH_MASK_4,0,255);
				lc640_write_int(EE_ETH_MASK_4,ETH_MASK_4);
				}
			speed=1;
			}
		}
     else if(sub_ind==6)
	     {
		if(but==butE_)
	     	{
	     	sub_ind1++;
			gran_ring_char(&sub_ind1,0,3);
	     	}
		else if(sub_ind1==0)
			{
			if((but==butR)||(but==butR_))
				{
				ETH_GW_1++;
				gran_ring(&ETH_GW_1,0,255);
				lc640_write_int(EE_ETH_GW_1,ETH_GW_1);
				}
			else if((but==butL)||(but==butL_))
				{
				ETH_GW_1--;
				gran_ring(&ETH_GW_1,0,255);
				lc640_write_int(EE_ETH_GW_1,ETH_GW_1);
				}
			speed=1;
			}
		else if(sub_ind1==1)
			{
			if((but==butR)||(but==butR_))
				{
				ETH_GW_2++;
				gran_ring(&ETH_GW_2,0,255);
				lc640_write_int(EE_ETH_GW_2,ETH_GW_2);
				}
			else if((but==butL)||(but==butL_))
				{
				ETH_GW_2--;
				gran_ring(&ETH_GW_2,0,255);
				lc640_write_int(EE_ETH_GW_2,ETH_GW_2);
				}
			speed=1;
			}
		else if(sub_ind1==2)
			{
			if((but==butR)||(but==butR_))
				{
				ETH_GW_3++;
				gran_ring(&ETH_GW_3,0,255);
				lc640_write_int(EE_ETH_GW_3,ETH_GW_3);
				}
			else if((but==butL)||(but==butL_))
				{
				ETH_GW_3--;
				gran_ring(&ETH_GW_3,0,255);
				lc640_write_int(EE_ETH_GW_3,ETH_GW_3);
				}
			speed=1;
			}
		else if(sub_ind1==3)
			{
			if((but==butR)||(but==butR_))
				{
				ETH_GW_4++;
				gran_ring(&ETH_GW_4,0,255);
				lc640_write_int(EE_ETH_GW_4,ETH_GW_4);
				}
			else if((but==butL)||(but==butL_))
				{
				ETH_GW_4--;
				gran_ring(&ETH_GW_4,0,255);
				lc640_write_int(EE_ETH_GW_4,ETH_GW_4);
				}
			speed=1;
			}
		}
      else if(sub_ind==8)
	     {
		if(but==butR)ETH_SNMP_PORT_READ++;
		else if(but==butR_)ETH_SNMP_PORT_READ+=10;
		else if(but==butL)ETH_SNMP_PORT_READ--;
		else if(but==butL_)ETH_SNMP_PORT_READ-=10;
		speed=1;
		lc640_write_int(EE_ETH_SNMP_PORT_READ,ETH_SNMP_PORT_READ);
		}

     else if(sub_ind==9)
	     {
		if(but==butR)ETH_SNMP_PORT_WRITE++;
		else if(but==butR_)ETH_SNMP_PORT_WRITE+=10;
		else if(but==butL)ETH_SNMP_PORT_WRITE--;
		else if(but==butL_)ETH_SNMP_PORT_WRITE-=10;
		speed=1;
		lc640_write_int(EE_ETH_SNMP_PORT_WRITE,ETH_SNMP_PORT_WRITE);
		}					
     else if(sub_ind==10)
	     {
		if(but==butE_)
	     	{
	     	sub_ind1++;
			gran_ring_char(&sub_ind1,0,8);
	     	}
		if((but==butR)||(but==butR_))
			{
			snmp_community[sub_ind1]++;
			if(snmp_community[sub_ind1]<32) snmp_community[sub_ind1]=32;
			else if ((snmp_community[sub_ind1]>32)&&(snmp_community[sub_ind1]<48)) snmp_community[sub_ind1]=48;
			else if ((snmp_community[sub_ind1]>57)&&(snmp_community[sub_ind1]<65)) snmp_community[sub_ind1]=65;
			else if ((snmp_community[sub_ind1]>90)&&(snmp_community[sub_ind1]<97)) snmp_community[sub_ind1]=97;
			else if (snmp_community[sub_ind1]>122) snmp_community[sub_ind1]=32;
				//gran_ring(&ETH_GW_1,0,255);
			lc640_write_int(EE_COMMUNITY+(sub_ind1*2),snmp_community[sub_ind1]);
			speed=1;
			}
		if((but==butL)||(but==butL_))
			{
			snmp_community[sub_ind1]--;
			if(snmp_community[sub_ind1]<32) snmp_community[sub_ind1]=122;
			else if ((snmp_community[sub_ind1]>32)&&(snmp_community[sub_ind1]<48)) snmp_community[sub_ind1]=32;
			else if ((snmp_community[sub_ind1]>57)&&(snmp_community[sub_ind1]<65)) snmp_community[sub_ind1]=57;
			else if ((snmp_community[sub_ind1]>90)&&(snmp_community[sub_ind1]<97)) snmp_community[sub_ind1]=90;
			else if (snmp_community[sub_ind1]>122) snmp_community[sub_ind1]=122;
			//gran_ring(&ETH_GW_1,0,255);
			lc640_write_int(EE_COMMUNITY+(sub_ind1*2),snmp_community[sub_ind1]);
			speed=1;
			}
		}
 
     else if(sub_ind==11)
	     {
		if(but==butE_)
	     	{
	     	sub_ind1++;
			gran_ring_char(&sub_ind1,0,3);
	     	}
		else if(sub_ind1==0)
			{
			if((but==butR)||(but==butR_))
				{
				ETH_TRAP1_IP_1++;
				gran_ring(&ETH_TRAP1_IP_1,0,255);
				lc640_write_int(EE_ETH_TRAP1_IP_1,ETH_TRAP1_IP_1);
				}
			else if((but==butL)||(but==butL_))
				{
				ETH_TRAP1_IP_1--;
				gran_ring(&ETH_TRAP1_IP_1,0,255);
				lc640_write_int(EE_ETH_TRAP1_IP_1,ETH_TRAP1_IP_1);
				}
			speed=1;
			}
		else if(sub_ind1==1)
			{
			if((but==butR)||(but==butR_))
				{
				ETH_TRAP1_IP_2++;
				gran_ring(&ETH_TRAP1_IP_2,0,255);
				lc640_write_int(EE_ETH_TRAP1_IP_2,ETH_TRAP1_IP_2);
				}
			else if((but==butL)||(but==butL_))
				{
				ETH_TRAP1_IP_2--;
				gran_ring(&ETH_TRAP1_IP_2,0,255);
				lc640_write_int(EE_ETH_TRAP1_IP_2,ETH_TRAP1_IP_2);
				}
			speed=1;
			}
		else if(sub_ind1==2)
			{
			if((but==butR)||(but==butR_))
				{
				ETH_TRAP1_IP_3++;
				gran_ring(&ETH_TRAP1_IP_3,0,255);
				lc640_write_int(EE_ETH_TRAP1_IP_3,ETH_TRAP1_IP_3);
				}
			else if((but==butL)||(but==butL_))
				{
				ETH_TRAP1_IP_3--;
				gran_ring(&ETH_TRAP1_IP_3,0,255);
				lc640_write_int(EE_ETH_TRAP1_IP_3,ETH_TRAP1_IP_3);
				}
			speed=1;
			}
		else if(sub_ind1==3)
			{
			if((but==butR)||(but==butR_))
				{
				ETH_TRAP1_IP_4++;
				gran_ring(&ETH_TRAP1_IP_4,0,255);
				lc640_write_int(EE_ETH_TRAP1_IP_4,ETH_TRAP1_IP_4);
				}
			else if((but==butL)||(but==butL_))
				{
				ETH_TRAP1_IP_4--;
				gran_ring(&ETH_TRAP1_IP_4,0,255);
				lc640_write_int(EE_ETH_TRAP1_IP_4,ETH_TRAP1_IP_4);
				}
			speed=1;
			}
		}	
     else if(sub_ind==13)
	     {
		if(but==butE_)
	     	{
	     	sub_ind1++;
			gran_ring_char(&sub_ind1,0,3);
	     	}
		else if(sub_ind1==0)
			{
			if((but==butR)||(but==butR_))
				{
				ETH_TRAP2_IP_1++;
				gran_ring(&ETH_TRAP2_IP_1,0,255);
				lc640_write_int(EE_ETH_TRAP2_IP_1,ETH_TRAP2_IP_1);
				}
			else if((but==butL)||(but==butL_))
				{
				ETH_TRAP2_IP_1--;
				gran_ring(&ETH_TRAP2_IP_1,0,255);
				lc640_write_int(EE_ETH_TRAP2_IP_1,ETH_TRAP2_IP_1);
				}
			speed=1;
			}
		else if(sub_ind1==1)
			{
			if((but==butR)||(but==butR_))
				{
				ETH_TRAP2_IP_2++;
				gran_ring(&ETH_TRAP2_IP_2,0,255);
				lc640_write_int(EE_ETH_TRAP2_IP_2,ETH_TRAP2_IP_2);
				}
			else if((but==butL)||(but==butL_))
				{
				ETH_TRAP2_IP_2--;
				gran_ring(&ETH_TRAP2_IP_2,0,255);
				lc640_write_int(EE_ETH_TRAP2_IP_2,ETH_TRAP2_IP_2);
				}
			speed=1;
			}
		else if(sub_ind1==2)
			{
			if((but==butR)||(but==butR_))
				{
				ETH_TRAP2_IP_3++;
				gran_ring(&ETH_TRAP2_IP_3,0,255);
				lc640_write_int(EE_ETH_TRAP2_IP_3,ETH_TRAP2_IP_3);
				}
			else if((but==butL)||(but==butL_))
				{
				ETH_TRAP2_IP_3--;
				gran_ring(&ETH_TRAP2_IP_3,0,255);
				lc640_write_int(EE_ETH_TRAP2_IP_3,ETH_TRAP2_IP_3);
				}
			speed=1;
			}
		else if(sub_ind1==3)
			{
			if((but==butR)||(but==butR_))
				{
				ETH_TRAP2_IP_4++;
				gran_ring(&ETH_TRAP2_IP_4,0,255);
				lc640_write_int(EE_ETH_TRAP2_IP_4,ETH_TRAP2_IP_4);
				}
			else if((but==butL)||(but==butL_))
				{
				ETH_TRAP2_IP_4--;
				gran_ring(&ETH_TRAP2_IP_4,0,255);
				lc640_write_int(EE_ETH_TRAP2_IP_4,ETH_TRAP2_IP_4);
				}
			speed=1;
			}
		}	
     else if(sub_ind==15)
	     {
		if(but==butE_)
	     	{
	     	sub_ind1++;
			gran_ring_char(&sub_ind1,0,3);
	     	}
		else if(sub_ind1==0)
			{
			if((but==butR)||(but==butR_))
				{
				ETH_TRAP3_IP_1++;
				gran_ring(&ETH_TRAP3_IP_1,0,255);
				lc640_write_int(EE_ETH_TRAP3_IP_1,ETH_TRAP3_IP_1);
				}
			else if((but==butL)||(but==butL_))
				{
				ETH_TRAP3_IP_1--;
				gran_ring(&ETH_TRAP3_IP_1,0,255);
				lc640_write_int(EE_ETH_TRAP3_IP_1,ETH_TRAP3_IP_1);
				}
			speed=1;
			}
		else if(sub_ind1==1)
			{
			if((but==butR)||(but==butR_))
				{
				ETH_TRAP3_IP_2++;
				gran_ring(&ETH_TRAP3_IP_2,0,255);
				lc640_write_int(EE_ETH_TRAP3_IP_2,ETH_TRAP3_IP_2);
				}
			else if((but==butL)||(but==butL_))
				{
				ETH_TRAP3_IP_2--;
				gran_ring(&ETH_TRAP3_IP_2,0,255);
				lc640_write_int(EE_ETH_TRAP3_IP_2,ETH_TRAP3_IP_2);
				}
			speed=1;
			}
		else if(sub_ind1==2)
			{
			if((but==butR)||(but==butR_))
				{
				ETH_TRAP3_IP_3++;
				gran_ring(&ETH_TRAP3_IP_3,0,255);
				lc640_write_int(EE_ETH_TRAP3_IP_3,ETH_TRAP3_IP_3);
				}
			else if((but==butL)||(but==butL_))
				{
				ETH_TRAP3_IP_3--;
				gran_ring(&ETH_TRAP3_IP_3,0,255);
				lc640_write_int(EE_ETH_TRAP3_IP_3,ETH_TRAP3_IP_3);
				}
			speed=1;
			}
		else if(sub_ind1==3)
			{
			if((but==butR)||(but==butR_))
				{
				ETH_TRAP3_IP_4++;
				gran_ring(&ETH_TRAP3_IP_4,0,255);
				lc640_write_int(EE_ETH_TRAP3_IP_4,ETH_TRAP3_IP_4);
				}
			else if((but==butL)||(but==butL_))
				{
				ETH_TRAP3_IP_4--;
				gran_ring(&ETH_TRAP3_IP_4,0,255);
				lc640_write_int(EE_ETH_TRAP3_IP_4,ETH_TRAP3_IP_4);
				}
			speed=1;
			}
		}	
     else if(sub_ind==17)
	     {
		if(but==butE_)
	     	{
	     	sub_ind1++;
			gran_ring_char(&sub_ind1,0,3);
	     	}
		else if(sub_ind1==0)
			{
			if((but==butR)||(but==butR_))
				{
				ETH_TRAP4_IP_1++;
				gran_ring(&ETH_TRAP4_IP_1,0,255);
				lc640_write_int(EE_ETH_TRAP4_IP_1,ETH_TRAP4_IP_1);
				}
			else if((but==butL)||(but==butL_))
				{
				ETH_TRAP4_IP_1--;
				gran_ring(&ETH_TRAP4_IP_1,0,255);
				lc640_write_int(EE_ETH_TRAP4_IP_1,ETH_TRAP4_IP_1);
				}
			speed=1;
			}
		else if(sub_ind1==1)
			{
			if((but==butR)||(but==butR_))
				{
				ETH_TRAP4_IP_2++;
				gran_ring(&ETH_TRAP4_IP_2,0,255);
				lc640_write_int(EE_ETH_TRAP4_IP_2,ETH_TRAP4_IP_2);
				}
			else if((but==butL)||(but==butL_))
				{
				ETH_TRAP4_IP_2--;
				gran_ring(&ETH_TRAP4_IP_2,0,255);
				lc640_write_int(EE_ETH_TRAP4_IP_2,ETH_TRAP4_IP_2);
				}
			speed=1;
			}
		else if(sub_ind1==2)
			{
			if((but==butR)||(but==butR_))
				{
				ETH_TRAP4_IP_3++;
				gran_ring(&ETH_TRAP4_IP_3,0,255);
				lc640_write_int(EE_ETH_TRAP4_IP_3,ETH_TRAP4_IP_3);
				}
			else if((but==butL)||(but==butL_))
				{
				ETH_TRAP4_IP_3--;
				gran_ring(&ETH_TRAP4_IP_3,0,255);
				lc640_write_int(EE_ETH_TRAP4_IP_3,ETH_TRAP4_IP_3);
				}
			speed=1;
			}
		else if(sub_ind1==3)
			{
			if((but==butR)||(but==butR_))
				{
				ETH_TRAP4_IP_4++;
				gran_ring(&ETH_TRAP4_IP_4,0,255);
				lc640_write_int(EE_ETH_TRAP4_IP_4,ETH_TRAP4_IP_4);
				}
			else if((but==butL)||(but==butL_))
				{
				ETH_TRAP4_IP_4--;
				gran_ring(&ETH_TRAP4_IP_4,0,255);
				lc640_write_int(EE_ETH_TRAP4_IP_4,ETH_TRAP4_IP_4);
				}
			speed=1;
			}
		}	
     else if(sub_ind==19)
	     {
		if(but==butE_)
	     	{
	     	sub_ind1++;
			gran_ring_char(&sub_ind1,0,3);
	     	}
		else if(sub_ind1==0)
			{
			if((but==butR)||(but==butR_))
				{
				ETH_TRAP5_IP_1++;
				gran_ring(&ETH_TRAP5_IP_1,0,255);
				lc640_write_int(EE_ETH_TRAP5_IP_1,ETH_TRAP5_IP_1);
				}
			else if((but==butL)||(but==butL_))
				{
				ETH_TRAP5_IP_1--;
				gran_ring(&ETH_TRAP5_IP_1,0,255);
				lc640_write_int(EE_ETH_TRAP5_IP_1,ETH_TRAP5_IP_1);
				}
			speed=1;
			}
		else if(sub_ind1==1)
			{
			if((but==butR)||(but==butR_))
				{
				ETH_TRAP5_IP_2++;
				gran_ring(&ETH_TRAP5_IP_2,0,255);
				lc640_write_int(EE_ETH_TRAP5_IP_2,ETH_TRAP5_IP_2);
				}
			else if((but==butL)||(but==butL_))
				{
				ETH_TRAP5_IP_2--;
				gran_ring(&ETH_TRAP5_IP_2,0,255);
				lc640_write_int(EE_ETH_TRAP5_IP_2,ETH_TRAP5_IP_2);
				}
			speed=1;
			}
		else if(sub_ind1==2)
			{
			if((but==butR)||(but==butR_))
				{
				ETH_TRAP5_IP_3++;
				gran_ring(&ETH_TRAP5_IP_3,0,255);
				lc640_write_int(EE_ETH_TRAP5_IP_3,ETH_TRAP5_IP_3);
				}
			else if((but==butL)||(but==butL_))
				{
				ETH_TRAP5_IP_3--;
				gran_ring(&ETH_TRAP5_IP_3,0,255);
				lc640_write_int(EE_ETH_TRAP5_IP_3,ETH_TRAP5_IP_3);
				}
			speed=1;
			}
		else if(sub_ind1==3)
			{
			if((but==butR)||(but==butR_))
				{
				ETH_TRAP5_IP_4++;
				gran_ring(&ETH_TRAP5_IP_4,0,255);
				lc640_write_int(EE_ETH_TRAP5_IP_4,ETH_TRAP5_IP_4);
				}
			else if((but==butL)||(but==butL_))
				{
				ETH_TRAP5_IP_4--;
				gran_ring(&ETH_TRAP5_IP_4,0,255);
				lc640_write_int(EE_ETH_TRAP5_IP_4,ETH_TRAP5_IP_4);
				}
			speed=1;
			}
		}													          
    else if(sub_ind==si_max)
	     {
	     if(but==butE)
	          {
	          tree_down(0,0);
	          }
          }	          	
	}

else if(ind==iSet_rav)
	{
	ret(1000);
	if(but==butD)
		{
		sub_ind++;
		gran_char(&sub_ind,0,4);
		}
	else if(but==butU)
		{
		sub_ind--;
		gran_char(&sub_ind,0,4);
		}
	else if(but==butD_)
		{
		sub_ind=4;
		}				
	
     else if((sub_ind>=0)&&(sub_ind<=2))
	     {
          if(but==butE)
			{
			tree_up(iSet_rav_,0,0,sub_ind);
			}
          }	
     else if(sub_ind==3)
	     {
          if(but==butE)
			{
			sub_ind1=30;
			lc640_write_int(EE_REL_AV1,0x0001); 
			lc640_write_int(EE_REL_AV2,0x0006);
			lc640_write_int(EE_REL_AV3,0x0038);
			}
		}

     else if(sub_ind==4)
	     {
	     if(but==butE)
	          {
			tree_down(0,0);
	          }
          }	          
	}     
else if(ind==iSet_rav_)
	{
	if(but==butD)
		{
		sub_ind++;
		if(sub_ind==3)
			{
			//sub_ind=3;
			index_set=2;
			}
		if(sub_ind==4)
			{
			sub_ind=5;
			index_set=4;
			}
		if(sub_ind==6)
			{
			sub_ind=7;
			index_set=6;
			}
		if(sub_ind==8)
			{
			sub_ind=9;
			//index_set=8;
			}
		gran_char(&sub_ind,0,9);
		}
	else if(but==butU)
		{
		sub_ind--;
		if(sub_ind==4)
			{
			sub_ind=3;
			}
		if(sub_ind==6)
			{
			sub_ind=5;
			}
		if(sub_ind==8)
			{
			sub_ind=7;
			}
		gran_char(&sub_ind,0,9);
		}
	else if(but==butD_)
		{
		sub_ind=9;
		}

	else if(sub_ind==0)
		{
		if((but==butR)||(but==butR_))
			{
			REL_AV[sub_ind1]|=(1<<0);
			}
		if((but==butL)||(but==butL_))
			{
			REL_AV[sub_ind1]&=~(1<<0);
			}
		if((but==butE)||(but==butE_))
			{
			REL_AV[sub_ind1]^=(1<<0);
			}
		if(sub_ind1==0)lc640_write_int(EE_REL_AV1,REL_AV[0]); 
		if(sub_ind1==1)lc640_write_int(EE_REL_AV2,REL_AV[1]);
		if(sub_ind1==2)lc640_write_int(EE_REL_AV3,REL_AV[2]);
		}	
	else if(sub_ind==1)
		{
		if((but==butR)||(but==butR_))
			{
			REL_AV[sub_ind1]|=(1<<1);
			}
		if((but==butL)||(but==butL_))
			{
			REL_AV[sub_ind1]&=~(1<<1);
			}
		if((but==butE)||(but==butE_))
			{
			REL_AV[sub_ind1]^=(1<<1);
			}
		if(sub_ind1==0)lc640_write_int(EE_REL_AV1,REL_AV[0]); 
		if(sub_ind1==1)lc640_write_int(EE_REL_AV2,REL_AV[1]);
		if(sub_ind1==2)lc640_write_int(EE_REL_AV3,REL_AV[2]);
		}
	else if(sub_ind==2)
		{
		if((but==butR)||(but==butR_))
			{
			REL_AV[sub_ind1]|=(1<<2);
			}
		if((but==butL)||(but==butL_))
			{
			REL_AV[sub_ind1]&=~(1<<2);
			}
		if((but==butE)||(but==butE_))
			{
			REL_AV[sub_ind1]^=(1<<2);
			}
		if(sub_ind1==0)lc640_write_int(EE_REL_AV1,REL_AV[0]); 
		if(sub_ind1==1)lc640_write_int(EE_REL_AV2,REL_AV[1]);
		if(sub_ind1==2)lc640_write_int(EE_REL_AV3,REL_AV[2]);
		}
	else if(sub_ind==3)
		{
		if((but==butR)||(but==butR_))
			{
			REL_AV[sub_ind1]|=(1<<3);
			}
		if((but==butL)||(but==butL_))
			{
			REL_AV[sub_ind1]&=~(1<<3);
			}
		if((but==butE)||(but==butE_))
			{
			REL_AV[sub_ind1]^=(1<<3);
			}
		if(sub_ind1==0)lc640_write_int(EE_REL_AV1,REL_AV[0]); 
		if(sub_ind1==1)lc640_write_int(EE_REL_AV2,REL_AV[1]);
		if(sub_ind1==2)lc640_write_int(EE_REL_AV3,REL_AV[2]);
		}
	else if(sub_ind==5)
		{
		if((but==butR)||(but==butR_))
			{
			REL_AV[sub_ind1]|=(1<<4);
			}
		if((but==butL)||(but==butL_))
			{
			REL_AV[sub_ind1]&=~(1<<4);
			}
		if((but==butE)||(but==butE_))
			{
			REL_AV[sub_ind1]^=(1<<4);
			}
		if(sub_ind1==0)lc640_write_int(EE_REL_AV1,REL_AV[0]); 
		if(sub_ind1==1)lc640_write_int(EE_REL_AV2,REL_AV[1]);
		if(sub_ind1==2)lc640_write_int(EE_REL_AV3,REL_AV[2]);
		}
	else if(sub_ind==7)
		{
		if((but==butR)||(but==butR_))
			{
			REL_AV[sub_ind1]|=(1<<5);
			}
		if((but==butL)||(but==butL_))
			{
			REL_AV[sub_ind1]&=~(1<<5);
			}
		if((but==butE)||(but==butE_))
			{
			REL_AV[sub_ind1]^=(1<<5);
			}
		if(sub_ind1==0)lc640_write_int(EE_REL_AV1,REL_AV[0]); 
		if(sub_ind1==1)lc640_write_int(EE_REL_AV2,REL_AV[1]);
		if(sub_ind1==2)lc640_write_int(EE_REL_AV3,REL_AV[2]);
		}
	else if(sub_ind==9)
		{
		if(but==butE)
			{
	          tree_down(0,0);
	          ret(0);
			}
		}																				
	}

else if(ind==iK)
	{
	ret(1000);
	if(but==butD)
		{
		sub_ind++;
		gran_char(&sub_ind,0,5);
		}
	else if(but==butU)
		{
		sub_ind--;
		gran_char(&sub_ind,0,5);
		}
	else if(but==butD_)
		{
		sub_ind=5;
		}				
	else if(but==butE)
		{
		if(sub_ind==0)
			{
			tree_up(iK_net,0,0,0);	
			ret(1000);		
			}
		else if(sub_ind==1)
			{
			tree_up(iK_bat,0,0,0);	
			ret(1000);
			}
		else if(sub_ind==2)
			{
			tree_up(iK_src,0,0,0);	
			ret(1000);
			}
		else if(sub_ind==3)
			{
			tree_up(iK_src,0,0,1);	
			ret(1000);
			}		
          
         else if(sub_ind==4)
			{
			tree_up(iK_t_out,0,0,0);	
			ret(1000);			
			}				
          else if(sub_ind==5)
			{
	          tree_down(-1,0);
	          ret(0);
               }	               			
		}			
	}

else if(ind==iK_bat)
	{
	static char si_max;

	if((AUSW_MAIN/100)==48)si_max=12;
	else si_max=13;
	
	ret(1000);
	if(but==butD)
		{
		sub_ind++;
		if((sub_ind==1)||(sub_ind==2))sub_ind=3;
		else if((sub_ind==4)||(sub_ind==5))sub_ind=6;
		else if((sub_ind==7)||(sub_ind==8))sub_ind=9;
		if((AUSW_MAIN/100)==48)gran_char(&sub_ind,0,si_max);
		else gran_char(&sub_ind,0,si_max);
		phase=0;
		}
	else if(but==butU)
		{
		sub_ind--;
		if((sub_ind==1)||(sub_ind==2))sub_ind=0;
		else if((sub_ind==4)||(sub_ind==5))sub_ind=3;
		else if((sub_ind==7)||(sub_ind==8))sub_ind=6;
		if((AUSW_MAIN/100)==48)gran_char(&sub_ind,0,si_max);
		else gran_char(&sub_ind,0,si_max);
		phase=0;
		}
	else if(but==butD_)
		{
		sub_ind=si_max;
		}			
	else if(sub_ind==0)
		{
		temp_SS=lc640_read_int(ptr_ku_bat);
	     if(but==butR) temp_SS++;
	     else if(but==butR_)	temp_SS+=50;
	     else if(but==butL) temp_SS--;
	     else if(but==butL_)	temp_SS-=50;
		gran(&temp_SS,1300,1600);
		lc640_write_int(ptr_ku_bat,temp_SS);					
		speed=1;			
		}
					
	else if(sub_ind==3)
		{
		temp_SS=lc640_read_int(ptr_ki_bat1);
		if(but==butR) temp_SS++;
		else if(but==butR_)	temp_SS+=80;
		else if(but==butL) temp_SS--;
		else if(but==butL_)	temp_SS-=80;
		else if(but==butE)
		     {
		     if(phase==0)
		          {
		          lc640_write_int(ptr_ki_bat0,adc_buff_[25]);
		          phase=1;
				}
		     }
		gran(&temp_SS,1000,6200);
		lc640_write_int(ptr_ki_bat1,temp_SS);					
		speed=1;
		phase=1;			
		}					
	else if(sub_ind==6)
		{
		temp_SS=lc640_read_int(ptr_kt_bat);
		if(but==butR) temp_SS++;
		else if(but==butR_)	temp_SS+=10;
		else if(but==butL) temp_SS--;
		else if(but==butL_)	temp_SS-=10;
		gran(&temp_SS,1800,2100);
		lc640_write_int(ptr_kt_bat,temp_SS);				
		speed=1;			
		}
	else if(sub_ind==9)
		{
		temp_SS=lc640_read_int(EE_KUBAT_PART0);
	     if(but==butR) temp_SS++;
	     else if(but==butR_)	temp_SS+=50;
	     else if(but==butL) temp_SS--;
	     else if(but==butL_)	temp_SS-=50;
		gran(&temp_SS,1300,1600);
		lc640_write_int(EE_KUBAT_PART0,temp_SS);					
		speed=1;			
		}
	else if(sub_ind==10)
		{
		temp_SS=lc640_read_int(EE_KUBAT_PART1);
	     if(but==butR) temp_SS++;
	     else if(but==butR_)	temp_SS+=50;
	     else if(but==butL) temp_SS--;
	     else if(but==butL_)	temp_SS-=50;
		gran(&temp_SS,1300,1600);
		lc640_write_int(EE_KUBAT_PART1,temp_SS);					
		speed=1;			
		}
	else if(sub_ind==11)
		{
		temp_SS=lc640_read_int(EE_KUBAT_PART2);
	     if(but==butR) temp_SS++;
	     else if(but==butR_)	temp_SS+=50;
	     else if(but==butL) temp_SS--;
	     else if(but==butL_)	temp_SS-=50;
		gran(&temp_SS,1300,1600);
		lc640_write_int(EE_KUBAT_PART2,temp_SS);					
		speed=1;			
		}
	else if((sub_ind==12)&&(sub_ind!=si_max))
		{
		temp_SS=lc640_read_int(EE_KUBAT_PART3);
	     if(but==butR) temp_SS++;
	     else if(but==butR_)	temp_SS+=50;
	     else if(but==butL) temp_SS--;
	     else if(but==butL_)	temp_SS-=50;
		gran(&temp_SS,1300,1600);
		lc640_write_int(EE_KUBAT_PART3,temp_SS);					
		speed=1;			
		}											
	else if(sub_ind==si_max)
		{
		if(but==butE)
			{
			tree_down(0,1);
			ret(0);
			}
		}			
	}
	
else if(ind==iK_src)
	{
	ret(1000);
	if(but==butD)
		{
		sub_ind++;
		if((sub_ind==1)||(sub_ind==2))sub_ind=3;
		else if((sub_ind==4)||(sub_ind==5))sub_ind=6;
		else if((sub_ind==7)||(sub_ind==8))sub_ind=9;
		gran_char(&sub_ind,0,9);
		phase=0;
		}
	else if(but==butU)
		{
		sub_ind--;
		if((sub_ind==1)||(sub_ind==2))sub_ind=0;
		else if((sub_ind==4)||(sub_ind==5))sub_ind=3;
		else if((sub_ind==7)||(sub_ind==8))sub_ind=6;
		gran_char(&sub_ind,0,9);
		phase=0;
		}
	else if(but==butD_)
		{
		sub_ind=9;
		}		
			
	else if(sub_ind==0)
		{
		if(phase==0)
		     {
		     if(but==butE)
		          {
		          if(sub_ind1==0)temp_SS=adc_buff_[15];
		          if(sub_ind1==1)temp_SS=adc_buff_[21];
		          lc640_write_int(ptr_ki0_src[sub_ind1],temp_SS);
		     	phase=1;
		          }
		     else phase=1;     
		     }
		else if(phase==1)
		     {
			temp_SS=lc640_read_int(ptr_ki_src[sub_ind1]);
		     if(but==butR) temp_SS++;
	     	else if(but==butR_)	temp_SS+=20;
	     	else if(but==butL) temp_SS--;
	     	else if(but==butL_)	temp_SS-=20;
			gran(&temp_SS,200,550);
	     	lc640_write_int(ptr_ki_src[sub_ind1],temp_SS);
			speed=1;			
	     	}
	     }	
					
	else if(sub_ind==3)
		{
		temp_SS=lc640_read_int(ptr_ku_src[sub_ind1]);
	     if(but==butR) temp_SS++;
		else if(but==butR_)	temp_SS+=50;
		else if(but==butL) temp_SS--;
		else if(but==butL_)	temp_SS-=50;
		gran(&temp_SS,900,1300);
		lc640_write_int(ptr_ku_src[sub_ind1],temp_SS);					
		speed=1;			
		}					
	else if(sub_ind==6)
		{
		temp_SS=lc640_read_int(ptr_kt_src[sub_ind1]);
		if(but==butR) temp_SS++;
		else if(but==butR_)	temp_SS+=30;
		else if(but==butL) temp_SS--;
		else if(but==butL_)	temp_SS-=30;
		gran(&temp_SS,1800,2100);
		lc640_write_int(ptr_kt_src[sub_ind1],temp_SS);					
		speed=1;			
		}	
	else if(sub_ind==9)
		{
		if(but==butE)
			{
			tree_down(0,1);
			ret(0);
			MSG_IND2OUT_EN_BAT=1;
			}
		}			
	}	
	

else if(ind==iK_net)
	{
	ret(1000);
	if(but==butD)
		{
		sub_ind++;
		gran_char(&sub_ind,0,1);
		phase=0;
		}
	else if(but==butU)
		{
		sub_ind--;
		gran_char(&sub_ind,0,1);
		phase=0;
		}
	else if(but==butD_)
		{
		sub_ind=1;
		}				
	else if(sub_ind==0)
		{
		temp_SS=lc640_read_int(ptr_ku_net);
		if(but==butR) temp_SS++;
		else if(but==butR_)	temp_SS+=50;
		else if(but==butL) temp_SS--;
		else if(but==butL_)	temp_SS-=50;
		gran(&temp_SS,500,700);
		lc640_write_int(ptr_ku_net,temp_SS);				
		speed=1;			
		}
	else if(sub_ind==1)
		{
		if(but==butE)
			{
			tree_down(0,1);
			ret(0);
			}
		}			
	}		
else if(ind==iK_t_out)
	{
	ret(1000);
	if(but==butD)
		{
		sub_ind++;
		gran_char(&sub_ind,0,NUMDT);

		}
	else if(but==butU)
		{
		sub_ind--;
		gran_char(&sub_ind,0,NUMDT);
		}	
	else if(but==butD_)
		{
		sub_ind=3;
		}				
 	else if((sub_ind>=0)&&(sub_ind<NUMDT))
 		{
 		temp_SS=lc640_read_int(KTOUT0);

		if(but==butR)
			{
			temp_SS++;
			}
		else if(but==butR_)
			{
			temp_SS+=5;
			}	
		else if(but==butL)
			{
			temp_SS--;
			}
		else if(but==butL_)
			{
			temp_SS-=5;
			}				
		speed=1;
		gran(&temp_SS,1800,2100);
		lc640_write_int(KTOUT0,temp_SS); 	
 		}    
  
     
     else if(sub_ind==NUMDT)
		{
		if(but==butE) 
			{
			tree_down(0,0);
			ret(0);
			}
          }			
	}	

else if((ind==iPrl_bat_in_out)||(ind==iSet_prl)||(ind==iK_prl)
	||(ind==iSpc_prl_vz)||(ind==iSpc_prl_ke)||(ind==iAusw_prl)
	||(ind==iPrltst))
	{
	ret(50);
	if(but==butR)
		{
		sub_ind++;
		gran_ring_char(&sub_ind,0,2);
		}
	else if(but==butL)
		{
		sub_ind--;
		gran_ring_char(&sub_ind,0,2);
		}	
	else if(but==butU)
		{
		parol[sub_ind]++;
		gran_ring_char(&parol[sub_ind],0,9);
		}	
	else if(but==butD)
		{
		parol[sub_ind]--;
		gran_ring_char(&parol[sub_ind],0,9);
		}	
	else if(but==butE)
		{
		unsigned short tempU;
		tempU=parol[2]+(parol[1]*10U)+(parol[0]*100U);
		
		if(ind==iPrl_bat_in_out)
		     {
		     if(BAT_IS_ON!=bisON)
		          {
		          if(tempU==PAROL_BAT_IN)
		               {
		               lc640_write_int(EE_BAT_IS_ON,bisON);
		               lc640_write_int(EE_BAT_DAY_OF_ON,LPC_RTC->DOM);
		               lc640_write_int(EE_BAT_MONTH_OF_ON,LPC_RTC->MONTH);
		               lc640_write_int(EE_BAT_YEAR_OF_ON,LPC_RTC->YEAR);
		               lc640_write_int(EE_BAT_C_REAL,0x5555);
		               lc640_write_int(EE_BAT_RESURS,0);
		               
//		               lc640_write(KE_PTR,0);
//					lc640_write(VZ_PTR,0);
//					lc640_write(WRK_PTR,0);
//					lc640_write(KE_CNT,0);
//					lc640_write(VZ_CNT,0);
//					lc640_write(WRK_CNT,0);
//					lc640_write(BAT_AVAR_CNT,0);
//					lc640_write(BAT_AVAR_PTR,0);					
		               
                         //a=b[--ptr_ind];
                         tree_down(0,0);
		               }
		          else
		               {
		               ind=iDnd;
		               //ret_ind(b[--ptr_ind].i,b[ptr_ind].s_i,5);
		               ret(20);
     	               }
		          }      
               else		          
		          {
		          if(tempU==PAROL_BAT_OUT)
		               {
		               lc640_write_int(EE_BAT_IS_ON,bisOFF);
		               lc640_write_int(EE_BAT_DAY_OF_ON,LPC_RTC->DOM);
		               lc640_write_int(EE_BAT_MONTH_OF_ON,LPC_RTC->MONTH);
		               lc640_write_int(EE_BAT_YEAR_OF_ON,LPC_RTC->YEAR);
		               //a=b[--ptr_ind];
		               tree_down(0,0);
		               ret(0);
		               
		               }
	               else
		               {
     	               ind=iDnd;
		               //ret_ind(b[--ptr_ind].i,b[ptr_ind].s_i,5);	
		               //tree_down(0,0);
		               ret(20);	               
		               }		               
		          }     
               }
		
		else if(ind==iSet_prl)
			{
	     	if(tempU==PAROL_SET) 
				{
				/*ind=iSet;
				sub_ind=0;
				index_set=0;*/
				tree_up(iSet,0,0,0);
				ret(1000);
				}
			else 
				{
				ind=iDnd;
				ret(20);
				}
			}
		else	if(ind==iK_prl)
			{
	     	if(tempU==PAROL_KALIBR) 
				{
				//ind=iK_pdp;
				//ret_ind(iK,0,15);
				tree_up(iK,0,0,0);
				tree_up(iK_pdp,0,0,0);
				
				ret(30);
				}
			else 
				{
				ind=iDnd;
				ret(20);
				}
			} 
	
		else	if(ind==iAusw_prl)
			{
	     	if(tempU==PAROL_AUSW) 
				{
				/*ind=iAusw_set;
				sub_ind=0;
				index_set=0;*/
				tree_up(iAusw_set,0,0,0);
				default_temp=10;
				ret(0);
				}
			else 
				{
				ind=iDnd;
				ret(15);
				}
			} 	
			
		else	if(ind==iSet_st_prl)
			{
	     	if(tempU==PAROL_DEFAULT) 
				{
	//			ind=iDefault;
				sub_ind=1;
				index_set=0;
				default_temp=10;
				}
			else 
				{
				ind=iDnd;
				ret_ind(iSet,0,10);
				}
			} 
						
		else if(ind==iPrltst)
			{
			if(tempU==PAROL_TST) 
				{
				tree_down(0,0);
				tree_up(iTst_full,0,0,0);
				//ind=iTst_full;
				//sub_ind=0;
				//index_set=0;
				tst_state[0]=tstWRK;
				tst_state[1]=tstWRK;
				tst_state[2]=tstWRK;
				tst_state[3]=tstWRK;
				tst_state[4]=tstWRK;
				tst_state[5]=tstWRK;
				}
	  		else 
				{
				ind=iDnd;
				}
			}
		else if(ind==iSpc_prl_ke)
			{
			if(tempU==PAROL_KE) 
				{
				/*ind=iKe;
				sub_ind=0;
				index_set=0;*/
				tree_up(iKe,0,0,0);
				}
	  		else 
				{	
				ind=iDnd;
				ret_ind(b[--ptr_ind].i,b[ptr_ind].s_i,5);
				}
			}
		else if(ind==iSpc_prl_vz)
			{
			if(tempU==PAROL_VZ) 
				{
				/*ind=iVz;
				sub_ind=0;
				index_set=0;*/
				tree_up(iVz,0,0,0);
     			}
	  		else 
				{
				ind=iDnd;
		          ret_ind(b[--ptr_ind].i,b[ptr_ind].s_i,5);
     	          }
			}
		}
	}
else if(ind==iJ_bat)
	{
	if(but==butD)
		{
		sub_ind++;
		gran_char(&sub_ind,0,6);
		}
	else if(but==butU)
		{
		sub_ind--;
		gran_char(&sub_ind,0,6);
		}
	else if(but==butD_)
		{
		sub_ind=6;
		}				
	else if((but==butL)&&(sub_ind!=1))
		{
		tree_down(0,0);
		ret_ind(0,0,0);
		}		
	else if(sub_ind==0)
	     {
	     if(but==butE)
	          {
	          b[ptr_ind++]=a;
	          if(BAT_IS_ON==bisON) ind=iPrl_bat_in_out;
	          else 
	               {
	               ind=iPdp1;
	               ret_ind(iPrl_bat_in_out,0,10);
	               }
	          parol_init();
	          }
	     }
	else if(sub_ind==1)
	     {
	     if(but==butR)BAT_C_NOM++;
	     else if(but==butR_)BAT_C_NOM+=10;
	     else if(but==butL)BAT_C_NOM--;
	     else if(but==butL_)BAT_C_NOM-=10;
	     gran(&BAT_C_NOM,0,200);
	     lc640_write_int(EE_BAT_C_NOM,BAT_C_NOM);
	     speed=1;
	     }		     
	else if(sub_ind==3)
	     {
		if(but==butE)
			{ 
               cap=0;
			deep=lc640_read_int(CNT_EVENT_LOG);
			ptr=lc640_read_int(PTR_EVENT_LOG);
			
			if(deep>63)
				{
				deep=0;
			     ptr=0;
			     }
			     			
			for(i=0;i<deep;i++)
				{
				lc640_read_long_ptr(EVENT_LOG+(32*ptr),av_head);
				
				if((av_head[0]=='B')&&(av_head[1]==sub_ind1)&&(av_head[2]=='K')) 	//ищем записи батарейных событий 'K'(контроли емкости)
					{
					cap++;
					content[cap-1]=ptr;
					}
					
				ptr=ptr_carry(ptr,64,-1); 
	
				} 
				
			/*ind=iBatJKE;
			sub_ind=0;*/

			tree_up(iJ_bat_ke_sel,0,0,0);
			
			av_j_si_max=cap;
			if(av_j_si_max>63)av_j_si_max=0;
			} 
	     } 	     
	else if(sub_ind==4)
	     {
/*	     if(but==butE)	
	     	{
	     	tree_up(iJ_bat_vz_sel,0,0,0);
	     	}*/
		if(but==butE)
			{ 
               cap=0;
			deep=lc640_read_int(CNT_EVENT_LOG);
			ptr=lc640_read_int(PTR_EVENT_LOG);
			
			if(deep>63)
				{
				deep=0;
			     ptr=0;
			     }
			     
			for(i=0;i<deep;i++)
				{
				lc640_read_long_ptr(EVENT_LOG+(32*ptr),av_head);
				
				if((av_head[0]=='B')/*&&(av_head[1]==sub_ind1)*/&&(av_head[2]=='Z')) 	//ищем записи батарейных событий 'z'(выравнивающий заряд)
					{
					cap++;
					content[cap-1]=ptr;
					}
					
				ptr=ptr_carry(ptr,64,-1); 
				} 
				
			tree_up(iJ_bat_vz_sel,0,0,0);   
			
			av_j_si_max=cap;
			if(av_j_si_max>63)av_j_si_max=0;
			} 
		else if(but==butR)
			{
			//vz_mem_hndl(0,5);
			//vz_mem_hndl(1,6);	       
			} 

	     } 
	else if(sub_ind==5)
	     {
		if(but==butE)
			{ 
               cap=0;
			deep=lc640_read_int(CNT_EVENT_LOG);
			ptr=lc640_read_int(PTR_EVENT_LOG);

			if(deep>63)
				{
				deep=0;
			     ptr=0;
			     }
			for(i=0;i<deep;i++)
				{
				lc640_read_long_ptr(EVENT_LOG+(32*ptr),av_head);
				
				if((av_head[0]=='B')&&(av_head[1]==0)&&(av_head[2]=='W')) 	//ищем записи батарейных событий 'W'(разряды)
					{
					cap++;
					content[cap-1]=ptr;
					}
				ptr=ptr_carry(ptr,64,-1); 
				} 
				
			tree_up(iJ_bat_wrk_sel,0,0,0);			

			av_j_si_max=cap;
			if(av_j_si_max>63)av_j_si_max=0;
			} 
		else if(but==butR)
			{
			//vz_mem_hndl(0,5);
			//vz_mem_hndl(1,6);	       
			} 

	     } 	         	
     else if(sub_ind==6)
	     {
	     if(but==butE)
	          {
	          tree_down(0,0);
	          }
	     }		     
		
	} 
else if(ind==iJ_bat_ke_sel)
	{
	if(but==butD)
		{
		sub_ind++;
		gran_char(&sub_ind,0,av_j_si_max);
		}
	else if(but==butU)
		{
		sub_ind--;
		gran_char(&sub_ind,0,av_j_si_max);
		} 
	else if(but==butD_)
		{
		sub_ind=av_j_si_max;
		
		}		
	else if(but==butE)
		{
 		if(sub_ind==av_j_si_max)
			{
			tree_down(0,0);
			}
		else 
			{
			sub_ind2=content[sub_ind];
			tree_up(iJ_bat_ke,0,0,sub_ind);
			ret(100);			
			}	
		}
	else if(but==butL)
		{	
		tree_down(0,0);
		}			
	else if(but==butR)ke_mem_hndl(355);	 
	}
	
else if(ind==iJ_bat_ke)
	{
	ret(100);
	if(but==butD)
		{
		if(index_set<av_j_si_max_)
			{
			index_set++;
			}
		else 
			{
			tree_down(0,0);
			ret_ind(0,0,0);
			}	
		}
	else if(but==butU)
		{
		if(index_set)index_set--;
		}	
	else 
		{
		tree_down(0,0);
		ret_ind(0,0,0);
		}	
	}	
	
else if(ind==iJ_bat_vz_sel)
	{
	if(but==butD)
		{
		sub_ind++;
		gran_char(&sub_ind,0,av_j_si_max);
		}
	else if(but==butU)
		{
		sub_ind--;
		gran_char(&sub_ind,0,av_j_si_max);
		} 
	else if(but==butD_)
		{
		sub_ind=av_j_si_max;
		
		}		
	else if(but==butE)
		{
		if(sub_ind==av_j_si_max)
			{
			tree_down(0,0);				
			}		
		else 
			{
			sub_ind2=content[sub_ind];
			tree_up(iJ_bat_vz,0,0,sub_ind);
			ret(100);
			}	
		}
	else if(but==butL)
		{	
		tree_down(0,0);
		}	 
	}
	
else if(ind==iJ_bat_vz)
	{
	ret(100);
	if(but==butD)
		{
		if(index_set<av_j_si_max_)
			{
			index_set++;
			}
		else 
			{
			tree_down(0,0);
			ret_ind(0,0,0);
			}	
		}
	else if(but==butU)
		{
		if(index_set)index_set--;
		}	
	else 
		{
		tree_down(0,0);
		ret_ind(0,0,0);
		}
	}	

else if(ind==iJ_bat_wrk_sel)
	{
	if(but==butD)
		{
		sub_ind++;
		gran_char(&sub_ind,0,av_j_si_max);
		}
	else if(but==butU)
		{
		sub_ind--;
		gran_char(&sub_ind,0,av_j_si_max);
		} 
	else if(but==butD_)
		{
		sub_ind=av_j_si_max;
		
		}		
	else if(but==butE)
		{
		 if(sub_ind==av_j_si_max)
			{
			tree_down(0,0);
			ret(0);
			}
		else 
			{
			sub_ind2=content[sub_ind];
			tree_up(iJ_bat_wrk,0,0,sub_ind);
			ret(100);
			}	
		} 
	else if(but==butL)
		{
		tree_down(0,0);
		}		
	else if(but==butR)
		{
		//wrk_mem_hndl(255);
		}		
	}
	
else if(ind==iJ_bat_wrk)
	{
	ret(100);
	if(but==butD)
		{
		if(index_set<4)
			{
			index_set++;
			}
		else 
			{
			tree_down(0,0);
			ret_ind(0,0,0);
			}	
		}
	else if(but==butU)
		{
		if(index_set)index_set--;
		}	
	else 
		{
		tree_down(0,0);
		ret_ind(0,0,0);
		}	
	}			   
but_an_end:
n_but=0;

}
	


//-----------------------------------------------
void but_drv(void)
{
//---------------------------------------------
//для избежания влияния LCD на ввод с кнопок
char i;

LPC_GPIO1->FIODIR|=(1<<27);
LPC_GPIO1->FIOCLR|=(1<<27);
LPC_GPIO1->FIODIR&=~((1<<21)|(1<<22)|(1<<23)|(1<<24)|(1<<25));
//LPC_PINCON->PINMODE2&=~(1<<14);
LPC_PINCON->PINMODE3&=~((1<<10)|(1<<11)|(1<<12)|(1<<13)|(1<<14)|(1<<15)|(1<<16)|(1<<17)|(1<<18)|(1<<19));

for(i=0;i<20;i++)
{
__nop();
__nop();
__nop();
__nop();
}
//---------------------------------------------------

but_n=LPC_GPIO1->FIOPIN&(0x1F<<21);	  // читаем 5 битов с порта

but_n>>=21;		// преобразуем в удобный вид

if((but_n==butOFF)||(but_n!=but_s))
 	{
 	speed=0;
 
   	if (((but0_cnt>=BUT_ON)||(but1_cnt!=0))&&(!l_but))
  		{
   	     n_but=1;
         but=but_s;

        }
   	if (but1_cnt>=but_onL_temp)
  		{
   	     n_but=1;
 
          but=but_s+LongPush;
        }
    l_but=0;
   	but_onL_temp=BUT_ONL;
    but0_cnt=0;
  	but1_cnt=0;          
     goto but_drv_out;
  	}
if(but_n==but_s)
 	{
  	but0_cnt++;
  	if(but0_cnt>=BUT_ON)
  		{
   		but0_cnt=0;
   		but1_cnt++;
   		if(but1_cnt>=but_onL_temp)
   			{              
    			but=but_s+LongPush;
    			but1_cnt=0;
    			n_but=1;
    			     
    			l_but=1;
//			if(speed)
//				{
//    				but_onL_temp=but_onL_temp>>1;
//        			if(but_onL_temp<=2) but_onL_temp=2;
//				}    
   			}
  		}
 	}
but_drv_out: 
but_s=but_n;


/*
PINSEL2&=~(1UL<<((BUT0-16)*2))&~(1UL<<(((BUT0-16)*2)+1))
	   &~(1UL<<((BUT1-16)*2))&~(1UL<<(((BUT1-16)*2)+1))
	   &~(1UL<<((BUT2-16)*2))&~(1UL<<(((BUT2-16)*2)+1))
	   &~(1UL<<((BUT3-16)*2))&~(1UL<<(((BUT3-16)*2)+1))
	   &~(1UL<<((BUT4-16)*2))&~(1UL<<(((BUT4-16)*2)+1)); 
*/
LPC_GPIO1->FIODIR&=~(1UL<<BUT0)&~(1UL<<BUT1)&~(1UL<<BUT2)&~(1UL<<BUT3)&~(1UL<<BUT4);
	   
}



//***********************************************
//***********************************************
//***********************************************
//***********************************************
//***********************************************
void SysTick_Handler (void) 	 /* SysTick Interrupt Handler (1ms)    */
{

//LPC_GPIO0->FIODIR |= (1<<26);
//LPC_GPIO0->FIODIR|=0x00000001;
//LPC_GPIO0->FIOSET|=0x00000001;           
SysTickCnt++;


bFF=GET_REG(LPC_GPIO0->FIOPIN,26,1);//  Vs P0.26

if(bFF!=bFF_)
	{
	Hz_out++;
	}        
bFF_=bFF; 



if(++t0cnt4>=1)
     {
     t0cnt4=0;
     b1000Hz=1;

     } 

if(++t0cnt5>=20)
     {
     t0cnt5=0;
     b50Hz=1;
     }
     
if(++t0cnt>=10)
     {
     t0cnt=0;
     b100Hz=1;

     Hz_cnt++;
     if(Hz_cnt>=500)
	     {	
	     Hz_cnt=0;
	     fnet=Hz_out;
	     Hz_out=0;
	     }

     if(++t0cnt0>=10)
	     {
	     t0cnt0=0;
	     b10Hz=1;
		beep_drv();
	     }

     if(t0cnt0==5)
	     {
		beep_drv();
	     }

     if(++t0cnt1>=20)
	     {
	     t0cnt1=0;
	     b5Hz=1;
		if(bFL5)bFL5=0;
		else bFL5=1;

		if(bRESET_RESET)
			{
			LPC_GPIO0->FIODIR|=(1<<10);
          	LPC_GPIO0->FIOPIN^=(1<<10);
			}       
	     }

     if(++t0cnt2>=50)
	     {
	     t0cnt2=0;
	     b2Hz=1;
		if(bFL2)bFL2=0;
		else bFL2=1;
	     }         

     if(++t0cnt3>=100)
	     {
	     t0cnt3=0;
	     b1Hz=1;


	     }
     }
//LPC_GPIO0->FIOCLR|=0x00000001;
}


//===============================================
//===============================================
//===============================================
//===============================================
int main (void) 	                        
{

char mac_adr[6] = { 0x00,0x73,0x04,50,60,70 };

SystemInit();
SysTick_Config(SystemFrequency/1000 - 1); /* Generate interrupt each 1 ms   */

////LPC_GPIO2->FIODIR  |= 1<<13;    // CS on 25LC640            
	
//FIO1MASK = 0x00000000;	 
LPC_GPIO0->FIODIR  |= 1<<22;
LPC_GPIO0->FIOCLR  |= 1<<22;

////SET_REG(LPC_GPIO0->FIODIR,0,10,1); //вход частоты (временный, проводком)
SET_REG(LPC_GPIO0->FIODIR,0,26,1);			   // init Vs
SET_REG(LPC_PINCON->PINMODE1,2,(26-16)*2,2);   // init Vs

lcd_init();
lcd_on();
lcd_clear();


////LPC_GPIO4->FIODIR |= (1<<29);           /* LEDs on PORT2 defined as Output    */

rtc_init();
pwm_init();
UARTInit(0, 9600);	/* baud rate setting */
//UART_2_Init(9600 );
ind=iMn;
mnemo_cnt=30;
//can1_init(BITRATE125K25MHZ);
//FullCAN_SetFilter(0,0x0e9);
//
//
//     /*     b[ptr_ind++]=a;
//          ind=iDeb;
//          sub_ind=3;*/
//
//SET_REG(LPC_GPIO0->FIODIR,1,8,1);
//SET_REG(LPC_GPIO0->FIODIR,1,9,1);
//
//SET_REG(LPC_GPIO0->FIOCLR,1,8,1);
//SET_REG(LPC_GPIO0->FIOCLR,1,9,1);

tst_state[0]=tstWRK;
tst_state[1]=tstWRK;
tst_state[2]=tstWRK;
tst_state[3]=tstWRK;	//включить батарею
tst_state[4]=tstWRK;
tst_state[5]=tstWRK;
tst_state[6]=tstWRK;
tst_state[7]=tstWRK;
tst_state[8]=tstWRK;
tst_state[9]=tstWRK;

#ifdef LAN_OFF
if(lc640_read_int(EE_ETH_IS_ON)==1) lc640_write_int(EE_ETH_IS_ON,0);
#endif


AUSW_MAIN_NUMBER=lc640_read_long(EE_AUSW_MAIN_NUMBER);
NUMIST=lc640_read_long(EE_NUMIST);

mac_adr[5]=*((char*)&AUSW_MAIN_NUMBER);
mac_adr[4]=*(((char*)&AUSW_MAIN_NUMBER)+1);
mac_adr[3]=*(((char*)&AUSW_MAIN_NUMBER)+2);
mem_copy (own_hw_adr, mac_adr, 6);

snmp_Community[0]=(char)lc640_read_int(EE_COMMUNITY);
//if((snmp_Community[0]==0)||(snmp_Community[0]==' '))snmp_Community[0]=0;
snmp_Community[1]=(char)lc640_read_int(EE_COMMUNITY+2);
if((snmp_Community[1]==0)||(snmp_Community[1]==' '))snmp_Community[1]=0;
snmp_Community[2]=(char)lc640_read_int(EE_COMMUNITY+4);
if((snmp_Community[2]==0)||(snmp_Community[2]==' '))snmp_Community[2]=0;
snmp_Community[3]=(char)lc640_read_int(EE_COMMUNITY+6);
if((snmp_Community[3]==0)||(snmp_Community[3]==' '))snmp_Community[3]=0;
snmp_Community[4]=(char)lc640_read_int(EE_COMMUNITY+8);
if((snmp_Community[4]==0)||(snmp_Community[4]==' '))snmp_Community[4]=0;
snmp_Community[5]=(char)lc640_read_int(EE_COMMUNITY+10);
if((snmp_Community[5]==0)||(snmp_Community[5]==' '))snmp_Community[5]=0;
snmp_Community[6]=(char)lc640_read_int(EE_COMMUNITY+12);
if((snmp_Community[6]==0)||(snmp_Community[6]==' '))snmp_Community[6]=0;
snmp_Community[7]=(char)lc640_read_int(EE_COMMUNITY+14);
if((snmp_Community[7]==0)||(snmp_Community[7]==' '))snmp_Community[7]=0;
snmp_Community[8]=(char)lc640_read_int(EE_COMMUNITY+16);
if((snmp_Community[8]==0)||(snmp_Community[8]==' '))snmp_Community[8]=0;
snmp_Community[9]=0;

bRESET_RESET=1;
if(lc640_read_int(EE_ETH_IS_ON)==1)
	{
	bgnd_par(		"                    ",
     		"    Инициализация   ",
     		"      Ethernet      ",
     		"                    ");
	bitmap_hndl();
	lcd_out(lcd_bitmap);
	init_TcpNet ();

	init_ETH();
	//mem_copy (&localm[NETIF_ETH], &ip_config, sizeof(ip_config));

	}
bRESET_RESET=0;
reload_hndl();

while (1) 
	{
     timer_poll ();
     main_TcpNet ();
//	static char temp;
	if(bRXIN0)
     	{
		bRXIN0=0;

		uart_in0();
		}

	if(bRXIN2)
     	{
		bRXIN2=0;

//		uart_in2();
		}
 	if(b1000Hz)
		{
		b1000Hz=0;

		adc_drv();
		}

 	if(b100Hz)
		{
		b100Hz=0;

		but_drv();
		but_an(); 
		}
		
	if(b33Hz)
		{
		b33Hz=0;
		}
			 
	if(b10Hz)
		{
		b10Hz=0;

	    unet_drv();
	    if(NUMIST)src_drv(0);
		if(NUMIST>1)src_drv(1);
		ret_hndl(); 
	    ind_hndl(); 
		bitmap_hndl();
		lcd_out(lcd_bitmap);
		//if(bRESET_RESET)
			{
			LPC_GPIO0->FIODIR|=(1<<10);
          	LPC_GPIO0->FIOPIN^=(1<<10);
			} 

 		}
			
	if(b5Hz)
		{
		b5Hz=0;

		samokalibr_drv();
		ret_ind_hndl();
		matemat();
	    	time_drv();
		kb_drv();				   
		zar_superviser_drv();
	    	u_necc_hndl();
		out_out();   			//иlok
		bat_drv();   			  
		rele_drv();  
		beep_hndl();  
		memo_read(); 
		snmp_data();
		fuse_drv();
		ext_drv();
		}

	if(b2Hz)
		{
		b2Hz=0;
		}
										
	if(b1Hz)
		{
		b1Hz=0;

		cntrl_hndl();
		cntrl_drv();
		ret_ind_sec_hndl();  
		spc_drv();				 
		zar_drv();
		cntrl_blok_hndl(); 
		avg_hndl();
		apv_drv();				 
		ubat_old_drv();
 		avz_hndl();
		mnemo_hndl(); 	

		if(++ind_reset_cnt>=60)
			{
			ind_reset_cnt=0;
//			lcd_init();
//			lcd_on();
//			lcd_clear();
			} 

//          //uart_out_adr2((char*)adc_buff_,6);
		}
	}
  
}

