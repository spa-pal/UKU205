#include "snmp_data_file.h" 
#include "eeprom_map.h"
#include "main.h"
#include "net_config.h"
//#include "main.h"
#include "control.h"
#include "LPC17xx.H"
#include <string.h>
#include "25lc640.h"
#include "common_func.h"

char snmp_community[10];

//Информация об устройстве
signed short snmp_device_code;
signed 	   snmp_sernum;
signed short snmp_sernum_lsb;
signed short snmp_sernum_msb;
char 	   snmp_location[100];
signed short snmp_numofbat;
signed short snmp_numofbps;
signed short snmp_numofevents;
char 	   snmp_model[30];		//Модель ИБЭПа


//Состояние первичной сети
signed short snmp_mains_power_voltage;
signed short snmp_mains_power_frequency;
signed short snmp_mains_power_status;
signed short snmp_mains_power_alarm;

//Состояние нагрузки
signed short snmp_load_voltage;
signed short snmp_load_current;

//Состояние БПСов
signed short snmp_bps_number[2];
signed short snmp_bps_voltage[2];
signed short snmp_bps_current[2];
signed short snmp_bps_temperature[2];
signed short snmp_bps_stat[2];

//Состояние Батареи
signed short snmp_bat_number;
signed short snmp_bat_voltage;
signed short snmp_bat_current;
signed short snmp_bat_temperature;
signed short snmp_bat_capacity;
signed short snmp_bat_charge;
signed short snmp_bat_status; 

//Состояние предохранителей
signed short snmp_fuse_av_stat[3];
signed short snmp_fuse_number[3];

//Состояние сухих контактов
signed short snmp_sk_av_stat[4];
signed short snmp_sk_number[4];

//Спецфункции
signed short snmp_spc_stat;
char snmp_spc_trap_message[100];
signed short snmp_spc_trap_value;
signed short snmp_spc_trap_value_0,snmp_spc_trap_value_1,snmp_spc_trap_value_2;

//Команды
signed short snmp_command;
signed short snmp_command_parametr;

//Журнал аварий
char snmp_log[64][128]=
				{
				"01@abcd@efgh@ijkl@01@        ",
				"02@abcd@efgh@ijkl@02@        ",
				"03@abcd@efgh@ijkl@03@        ",
				"04@abcd@efgh@ijkl@04@        ",
				"05@abcd@efgh@ijkl@05@        ",
				"06@abcd@efgh@ijkl@06@        ",
				"07@abcd@efgh@ijkl@07@        ",
				"08@abcd@efgh@ijkl@08@        ",
				"09@abcd@efgh@ijkl@09@        ",
				"10@abcd@efgh@ijkl@10@        ",
				"11@abcd@efgh@ijkl@11@        ",
				"12@abcd@efgh@ijkl@12@        ",
				"13@abcd@efgh@ijkl@13@        ",
				"14@abcd@efgh@ijkl@14@        ",
				"15@abcd@efgh@ijkl@15@        ",
				"16@abcd@efgh@ijkl@16@        ",
				"17@abcd@efgh@ijkl@17@        ",
				"18@abcd@efgh@ijkl@18@        ",
				"19@abcd@efgh@ijkl@19@        ",
				"20@abcd@efgh@ijkl@20@        ",
				"21@abcd@efgh@ijkl@21@        ",
				"22@abcd@efgh@ijkl@22@        ",
				"23@abcd@efgh@ijkl@23@        ",
				"24@abcd@efgh@ijkl@24@        ",
				"25@abcd@efgh@ijkl@25@        ",
				"26@abcd@efgh@ijkl@26@        ",
				"27@abcd@efgh@ijkl@27@        ",
				"28@abcd@efgh@ijkl@28@        ",
				"29@abcd@efgh@ijkl@29@        ",
				"30@abcd@efgh@ijkl@30@        "
				};

//Установочные параметры
signed short snmp_main_bps;
signed short snmp_zv_en;
signed short snmp_alarm_auto_disable;
signed short snmp_bat_test_time;
signed short snmp_u_max;
signed short snmp_u_0_grad;
signed short snmp_u_20_grad;
signed short snmp_u_sign;
signed short snmp_u_min_power;
signed short snmp_u_withouth_bat;
signed short snmp_control_current;
signed short snmp_max_charge_current;
signed short snmp_max_current;
signed short snmp_max_current_koef;
signed short snmp_max_current_koef;
signed short snmp_up_charge_koef;
signed short snmp_powerup_psu_timeout;
signed short snmp_max_temperature;

char temp_str[20];

char snmp_trap_send_i,snmp_trap_send_ii;
unsigned short obj[10];
unsigned char temp_ip[4];

//Литиевые батареи
short snmp_lakb_number[7];				//Номер ЛАКБ
short snmp_lakb_voltage[7];				//Напряжение ЛАКБ
short snmp_lakb_max_cell_voltage[7];		//Максимальное напряжение ячейки ЛАКБ
short snmp_lakb_min_cell_voltage[7];		//Минимальное напряжение ячейки ЛАКБ
short snmp_lakb_max_cell_temperature[7];	//Максимальная температура ячейки ЛАКБ
short snmp_lakb_min_cell_temperature[7];	//Минимальная температура ячейки ЛАКБ
short snmp_lakb_ch_curr[7];				//Ток заряда ЛАКБ
short snmp_lakb_dsch_curr[7];				//Ток разряда ЛАКБ
short snmp_lakb_rat_cap[7];				//Номинальная емкость ЛАКБ
short snmp_lakb_soh[7];					//Остаточная емкость ЛАКБ
short snmp_lakb_soc[7];					//Заряд ЛАКБ
short snmp_lakb_cclv[7];  				//Максимальный ток заряда ЛАКБ
short snmp_lakb_rbt[7];					//Оцениваемое время работы ЛАКБ
short snmp_lakb_flags1[7];				//Первый флаг состояния ЛАКБ
short snmp_lakb_flags2[7];				//Второй флаг состояния ЛАКБ
char snmp_lakb_damp1[7][150];				//Первая строка передаваемого дампа
char snmp_lakb_damp2[100];				//Первая строка передаваемого дампа
//-----------------------------------------------
void snmp_data (void) 
{
char i;

snmp_mains_power_voltage=Unet;
snmp_mains_power_frequency=fnet;
snmp_mains_power_status=0;
/*#if(UKU_VERSION==900)
snmp_mains_power_status=2;
#endif*/
if(St&0x01)snmp_mains_power_status|=0x01;
if(St&0x01)snmp_mains_power_alarm=1;

snmp_load_voltage=Uload;
snmp_load_current=Iload;
snmp_numofbps=2;

snmp_numofevents=lc640_read_int(CNT_EVENT_LOG);

for(i=0;i<snmp_numofevents;i++)event2snmp(i);
//snmp_bpsnumber[0]=1;
//snmp_bpsnumber[1]=2;

snmp_sernum=AUSW_MAIN_NUMBER;
snmp_sernum_lsb=0x1122;
snmp_sernum_msb=0x3344;
snmp_device_code=AUSW_MAIN;

//memcpy(snmp_location,"lkhg;la",);


snmp_numofbat=1;



snmp_bps_number[0]=1;
snmp_bps_voltage[0]=Us[0];
snmp_bps_current[0]=Is[0];
snmp_bps_temperature[0]=t_i[0];
snmp_bps_stat[0]=0;												//Байт состояния БПСа.
if(St_[0]&(1<<2))snmp_bps_stat[0]=(1<<3); 							//авария по Umin
else if(St_[0]&(1<<3))snmp_bps_stat[0]=(1<<2); 						//авария по Umax
else if(St_[0]&(1<<4))snmp_bps_stat[0]=(1<<1); 						//авария по Tmax
else if(St_[0]&(1<<5))snmp_bps_stat[0]=(1<<5); 						//заблокирован
else if((!(St_[0]&0x3c))&&(!St&0x01)&&(!OFFBP1))snmp_bps_stat[0]=1; 		//Работает



snmp_bps_number[1]=2;
snmp_bps_voltage[1]=Us[1];
snmp_bps_current[1]=Is[1];
snmp_bps_temperature[1]=t_i[1];
snmp_bps_stat[1]=0;												//Байт состояния БПСа.
if(St_[1]&(1<<2))snmp_bps_stat[1]=(1<<3); 							//авария по Umin
else if(St_[1]&(1<<3))snmp_bps_stat[1]=(1<<2); 						//авария по Umax
else if(St_[1]&(1<<4))snmp_bps_stat[1]=(1<<1); 						//авария по Tmax
else if(St_[1]&(1<<5))snmp_bps_stat[1]=(1<<5); 						//заблокирован
else if((!(St_[1]&0x3c))&&(!St&0x01)&&(!OFFBP2))snmp_bps_stat[1]=1; 		//Работает

snmp_bat_number=1;
snmp_bat_voltage=Ubat;
snmp_bat_current=Ibat;
snmp_bat_temperature=tbat;
if(BAT_C_REAL==0x5555)
	{
	snmp_bat_capacity=BAT_C_NOM*10;
	}
else
	{
	snmp_bat_capacity=BAT_C_REAL;
	}
snmp_bat_charge=zar_percent;
snmp_bat_status=0;
if(St&0x02)snmp_bat_status|=0x01;
if(Ibat>0)snmp_bat_status|=0x02;


if(spc_stat==spc_OFF) snmp_spc_stat=0;
else if(spc_stat==spc_KE) snmp_spc_stat=1;
else if(spc_stat==spc_VZ) snmp_spc_stat=10;


snmp_main_bps=MAIN_BPS+1;
snmp_zv_en=ZV_ON;
snmp_alarm_auto_disable=AV_OFF_AVT;
snmp_bat_test_time=TBAT;
snmp_u_max=UMAX;
snmp_u_0_grad=UBMAX;
snmp_u_20_grad=UB20;
snmp_u_sign=USIGN;
snmp_u_min_power=UMN;
snmp_u_withouth_bat=U0B;
snmp_control_current=IKB;
snmp_max_charge_current=IZMAX;
snmp_max_current=IMAX;
snmp_max_current_koef=KIMAX;
snmp_up_charge_koef=KVZ;
snmp_powerup_psu_timeout=TZAS;
snmp_max_temperature=TMAX;

snmp_fuse_number[0]=1;
snmp_fuse_number[1]=2;
snmp_fuse_number[2]=3;
snmp_fuse_av_stat[0]=fuse_av_stat[0];
snmp_fuse_av_stat[1]=fuse_av_stat[1];
snmp_fuse_av_stat[2]=fuse_av_stat[2];

snmp_sk_number[0]=1;
snmp_sk_number[1]=2;
snmp_sk_number[2]=3;
snmp_sk_number[3]=4;
if(sk_stat[0]==skON)snmp_sk_av_stat[0]=1;
else snmp_sk_av_stat[0]=0;
if(sk_stat[1]==skON)snmp_sk_av_stat[1]=1;
else snmp_sk_av_stat[1]=0;
if(sk_stat[2]==skON)snmp_sk_av_stat[2]=1;
else snmp_sk_av_stat[2]=0;
if(sk_stat[3]==skON)snmp_sk_av_stat[3]=1;
else snmp_sk_av_stat[3]=0;


//Литиевые батареи
snmp_lakb_number[0]=1;								//Номер ЛАКБ
snmp_lakb_number[1]=2;								//Номер ЛАКБ
snmp_lakb_number[2]=3;								//Номер ЛАКБ
snmp_lakb_number[3]=4;								//Номер ЛАКБ
snmp_lakb_number[4]=5;								//Номер ЛАКБ
snmp_lakb_number[5]=6;								//Номер ЛАКБ
snmp_lakb_number[6]=7;								//Номер ЛАКБ

for (i=0;i<7;i++)
	{
	snmp_lakb_voltage[i]=lakb[i]._tot_bat_volt;				//Напряжение ЛАКБ
	snmp_lakb_max_cell_voltage[i]=lakb[i]._max_cell_volt;		//Максимальное напряжение ячейки ЛАКБ
	snmp_lakb_min_cell_voltage[i]=lakb[i]._min_cell_volt;		//Минимальное напряжение ячейки ЛАКБ
	snmp_lakb_max_cell_temperature[i]=lakb[i]._max_cell_temp;	//Максимальная температура ячейки ЛАКБ
	//snmp_lakb_max_cell_temperature[1]=lakb[1]._max_cell_temp;	//Максимальная температура ячейки ЛАКБ
	snmp_lakb_min_cell_temperature[i]=lakb[i]._min_cell_temp;	//Минимальная температура ячейки ЛАКБ
	//snmp_lakb_min_cell_temperature[1]=lakb[1]._min_cell_temp;	//Минимальная температура ячейки ЛАКБ
	snmp_lakb_ch_curr[i]=lakb[i]._ch_curr;					//Ток заряда ЛАКБ
	//snmp_lakb_ch_curr[1]=lakb[1]._ch_curr;					//Ток заряда ЛАКБ
	snmp_lakb_dsch_curr[i]=lakb[i]._dsch_curr;				//Ток разряда ЛАКБ
	//snmp_lakb_dsch_curr[1]=lakb[1]._dsch_curr;				//Ток разряда ЛАКБ
	snmp_lakb_rat_cap[i]=lakb[i]._rat_cap;					//Номинальная емкость ЛАКБ
	//snmp_lakb_rat_cap[1]=lakb[1]._rat_cap;					//Номинальная емкость ЛАКБ
	snmp_lakb_soh[i]=lakb[i]._s_o_h;						//Остаточная емкость ЛАКБ
	//snmp_lakb_soh[1]=lakb[1]._s_o_h;						//Остаточная емкость ЛАКБ
	snmp_lakb_soc[i]=lakb[i]._s_o_c;						//Заряд ЛАКБ
	//snmp_lakb_soc[1]=lakb[1]._s_o_c;						//Заряд ЛАКБ
	snmp_lakb_cclv[i]=lakb[i]._c_c_l_v;  					//Максимальный ток заряда ЛАКБ
	//snmp_lakb_cclv[1]=lakb[1]._c_c_l_v;  					//Максимальный ток заряда ЛАКБ
	snmp_lakb_rbt[i]=lakb[i]._r_b_t;						//Оцениваемое время работы ЛАКБ
	//snmp_lakb_rbt[1]=lakb[1]._r_b_t;						//Оцениваемое время работы ЛАКБ
	snmp_lakb_flags1[i]=lakb[i]._flags1;					//Первый флаг состояния ЛАКБ
	//snmp_lakb_flags1[1]=lakb[1]._flags1;					//Первый флаг состояния ЛАКБ
	snmp_lakb_flags2[i]=lakb[i]._flags2;					//Второй флаг состояния ЛАКБ
	//snmp_lakb_flags2[1]=lakb[1]._flags2;					//Второй флаг состояния ЛАКБ
	}

/*
lakb_damp[0][0]=0x11;
lakb_damp[0][1]=0xab;
lakb_damp[0][2]=0xff;
lakb_damp[0][3]=0xb5;
lakb_damp[0][4]=0xf0;
lakb_damp[0][5]=0xea;
lakb_damp[0][6]=0xae;
lakb_damp[0][7]=0x79;
lakb_damp[0][8]=0x46;
lakb_damp[0][9]=0x35;
lakb_damp[0][10]=0x78;
lakb_damp[0][11]=0x59;
lakb_damp[0][12]=0x63;
lakb_damp[0][13]=0x64;
lakb_damp[0][14]=0x90;


lakb_damp[0][33]=0x33;


lakb_damp[2][14]=0x14;


lakb_damp[3][20]=0x20;


lakb_damp[4][42]=0x42;

lakb_damp[4][35]=0x35;

//lakb_damp[5][46]=0x47;

lakb_damp[6][6]=0x6;


//lakb_damp[5][43]=0x43;
lakb_damp[6][5]=0x65;*/


for (i=0;i<7;i++)
{

snmp_lakb_damp1[i][0]=ABCDEF[(lakb_damp[i][0])>>4];			//Дамп
snmp_lakb_damp1[i][1]=ABCDEF[(lakb_damp[i][0])&0x0f];			//Дамп
snmp_lakb_damp1[i][2]=' ';
snmp_lakb_damp1[i][3]=ABCDEF[(lakb_damp[i][1])>>4];			//Дамп
snmp_lakb_damp1[i][4]=ABCDEF[(lakb_damp[i][1])&0x0f];			//Дамп
snmp_lakb_damp1[i][5]=' ';
snmp_lakb_damp1[i][6]=ABCDEF[(lakb_damp[i][2])>>4];			//Дамп
snmp_lakb_damp1[i][7]=ABCDEF[(lakb_damp[i][2])&0x0f];			//Дамп
snmp_lakb_damp1[i][8]=' ';
snmp_lakb_damp1[i][9]=ABCDEF[(lakb_damp[i][3])>>4];			//Дамп
snmp_lakb_damp1[i][10]=ABCDEF[(lakb_damp[i][3])&0x0f];			//Дамп
snmp_lakb_damp1[i][11]=' ';
snmp_lakb_damp1[i][12]=ABCDEF[(lakb_damp[i][4])>>4];			//Дамп
snmp_lakb_damp1[i][13]=ABCDEF[(lakb_damp[i][4])&0x0f];			//Дамп
snmp_lakb_damp1[i][14]=' '; 
snmp_lakb_damp1[i][15]=ABCDEF[(lakb_damp[i][5])>>4];			//Дамп
snmp_lakb_damp1[i][16]=ABCDEF[(lakb_damp[i][5])&0x0f];			//Дамп
snmp_lakb_damp1[i][17]=' ';
snmp_lakb_damp1[i][18]=ABCDEF[(lakb_damp[i][6])>>4];			//Дамп
snmp_lakb_damp1[i][19]=ABCDEF[(lakb_damp[i][6])&0x0f];			//Дамп
snmp_lakb_damp1[i][20]=' ';
snmp_lakb_damp1[i][21]=ABCDEF[(lakb_damp[i][7])>>4];			//Дамп
snmp_lakb_damp1[i][22]=ABCDEF[(lakb_damp[i][7])&0x0f];			//Дамп
snmp_lakb_damp1[i][23]=' ';
snmp_lakb_damp1[i][24]=ABCDEF[(lakb_damp[i][8])>>4];			//Дамп
snmp_lakb_damp1[i][25]=ABCDEF[(lakb_damp[i][8])&0x0f];			//Дамп
snmp_lakb_damp1[i][26]=' ';
snmp_lakb_damp1[i][27]=ABCDEF[(lakb_damp[i][9])>>4];			//Дамп
snmp_lakb_damp1[i][28]=ABCDEF[(lakb_damp[i][9])&0x0f];			//Дамп
snmp_lakb_damp1[i][29]=0; 

snmp_lakb_damp1[i][30]=ABCDEF[(lakb_damp[i][10])>>4];			//Дамп
snmp_lakb_damp1[i][31]=ABCDEF[(lakb_damp[i][10])&0x0f];			//Дамп
snmp_lakb_damp1[i][32]=' ';
snmp_lakb_damp1[i][33]=ABCDEF[(lakb_damp[i][11])>>4];			//Дамп
snmp_lakb_damp1[i][34]=ABCDEF[(lakb_damp[i][11])&0x0f];			//Дамп
snmp_lakb_damp1[i][35]=' ';
snmp_lakb_damp1[i][36]=ABCDEF[(lakb_damp[i][12])>>4];			//Дамп
snmp_lakb_damp1[i][37]=ABCDEF[(lakb_damp[i][12])&0x0f];			//Дамп
snmp_lakb_damp1[i][38]=' ';
snmp_lakb_damp1[i][39]=ABCDEF[(lakb_damp[i][13])>>4];			//Дамп
snmp_lakb_damp1[i][40]=ABCDEF[(lakb_damp[i][13])&0x0f];			//Дамп
snmp_lakb_damp1[i][41]=' ';
snmp_lakb_damp1[i][42]=ABCDEF[(lakb_damp[i][14])>>4];			//Дамп
snmp_lakb_damp1[i][43]=ABCDEF[(lakb_damp[i][14])&0x0f];			//Дамп
snmp_lakb_damp1[i][44]=' '; 
snmp_lakb_damp1[i][45]=ABCDEF[(lakb_damp[i][15])>>4];			//Дамп
snmp_lakb_damp1[i][46]=ABCDEF[(lakb_damp[i][15])&0x0f];			//Дамп
snmp_lakb_damp1[i][47]=' ';
snmp_lakb_damp1[i][48]=ABCDEF[(lakb_damp[i][16])>>4];			//Дамп
snmp_lakb_damp1[i][49]=ABCDEF[(lakb_damp[i][16])&0x0f];			//Дамп
snmp_lakb_damp1[i][50]=' ';
snmp_lakb_damp1[i][51]=ABCDEF[(lakb_damp[i][17])>>4];			//Дамп
snmp_lakb_damp1[i][52]=ABCDEF[(lakb_damp[i][17])&0x0f];			//Дамп
snmp_lakb_damp1[i][53]=' ';
snmp_lakb_damp1[i][54]=ABCDEF[(lakb_damp[i][18])>>4];			//Дамп
snmp_lakb_damp1[i][55]=ABCDEF[(lakb_damp[i][18])&0x0f];			//Дамп
snmp_lakb_damp1[i][56]=' ';
snmp_lakb_damp1[i][57]=ABCDEF[(lakb_damp[i][19])>>4];			//Дамп
snmp_lakb_damp1[i][58]=ABCDEF[(lakb_damp[i][19])&0x0f];			//Дамп
snmp_lakb_damp1[i][59]=0; 


snmp_lakb_damp1[i][60]=ABCDEF[(lakb_damp[i][20])>>4];			//Дамп
snmp_lakb_damp1[i][61]=ABCDEF[(lakb_damp[i][20])&0x0f];			//Дамп
snmp_lakb_damp1[i][62]=' ';
snmp_lakb_damp1[i][63]=ABCDEF[(lakb_damp[i][21])>>4];			//Дамп
snmp_lakb_damp1[i][64]=ABCDEF[(lakb_damp[i][21])&0x0f];			//Дамп
snmp_lakb_damp1[i][65]=' ';
snmp_lakb_damp1[i][66]=ABCDEF[(lakb_damp[i][22])>>4];			//Дамп
snmp_lakb_damp1[i][67]=ABCDEF[(lakb_damp[i][22])&0x0f];			//Дамп
snmp_lakb_damp1[i][68]=' ';
snmp_lakb_damp1[i][69]=ABCDEF[(lakb_damp[i][23])>>4];			//Дамп
snmp_lakb_damp1[i][70]=ABCDEF[(lakb_damp[i][23])&0x0f];			//Дамп
snmp_lakb_damp1[i][71]=' ';
snmp_lakb_damp1[i][72]=ABCDEF[(lakb_damp[i][24])>>4];			//Дамп
snmp_lakb_damp1[i][73]=ABCDEF[(lakb_damp[i][24])&0x0f];			//Дамп
snmp_lakb_damp1[i][74]=' '; 
snmp_lakb_damp1[i][75]=ABCDEF[(lakb_damp[i][25])>>4];			//Дамп
snmp_lakb_damp1[i][76]=ABCDEF[(lakb_damp[i][25])&0x0f];			//Дамп
snmp_lakb_damp1[i][77]=' ';
snmp_lakb_damp1[i][78]=ABCDEF[(lakb_damp[i][26])>>4];			//Дамп
snmp_lakb_damp1[i][79]=ABCDEF[(lakb_damp[i][26])&0x0f];			//Дамп
snmp_lakb_damp1[i][80]=' ';
snmp_lakb_damp1[i][81]=ABCDEF[(lakb_damp[i][27])>>4];			//Дамп
snmp_lakb_damp1[i][82]=ABCDEF[(lakb_damp[i][27])&0x0f];			//Дамп
snmp_lakb_damp1[i][83]=' ';
snmp_lakb_damp1[i][84]=ABCDEF[(lakb_damp[i][28])>>4];			//Дамп
snmp_lakb_damp1[i][85]=ABCDEF[(lakb_damp[i][28])&0x0f];			//Дамп
snmp_lakb_damp1[i][86]=' ';
snmp_lakb_damp1[i][87]=ABCDEF[(lakb_damp[i][29])>>4];			//Дамп
snmp_lakb_damp1[i][88]=ABCDEF[(lakb_damp[i][29])&0x0f];			//Дамп
snmp_lakb_damp1[i][89]=0;


snmp_lakb_damp1[i][90]=ABCDEF[(lakb_damp[i][30])>>4];			//Дамп
snmp_lakb_damp1[i][91]=ABCDEF[(lakb_damp[i][30])&0x0f];			//Дамп
snmp_lakb_damp1[i][92]=' ';
snmp_lakb_damp1[i][93]=ABCDEF[(lakb_damp[i][31])>>4];			//Дамп
snmp_lakb_damp1[i][94]=ABCDEF[(lakb_damp[i][31])&0x0f];			//Дамп
snmp_lakb_damp1[i][95]=' ';
snmp_lakb_damp1[i][96]=ABCDEF[(lakb_damp[i][32])>>4];			//Дамп
snmp_lakb_damp1[i][97]=ABCDEF[(lakb_damp[i][32])&0x0f];			//Дамп
snmp_lakb_damp1[i][98]=' ';
snmp_lakb_damp1[i][99]=ABCDEF[(lakb_damp[i][33])>>4];			//Дамп
snmp_lakb_damp1[i][100]=ABCDEF[(lakb_damp[i][33])&0x0f];			//Дамп
snmp_lakb_damp1[i][101]=' ';
snmp_lakb_damp1[i][102]=ABCDEF[(lakb_damp[i][34])>>4];			//Дамп
snmp_lakb_damp1[i][103]=ABCDEF[(lakb_damp[i][34])&0x0f];			//Дамп
snmp_lakb_damp1[i][104]=' '; 
snmp_lakb_damp1[i][105]=ABCDEF[(lakb_damp[i][35])>>4];			//Дамп
snmp_lakb_damp1[i][106]=ABCDEF[(lakb_damp[i][35])&0x0f];			//Дамп
snmp_lakb_damp1[i][107]=' ';
snmp_lakb_damp1[i][108]=ABCDEF[(lakb_damp[i][36])>>4];			//Дамп
snmp_lakb_damp1[i][109]=ABCDEF[(lakb_damp[i][36])&0x0f];			//Дамп
snmp_lakb_damp1[i][110]=' ';
snmp_lakb_damp1[i][111]=ABCDEF[(lakb_damp[i][37])>>4];			//Дамп
snmp_lakb_damp1[i][112]=ABCDEF[(lakb_damp[i][37])&0x0f];			//Дамп
snmp_lakb_damp1[i][113]=' ';
snmp_lakb_damp1[i][114]=ABCDEF[(lakb_damp[i][38])>>4];			//Дамп
snmp_lakb_damp1[i][115]=ABCDEF[(lakb_damp[i][38])&0x0f];			//Дамп
snmp_lakb_damp1[i][116]=' ';
snmp_lakb_damp1[i][117]=ABCDEF[(lakb_damp[i][39])>>4];			//Дамп
snmp_lakb_damp1[i][118]=ABCDEF[(lakb_damp[i][39])&0x0f];			//Дамп
snmp_lakb_damp1[i][119]=0; 
 

snmp_lakb_damp1[i][120]=ABCDEF[(lakb_damp[i][40])>>4];			//Дамп
snmp_lakb_damp1[i][121]=ABCDEF[(lakb_damp[i][40])&0x0f];			//Дамп
snmp_lakb_damp1[i][122]=' ';
snmp_lakb_damp1[i][123]=ABCDEF[(lakb_damp[i][41])>>4];			//Дамп
snmp_lakb_damp1[i][124]=ABCDEF[(lakb_damp[i][41])&0x0f];			//Дамп
snmp_lakb_damp1[i][125]=' ';
snmp_lakb_damp1[i][126]=ABCDEF[(lakb_damp[i][42])>>4];			//Дамп
snmp_lakb_damp1[i][127]=ABCDEF[(lakb_damp[i][42])&0x0f];			//Дамп
snmp_lakb_damp1[i][128]=' ';
snmp_lakb_damp1[i][129]=ABCDEF[(lakb_damp[i][43])>>4];			//Дамп
snmp_lakb_damp1[i][130]=ABCDEF[(lakb_damp[i][43])&0x0f];			//Дамп
snmp_lakb_damp1[i][131]=' ';
snmp_lakb_damp1[i][132]=ABCDEF[(lakb_damp[i][44])>>4];			//Дамп
snmp_lakb_damp1[i][133]=ABCDEF[(lakb_damp[i][44])&0x0f];			//Дамп
snmp_lakb_damp1[i][134]=' '; 
snmp_lakb_damp1[i][135]=ABCDEF[(lakb_damp[i][45])>>4];			//Дамп
snmp_lakb_damp1[i][136]=ABCDEF[(lakb_damp[i][45])&0x0f];			//Дамп
snmp_lakb_damp1[i][137]=' ';
snmp_lakb_damp1[i][138]=ABCDEF[(lakb_damp[i][46])>>4];			//Дамп
snmp_lakb_damp1[i][139]=ABCDEF[(lakb_damp[i][46])&0x0f];			//Дамп
snmp_lakb_damp1[i][140]=' ';
snmp_lakb_damp1[i][141]=ABCDEF[(lakb_damp[i][47])>>4];			//Дамп
snmp_lakb_damp1[i][142]=ABCDEF[(lakb_damp[i][47])&0x0f];			//Дамп
snmp_lakb_damp1[i][143]=' ';
snmp_lakb_damp1[i][144]=ABCDEF[(lakb_damp[i][48])>>4];			//Дамп
snmp_lakb_damp1[i][145]=ABCDEF[(lakb_damp[i][48])&0x0f];			//Дамп
snmp_lakb_damp1[i][146]=' ';
snmp_lakb_damp1[i][147]=ABCDEF[(lakb_damp[i][49])>>4];			//Дамп
snmp_lakb_damp1[i][148]=ABCDEF[(lakb_damp[i][49])&0x0f];			//Дамп
snmp_lakb_damp1[i][149]=0; 

}

}

//-----------------------------------------------
void snmp_sernum_write (int mode) 
{
if(mode==MIB_WRITE)
	{
	lc640_write_long(EE_AUSW_MAIN_NUMBER,snmp_sernum);
	lc640_write_long(EE_AUSW_UKU_NUMBER,snmp_sernum);
	}
}

//-----------------------------------------------
void snmp_location_write (int mode) 
{
char i;
if(mode==MIB_WRITE)
	{
	for(i=0;i<64;i++)
		{
		lc640_write(EE_LOCATION+i,snmp_location[i]);
		}
	}
}

//-----------------------------------------------
void snmp_model_write (int mode) 
{
char i;
if(mode==MIB_WRITE)
	{
	for(i=0;i<30;i++)
		{
		lc640_write(EE_MODEL+i,snmp_model[i]);
		}
	}
}

//-----------------------------------------------
void snmp_main_bps_write (int mode)
{
if(mode==MIB_WRITE)
	{
	lc640_write_int(EE_MAIN_BPS,snmp_main_bps-1);
	}
}

//-----------------------------------------------
void snmp_zv_on_write (int mode)
{
if(mode==MIB_WRITE)
	{
	lc640_write_int(EE_ZV_ON,snmp_zv_en);
	}
}

//-----------------------------------------------
void snmp_alarm_auto_disable_write (int mode)
{
if(mode==MIB_WRITE)
	{
     lc640_write_int(EE_AV_OFF_AVT,snmp_alarm_auto_disable);
	}
}

//-----------------------------------------------
void snmp_bat_test_time_write (int mode)
{
if(mode==MIB_WRITE)
	{
     lc640_write_int(EE_TBAT,snmp_bat_test_time);
	}
}

//-----------------------------------------------
void snmp_u_max_write (int mode)
{
if(mode==MIB_WRITE)
	{
     lc640_write_int(EE_UMAX,snmp_u_max);
	}
}

//-----------------------------------------------
void snmp_u_0_grad_write (int mode)
{
if(mode==MIB_WRITE)
	{
     lc640_write_int(EE_UBMAX,snmp_u_0_grad);
	}
}
//-----------------------------------------------
void snmp_u_20_grad_write (int mode)
{
if(mode==MIB_WRITE)
	{
     lc640_write_int(EE_UB20,snmp_u_20_grad);
	}
}

//-----------------------------------------------
void snmp_u_sign_write (int mode)
{
if(mode==MIB_WRITE)
	{
     lc640_write_int(EE_USIGN,snmp_u_sign);
	}
}
//-----------------------------------------------
void snmp_u_min_power_write (int mode)
{
if(mode==MIB_WRITE)
	{
     lc640_write_int(EE_UMN,snmp_u_min_power);
	}
}
//-----------------------------------------------
void snmp_u_withouth_bat_write (int mode)
{
if(mode==MIB_WRITE)
	{
     lc640_write_int(EE_U0B,snmp_u_withouth_bat);
	}
}

//-----------------------------------------------
void snmp_control_current_write (int mode)
{
if(mode==MIB_WRITE)
	{
     lc640_write_int(EE_IKB,snmp_control_current);
	}
}

//-----------------------------------------------
void snmp_max_charge_current_write (int mode)
{
if(mode==MIB_WRITE)
	{
     lc640_write_int(EE_IZMAX,snmp_max_charge_current);
	}
}

//-----------------------------------------------
void snmp_max_current_write (int mode)
{
if(mode==MIB_WRITE)
	{
     lc640_write_int(EE_IMAX,snmp_max_current);
	}
}

//-----------------------------------------------
void snmp_max_current_koef_write (int mode)
{
if(mode==MIB_WRITE)
	{
     lc640_write_int(EE_KIMAX,snmp_max_current_koef);
	}
}

//-----------------------------------------------
void snmp_up_charge_koef_write (int mode)
{
if(mode==MIB_WRITE)
	{
     lc640_write_int(EE_KVZ,snmp_up_charge_koef);
	}
}

//-----------------------------------------------
void snmp_powerup_psu_timeout_write (int mode)
{
if(mode==MIB_WRITE)
	{
     lc640_write_int(EE_TZAS,snmp_powerup_psu_timeout);
	}
}

//-----------------------------------------------
void snmp_max_temperature_write (int mode)
{
if(mode==MIB_WRITE)
	{
     lc640_write_int(EE_TMAX,snmp_max_temperature);
	}
}

//-----------------------------------------------
void snmp_set_primary (int mode)
{

if(mode==MIB_WRITE)
	{
	if(snmp_command_parametr==1) 
		{
		lc640_write_int(EE_MAIN_BPS,0);
	     cnt_src[1]=10;
		snmp_command_parametr=COMMAND_OK;
		}
	
	else if(snmp_command_parametr==2)
		{
	     lc640_write_int(EE_MAIN_BPS,1);
		snmp_command_parametr=COMMAND_OK;
		}	
	else snmp_command_parametr=WRONG_PARAMETER;
	}
}

//-----------------------------------------------
void snmp_disable (int mode)
{
if(mode==MIB_WRITE)
	{
	if(snmp_command_parametr==1) 
		{
		St_[0]|=0x20;
		St_[1]&=0xdf;
		St&=0xfb;
		cnt_src[1]=10;
		snmp_plazma++;
		snmp_plazma++;
		snmp_command_parametr=COMMAND_OK;
		}
	
	else if(snmp_command_parametr==2)
		{
		St_[1]|=0x20;
		St_[0]&=0xdf;
		St&=0xfb;
		cnt_src[0]=10;	
		snmp_plazma++;
		snmp_command_parametr=COMMAND_OK;
		}	
	else snmp_command_parametr=WRONG_PARAMETER;
	}
}

//-----------------------------------------------
void snmp_unlock (int mode)
{
if(mode==MIB_WRITE)
	{
	if(snmp_command_parametr==1) 
		{
		snmp_command=COMMAND_OK;
		St_[0]&=0xdf;
		St_[1]&=0xdf;
		}
	
	else snmp_command_parametr=WRONG_PARAMETER;
	}
}

//-----------------------------------------------
void snmp_parallel_switch (int mode)
{
if(mode==MIB_WRITE)
	{
	if(snmp_command_parametr==1) 
		{
		snmp_command=COMMAND_OK;
	//	lc640_write_int(EE_PAR,1);
		}
	else if(snmp_command_parametr==0) 
		{
		snmp_command=COMMAND_OK;
	//	lc640_write_int(EE_PAR,0);
		}
	
	else snmp_command_parametr=WRONG_PARAMETER;
	}
}

//-----------------------------------------------
void snmp_vz_start (int mode)
{
if(mode==MIB_WRITE)
	{
	if((snmp_command_parametr>=1)&&(snmp_command_parametr<=10)) 
		{
		if(!(St&0x03)&&(spc_stat==spc_OFF))
			{
			spc_stat=spc_VZ;
			cnt_vz_sec=0L;
			cnt_vz_sec_=(4000L*(signed long)snmp_command_parametr);
			vz_mem_hndl(1);
			}
		else snmp_command_parametr=COMMAND_FAIL;
		}
	
	else snmp_command_parametr=WRONG_PARAMETER;
	}
}

//-----------------------------------------------
void snmp_ke_start (int mode)
{
if(mode==MIB_WRITE)
	{
	if(snmp_command_parametr==0) 
		{
		if(ke_start()==0)snmp_command=COMMAND_OK;
		else snmp_command=COMMAND_FAIL;
		}
	
	else snmp_command_parametr=WRONG_PARAMETER;
	}
}

//-----------------------------------------------
void snmp_spc_stop (int mode)
{
if(mode==MIB_WRITE)
	{
	spc_stat=spc_OFF;
	snmp_command=COMMAND_OK;
	}
}


//-----------------------------------------------
void snmp_command_execute (int mode)
{
if(mode==MIB_WRITE)
	{
	
	//snmp_command=0x5555;

	switch (snmp_command)
		{
		case SNMP_BPS_DISABLE:
			{
			snmp_command=COMMAND_OK;

		/*	switch (snmp_command_parametr)
				{
			
				case 1: 
				{
				St_[0]|=0x20;
				St_[1]&=0xdf;
				St&=0xfb;
				cnt_src[1]=10;
				snmp_plazma++;
				snmp_plazma++;
				break;
				}
			
				case 2:
				{
				St_[1]|=0x20;
				St_[0]&=0xdf;
				St&=0xfb;
				cnt_src[0]=10;	
				snmp_plazma++;
				break;
				}*/	
			
				//break;
			//	}
			if(snmp_command_parametr==1) 
				{
				St_[0]|=0x20;
				St_[1]&=0xdf;
				St&=0xfb;
				cnt_src[1]=10;
				snmp_plazma++;
				snmp_plazma++;
				}
			
			else if(snmp_command_parametr==2)
				{
				St_[1]|=0x20;
				St_[0]&=0xdf;
				St&=0xfb;
				cnt_src[0]=10;	
				snmp_plazma++;
				}	
			
			break;
			}

		case SNMP_BPS_UNDISABLE:
			{
			snmp_command=COMMAND_OK;
			St_[0]&=0xdf;
			St_[1]&=0xdf;
			break;
			}

		case SNMP_SPEC_VZ:
			{
			if((snmp_command_parametr>=1)&&(snmp_command_parametr<=24))
				{
				if(!(St&0x03)&&(NUMBAT))
					{
					snmp_command=COMMAND_OK;
					spc_stat=spc_VZ;
					cnt_vz_sec_=3600UL*snmp_command_parametr;
					}
				else
 					{
					snmp_command=COMMAND_FAIL;	
 					}
				}
			else 
				{
				snmp_command=WRONG_PARAMETER;
				}
			break;
			}

		case SNMP_SPEC_KE:
			{
	  		if(!(St&0x02)&&(NUMBAT))
				{
				spc_stat=spc_KE;
			//zar_cnt_ee_ke=0;
				zar_cnt=0L;
				snmp_command=COMMAND_OK;
				}
			else
				{
				snmp_command=COMMAND_FAIL;	
				}
			break;
			}

		case SNMP_SPEC_DISABLE:
			{
			spc_stat=spc_OFF;
			snmp_command=COMMAND_OK;
			break;
			}


		default:
			{
			snmp_command=COMMAND_INVALID;
			break;
			}
		}
/*		else if((UIB2[1]==0x52)&&(UIB2[4]==5)&&(UIB2[5]==5)&&(UIB2[6])&&(UIB2[6]<=NUMIST)&&(UIB2[6]==UIB2[7])) 	//Выключение источника 
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
		} */



	}
}

//-----------------------------------------------
char* datatime2str(char day,char month,char year, char hour, char min, char sec)
{

memcpy(temp_str,"00/янв/00  00:00:00       ",20);

temp_str[1]=(day%10)+0x30;
temp_str[0]=(day/10)+0x30;

memcpy(&temp_str[3],sm_mont[month],3);

temp_str[8]=(year%10)+0x30;
temp_str[7]=(year/10)+0x30;

temp_str[12]=(hour%10)+0x30;
temp_str[11]=(hour/10)+0x30;

temp_str[15]=(min%10)+0x30;
temp_str[14]=(min/10)+0x30;

temp_str[18]=(sec%10)+0x30;
temp_str[17]=(sec/10)+0x30;

return temp_str;
}

//-----------------------------------------------
void event2snmp(char num)
{
char /*iii,*/index;
char dt[4],dt_[4],dt__[4],dt___[4],dt____[4],dt4[4];
unsigned int tempii;    

memcpy(&snmp_log[num][0],"                                                                                ",78);
//memcpy(&snmp_log[num][0],"BKL",10);


		
tempii=lc640_read_int(PTR_EVENT_LOG);
tempii=ptr_carry(tempii,64,-1*((signed)num));
tempii*=32;
tempii+=EVENT_LOG;
     
lc640_read_long_ptr(tempii,dt);
lc640_read_long_ptr(tempii+4,dt4);
lc640_read_long_ptr(tempii+8,dt_);
lc640_read_long_ptr(tempii+12,dt__);
lc640_read_long_ptr(tempii+16,dt___);
lc640_read_long_ptr(tempii+20,dt____);
//iii=find(simbol);
     
if(dt[0]=='U')	 		//Включение ИБЭПа
    	{ 
    	if(dt[2]=='R')
    		{
		memcpy(&snmp_log[num][0],"Включение ИБЭПа@                                      ",50);
		memcpy(&snmp_log[num][17],datatime2str(dt_[2],dt_[1],dt_[0],dt__[0],dt__[1],dt__[2]),20);
		memcpy(&snmp_log[num][40],"@                   ",20);
    		}
     }   

     
else if(dt[0]=='P')		//Авария питающей сети
	{
	index=0;
     memcpy(&snmp_log[num][index],"Авария питающей сети @  ",23);	
	index+=23;
	memcpy(&snmp_log[num][index],datatime2str(dt_[2],dt_[1],dt_[0],dt__[0],dt__[1],dt__[2]),19);
	index+=19;
	snmp_log[num][index]='@';
	index++;

	if((dt___[0]=='A')&&(dt___[1]=='A'))
		{
		memcpy(&snmp_log[num][index]," не устранена  ",13);
		index+=13;
		}
	else 
		{
		memcpy(&snmp_log[num][index]," устранена   ",11);
		index+=11;
			
		memcpy(&snmp_log[num][index],datatime2str(dt___[2],dt___[1],dt___[0],dt____[0],dt____[1],dt____[2]),19);
		}

     }  

else if(dt[0]=='B')		//События батареи
    	{
	index=0;
    	if(dt[2]=='C')
    		{
		memcpy(&snmp_log[num][index],"Батарея.  Авария!!! @  ",21);
		index+=21;
		memcpy(&snmp_log[num][index],datatime2str(dt_[2],dt_[1],dt_[0],dt__[0],dt__[1],dt__[2]),19);
		index+=19;
		
		snmp_log[num][index]='@';
		index++;

		memcpy(&snmp_log[num][index],"Батарея не обнаружена, ",22);
		index+=22;

		if((dt___[0]=='A')&&(dt___[1]=='A'))
			{
			memcpy(&snmp_log[num][index]," не устранена  ",13);
			index+=13;
			}
		else 
			{
			memcpy(&snmp_log[num][index]," устранена   ",11);
			index+=11;
			memcpy(&snmp_log[num][index],datatime2str(dt___[2],dt___[1],dt___[0],dt____[0],dt____[1],dt____[2]),19);
			}

    		}
    	if(dt[2]=='Z')
    		{

		memcpy(&snmp_log[num][index],"Батарея.  Выравнивающий заряд @  ",32);
		index+=32;
		memcpy(&snmp_log[num][index],datatime2str(dt_[2],dt_[1],dt_[0],dt__[0],dt__[1],dt__[2]),19);
		index+=19;
		
		snmp_log[num][index]='@';
		index++;

		if((dt___[0]=='A')&&(dt___[1]=='A'))
			{
			memcpy(&snmp_log[num][index]," не завершен  ",13);
			index+=13;
			}
		else 
			{
			memcpy(&snmp_log[num][index]," завершен   ",11);
			index+=11;
			memcpy(&snmp_log[num][index],datatime2str(dt___[2],dt___[1],dt___[0],dt____[0],dt____[1],dt____[2]),19);
			}


/*
    		lcd_buffer[iii++]='В';
    		lcd_buffer[iii++]='З';
    		lcd_buffer[iii++]=' ';    		
    		lcd_buffer[iii++]=' ';
    		lcd_buffer[iii++]=' ';  
    		lcd_buffer[iii++]=' ';
    		lcd_buffer[iii++]=' ';
    		
    		if((dt_[0]==LPC_RTC->YEAR)&&(dt_[1]==LPC_RTC->MONTH)&&(dt_[2]==LPC_RTC->DOM))
    			{
    	    		lcd_buffer[iii++]='0';
    			lcd_buffer[iii++]='!';
    	    		lcd_buffer[iii++]=':'; 
    			lcd_buffer[iii++]='0';
    			lcd_buffer[iii++]='@';
    			lcd_buffer[iii++]=':';
    			lcd_buffer[iii++]='0';
    			lcd_buffer[iii++]='#';
    	    		int2lcd(dt__[0],'!',0);
    			int2lcd(dt__[1],'@',0);
    			int2lcd(dt__[2],'#',0);    		     		
    			}	                   
 		else      	
    			{
    	 		lcd_buffer[iii++]=' ';
    			lcd_buffer[iii++]='0';
    			lcd_buffer[iii++]='!';
    			lcd_buffer[iii++]='@'; 
    			lcd_buffer[iii++]=' ';
    			lcd_buffer[iii++]=' ';
    			lcd_buffer[iii++]='0';
    			lcd_buffer[iii++]='#';
    			int2lcd(dt_[2],'!',0);
    			
    			int2lcd(dt_[0],'#',0);   
    			if(!((dt_[1]>=1)&&(dt_[1]<=12)))dt_[1]=1;
			sub_bgnd(sm_mont[dt_[1]],'@',0);  
    			} */ 		
    		}    		
/*
    	if(dt[2]=='W')
    		{
    		lcd_buffer[iii++]='Б';
    		lcd_buffer[iii++]='а';
    		lcd_buffer[iii++]='т';
		if(dt[1]<9)
    			{
    			lcd_buffer[iii++]=0x31+dt[1];
    			lcd_buffer[iii++]='р';
    			lcd_buffer[iii++]=' ';
    			}
    		else if((dt[1]>=9)&&(dt[1]<99))
    			{
    			lcd_buffer[iii++]=0x30+((dt[1]+1)/10);
    			lcd_buffer[iii++]=0x30+((dt[1]+1)%10);
    			lcd_buffer[iii++]='р';
    			}
    		else 
    			{
    			lcd_buffer[iii++]=' ';
    			lcd_buffer[iii++]=' '; 
    			lcd_buffer[iii++]=' ';    		
    			} 
    		lcd_buffer[iii++]=' ';    		
    		
    		
		
    		if((dt_[0]==LPC_RTC->YEAR)&&(dt_[1]==LPC_RTC->MONTH)&&(dt_[2]==LPC_RTC->DOM))
    			{
    	    		lcd_buffer[iii++]='0';
    			lcd_buffer[iii++]='!';
    	    		lcd_buffer[iii++]=':'; 
    			lcd_buffer[iii++]='0';
    			lcd_buffer[iii++]='@';
    			lcd_buffer[iii++]=':';
    			lcd_buffer[iii++]='0';
    			lcd_buffer[iii++]='#';
    	    		int2lcd(dt__[0],'!',0);
    			int2lcd(dt__[1],'@',0);
    			int2lcd(dt__[2],'#',0);    		     		
    			}	                   
 		else      	
    			{
    	 		lcd_buffer[iii++]=' ';
    			lcd_buffer[iii++]='0';
    			lcd_buffer[iii++]='!';
    			lcd_buffer[iii++]='@'; 
    			lcd_buffer[iii++]=' ';
    			lcd_buffer[iii++]=' ';
    			lcd_buffer[iii++]='0';
    			lcd_buffer[iii++]='#';
    			int2lcd(dt_[2],'!',0);
    			
    			int2lcd(dt_[0],'#',0);   
    			if(!((dt_[1]>=1)&&(dt_[1]<=12)))dt_[1]=1;
			sub_bgnd(sm_mont[dt_[1]],'@',0);  
    			}  		
    		} */   		
 
 	if(dt[2]=='K')
    		{

		memcpy(&snmp_log[num][index],"Батарея.  Контроль емкости @  ",29);
		index+=29;
		memcpy(&snmp_log[num][index],datatime2str(dt_[2],dt_[1],dt_[0],dt__[0],dt__[1],dt__[2]),19);
		index+=19;
		
		snmp_log[num][index]='@';
		index++;

		if((dt___[0]=='A')&&(dt___[1]=='A'))
			{
			memcpy(&snmp_log[num][index]," не завершен  ",13);
			index+=13;
			}
		else 
			{
			short temp_US;
			memcpy(&snmp_log[num][index]," завершен   ",11);
			index+=11;
			memcpy(&snmp_log[num][index],datatime2str(dt___[2],dt___[1],dt___[0],dt____[0],dt____[1],dt____[2]),19);
			memcpy(&snmp_log[num][index],", Uнач   , В, Uкон   , В, Cбат    , А*ч ", 39);
			
			temp_US=dt_[3]+(dt__[3]*256);

			snmp_log[num][index+10]=(temp_US%10)+0x30;
			temp_US/=10;
			snmp_log[num][index+8]=(temp_US%10)+0x30;
			temp_US/=10;
			if(temp_US)snmp_log[num][index+7]=(temp_US%10)+0x30;
			else snmp_log[num][index+7]=0x20;


			temp_US=dt4[2]+(dt4[3]*256);

			snmp_log[num][index+22]=(temp_US%10)+0x30;
			temp_US/=10;
			snmp_log[num][index+20]=(temp_US%10)+0x30;
			temp_US/=10;
			if(temp_US)snmp_log[num][index+19]=(temp_US%10)+0x30;
			else snmp_log[num][index+19]=0x20;


			temp_US=dt4[0]+(dt4[1]*256);

			snmp_log[num][index+35]=(temp_US%10)+0x30;
			temp_US/=10;
			snmp_log[num][index+33]=(temp_US%10)+0x30;
			temp_US/=10;
			if(temp_US)snmp_log[num][index+32]=(temp_US%10)+0x30;
			else snmp_log[num][index+32]=0x20;
			}



		/*
    		lcd_buffer[iii++]='Б';
    		lcd_buffer[iii++]='а';
    		lcd_buffer[iii++]='т';
		if(dt[1]<9)
    			{
    			lcd_buffer[iii++]=0x31+dt[1];
    			lcd_buffer[iii++]='к';
    			lcd_buffer[iii++]='е';
    			}
    		else if((dt[1]>=9)&&(dt[1]<99))
    			{
    			lcd_buffer[iii++]=0x30+((dt[1]+1)/10);
    			lcd_buffer[iii++]=0x30+((dt[1]+1)%10);
    			lcd_buffer[iii++]='к';
    			}
    		else 
    			{
    			lcd_buffer[iii++]=' ';
    			lcd_buffer[iii++]=' '; 
    			lcd_buffer[iii++]=' ';    		
    			} 
    		lcd_buffer[iii++]=' ';    		
    		
    		if((dt_[0]==LPC_RTC->YEAR)&&(dt_[1]==LPC_RTC->MONTH)&&(dt_[2]==LPC_RTC->DOM))
    			{
    	    		lcd_buffer[iii++]='0';
    			lcd_buffer[iii++]='!';
    	    		lcd_buffer[iii++]=':'; 
    			lcd_buffer[iii++]='0';
    			lcd_buffer[iii++]='@';
    			lcd_buffer[iii++]=':';
    			lcd_buffer[iii++]='0';
    			lcd_buffer[iii++]='#';
    	    		int2lcd(dt__[0],'!',0);
    			int2lcd(dt__[1],'@',0);
    			int2lcd(dt__[2],'#',0);    		     		
    			}	                   
 		else      	
    			{
    	 		lcd_buffer[iii++]=' ';
    			lcd_buffer[iii++]='0';
    			lcd_buffer[iii++]='!';
    			lcd_buffer[iii++]='@'; 
    			lcd_buffer[iii++]=' ';
    			lcd_buffer[iii++]=' ';
    			lcd_buffer[iii++]='0';
    			lcd_buffer[iii++]='#';
    			int2lcd(dt_[2],'!',0);
    			
    			int2lcd(dt_[0],'#',0);   
    			if(!((dt_[1]>=1)&&(dt_[1]<=12)))dt_[1]=1;
			sub_bgnd(sm_mont[dt_[1]],'@',0);  
    			} */ 		
    		}    		

    		     	     	
    	}     	    
     	
else if(dt[0]=='S')
    	{
	index=0;
	memcpy(&snmp_log[num][0],"БПС №      ",6);
	index=6;
	snmp_log[num][5]=0x31+dt[1];
	index=7;
	memcpy(&snmp_log[num][index],"   Авария!!!@  ",14);
	index+=14;
		//memcpy(&snmp_log[num][0/*+num*/],"00/янв/11 00:00:00 @",20);
	memcpy(&snmp_log[num][index],datatime2str(dt_[2],dt_[1],dt_[0],dt__[0],dt__[1],dt__[2]),19);
	index+=20;

	if(dt[2]=='L')
		{
		memcpy(&snmp_log[num][40],"@отключился             ",20);
		index=65;
		}
	else if(dt[2]=='T')
		{
		memcpy(&snmp_log[num][index],"@ перегрев   ",10);
		index+=10;
		}		
	else if(dt[2]=='U')
		{
		memcpy(&snmp_log[num][index],"@ завышено Uвых.  ",16);
		index+=16;
		}		
	else if(dt[2]=='u')
		{
		memcpy(&snmp_log[num][index],"@ занижено Uвых.  ",16);
		index+=16;
		}
	else 		
		{
		memcpy(&snmp_log[num][index],"@ фигня  ",7);
		index+=7;
		}


	if((dt___[0]=='A')&&(dt___[1]=='A'))
		{
		memcpy(&snmp_log[num][index],", не устранена  ",15);
		index+=15;
		}
	else 
		{
		memcpy(&snmp_log[num][index],",  устранена   ",13);
		index+=13;
			
		memcpy(&snmp_log[num][index],datatime2str(dt___[2],dt___[1],dt___[0],dt____[0],dt____[1],dt____[2]),19);


		    /*
			ptrs[4]="  0[]  0< 0>:0=:0)  ";
			bgnd_par(ptrs[index_set],ptrs[1+index_set],ptrs[2+index_set],ptrs[3+index_set]);
			int2lcd(av_data_off[4],'>',0);
			int2lcd(av_data_off[5],'=',0);
			int2lcd(av_data_off[6],')',0);
			int2lcd(av_data_off[2],'[',0);
			int2lcd(av_data_off[0],'<',0); 
			if(!((av_data_off[1]>=1)&&(av_data_off[1]<=12)))av_data_off[1]=1;
			sub_bgnd(sm_mont[av_data_off[1]],']',0);   */





 /*
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
			
						
			}	
		int2lcd(av_head[1]+1,'+',0);
		int2lcd(av_data_on[4],'@',0);
		int2lcd(av_data_on[5],'#',0);
		int2lcd(av_data_on[6],'$',0);
		int2lcd(av_data_on[2],'%',0);
		int2lcd(av_data_on[0],'^',0); 
		if(!((av_data_on[1]>=1)&&(av_data_on[1]<=12)))av_data_on[1]=1;
		sub_bgnd(sm_mont[av_data_on[1]],'(',0);
		*/
	    }



/*    	lcd_buffer[iii++]='Б';
    	lcd_buffer[iii++]='П';
    	lcd_buffer[iii++]='С';
 	if(dt[1]<9)
    		{
    		lcd_buffer[iii++]=0x31+dt[1];
    		lcd_buffer[iii++]=' ';
    		}
    	else if((dt[1]>=9)&&(dt[1]<99))
    		{
    		lcd_buffer[iii++]=0x30+((dt[1]+1)/10);
    		lcd_buffer[iii++]=0x30+((dt[1]+1)%10);
    		}
    	else 
    		{
    		lcd_buffer[iii++]=' ';
    		lcd_buffer[iii++]=' ';     		
    		} 
    	lcd_buffer[iii++]=' '; 
    	lcd_buffer[iii++]=' ';
    	
    	if((dt_[0]==LPC_RTC->YEAR)&&(dt_[1]==LPC_RTC->MONTH)&&(dt_[2]==LPC_RTC->DOM))
    		{
    	    	lcd_buffer[iii++]='0';
    		lcd_buffer[iii++]='!';
    	    	lcd_buffer[iii++]=':'; 
    		lcd_buffer[iii++]='0';
    		lcd_buffer[iii++]='@';
    		lcd_buffer[iii++]=':';
    		lcd_buffer[iii++]='0';
    		lcd_buffer[iii++]='#';
    	    	int2lcd(dt__[0],'!',0);
    		int2lcd(dt__[1],'@',0);
    		int2lcd(dt__[2],'#',0);    		     		
    		}	                   
 	else      	
    		{
    	 	lcd_buffer[iii++]=' ';
    		lcd_buffer[iii++]='0';
    		lcd_buffer[iii++]='!';
    		lcd_buffer[iii++]='@'; 
    		lcd_buffer[iii++]=' ';
    		lcd_buffer[iii++]=' ';
    		lcd_buffer[iii++]='0';
    		lcd_buffer[iii++]='#';
    		int2lcd(dt_[2],'!',0);
    		int2lcd(dt_[0],'#',0);   
    		if(!((dt_[1]>=1)&&(dt_[1]<=12)))dt_[1]=1;
		sub_bgnd(sm_mont[dt_[1]],'@',0);  
		} */   	
    	}
  /*   	
else if(dt[0]=='B')
    	{
    	lcd_buffer[iii++]='Б';
    	lcd_buffer[iii++]='а';
    	lcd_buffer[iii++]='т';
 	if(dt[1]<9)
    		{
    		lcd_buffer[iii++]=0x31+dt[1];
    		lcd_buffer[iii++]=' ';
    		}
    	else if((dt[1]>=9)&&(dt[1]<99))
    		{
    		lcd_buffer[iii++]=0x30+((dt[1]+1)/10);
    		lcd_buffer[iii++]=0x30+((dt[1]+1)%10);
    		}
    	else 
    		{
    		lcd_buffer[iii++]=' ';
    		lcd_buffer[iii++]=' ';     		
    		} 
    	lcd_buffer[iii++]=' ';
    	}     	    
else if(dt[0]=='I')
    	{
    	lcd_buffer[iii++]='И';
    	lcd_buffer[iii++]='н';
    	lcd_buffer[iii++]='в';
	if(dt[1]<9)
    		{
    		lcd_buffer[iii++]=0x31+dt[1];
    		lcd_buffer[iii++]=' ';
    		}
    	else if((dt[1]>=9)&&(dt[1]<99))
    		{
    		lcd_buffer[iii++]=0x30+((dt[1]+1)/10);
    		lcd_buffer[iii++]=0x30+((dt[1]+1)%10);
    		}
    	else 
    		{
    		lcd_buffer[iii++]=' ';
    		lcd_buffer[iii++]=' ';     		
    		} 
    	lcd_buffer[iii++]=' ';
    	} */   
}

//-----------------------------------------------
void snmp_trap_send(char* str, signed short in0, signed short in1, signed short in2)
{
/*
for(snmp_trap_send_i=0;snmp_trap_send_i<100;snmp_trap_send_i++)
	{
	snmp_spc_trap_message[snmp_trap_send_i]=0;
	} */

mem_copy(snmp_spc_trap_message,"                                                  ",50);
mem_copy(snmp_spc_trap_message,str,50);

obj[0]=4;
obj[1]=0;
obj[2]=1;
obj[3]=2;
obj[4]=3;

snmp_spc_trap_value_0=in0;
snmp_spc_trap_value_1=in1;
snmp_spc_trap_value_2=in2;
/*	temp_ip[0]= 192;
	temp_ip[1]= 168;
	temp_ip[2]= 1;
	temp_ip[3]= 9;
	__disable_irq();
		snmp_trap (temp_ip, 6, 3, obj);
	__enable_irq();*/			



//obj[0] = 0;
/*
snmp_trap_send_ii=1;
if(str!=0)
	{
	obj[0]++;
	for(snmp_trap_send_i=0;snmp_trap_send_i<100&&(str[snmp_trap_send_i]);snmp_trap_send_i++)
		{
		snmp_spc_trap_message[snmp_trap_send_i]=str[snmp_trap_send_i];
		}
	obj[snmp_trap_send_ii] = 7;
	snmp_trap_send_ii++;
	}
if(in0!=0xffff)
	{
	obj[0]++;
	snmp_spc_trap_value_0=in0;
	obj[snmp_trap_send_ii] = 8;
	snmp_trap_send_ii++;
	}
if(in1!=0xffff)
	{
	obj[0]++;
	snmp_spc_trap_value_1=in1;
	obj[snmp_trap_send_ii] = 9;
	snmp_trap_send_ii++;
	}
if(in2!=0xffff)
	{
	obj[0]++;
	snmp_spc_trap_value_2=in2;
	obj[snmp_trap_send_ii] = 10;
	snmp_trap_send_ii++;
	}
 */

if((ETH_TRAP1_IP_1!=255)&&(ETH_TRAP1_IP_2!=255)&&(ETH_TRAP1_IP_3!=255)&&(ETH_TRAP1_IP_4!=255))
	{
	temp_ip[0]= ETH_TRAP1_IP_1;
	temp_ip[1]= ETH_TRAP1_IP_2;
	temp_ip[2]= ETH_TRAP1_IP_3;
	temp_ip[3]= ETH_TRAP1_IP_4;
	snmp_trap (temp_ip, 6, 3, obj);
	}
if((ETH_TRAP2_IP_1!=255)&&(ETH_TRAP2_IP_2!=255)&&(ETH_TRAP2_IP_3!=255)&&(ETH_TRAP2_IP_4!=255))
	{
	temp_ip[0]= ETH_TRAP2_IP_1;
	temp_ip[1]= ETH_TRAP2_IP_2;
	temp_ip[2]= ETH_TRAP2_IP_3;
	temp_ip[3]= ETH_TRAP2_IP_4;
	snmp_trap (temp_ip, 6, 3, obj);
	}

if((ETH_TRAP3_IP_1!=255)&&(ETH_TRAP3_IP_2!=255)&&(ETH_TRAP3_IP_3!=255)&&(ETH_TRAP3_IP_4!=255))
	{
	temp_ip[0]= ETH_TRAP3_IP_1;
	temp_ip[1]= ETH_TRAP3_IP_2;
	temp_ip[2]= ETH_TRAP3_IP_3;
	temp_ip[3]= ETH_TRAP3_IP_4;
	snmp_trap (temp_ip, 6, 3, obj);
	}

if((ETH_TRAP4_IP_1!=255)&&(ETH_TRAP4_IP_2!=255)&&(ETH_TRAP4_IP_3!=255)&&(ETH_TRAP4_IP_4!=255))
	{
	temp_ip[0]= ETH_TRAP4_IP_1;
	temp_ip[1]= ETH_TRAP4_IP_2;
	temp_ip[2]= ETH_TRAP4_IP_3;
	temp_ip[3]= ETH_TRAP4_IP_4;
	snmp_trap (temp_ip, 6, 3, obj);
	}

if((ETH_TRAP5_IP_1!=255)&&(ETH_TRAP5_IP_2!=255)&&(ETH_TRAP5_IP_3!=255)&&(ETH_TRAP5_IP_4!=255))
	{
	temp_ip[0]= ETH_TRAP5_IP_1;
	temp_ip[1]= ETH_TRAP5_IP_2;
	temp_ip[2]= ETH_TRAP5_IP_3;
	temp_ip[3]= ETH_TRAP5_IP_4;
	snmp_trap (temp_ip, 6, 3, obj);
	}

/*for(snmp_trap_send_i=0;snmp_trap_send_i<100;snmp_trap_send_i++)
	{
	snmp_spc_trap_message[snmp_trap_send_i]=0;
	}*/
/*
mem_copy(snmp_spc_trap_message,"ADSFFHFGJHFGHKKY                              ",20);
snmp_spc_trap_value_0=11;
snmp_spc_trap_value_1=22;
snmp_spc_trap_value_2=33;



	obj[0]=4;
	obj[1]=0;
	obj[2]=1;
	obj[3]=2;
	obj[4]=3;

	temp_ip[0]= 192;
	temp_ip[1]= 168;
	temp_ip[2]= 1;
	temp_ip[3]= 9;
	__disable_irq();
		snmp_trap (temp_ip, 6, 3, obj);
	__enable_irq();*/			
}


 
