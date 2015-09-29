#define CAN_DEVICE_VERSION    0x050A

#define OLD_CAN_PROTOKOL    

#define NUM_OF_SET_TABL	1
#define DEEP_OF_SET_TABL 18

#define LCD_SIZE 200

#define MASK(lengt) 		(0xffffffff>>(32-lengt))
#define MASK_OFFSET(shift,lengt)	(MASK(lengt)<<shift)

#define GET_REG( reg, shift, lengt) 		( (reg & MASK_OFFSET(shift,lengt)) >> shift)
#define SET_REG( reg, val, shift, lengt)  	reg = ( (reg & ~MASK_OFFSET(shift,lengt)) | ((val&MASK(lengt)) << shift) )
//#define CHK_REG( reg, mask ) ( (reg) & (mask) == (mask) )




#define OFFBP1 GET_REG(LPC_GPIO2->FIOPIN,0,1)		// blok
#define OFFBP2 GET_REG(LPC_GPIO2->FIOPIN,1,1)		// blok

#ifndef NULL
#define NULL    ((void *)0)
#endif

#ifndef FALSE
#define FALSE   (0)
#endif

#ifndef TRUE
#define TRUE    (1)
#endif

#define BIN__N(x) (x) | x>>3 | x>>6 | x>>9
#define BIN__B(x) (x) & 0xf | (x)>>12 & 0xf0
#define BIN8(v) (BIN__B(BIN__N(0x##v)))

#define SEC_IN_HOUR	3600L

#define KOEFPOT 100

#define BAUD_RATE0 9600UL
#define BAUD_RATE1 9600UL

#define HARDVARE_VERSION	 5

#define AVAR_UNET_ON 	5
#define AV_UMAX_ON 		100
#define AV_UMIN_ON 		30 
#define AV_T_ON 		20
#define AV_UBAT_ON 		25
#define AV_IBAT_ON 		15 

//#define PAROL_ALL_ZERO


#ifndef PAROL_ALL_ZERO
#define PAROL_KALIBR 873
#define PAROL_SET 184
#define PAROL_MODE 0 
#define PAROL_ALLER 815 
#define PAROL_BAT_IN 722 
#define PAROL_BAT_OUT 722
#define PAROL_KE 125
#define PAROL_VZ 126
#define PAROL_TST 999 
#define PAROL_DEFAULT 295
#define PAROL_AUSW 949
#endif                

#ifdef PAROL_ALL_ZERO
#define PAROL_KALIBR 0
#define PAROL_SET 0
#define PAROL_MODE 0 
#define PAROL_ALLER 0 
#define PAROL_BAT_IN 0 
#define PAROL_BAT_OUT 0
#define PAROL_KE 0
#define PAROL_VZ 0
#define PAROL_TST 0
#define PAROL_DEFAULT 0
#define PAROL_AUSW 0
#endif

#define BUT0	25
#define BUT1	21
#define BUT2	23
#define BUT3	22
#define BUT4	24   
#define BUT_MASK (1UL<<BUT0)|(1UL<<BUT1)|(1UL<<BUT2)|(1UL<<BUT3)|(1UL<<BUT4)

#define OID_ENTERPRISE 	OID0(1,3), 6, 1, 4, 1, 130, 131, 31
#define OID_DEVICE 		10
#define DISPLAY_DEVICE_INFO			1
#define DISPLAY_DEVICE_INFO_CODE			1
#define DISPLAY_DEVICE_INFO_SERIAL			2
#define DISPLAY_DEVICE_INFO_SERIAL_LSB		3
#define DISPLAY_DEVICE_INFO_SERIAL_MSB		4
#define DISPLAY_DEVICE_INFO_LOCATION 		5
#define DISPLAY_DEVICE_INFO_NUMOFBAT 		6
#define DISPLAY_DEVICE_INFO_NUMOFBPS 		7
#define DISPLAY_DEVICE_INFO_NUMOFEVENTS		8
#define DISPLAY_DEVICE_INFO_MODEL			9

#define DISPLAY_MAINS_POWER			2
#define DISPLAY_MAINS_POWER_VOLTAGE		1
#define DISPLAY_MAINS_POWER_FREQUENCY		2
#define DISPLAY_MAINS_POWER_STATUS			3
#define DISPLAY_MAINS_POWER_ALARM			4

#define DISPLAY_LOAD				3
#define DISPLAY_LOAD_VOLTAGE				1
#define DISPLAY_LOAD_CURRENT				2

#define DISPLAY_PSU					4
#define DISPLAY_PSU_ENTRY_NUMBER			1,1
#define DISPLAY_PSU_ENTRY_VOLTAGE 			1,2
#define DISPLAY_PSU_ENTRY_CURRENT			1,3
#define DISPLAY_PSU_ENTRY_TEMPERATURE		1,4
#define DISPLAY_PSU_ENTRY_STATUS			1,5

#define DISPLAY_BAT					5
#define DISPLAY_BAT_ENTRY_NUMBER			1,1
#define DISPLAY_BAT_ENTRY_VOLTAGE			1,2
#define DISPLAY_BAT_ENTRY_CURRENT			1,3
#define DISPLAY_BAT_ENTRY_TEMPERATURE		1,4
#define DISPLAY_BAT_ENTRY_CAPACITY			1,5
#define DISPLAY_BAT_ENTRY_CHARGE			1,6
#define DISPLAY_BAT_ENTRY_STATUS			1,7

#define DISPLAY_SPEC				6
#define DISPLAY_SPEC_STAT				1
#define DISPLAY_SPEC_TRAP_MESSAGE			4
#define DISPLAY_SPEC_TRAP_VALUE 			5


#define LCD_SIZE 200

#define SNMP_COMMAND				8
#define COMMAND_SET_PRIMARY				1
#define COMMAND_DISABLE					2
#define COMMAND_UNLOCK					3
#define COMMAND_PARALLEL					4
#define COMMAND_COMP_CHARG				5
#define COMMAND_CAP_CONTR				6
#define COMMAND_DISABLE_SPEC				7

#define SYSPARAMS					10
#define SYSPARAMSMAINPSU					1
#define SYSPARAMSSOUNDALARMEN				2
#define SYSPARAMSALARMAUTODISABLE			3
#define SYSPARAMS_BAT_TEST_TIME			4
#define SYSPARAMS_U_MAX					5
#define SYSPARAMS_U_0_GRAD				6
#define SYSPARAMS_U_20_GRAD				7 
#define SYSPARAMS_U_SIGN					8
#define SYSPARAMS_U_MIN_POWER				9
#define SYSPARAMS_U_WITHOUT_BAT			10
#define SYSPARAMS_IBK					11
#define SYSPARAMS_IZMAX					12
#define SYSPARAMS_IMAX					13
#define SYSPARAMS_KIMAX					14
#define SYSPARAMS_KVZ					15
#define SYSPARAMS_TZAS					16
#define SYSPARAMS_TMAX					17	

#define DISPLAY_FUSE					14
#define DISPLAY_FUSE_ENTRY_NUMBER			1,1
#define DISPLAY_FUSE_ENTRY_AV_STAT			1,2

#define DISPLAY_SK						15
#define DISPLAY_SK_ENTRY_NUMBER			1,1
#define DISPLAY_SK_ENTRY_AV_STAT			1,2

#define DISPLAY_LAKB				18
#define DISPLAY_LAKB_ENTRY_NUMBER			1,1
#define DISPLAY_LAKB_MAX_CELL_VOLTAGE		1,2
#define DISPLAY_LAKB_MIN_CELL_VOLTAGE		1,3
#define DISPLAY_LAKB_MAX_CELL_TEMPERATURE	1,4
#define DISPLAY_LAKB_MIN_CELL_TEMPERATURE	1,5
#define DISPLAY_LAKB_VOLTAGE				1,6
#define DISPLAY_LAKB_CH_CURR				1,7
#define DISPLAY_LAKB_DSCH_CURR			1,8
#define DISPLAY_LAKB_RAT_CAP				1,9
#define DISPLAY_LAKB_SOH					1,10
#define DISPLAY_LAKB_SOC					1,11	
#define DISPLAY_LAKB_CCLV				1,12
#define DISPLAY_LAKB_RBT					1,13
#define DISPLAY_LAKB_FLAGS1				1,14
#define DISPLAY_LAKB_FLAGS2				1,15
#define DISPLAY_LAKB_DAMP1				1,16
#define DISPLAY_LAKB_DAMP2				1,17
#define DISPLAY_LAKB_DAMP3				1,18
#define DISPLAY_LAKB_DAMP4				1,19
#define DISPLAY_LAKB_DAMP5				1,20

#define COMMAND_OK		0x5555
#define COMMAND_FAIL	0xaaaa
#define COMMAND_INVALID	0xeeee
#define WRONG_PARAMETER	0xeeef

#define SNMP_BPS_DISABLE		5
#define SNMP_BPS_UNDISABLE	8
#define SNMP_SPEC_VZ		3
#define SNMP_SPEC_KE		4
#define SNMP_SPEC_DISABLE	7

#define DISPLAY_LOG					9
#define DISPLAY_LOG_ENTRY_EVENTS 			1,1
//#define DISPLAY_LOG_ENTRY_EVENTS 			1,1

#define DISPLAY_SPEC_TRAP_MESSAGE			5
#define DISPLAY_SPEC_TRAP_VALUE_0			6
#define DISPLAY_SPEC_TRAP_VALUE_1			7
#define DISPLAY_SPEC_TRAP_VALUE_2			8

#define SPA_OID	130, 131, 31,

//-----------------------------------------------
//Таймер
extern char b1000Hz,b100Hz,b50Hz,b10Hz,b5Hz,b2Hz,b1Hz,b33Hz;
extern short t0cnt,t0cnt0,t0cnt1,t0cnt2,t0cnt3,t0cnt4,t0cnt5,t0cnt6,t0_cnt7,t0_cnt8;
extern char bFL5,bFL2,bFL,bFL_;

//-----------------------------------------------
//АЦП
//extern int adc_buff[10][16],adc_buff1[10][16],adc_buff2[10][16];
//extern int adc_buff_[10],adc_buff1_[10],adc_buff2_[10],unet_buff[18],unet_buff_,adc_chk_buff[3];
//extern char adc_cnt,adc_cnt1,adc_ch,adc_check;
extern char adc_cnt,adc_cnt1,adc_ch,adc_check;
extern signed short adc_self_ch_buff[3],adc_self_ch_disp[3];
extern signed short adc_buff[26][16];
extern signed short adc_buff_[26];
extern char adc_self_ch_cnt;
extern signed short adc_result;

//-----------------------------------------------
//Состояние сети
extern short Hz_cnt,fnet,Hz_out;
extern char bFF,bFF_;
extern signed short Unet;
extern char unet_drv_cnt;
extern signed short unet_store;

//-----------------------------------------------
//Индикация
typedef struct  
{
enum {iMn,iSrv_sl,iNet,iSet,iBat,iBat_li,iSrc,iS2,iSet_prl,iK_prl,iDnd,iK,
	iSpcprl,iSpc,k,Crash_0,Crash_1,iKednd,
	iLoad,iSpc_prl_vz,iSpc_prl_ke,iKe,iVz,iAVAR,iStr,iVrs,iPrltst,iApv,
	iK_src/*,iK_bat*/,iK_load,iK_net,iTst,iTst_klbr,iTst_BPS1,iTst_BPS2,
	iTst_BPS12,iDebug,iDef,iSet_st_prl,iK_pdp,iSet_T,iDeb,iJ_bat,
	iPrl_bat_in_out,iPdp1,iJAv_sel,iJAv_net_sel,iJAv_net,iJAv_src1,
	iJAv_src2,iJAv_bat,iJAv_bat_sel,iAusw,iAusw_prl,iAusw_set,iK_t_out,
	iJ_bat_ke_sel,iJ_bat_ke,iJ_bat_vz_sel,iJ_bat_vz,iJ_bat_wrk_sel,iJ_bat_wrk,
	iExt_set,iExt_dt,iExt_sk,iAvz,iJAv_src1_sel,iJAv_src2_sel,iLog,iLog_,
	iLan_set,iSet_,iTst_full,iSet_rav,iSet_rav_,iBatSetupQuest,iModbus_set}i;

signed char s_i;
signed char s_i1;
signed char s_i2;
signed char i_s;
} stuct_ind;

extern stuct_ind a,b[10];

#define ind     a.i
#define sub_ind     a.s_i
#define sub_ind1     a.s_i1
#define sub_ind2     a.s_i2
#define index_set     a.i_s

extern char dig[5];
extern char zero_on;
extern char lcd_buffer[LCD_SIZE];
extern char lcd_bitmap[1024];
extern const char ABCDEF[];
extern signed char ptr_ind;
extern char mnemo_cnt;
extern signed char parol[3];
extern char fl_simv_num,fl_simv_len;
extern char default_temp;
extern signed short av_j_si_max,av_j_si_max_;
extern char simax;
extern char phase;
extern const char sm_[]; //
extern const char sm_exit[]; //
extern const char smon[]; 
extern const char smoff[];
extern const char sm_mont[13][4]; //
extern char content[63]; 

//-----------------------------------------------
//Коэффициенты, отображаемые из EEPROM
extern signed short Ktsrc[2];
extern signed short Kusrc[2];
extern signed short Kisrc[2];
extern signed short Ki0src[2];
extern signed short Kubat;
extern signed short Kubat_part[4]; 		//калибровка промежуточных напряжений батареи
extern signed short Kibat0;
extern signed short Kibat1;
extern signed short Ktbat;
extern signed short Kunet;
extern signed short Ktout[3];

extern signed short MAIN_BPS;
extern signed short UMAX;
extern signed short UBMAX;
extern signed short UB20;
extern signed short TMAX;
extern signed short AV_OFF_AVT;
extern signed short USIGN;
extern signed short UMN;
extern signed short ZV_ON;
extern signed short IKB;
extern signed short KVZ;
extern signed short IMAX;
extern signed short KIMAX;
extern signed short APV_ON;
extern signed short IZMAX;
extern signed short U0B;
extern signed short TZAS;
extern signed short VZ_HR;
extern signed short TBAT;
extern signed short UBM_AV;
extern signed short REL_AV[3];

extern signed short NUMBAT;
extern signed short NUMIST;
extern signed short NUMDT;
extern signed short NUMSK;
extern signed short BAT_C_REAL;
extern signed short BAT_C_NOM;
extern signed short BAT_RESURS;
extern signed short NUMLBAT;

//signed short ZAR_CNT,ZAR_CNT_KE;
//signed short BAT_RESURS;

extern signed short AUSW_MAIN;
extern signed long 	AUSW_MAIN_NUMBER;
extern signed short AUSW_DAY;
extern signed short AUSW_MONTH;
extern signed short AUSW_YEAR;
extern signed short AUSW_UKU;
extern signed short AUSW_UKU_SUB;
extern signed long AUSW_UKU_NUMBER;
extern signed long	AUSW_BPS1_NUMBER;
extern signed long  AUSW_BPS2_NUMBER;
extern signed short AUSW_RS232;
extern signed short AUSW_PDH;
extern signed short AUSW_SDH;
extern signed short AUSW_ETH;

extern signed short TMAX_EXT_EN[3];
extern signed short TMAX_EXT[3];
extern signed short TMIN_EXT_EN[3];
extern signed short TMIN_EXT[3];
extern signed short T_EXT_REL_EN[3];
extern signed short T_EXT_ZVUK_EN[3];
extern signed short T_EXT_LCD_EN[3];
extern signed short T_EXT_SNMP_TRAP_EN[3];

extern signed short SK_SIGN[4];
extern signed short SK_REL_EN[4];
extern signed short SK_ZVUK_EN[4];
extern signed short SK_LCD_EN[4];
extern signed short SK_SNMP_TRAP_EN[4];

typedef enum {AVZ_1=1,AVZ_2=2,AVZ_3=3,AVZ_6=6,AVZ_12=12,AVZ_OFF=0}enum_AVZ;
extern enum_AVZ AVZ;

extern unsigned short HOUR_AVZ;
extern unsigned short MIN_AVZ;
extern unsigned short SEC_AVZ;
extern unsigned short DATE_AVZ;
extern unsigned short MONTH_AVZ;
extern unsigned short YEAR_AVZ;
extern unsigned short AVZ_TIME;

extern signed short ETH_IS_ON;
extern signed short ETH_DHCP_ON;
extern signed short ETH_IP_1;
extern signed short ETH_IP_2;
extern signed short ETH_IP_3;
extern signed short ETH_IP_4;
extern signed short ETH_MASK_1;
extern signed short ETH_MASK_2;
extern signed short ETH_MASK_3;
extern signed short ETH_MASK_4;
extern signed short ETH_TRAP1_IP_1;
extern signed short ETH_TRAP1_IP_2;
extern signed short ETH_TRAP1_IP_3;
extern signed short ETH_TRAP1_IP_4;
extern signed short ETH_TRAP2_IP_1;
extern signed short ETH_TRAP2_IP_2;
extern signed short ETH_TRAP2_IP_3;
extern signed short ETH_TRAP2_IP_4;
extern signed short ETH_TRAP3_IP_1;
extern signed short ETH_TRAP3_IP_2;
extern signed short ETH_TRAP3_IP_3;
extern signed short ETH_TRAP3_IP_4;
extern signed short ETH_TRAP4_IP_1;
extern signed short ETH_TRAP4_IP_2;
extern signed short ETH_TRAP4_IP_3;
extern signed short ETH_TRAP4_IP_4;
extern signed short ETH_TRAP5_IP_1;
extern signed short ETH_TRAP5_IP_2;
extern signed short ETH_TRAP5_IP_3;
extern signed short ETH_TRAP5_IP_4;
extern signed short ETH_SNMP_PORT_READ;
extern signed short ETH_SNMP_PORT_WRITE;
extern signed short ETH_GW_1;
extern signed short ETH_GW_2;
extern signed short ETH_GW_3;
extern signed short ETH_GW_4;
extern signed short MODBUS_ADRESS;
extern signed short MODBUS_BAUDRATE;

//extern char snmp_community[10];

extern signed short dumm[8];

typedef enum  {bisON=0x0055,bisOFF=0x00aa}enum_BAT_IS_ON;
extern enum_BAT_IS_ON BAT_IS_ON;
typedef enum  {apvON=0x0055,apvOFF=0x00aa}enum_apv_on;
extern enum_apv_on apv_on1,apv_on2;

extern signed short BAT_DAY_OF_ON,BAT_MONTH_OF_ON,BAT_YEAR_OF_ON;
extern signed short apv_on2_time;

typedef enum {mnON=0x55,mnOFF=0xAA}enum_MNEMO_ON;
extern enum_MNEMO_ON MNEMO_ON;
extern unsigned short MNEMO_TIME;


//-----------------------------------------------
//Состояние системы
extern char St;
extern short main_cnt;

//-----------------------------------------------
//Состояние батареи
extern signed short tbat,Ubat,Ibat;
//extern signed short Ubat_part[4];			//напряжения промежуточных точек батареи относительно земли
//extern signed short Ubat_e[5];		    	//напряжения банок батареи
extern signed long zar_cnt,zar_cnt_ke;
extern signed char zar_percent;
extern char cnt_wrk;
typedef enum {wrkON=0x55,wrkOFF=0xAA}enum_wrk;
extern enum_wrk wrk;
extern unsigned long wrk_date[2];
extern signed long ibat_integr_;
extern signed short ibat_integr;
extern signed short cnt_resurs;
extern char cnt_ibat,cnt_ubat;
extern char cnt_irazr;
extern char bUrazr;
extern char bUOFF,bIOFF;
extern char cnt_irazr;
extern char bIrazr;
extern char bI;
extern char NDB;
extern signed short ubat_old[8];
extern signed short ubat_old_cnt;
extern char cnt_bat;					//счетчик задержки отключения контактора батареи


//-----------------------------------------------
//Состояние источников
extern char St_[2];
extern signed short t_i[2],Us[2],Is[2];
extern char ND[2];

//-----------------------------------------------
//Состояние нагрузки
extern signed short Iload,Uload;

//-----------------------------------------------
//Состояние внешних датчиков
extern signed short tout[3];
extern char tout_max_cnt[3],tout_min_cnt[3];
typedef enum {tNORM,tMAX,tMIN}enum_tout_stat;
extern enum_tout_stat tout_stat[3];
extern char in_stat_out_old[4];
typedef enum {skON=0x55,skOFF=0xaa}enum_sk_stat;
extern enum_sk_stat sk_stat[4];
extern char ND_out[3];
extern char in_stat_out[4];

//-----------------------------------------------
//Кнопки
extern char speed,l_but,n_but;
extern char but;                            
extern unsigned long but_n,but_s;
extern char but0_cnt;
extern char but1_cnt;
extern char but_onL_temp;

 //-----------------------------------------------
//Сообщения
extern char MSG_IND2OUT_DIS_SRC1,MSG_IND2OUT_DIS_SRC1_OLD;
extern char MSG_IND2OUT_DIS_SRC2,MSG_IND2OUT_DIS_SRC2_OLD;
extern char MSG_IND2OUT_EN_SRC1,MSG_IND2OUT_EN_SRC1_OLD;
extern char MSG_IND2OUT_EN_SRC2,MSG_IND2OUT_EN_SRC2_OLD;
extern char MSG_IND2OUT_EN_RELSAM,MSG_IND2OUT_EN_RELSAM_OLD;
extern short MSG_IND2PWM_SRC1,MSG_IND2PWM_SRC2;
extern char MSG_IND2OUT_EN_BAT,MSG_IND2OUT_EN_BAT_OLD;
extern char MSG_IND2OUT_DIS_BAT,MSG_IND2OUT_DIS_BAT_OLD;


//-----------------------------------------------
//Тест
typedef enum {tstOFF,tstON,tstU,tstWRK} enum_tst_state;
extern enum_tst_state tst_state[10];

//-----------------------------------------------
//Самокалибровка
extern unsigned short skb_cnt;

//-----------------------------------------------
//Время
extern unsigned short hour_cnt_5hz,hour_cnt_1hz;
extern char cnt_ind;

//-----------------------------------------------
//Отладка
extern unsigned short rotor_can[6];
extern unsigned short cnt_sec;
extern char plazma_suzun;
extern char uart_plazma;
//extern char plazma_can,plazma_can1,plazma_can2,plazma_can3,plazma_can4;
extern short can_plazma[10];
extern short snmp_plazma;

//-----------------------------------------------
//Имя устройства

extern const char* DEVICE_NAME[20];

//***********************************************
//Состояние батарей
typedef struct
     {
	char 		_cnt_to_block;
	signed short	_Ub;
     signed short	_Ubm;
     signed short	_dUbm;
	signed short	_Ib;
	signed short	_Tb;
	char 		_nd;
	char 		_cnt_wrk;
	char 		_wrk;
	unsigned short _zar;
	char 		_full_ver;
	signed long 	_zar_cnt;
	signed long 	_zar_cnt_ke;
	unsigned short _Iintegr,_Iintegr_; 
	signed short 	_u_old[8];
	signed short	_u_old_cnt;
	unsigned long 	_wrk_date[2];
	char 		_rel_stat;
	char			_av;
	char			_time_cnt;
	char 		_temper_stat;
	//0бит - подогрев
	//1бит - перегрев
	signed short 	_sign_temper_cnt;
	signed short 	_max_temper_cnt;
	signed long 	_resurs_cnt;
	signed short 	_cnt_as; 	//счетчик несимметрии, считает до 5 минут при выполнении условий несимметрии, когда досчитывает - пишет в журнал
     //signed short   _max_cell_volt;
	//signed short   _min_cell_volt;
	} BAT_STAT; 
extern BAT_STAT bat[7],bat_ips;
extern signed short		bat_u_old_cnt;
extern signed short 	Ib_ips_termokompensat;

//***********************************************
//Литиевые АКБ
typedef struct
     {
	signed short	_max_cell_volt;
	signed short	_min_cell_volt;
	signed short	_max_cell_temp;
	signed short	_min_cell_temp;
	signed short	_tot_bat_volt;
	signed short	_ch_curr;
	signed short	_dsch_curr;
	signed short	_rat_cap;
	signed short	_s_o_h;
	signed short	_s_o_c;
	signed short	_c_c_l_v;
	signed short	_r_b_t;
	signed short	_b_p_ser_num;
	signed short   _flags1;
	signed short 	_flags2;
	signed short 	_bRS485ERR;
	signed short	_rs485_cnt;
	signed short 	_cnt;
	signed short 	_battCommState;	//0 - норма, 1 - отсутствует связь промежуточной платы и батареи(RS485), 2 - отсутствует связь с промежуточной платой (KAN) 	
	signed short   _battIsOn;		//0 - отсутствует, 1 - присутствует
	signed short 	_isOnCnt;
	signed short 	_Ib;
	signed short 	_Ub;
	signed short 	_batCurrStat;		//0 - батарея отсутствует
								//1 - батарея заряжается, ток больше чем Iбк но меньше чем Iзмакс
								//2 - батарея заряжается критическим током, ток больше чем Iзмакс
								//3 - ток батареи больше чем -Iбк и меньше чем Iбк
								//4 - батарея разряжается, ток разряда больше чем Iбк
								//5 - батарея разряжается критическим током, ток разряда больше чем SOH/10
	} LAKB_STAT; 
extern LAKB_STAT lakb[7];
extern char lakb_damp[7][60];
extern char bLAKB_KONF_CH;
extern char bLAKB_KONF_CH_old;
extern char lakb_ison_mass[7];
extern short lakb_mn_ind_cnt;
extern char bLAKB_KONF_CH_EN;
extern char bRS485ERR,bCANERR;
extern short LBAT_STRUKT;
extern short lakb_can_cnt;
extern char bLIBATERR,bLIBATERRold;



//-----------------------------------------------
void Delay (unsigned long tick); 
void bitmap_hndl(void);
void SysTick_Handler (void);
//-----------------------------------------------
signed short abs_pal(signed short in);
