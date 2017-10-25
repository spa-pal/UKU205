/*----------------------------------------------------------------------------
 *      RL-ARM - TCPnet
 *----------------------------------------------------------------------------
 *      Name:    SNMP_MIB.C
 *      Purpose: SNMP Agent Management Information Base Module
 *      Rev.:    V4.12
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2010 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <Net_Config.h>
#include "LPC17xx.H"
#include "main.H"
#include "control.H"
#include "snmp_data_file.h" 

/* snmp_demo.c */
extern U8   get_button (void);
extern void LED_out (U32 val);
extern BOOL LCDupdate;
extern U8   lcd_text[2][16+1];

/* System */
extern U32  snmp_SysUpTime;

/* Local variables */
//static U8   LedOut;
//static U8   KeyIn;

/* MIB Read Only integer constants */
static const U8 sysServices = 79;
static const U16 sysMainsVoltage = 220;
static const U8 displayPsuQauntity = 2;
static const U8 TestForTableValues = 57;

 char* const aaa = "Novosibirsk, Russia";

int a_;
char aa_;
char* aaa_="abc";

/* MIB Entry event Callback functions. */
//static void write_leds (int mode);
//static void read_key (int mode);
//static void upd_display (int mode);

/*----------------------------------------------------------------------------
 *      MIB Data Table
 *---------------------------------------------------------------------------*/

 MIB_ENTRY snmp_mib[] = {


	{ MIB_OCTET_STR | MIB_ATR_RO, 12, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_SPEC,DISPLAY_SPEC_TRAP_MESSAGE , 0},			MIB_STR(snmp_spc_trap_message),     NULL},
	{ MIB_INTEGER | MIB_ATR_RO, 	12, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_SPEC,DISPLAY_SPEC_TRAP_VALUE_0 , 0},			MIB_INT(snmp_spc_trap_value_0),     NULL},
	{ MIB_INTEGER | MIB_ATR_RO, 	12, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_SPEC,DISPLAY_SPEC_TRAP_VALUE_1 , 0},			MIB_INT(snmp_spc_trap_value_1),     NULL},
	{ MIB_INTEGER | MIB_ATR_RO, 	12, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_SPEC,DISPLAY_SPEC_TRAP_VALUE_2 , 0},			MIB_INT(snmp_spc_trap_value_2),     NULL},



  /* ---------- System MIB ----------- */

  /* SysDescr Entry */
  { MIB_OCTET_STR | MIB_ATR_RO,	     8, {OID0(1,3), 6, 1, 2, 1, 1, 1, 0},      MIB_STR("First ARM SNMP agent for SibPromAutomatika"),     NULL },
  /* SysObjectID Entry */
  { MIB_OBJECT_ID | MIB_ATR_RO,	     8, {OID0(1,3), 6, 1, 2, 1, 1, 2, 0},	    MIB_STR("\x2b\x06\x01\x04\x01\x82\x83\x1F"),    NULL },
  /* SysUpTime Entry */
  { MIB_TIME_TICKS | MIB_ATR_RO,     8, {OID0(1,3), 6, 1, 2, 1, 1, 3, 0},    4, &snmp_SysUpTime,    NULL },
  /* SysContact Entry */
  { MIB_OCTET_STR | MIB_ATR_RO,	     8, {OID0(1,3), 6, 1, 2, 1, 1, 4, 0},    MIB_STR("Skype:danilov_aa"),    NULL },
  /* SysName Entry */
  { MIB_OCTET_STR | MIB_ATR_RO,		    8, {OID0(1,3), 6, 1, 2, 1, 1, 5, 0},    MIB_STR("UKU203LAN"),    NULL },
  /* SysLocation Entry */
  { MIB_OCTET_STR | MIB_ATR_RO,		     8, {OID0(1,3), 6, 1, 2, 1, 1, 6, 0},    MIB_STR("Novosibirsk, Russia"),    NULL },
  /* SysServices Entry */
  { MIB_INTEGER | MIB_ATR_RO,			    8, {OID0(1,3), 6, 1, 2, 1, 1, 7, 0},    MIB_INT(sysServices),    NULL },

  /* ---------- Experimental MIB ----------- */

	//{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID0(1,3), 6, 1, 4, 1, 130, 131, 31, 1, 3, 1, 1, 0},  4,(void*)&aaa_,  NULL},	//Название ИБЭПа
	//{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID0(1,3), 6, 1, 4, 1, 130, 131, 31, 1, 3, 1, 2, 0},MIB_INT(AUSW_MAIN_NUMBER),  NULL},		//Серийный номер



  	{ MIB_INTEGER | MIB_ATR_RO,  	12, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_DEVICE_INFO, DISPLAY_DEVICE_INFO_CODE, 0},  	MIB_INT(snmp_device_code),  		NULL},   	//код устройства
	{ MIB_INTEGER ,		  	12, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_DEVICE_INFO, DISPLAY_DEVICE_INFO_SERIAL, 0},	MIB_INT(snmp_sernum),	  		snmp_sernum_write },	//серийный номер	
  	{ MIB_INTEGER | MIB_ATR_RO,  	12, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_DEVICE_INFO, DISPLAY_DEVICE_INFO_SERIAL_LSB, 0}, MIB_INT(snmp_sernum_lsb),  		NULL},	//серийный номер младший байт
	{ MIB_INTEGER | MIB_ATR_RO,  	12, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_DEVICE_INFO, DISPLAY_DEVICE_INFO_SERIAL_MSB, 0},	MIB_INT(snmp_sernum_msb),  		NULL},	//серийный номер старший байт
 	{ MIB_OCTET_STR, 			12, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_DEVICE_INFO, DISPLAY_DEVICE_INFO_LOCATION, 0},  	MIB_STR(snmp_location),  		snmp_location_write},	//местоположение устройства
  	{ MIB_INTEGER | MIB_ATR_RO,  	12, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_DEVICE_INFO, DISPLAY_DEVICE_INFO_NUMOFBAT, 0}, 	MIB_INT(snmp_numofbat),  		NULL},	//количество введенных батарей
	{ MIB_INTEGER | MIB_ATR_RO,  	12, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_DEVICE_INFO, DISPLAY_DEVICE_INFO_NUMOFBPS, 0},	MIB_INT(snmp_numofbps),  		NULL},	//количество введенных источников
	{ MIB_INTEGER | MIB_ATR_RO,  	12, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_DEVICE_INFO, DISPLAY_DEVICE_INFO_NUMOFEVENTS, 0},MIB_INT(snmp_numofevents),  		NULL},	//количество введенных источников
	{ MIB_OCTET_STR,  			12, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_DEVICE_INFO, DISPLAY_DEVICE_INFO_MODEL, 0},		MIB_STR(snmp_model),  		  	snmp_model_write},	//модель ИБЭПа



  	{ MIB_INTEGER | MIB_ATR_RO,  	12, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_MAINS_POWER, DISPLAY_MAINS_POWER_VOLTAGE, 0},  	MIB_INT(snmp_mains_power_voltage), NULL},	//напряжение сети	
  	{ MIB_INTEGER | MIB_ATR_RO,  	12, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_MAINS_POWER, DISPLAY_MAINS_POWER_FREQUENCY, 0},  MIB_INT(snmp_mains_power_frequency),NULL},	//частота сети
	{ MIB_INTEGER | MIB_ATR_RO,  	12, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_MAINS_POWER, DISPLAY_MAINS_POWER_STATUS, 0},  	MIB_INT(snmp_mains_power_status),  NULL},	//состояние сети 
 	{ MIB_INTEGER | MIB_ATR_RO,  	12, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_MAINS_POWER, DISPLAY_MAINS_POWER_ALARM, 0},  	MIB_INT(snmp_mains_power_alarm),  	NULL},	//аварии сети


	{ MIB_INTEGER | MIB_ATR_RO,  	12, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOAD, DISPLAY_LOAD_VOLTAGE, 0},  				MIB_INT(snmp_load_voltage),  		NULL},	//напряжение нагрузки
  	{ MIB_INTEGER | MIB_ATR_RO,  	12, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOAD, DISPLAY_LOAD_CURRENT, 0},  				MIB_INT(snmp_load_current),  		NULL},	//ток нагрузки

	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_PSU, DISPLAY_PSU_ENTRY_NUMBER, 1},  			MIB_INT(snmp_bps_number[0]),  	NULL},	//Номер БПСа
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_PSU, DISPLAY_PSU_ENTRY_NUMBER, 2},  			MIB_INT(snmp_bps_number[1]),  	NULL},	//Номер БПСа
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_PSU, DISPLAY_PSU_ENTRY_VOLTAGE, 1},  			MIB_INT(snmp_bps_voltage[0]),  	NULL},	//Напряжение БПС1
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_PSU, DISPLAY_PSU_ENTRY_VOLTAGE, 2},  			MIB_INT(snmp_bps_voltage[1]),  	NULL},	//Напряжение БПС2
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_PSU, DISPLAY_PSU_ENTRY_CURRENT, 1},  			MIB_INT(snmp_bps_current[0]),  	NULL},	//Ток БПС1
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_PSU, DISPLAY_PSU_ENTRY_CURRENT, 2},  			MIB_INT(snmp_bps_current[1]),  	NULL},	//Ток БПС2
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_PSU, DISPLAY_PSU_ENTRY_TEMPERATURE, 1},  		MIB_INT(snmp_bps_temperature[0]),  NULL},	//Ток БПС1
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_PSU, DISPLAY_PSU_ENTRY_TEMPERATURE, 2},  		MIB_INT(snmp_bps_temperature[1]),  NULL},	//Ток БПС2
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_PSU, DISPLAY_PSU_ENTRY_STATUS, 1},  			MIB_INT(snmp_bps_stat[0]),  NULL},			//Состояние БПС1
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_PSU, DISPLAY_PSU_ENTRY_STATUS, 2},  			MIB_INT(snmp_bps_stat[1]),  NULL},			//Состояние БПС2

	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_FUSE, DISPLAY_FUSE_ENTRY_NUMBER, 1},  			MIB_INT(snmp_fuse_number[0]),  	NULL},	//Номер БПСа
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_FUSE, DISPLAY_FUSE_ENTRY_NUMBER, 2},  			MIB_INT(snmp_fuse_number[1]),  	NULL},	//Номер БПСа
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_FUSE, DISPLAY_FUSE_ENTRY_NUMBER, 3},  			MIB_INT(snmp_fuse_number[2]),  	NULL},	//Номер БПСа
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_FUSE, DISPLAY_FUSE_ENTRY_AV_STAT, 1},  		MIB_INT(snmp_fuse_av_stat[0]),  	NULL},	//Напряжение БПС1
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_FUSE, DISPLAY_FUSE_ENTRY_AV_STAT, 2},  		MIB_INT(snmp_fuse_av_stat[1]),  	NULL},	//Напряжение БПС2
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_FUSE, DISPLAY_FUSE_ENTRY_AV_STAT, 3},  		MIB_INT(snmp_fuse_av_stat[2]),  	NULL},	//Напряжение БПС2

	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_SK, DISPLAY_SK_ENTRY_NUMBER, 1},  			MIB_INT(snmp_sk_number[0]),  	NULL},	//Номер БПСа
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_SK, DISPLAY_SK_ENTRY_NUMBER, 2},  			MIB_INT(snmp_sk_number[1]),  	NULL},	//Номер БПСа
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_SK, DISPLAY_SK_ENTRY_NUMBER, 3},  			MIB_INT(snmp_sk_number[2]),  	NULL},	//Номер БПСа
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_SK, DISPLAY_SK_ENTRY_NUMBER, 4},  			MIB_INT(snmp_sk_number[3]),  	NULL},	//Номер БПСа
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_SK, DISPLAY_SK_ENTRY_AV_STAT, 1},  			MIB_INT(snmp_sk_av_stat[0]),  	NULL},	//Напряжение БПС1
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_SK, DISPLAY_SK_ENTRY_AV_STAT, 2},  			MIB_INT(snmp_sk_av_stat[1]),  	NULL},	//Напряжение БПС2
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_SK, DISPLAY_SK_ENTRY_AV_STAT, 3},  			MIB_INT(snmp_sk_av_stat[2]),  	NULL},	//Напряжение БПС2
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_SK, DISPLAY_SK_ENTRY_AV_STAT, 4},  			MIB_INT(snmp_sk_av_stat[3]),  	NULL},	//Напряжение БПС2

	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_BAT, DISPLAY_BAT_ENTRY_NUMBER, 1},				MIB_INT(snmp_bat_number),  	NULL},	//Номер БПСа
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_BAT, DISPLAY_BAT_ENTRY_VOLTAGE, 1},  				MIB_INT(snmp_bat_voltage),  	NULL},	//Номер БПСа
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_BAT, DISPLAY_BAT_ENTRY_CURRENT, 1},  				MIB_INT(snmp_bat_current),  	NULL},	//Номер БПСа
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_BAT, DISPLAY_BAT_ENTRY_TEMPERATURE, 1},  			MIB_INT(snmp_bat_temperature),NULL},	//Номер БПСа
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_BAT, DISPLAY_BAT_ENTRY_CAPACITY, 1},  				MIB_INT(snmp_bat_capacity),  	NULL},	//Номер БПСа
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_BAT, DISPLAY_BAT_ENTRY_CHARGE, 1},  				MIB_INT(snmp_bat_charge),  	NULL},	//Номер БПСа
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_BAT, DISPLAY_BAT_ENTRY_STATUS, 1},  				MIB_INT(snmp_bat_status),  	NULL},	//Номер БПСа

	{ MIB_INTEGER | MIB_ATR_RO, 	12, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_SPEC,DISPLAY_SPEC_STAT , 0},					MIB_INT(snmp_spc_stat),     NULL},
	{ MIB_OCTET_STR | MIB_ATR_RO, 12, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_SPEC,DISPLAY_SPEC_TRAP_MESSAGE , 0},			MIB_STR(snmp_spc_trap_message),     NULL},
	{ MIB_INTEGER | MIB_ATR_RO, 	12, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_SPEC,DISPLAY_SPEC_TRAP_VALUE , 0},			MIB_INT(snmp_spc_trap_value),     NULL},


//	{ MIB_INTEGER,  			12, {OID_ENTERPRISE, OID_DEVICE, SNMP_COMMAND, COMMAND_ANSWER, 0},					MIB_INT(snmp_command),  	snmp_command_execute},		//номер первого бпса 
//	{ MIB_INTEGER,  			12, {OID_ENTERPRISE, OID_DEVICE, SNMP_COMMAND, COMMAND_PARAMETR, 0},					MIB_INT(snmp_command_parametr),  	NULL},		//номер первого бпса
//	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, 4, 1, 3, 1},MIB_INT(snmp_bpsnumber[0]),  	NULL},		//номер первого бпса
//	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, 4, 1, 4, 1},MIB_INT(snmp_bpsnumber[0]),  	NULL},		//номер первого бпса
 // 	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, 4, 1, 5, 1},MIB_INT(snmp_bpsnumber[0]),  	NULL},		//номер первого бпса
//	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, 4, 1, 6, 1},MIB_INT(snmp_bpsnumber[0]),  	NULL},		//номер первого бпса
//	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, 4, 1, 9, 1},MIB_INT(snmp_bpsnumber[0]),  	NULL},		//номер первого бпса
	{ MIB_INTEGER,  			12, {OID_ENTERPRISE, OID_DEVICE, SNMP_COMMAND, COMMAND_SET_PRIMARY, 0},				MIB_INT(snmp_command_parametr),  	snmp_set_primary},
	{ MIB_INTEGER,  			12, {OID_ENTERPRISE, OID_DEVICE, SNMP_COMMAND, COMMAND_DISABLE, 0},					MIB_INT(snmp_command_parametr),  	snmp_disable},
	{ MIB_INTEGER,  			12, {OID_ENTERPRISE, OID_DEVICE, SNMP_COMMAND, COMMAND_UNLOCK, 0},					MIB_INT(snmp_command_parametr),  	snmp_unlock},
	{ MIB_INTEGER,  			12, {OID_ENTERPRISE, OID_DEVICE, SNMP_COMMAND, COMMAND_PARALLEL, 0},					MIB_INT(snmp_command_parametr),  	snmp_parallel_switch},
	{ MIB_INTEGER,  			12, {OID_ENTERPRISE, OID_DEVICE, SNMP_COMMAND, COMMAND_COMP_CHARG, 0},				MIB_INT(snmp_command_parametr),  	snmp_vz_start},
	{ MIB_INTEGER,  			12, {OID_ENTERPRISE, OID_DEVICE, SNMP_COMMAND, COMMAND_CAP_CONTR, 0},					MIB_INT(snmp_command_parametr),  	snmp_ke_start},
	{ MIB_INTEGER,  			12, {OID_ENTERPRISE, OID_DEVICE, SNMP_COMMAND, COMMAND_DISABLE_SPEC, 0},				MIB_INT(snmp_command_parametr),  	snmp_spc_stop},


//	{ MIB_INTEGER, 			12, {OID_ENTERPRISE, OID_DEVICE, 8, 3, 0},   MIB_INT(snmp_bpsnumber[0]),    NULL},
//	{ MIB_INTEGER, 			12, {OID_ENTERPRISE, OID_DEVICE, 8, 1, 0},   MIB_INT(snmp_bpsnumber[0]),    NULL},



	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID0(1,3), 6, 1, 4, 1, 130, 131, 31, 1, 2, 4, 1, 0},  MIB_INT(NUMBAT),  NULL},	//количество введенных батарей

	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID0(1,3), 6, 1, 4, 1, 130, 131, 31, 1, 2, 7, 4, 0},	     MIB_STR("Novosibirsk, Russia"),     NULL},
	{ MIB_INTEGER, 			13, {OID0(1,3), 6, 1, 4, 1, 130, 131, 31, 1, 2, 7, 5, 0},	     MIB_INT(displayPsuQauntity),     NULL},
 /* { MIB_INTEGER | MIB_ATR_RO,  	13, {OID0(1,3), 6, 1, 4, 1, 130, 131, 31, 2, 2, 3, 1, 0},  MIB_INT(plazma_mib),  NULL},
  { MIB_INTEGER | MIB_ATR_RO,  	13, {OID0(1,3), 6, 1, 4, 1, 130, 131, 31, 2, 2, 3, 2, 0},  MIB_INT(plazma_mib1),  NULL},
  { MIB_INTEGER,  	13, {OID0(1,3), 6, 1, 4, 1, 130, 131, 31, 1, 2, 1, 2, 0},    MIB_INT(LPC_RTC->SEC),    NULL}, */
  
	


	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 1},  			MIB_STR(&snmp_log[0][0]),  	NULL},	//Первое событие из журнала
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 2},  			MIB_STR(&snmp_log[1][0]),  	NULL},	//2-е событие из журнала
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 3},  			MIB_STR(&snmp_log[2][0]),  	NULL},	//3-е событие из журнала
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 4},  			MIB_STR(&snmp_log[3][0]),  	NULL},	//4-е событие из журнала
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 5},  			MIB_STR(&snmp_log[4][0]),  	NULL},	//5-е событие из журнала
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 6},  			MIB_STR(&snmp_log[5][0]),  	NULL},	//6-е событие из журнала
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 7},  			MIB_STR(&snmp_log[6][0]),  	NULL},	//7-е событие из журнала
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 8},  			MIB_STR(&snmp_log[7][0]),  	NULL},	//8-е событие из журнала
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 9},  			MIB_STR(&snmp_log[8][0]),  	NULL},	//9-е событие из журнала
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 10},  			MIB_STR(&snmp_log[9][0]),  	NULL},	//10-е событие из журнала

	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 11},  			MIB_STR(&snmp_log[10][0]),  	NULL},	//11-е событие из журнала
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 12},  			MIB_STR(&snmp_log[11][0]),  	NULL},	//12-е событие из журнала
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 13},  			MIB_STR(&snmp_log[12][0]),  	NULL},	//13-е событие из журнала
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 14},  			MIB_STR(&snmp_log[13][0]),  	NULL},	//14-е событие из журнала
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 15},  			MIB_STR(&snmp_log[14][0]),  	NULL},	//15-е событие из журнала
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 16},  			MIB_STR(&snmp_log[15][0]),  	NULL},	//16-е событие из журнала
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 17},  			MIB_STR(&snmp_log[16][0]),  	NULL},	//17-е событие из журнала
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 18},  			MIB_STR(&snmp_log[17][0]),  	NULL},	//18-е событие из журнала
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 19},  			MIB_STR(&snmp_log[18][0]),  	NULL},	//19-е событие из журнала
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 20},  			MIB_STR(&snmp_log[19][0]),  	NULL},	//20-е событие из журнала

	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 21},  			MIB_STR(&snmp_log[20][0]),  	NULL},	//21-е событие из журнала
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 22},  			MIB_STR(&snmp_log[21][0]),  	NULL},	//22-е событие из журнала
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 23},  			MIB_STR(&snmp_log[22][0]),  	NULL},	//23-е событие из журнала
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 24},  			MIB_STR(&snmp_log[23][0]),  	NULL},	//24-е событие из журнала
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 25},  			MIB_STR(&snmp_log[24][0]),  	NULL},	//25-е событие из журнала
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 26},  			MIB_STR(&snmp_log[25][0]),  	NULL},	//26-е событие из журнала
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 27},  			MIB_STR(&snmp_log[26][0]),  	NULL},	//27-е событие из журнала
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 28},  			MIB_STR(&snmp_log[27][0]),  	NULL},	//28-е событие из журнала
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 29},  			MIB_STR(&snmp_log[28][0]),  	NULL},	//29-е событие из журнала
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 30},  			MIB_STR(&snmp_log[29][0]),  	NULL},	//30-е событие из журнала

	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 31},  			MIB_STR(&snmp_log[30][0]),  	NULL},	//31-е событие из журнала
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 32},  			MIB_STR(&snmp_log[31][0]),  	NULL},	//32-е событие из журнала
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 33},  			MIB_STR(&snmp_log[32][0]),  	NULL},	//33-е событие из журнала
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 34},  			MIB_STR(&snmp_log[33][0]),  	NULL},	//34-е событие из журнала
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 35},  			MIB_STR(&snmp_log[34][0]),  	NULL},	//35-е событие из журнала
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 36},  			MIB_STR(&snmp_log[35][0]),  	NULL},	//36-е событие из журнала
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 37},  			MIB_STR(&snmp_log[36][0]),  	NULL},	//37-е событие из журнала
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 38},  			MIB_STR(&snmp_log[37][0]),  	NULL},	//38-е событие из журнала
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 39},  			MIB_STR(&snmp_log[38][0]),  	NULL},	//39-е событие из журнала
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 40},  			MIB_STR(&snmp_log[39][0]),  	NULL},	//40-е событие из журнала

	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 41},  			MIB_STR(&snmp_log[40][0]),  	NULL},	//41-е событие из журнала
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 42},  			MIB_STR(&snmp_log[41][0]),  	NULL},	//42-е событие из журнала
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 43},  			MIB_STR(&snmp_log[42][0]),  	NULL},	//43-е событие из журнала
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 44},  			MIB_STR(&snmp_log[43][0]),  	NULL},	//44-е событие из журнала
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 45},  			MIB_STR(&snmp_log[44][0]),  	NULL},	//45-е событие из журнала
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 46},  			MIB_STR(&snmp_log[45][0]),  	NULL},	//46-е событие из журнала
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 47},  			MIB_STR(&snmp_log[46][0]),  	NULL},	//47-е событие из журнала
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 48},  			MIB_STR(&snmp_log[47][0]),  	NULL},	//48-е событие из журнала
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 49},  			MIB_STR(&snmp_log[48][0]),  	NULL},	//49-е событие из журнала
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 50},  			MIB_STR(&snmp_log[49][0]),  	NULL},	//50-е событие из журнала

	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 51},  			MIB_STR(&snmp_log[50][0]),  	NULL},	//51-е событие из журнала
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 52},  			MIB_STR(&snmp_log[51][0]),  	NULL},	//52-е событие из журнала
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 53},  			MIB_STR(&snmp_log[52][0]),  	NULL},	//53-е событие из журнала
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 54},  			MIB_STR(&snmp_log[53][0]),  	NULL},	//54-е событие из журнала
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 55},  			MIB_STR(&snmp_log[54][0]),  	NULL},	//55-е событие из журнала
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 56},  			MIB_STR(&snmp_log[55][0]),  	NULL},	//56-е событие из журнала
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 57},  			MIB_STR(&snmp_log[56][0]),  	NULL},	//57-е событие из журнала
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 58},  			MIB_STR(&snmp_log[57][0]),  	NULL},	//58-е событие из журнала
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 59},  			MIB_STR(&snmp_log[58][0]),  	NULL},	//59-е событие из журнала
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 60},  			MIB_STR(&snmp_log[59][0]),  	NULL},	//60-е событие из журнала

	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 61},  			MIB_STR(&snmp_log[60][0]),  	NULL},	//61-е событие из журнала
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 62},  			MIB_STR(&snmp_log[61][0]),  	NULL},	//62-е событие из журнала
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 63},  			MIB_STR(&snmp_log[62][0]),  	NULL},	//63-е событие из журнала
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LOG, DISPLAY_LOG_ENTRY_EVENTS, 64},  			MIB_STR(&snmp_log[63][0]),  	NULL},	//64-е событие из журнала

	{ MIB_INTEGER,  			12, {OID_ENTERPRISE, OID_DEVICE, SYSPARAMS, SYSPARAMSMAINPSU, 0},					MIB_INT(snmp_main_bps),  	snmp_main_bps_write},		//номер первого бпса 
	{ MIB_INTEGER,  			12, {OID_ENTERPRISE, OID_DEVICE, SYSPARAMS, SYSPARAMSSOUNDALARMEN, 0},				MIB_INT(snmp_zv_en),  		snmp_zv_on_write},		//номер первого бпса 
	{ MIB_INTEGER,  			12, {OID_ENTERPRISE, OID_DEVICE, SYSPARAMS, SYSPARAMSALARMAUTODISABLE, 0},				MIB_INT(snmp_alarm_auto_disable),snmp_alarm_auto_disable_write},		//номер первого бпса 
	{ MIB_INTEGER,  			12, {OID_ENTERPRISE, OID_DEVICE, SYSPARAMS, SYSPARAMS_BAT_TEST_TIME, 0},				MIB_INT(snmp_bat_test_time),	snmp_bat_test_time_write},		//номер первого бпса 
	{ MIB_INTEGER,  			12, {OID_ENTERPRISE, OID_DEVICE, SYSPARAMS, SYSPARAMS_U_MAX, 0},						MIB_INT(snmp_u_max),		snmp_u_max_write},		//номер первого бпса 
	{ MIB_INTEGER,  			12, {OID_ENTERPRISE, OID_DEVICE, SYSPARAMS, SYSPARAMS_U_0_GRAD, 0},					MIB_INT(snmp_u_0_grad),		snmp_u_0_grad_write},		//номер первого бпса 
	{ MIB_INTEGER,  			12, {OID_ENTERPRISE, OID_DEVICE, SYSPARAMS, SYSPARAMS_U_20_GRAD, 0},					MIB_INT(snmp_u_20_grad),		snmp_u_20_grad_write},		//номер первого бпса 
	{ MIB_INTEGER,  			12, {OID_ENTERPRISE, OID_DEVICE, SYSPARAMS, SYSPARAMS_U_SIGN, 0},					MIB_INT(snmp_u_sign),		snmp_u_sign_write},		//номер первого бпса 
	{ MIB_INTEGER,  			12, {OID_ENTERPRISE, OID_DEVICE, SYSPARAMS, SYSPARAMS_U_MIN_POWER, 0},				MIB_INT(snmp_u_min_power),	snmp_u_min_power_write},		//номер первого бпса 
	{ MIB_INTEGER,  			12, {OID_ENTERPRISE, OID_DEVICE, SYSPARAMS, SYSPARAMS_U_WITHOUT_BAT, 0},				MIB_INT(snmp_u_withouth_bat),	snmp_u_withouth_bat_write},		//номер первого бпса 
	{ MIB_INTEGER,  			12, {OID_ENTERPRISE, OID_DEVICE, SYSPARAMS, SYSPARAMS_IBK, 0},						MIB_INT(snmp_control_current),snmp_control_current_write},		//номер первого бпса 
	{ MIB_INTEGER,  			12, {OID_ENTERPRISE, OID_DEVICE, SYSPARAMS, SYSPARAMS_IZMAX, 0},						MIB_INT(snmp_max_charge_current),	snmp_max_charge_current_write},		//номер первого бпса 
	{ MIB_INTEGER,  			12, {OID_ENTERPRISE, OID_DEVICE, SYSPARAMS, SYSPARAMS_IMAX, 0},						MIB_INT(snmp_max_current),	snmp_max_current_write},		//номер первого бпса 
	{ MIB_INTEGER,  			12, {OID_ENTERPRISE, OID_DEVICE, SYSPARAMS, SYSPARAMS_KIMAX, 0},						MIB_INT(snmp_max_current_koef),	snmp_max_current_koef_write},		//номер первого бпса 
	{ MIB_INTEGER,  			12, {OID_ENTERPRISE, OID_DEVICE, SYSPARAMS, SYSPARAMS_KVZ, 0},						MIB_INT(snmp_up_charge_koef),	snmp_up_charge_koef_write},		//номер первого бпса 
	{ MIB_INTEGER,  			12, {OID_ENTERPRISE, OID_DEVICE, SYSPARAMS, SYSPARAMS_TZAS, 0},						MIB_INT(snmp_powerup_psu_timeout),	snmp_powerup_psu_timeout_write},		//номер первого бпса 
	{ MIB_INTEGER,  			12, {OID_ENTERPRISE, OID_DEVICE, SYSPARAMS, SYSPARAMS_TMAX, 0},						MIB_INT(snmp_max_temperature),	snmp_max_temperature_write},		//номер первого бпса 

	{ MIB_INTEGER,  			12, {OID_ENTERPRISE, OID_DEVICE, SYSPARAMS, SYSPARAMSMAINPSU, 0},					MIB_INT(snmp_main_bps),  	snmp_main_bps_write},		//номер первого бпса 
	{ MIB_INTEGER,  			12, {OID_ENTERPRISE, OID_DEVICE, SYSPARAMS, SYSPARAMSSOUNDALARMEN, 0},				MIB_INT(snmp_zv_en),  		snmp_zv_on_write},		//номер первого бпса 
	{ MIB_INTEGER,  			12, {OID_ENTERPRISE, OID_DEVICE, SYSPARAMS, SYSPARAMSALARMAUTODISABLE, 0},				MIB_INT(snmp_alarm_auto_disable),snmp_alarm_auto_disable_write},		//номер первого бпса 
	{ MIB_INTEGER,  			12, {OID_ENTERPRISE, OID_DEVICE, SYSPARAMS, SYSPARAMS_BAT_TEST_TIME, 0},				MIB_INT(snmp_bat_test_time),	snmp_bat_test_time_write},		//номер первого бпса 
	{ MIB_INTEGER,  			12, {OID_ENTERPRISE, OID_DEVICE, SYSPARAMS, SYSPARAMS_U_MAX, 0},						MIB_INT(snmp_u_max),		snmp_u_max_write},		//номер первого бпса 
	{ MIB_INTEGER,  			12, {OID_ENTERPRISE, OID_DEVICE, SYSPARAMS, SYSPARAMS_U_0_GRAD, 0},					MIB_INT(snmp_u_0_grad),		snmp_u_0_grad_write},		//номер первого бпса 
	{ MIB_INTEGER,  			12, {OID_ENTERPRISE, OID_DEVICE, SYSPARAMS, SYSPARAMS_U_20_GRAD, 0},					MIB_INT(snmp_u_20_grad),		snmp_u_20_grad_write},		//номер первого бпса 
	{ MIB_INTEGER,  			12, {OID_ENTERPRISE, OID_DEVICE, SYSPARAMS, SYSPARAMS_U_SIGN, 0},					MIB_INT(snmp_u_sign),		snmp_u_sign_write},		//номер первого бпса 
	{ MIB_INTEGER,  			12, {OID_ENTERPRISE, OID_DEVICE, SYSPARAMS, SYSPARAMS_U_MIN_POWER, 0},				MIB_INT(snmp_u_min_power),	snmp_u_min_power_write},		//номер первого бпса 
	{ MIB_INTEGER,  			12, {OID_ENTERPRISE, OID_DEVICE, SYSPARAMS, SYSPARAMS_U_WITHOUT_BAT, 0},				MIB_INT(snmp_u_withouth_bat),	snmp_u_withouth_bat_write},		//номер первого бпса 
	{ MIB_INTEGER,  			12, {OID_ENTERPRISE, OID_DEVICE, SYSPARAMS, SYSPARAMS_IBK, 0},						MIB_INT(snmp_control_current),snmp_control_current_write},		//номер первого бпса 
	{ MIB_INTEGER,  			12, {OID_ENTERPRISE, OID_DEVICE, SYSPARAMS, SYSPARAMS_IZMAX, 0},						MIB_INT(snmp_max_charge_current),	snmp_max_charge_current_write},		//номер первого бпса 
	{ MIB_INTEGER,  			12, {OID_ENTERPRISE, OID_DEVICE, SYSPARAMS, SYSPARAMS_IMAX, 0},						MIB_INT(snmp_max_current),	snmp_max_current_write},		//номер первого бпса 
	{ MIB_INTEGER,  			12, {OID_ENTERPRISE, OID_DEVICE, SYSPARAMS, SYSPARAMS_KIMAX, 0},						MIB_INT(snmp_max_current_koef),	snmp_max_current_koef_write},		//номер первого бпса 
	{ MIB_INTEGER,  			12, {OID_ENTERPRISE, OID_DEVICE, SYSPARAMS, SYSPARAMS_KVZ, 0},						MIB_INT(snmp_up_charge_koef),	snmp_up_charge_koef_write},		//номер первого бпса 
	{ MIB_INTEGER,  			12, {OID_ENTERPRISE, OID_DEVICE, SYSPARAMS, SYSPARAMS_TZAS, 0},						MIB_INT(snmp_powerup_psu_timeout),	snmp_powerup_psu_timeout_write},		//номер первого бпса 
	{ MIB_INTEGER,  			12, {OID_ENTERPRISE, OID_DEVICE, SYSPARAMS, SYSPARAMS_TMAX, 0},						MIB_INT(snmp_max_temperature),	snmp_max_temperature_write},		//номер первого бпса 

	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_ENTRY_NUMBER, 1},  			MIB_INT(snmp_lakb_number[0]),  	NULL},	//Номер ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_ENTRY_NUMBER, 2},  			MIB_INT(snmp_lakb_number[1]),  	NULL},	//Номер ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_ENTRY_NUMBER, 3},  			MIB_INT(snmp_lakb_number[2]),  	NULL},	//Номер ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_ENTRY_NUMBER, 4},  			MIB_INT(snmp_lakb_number[3]),  	NULL},	//Номер ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_ENTRY_NUMBER, 5},  			MIB_INT(snmp_lakb_number[4]),  	NULL},	//Номер ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_ENTRY_NUMBER, 6},  			MIB_INT(snmp_lakb_number[5]),  	NULL},	//Номер ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_ENTRY_NUMBER, 7},  			MIB_INT(snmp_lakb_number[6]),  	NULL},	//Номер ЛАКБ

	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_MAX_CELL_VOLTAGE, 1},  		MIB_INT(snmp_lakb_max_cell_voltage[0]),  	NULL},	//Максимальное напряжение ячейки ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_MAX_CELL_VOLTAGE, 2},  		MIB_INT(snmp_lakb_max_cell_voltage[1]),  	NULL},	//Максимальное напряжение ячейки ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_MAX_CELL_VOLTAGE, 3},  		MIB_INT(snmp_lakb_max_cell_voltage[2]),  	NULL},	//Максимальное напряжение ячейки ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_MAX_CELL_VOLTAGE, 4},  		MIB_INT(snmp_lakb_max_cell_voltage[3]),  	NULL},	//Максимальное напряжение ячейки ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_MAX_CELL_VOLTAGE, 5},  		MIB_INT(snmp_lakb_max_cell_voltage[4]),  	NULL},	//Максимальное напряжение ячейки ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_MAX_CELL_VOLTAGE, 6},  		MIB_INT(snmp_lakb_max_cell_voltage[5]),  	NULL},	//Максимальное напряжение ячейки ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_MAX_CELL_VOLTAGE, 7},  		MIB_INT(snmp_lakb_max_cell_voltage[6]),  	NULL},	//Максимальное напряжение ячейки ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_MIN_CELL_VOLTAGE, 1},  		MIB_INT(snmp_lakb_min_cell_voltage[0]),  	NULL},	//Минимальное напряжение ячейки ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_MIN_CELL_VOLTAGE, 2},  		MIB_INT(snmp_lakb_min_cell_voltage[1]),  	NULL},	//Минимальное напряжение ячейки ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_MIN_CELL_VOLTAGE, 3},  		MIB_INT(snmp_lakb_min_cell_voltage[2]),  	NULL},	//Минимальное напряжение ячейки ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_MIN_CELL_VOLTAGE, 4},  		MIB_INT(snmp_lakb_min_cell_voltage[3]),  	NULL},	//Минимальное напряжение ячейки ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_MIN_CELL_VOLTAGE, 5},  		MIB_INT(snmp_lakb_min_cell_voltage[4]),  	NULL},	//Минимальное напряжение ячейки ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_MIN_CELL_VOLTAGE, 6},  		MIB_INT(snmp_lakb_min_cell_voltage[5]),  	NULL},	//Минимальное напряжение ячейки ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_MIN_CELL_VOLTAGE, 7},  		MIB_INT(snmp_lakb_min_cell_voltage[6]),  	NULL},	//Минимальное напряжение ячейки ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_MAX_CELL_TEMPERATURE, 1},  	MIB_INT(snmp_lakb_max_cell_temperature[0]),  	NULL},	//Максимальная температура ячейки ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_MAX_CELL_TEMPERATURE, 2},  	MIB_INT(snmp_lakb_max_cell_temperature[1]),  	NULL},	//Максимальная температура ячейки ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_MAX_CELL_TEMPERATURE, 3},  	MIB_INT(snmp_lakb_max_cell_temperature[2]),  	NULL},	//Максимальная температура ячейки ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_MAX_CELL_TEMPERATURE, 4},  	MIB_INT(snmp_lakb_max_cell_temperature[3]),  	NULL},	//Максимальная температура ячейки ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_MAX_CELL_TEMPERATURE, 5},  	MIB_INT(snmp_lakb_max_cell_temperature[4]),  	NULL},	//Максимальная температура ячейки ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_MAX_CELL_TEMPERATURE, 6},  	MIB_INT(snmp_lakb_max_cell_temperature[5]),  	NULL},	//Максимальная температура ячейки ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_MAX_CELL_TEMPERATURE, 7},  	MIB_INT(snmp_lakb_max_cell_temperature[6]),  	NULL},	//Максимальная температура ячейки ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_MIN_CELL_TEMPERATURE, 1},  	MIB_INT(snmp_lakb_min_cell_temperature[0]),  	NULL},	//Минимальная температура ячейки ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_MIN_CELL_TEMPERATURE, 2},  	MIB_INT(snmp_lakb_min_cell_temperature[1]),  	NULL},	//Минимальная температура ячейки ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_MIN_CELL_TEMPERATURE, 3},  	MIB_INT(snmp_lakb_min_cell_temperature[2]),  	NULL},	//Минимальная температура ячейки ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_MIN_CELL_TEMPERATURE, 4},  	MIB_INT(snmp_lakb_min_cell_temperature[3]),  	NULL},	//Минимальная температура ячейки ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_MIN_CELL_TEMPERATURE, 5},  	MIB_INT(snmp_lakb_min_cell_temperature[4]),  	NULL},	//Минимальная температура ячейки ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_MIN_CELL_TEMPERATURE, 6},  	MIB_INT(snmp_lakb_min_cell_temperature[5]),  	NULL},	//Минимальная температура ячейки ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_MIN_CELL_TEMPERATURE, 7},  	MIB_INT(snmp_lakb_min_cell_temperature[6]),  	NULL},	//Минимальная температура ячейки ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_VOLTAGE, 1},  				MIB_INT(snmp_lakb_voltage[0]),  	NULL},	//Напряжение ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_VOLTAGE, 2},  				MIB_INT(snmp_lakb_voltage[1]),  	NULL},	//Напряжение ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_VOLTAGE, 3},  				MIB_INT(snmp_lakb_voltage[2]),  	NULL},	//Напряжение ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_VOLTAGE, 4},  				MIB_INT(snmp_lakb_voltage[3]),  	NULL},	//Напряжение ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_VOLTAGE, 5},  				MIB_INT(snmp_lakb_voltage[4]),  	NULL},	//Напряжение ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_VOLTAGE, 6},  				MIB_INT(snmp_lakb_voltage[5]),  	NULL},	//Напряжение ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_VOLTAGE, 7},  				MIB_INT(snmp_lakb_voltage[6]),  	NULL},	//Напряжение ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_CH_CURR, 1},  				MIB_INT(snmp_lakb_ch_curr[0]),  	NULL},	//Ток заряда ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_CH_CURR, 2},  				MIB_INT(snmp_lakb_ch_curr[1]),  	NULL},	//Ток заряда ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_CH_CURR, 3},  				MIB_INT(snmp_lakb_ch_curr[2]),  	NULL},	//Ток заряда ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_CH_CURR, 4},  				MIB_INT(snmp_lakb_ch_curr[3]),  	NULL},	//Ток заряда ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_CH_CURR, 5},  				MIB_INT(snmp_lakb_ch_curr[4]),  	NULL},	//Ток заряда ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_CH_CURR, 6},  				MIB_INT(snmp_lakb_ch_curr[5]),  	NULL},	//Ток заряда ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_CH_CURR, 7},  				MIB_INT(snmp_lakb_ch_curr[6]),  	NULL},	//Ток заряда ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_DSCH_CURR, 1},  			MIB_INT(snmp_lakb_dsch_curr[0]),  	NULL},	//Ток разряда ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_DSCH_CURR, 2},  			MIB_INT(snmp_lakb_dsch_curr[1]),  	NULL},	//Ток разряда ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_DSCH_CURR, 3},  			MIB_INT(snmp_lakb_dsch_curr[2]),  	NULL},	//Ток разряда ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_DSCH_CURR, 4},  			MIB_INT(snmp_lakb_dsch_curr[3]),  	NULL},	//Ток разряда ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_DSCH_CURR, 5},  			MIB_INT(snmp_lakb_dsch_curr[4]),  	NULL},	//Ток разряда ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_DSCH_CURR, 6},  			MIB_INT(snmp_lakb_dsch_curr[5]),  	NULL},	//Ток разряда ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_DSCH_CURR, 7},  			MIB_INT(snmp_lakb_dsch_curr[6]),  	NULL},	//Ток разряда ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_RAT_CAP, 1},  				MIB_INT(snmp_lakb_rat_cap[0]),  	NULL},	//Номинальная емкость ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_RAT_CAP, 2},  				MIB_INT(snmp_lakb_rat_cap[1]),  	NULL},	//Номинальная емкость ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_RAT_CAP, 3},  				MIB_INT(snmp_lakb_rat_cap[2]),  	NULL},	//Номинальная емкость ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_RAT_CAP, 4},  				MIB_INT(snmp_lakb_rat_cap[3]),  	NULL},	//Номинальная емкость ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_RAT_CAP, 5},  				MIB_INT(snmp_lakb_rat_cap[4]),  	NULL},	//Номинальная емкость ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_RAT_CAP, 6},  				MIB_INT(snmp_lakb_rat_cap[5]),  	NULL},	//Номинальная емкость ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_RAT_CAP, 7},  				MIB_INT(snmp_lakb_rat_cap[6]),  	NULL},	//Номинальная емкость ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_SOH, 1},  				MIB_INT(snmp_lakb_soh[0]),  		NULL},	//Остаточная емкость ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_SOH, 2},  				MIB_INT(snmp_lakb_soh[1]),  		NULL},	//Остаточная емкость ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_SOH, 3},  				MIB_INT(snmp_lakb_soh[2]),  		NULL},	//Остаточная емкость ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_SOH, 4},  				MIB_INT(snmp_lakb_soh[3]),  		NULL},	//Остаточная емкость ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_SOH, 5},  				MIB_INT(snmp_lakb_soh[4]),  		NULL},	//Остаточная емкость ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_SOH, 6},  				MIB_INT(snmp_lakb_soh[5]),  		NULL},	//Остаточная емкость ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_SOH, 7},  				MIB_INT(snmp_lakb_soh[6]),  		NULL},	//Остаточная емкость ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_SOC, 1},  				MIB_INT(snmp_lakb_soc[0]),  		NULL},	//Заряд ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_SOC, 2},  				MIB_INT(snmp_lakb_soc[1]),  		NULL},	//Заряд ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_SOC, 3},  				MIB_INT(snmp_lakb_soc[2]),  		NULL},	//Заряд ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_SOC, 4},  				MIB_INT(snmp_lakb_soc[3]),  		NULL},	//Заряд ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_SOC, 5},  				MIB_INT(snmp_lakb_soc[4]),  		NULL},	//Заряд ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_SOC, 6},  				MIB_INT(snmp_lakb_soc[5]),  		NULL},	//Заряд ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_SOC, 7},  				MIB_INT(snmp_lakb_soc[6]),  		NULL},	//Заряд ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_CCLV, 1},  				MIB_INT(snmp_lakb_cclv[0]),  		NULL},	//Максимальный ток заряда ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_CCLV, 2},  				MIB_INT(snmp_lakb_cclv[1]),  		NULL},	//Максимальный ток заряда ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_CCLV, 3},  				MIB_INT(snmp_lakb_cclv[2]),  		NULL},	//Максимальный ток заряда ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_CCLV, 4},  				MIB_INT(snmp_lakb_cclv[3]),  		NULL},	//Максимальный ток заряда ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_CCLV, 5},  				MIB_INT(snmp_lakb_cclv[4]),  		NULL},	//Максимальный ток заряда ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_CCLV, 6},  				MIB_INT(snmp_lakb_cclv[5]),  		NULL},	//Максимальный ток заряда ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_CCLV, 7},  				MIB_INT(snmp_lakb_cclv[6]),  		NULL},	//Максимальный ток заряда ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_RBT, 1},  				MIB_INT(snmp_lakb_rbt[0]),  		NULL},	//Оцениваемое время работы ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_RBT, 2},  				MIB_INT(snmp_lakb_rbt[1]),  		NULL},	//Оцениваемое время работы ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_RBT, 3},  				MIB_INT(snmp_lakb_rbt[2]),  		NULL},	//Оцениваемое время работы ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_RBT, 4},  				MIB_INT(snmp_lakb_rbt[3]),  		NULL},	//Оцениваемое время работы ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_RBT, 5},  				MIB_INT(snmp_lakb_rbt[4]),  		NULL},	//Оцениваемое время работы ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_RBT, 6},  				MIB_INT(snmp_lakb_rbt[5]),  		NULL},	//Оцениваемое время работы ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_RBT, 7},  				MIB_INT(snmp_lakb_rbt[6]),  		NULL},	//Оцениваемое время работы ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_FLAGS1, 1},  				MIB_INT(snmp_lakb_flags1[0]),  		NULL},	//Первый флаг состояния ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_FLAGS1, 2},  				MIB_INT(snmp_lakb_flags1[1]),  		NULL},	//Первый флаг состояния ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_FLAGS1, 3},  				MIB_INT(snmp_lakb_flags1[0]),  		NULL},	//Первый флаг состояния ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_FLAGS1, 4},  				MIB_INT(snmp_lakb_flags1[1]),  		NULL},	//Первый флаг состояния ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_FLAGS1, 5},  				MIB_INT(snmp_lakb_flags1[0]),  		NULL},	//Первый флаг состояния ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_FLAGS1, 6},  				MIB_INT(snmp_lakb_flags1[1]),  		NULL},	//Первый флаг состояния ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_FLAGS1, 7},  				MIB_INT(snmp_lakb_flags1[0]),  		NULL},	//Первый флаг состояния ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_FLAGS2, 1},  				MIB_INT(snmp_lakb_flags2[0]),  		NULL},	//Второй флаг состояния ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_FLAGS2, 2},  				MIB_INT(snmp_lakb_flags2[1]),  		NULL},	//Второй флаг состояния ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_FLAGS2, 3},  				MIB_INT(snmp_lakb_flags2[0]),  		NULL},	//Второй флаг состояния ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_FLAGS2, 4},  				MIB_INT(snmp_lakb_flags2[1]),  		NULL},	//Второй флаг состояния ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_FLAGS2, 5},  				MIB_INT(snmp_lakb_flags2[0]),  		NULL},	//Второй флаг состояния ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_FLAGS2, 6},  				MIB_INT(snmp_lakb_flags2[1]),  		NULL},	//Второй флаг состояния ЛАКБ
	{ MIB_INTEGER | MIB_ATR_RO,  	13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_FLAGS2, 7},  				MIB_INT(snmp_lakb_flags2[0]),  		NULL},	//Второй флаг состояния ЛАКБ
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_DAMP1, 1},  				MIB_INT(snmp_lakb_damp1[0][0]),  		NULL},	//Отладка
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_DAMP1, 2},  				MIB_INT(snmp_lakb_damp1[1][0]),  		NULL},	//Отладка
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_DAMP1, 3},  				MIB_INT(snmp_lakb_damp1[2][0]),  		NULL},	//Отладка
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_DAMP1, 4},  				MIB_INT(snmp_lakb_damp1[3][0]),  		NULL},	//Отладка
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_DAMP1, 5},  				MIB_INT(snmp_lakb_damp1[4][0]),  		NULL},	//Отладка
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_DAMP1, 6},  				MIB_INT(snmp_lakb_damp1[5][0]),  		NULL},	//Отладка
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_DAMP1, 7},  				MIB_INT(snmp_lakb_damp1[6][0]),  		NULL},	//Отладка
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_DAMP2, 1},  				MIB_INT(snmp_lakb_damp1[0][30]),  		NULL},	//Отладка
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_DAMP2, 2},  				MIB_INT(snmp_lakb_damp1[1][30]),  		NULL},	//Отладка
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_DAMP2, 3},  				MIB_INT(snmp_lakb_damp1[2][30]),  		NULL},	//Отладка
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_DAMP2, 4},  				MIB_INT(snmp_lakb_damp1[3][30]),  		NULL},	//Отладка
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_DAMP2, 5},  				MIB_INT(snmp_lakb_damp1[4][30]),  		NULL},	//Отладка
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_DAMP2, 6},  				MIB_INT(snmp_lakb_damp1[5][30]),  		NULL},	//Отладка
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_DAMP2, 7},  				MIB_INT(snmp_lakb_damp1[6][30]),  		NULL},	//Отладка
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_DAMP3, 1},  				MIB_INT(snmp_lakb_damp1[0][60]),  		NULL},	//Отладка
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_DAMP3, 2},  				MIB_INT(snmp_lakb_damp1[1][60]),  		NULL},	//Отладка
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_DAMP3, 3},  				MIB_INT(snmp_lakb_damp1[2][60]),  		NULL},	//Отладка
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_DAMP3, 4},  				MIB_INT(snmp_lakb_damp1[3][60]),  		NULL},	//Отладка
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_DAMP3, 5},  				MIB_INT(snmp_lakb_damp1[4][60]),  		NULL},	//Отладка
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_DAMP3, 6},  				MIB_INT(snmp_lakb_damp1[5][60]),  		NULL},	//Отладка
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_DAMP3, 7},  				MIB_INT(snmp_lakb_damp1[6][60]),  		NULL},	//Отладка
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_DAMP4, 1},  				MIB_INT(snmp_lakb_damp1[0][90]),  		NULL},	//Отладка
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_DAMP4, 2},  				MIB_INT(snmp_lakb_damp1[1][90]),  		NULL},	//Отладка
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_DAMP4, 3},  				MIB_INT(snmp_lakb_damp1[2][90]),  		NULL},	//Отладка
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_DAMP4, 4},  				MIB_INT(snmp_lakb_damp1[3][90]),  		NULL},	//Отладка
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_DAMP4, 5},  				MIB_INT(snmp_lakb_damp1[4][90]),  		NULL},	//Отладка
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_DAMP4, 6},  				MIB_INT(snmp_lakb_damp1[5][90]),  		NULL},	//Отладка
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_DAMP4, 7},  				MIB_INT(snmp_lakb_damp1[6][90]),  		NULL},	//Отладка
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_DAMP5, 1},  				MIB_INT(snmp_lakb_damp1[0][120]),  		NULL},	//Отладка
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_DAMP5, 2},  				MIB_INT(snmp_lakb_damp1[1][120]),  		NULL},	//Отладка
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_DAMP5, 3},  				MIB_INT(snmp_lakb_damp1[2][120]),  		NULL},	//Отладка
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_DAMP5, 4},  				MIB_INT(snmp_lakb_damp1[3][120]),  		NULL},	//Отладка
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_DAMP5, 5},  				MIB_INT(snmp_lakb_damp1[4][120]),  		NULL},	//Отладка
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_DAMP5, 6},  				MIB_INT(snmp_lakb_damp1[5][120]),  		NULL},	//Отладка
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_DAMP5, 7},  				MIB_INT(snmp_lakb_damp1[6][120]),  		NULL},	//Отладка
/*	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_DAMP6, 1},  				MIB_INT(snmp_lakb_damp1[0][120]),  		NULL},	//Отладка
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_DAMP6, 2},  				MIB_INT(snmp_lakb_damp1[1][120]),  		NULL},	//Отладка
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_DAMP6, 3},  				MIB_INT(snmp_lakb_damp1[2][0]),  		NULL},	//Отладка
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_DAMP6, 4},  				MIB_INT(snmp_lakb_damp1[3][0]),  		NULL},	//Отладка
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_DAMP6, 5},  				MIB_INT(snmp_lakb_damp1[4][0]),  		NULL},	//Отладка
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_DAMP6, 6},  				MIB_INT(snmp_lakb_damp1[5][0]),  		NULL},	//Отладка
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_DAMP6, 7},  				MIB_INT(snmp_lakb_damp1[6][0]),  		NULL},	//Отладка
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_DAMP7, 1},  				MIB_INT(snmp_lakb_damp1[0][0]),  		NULL},	//Отладка
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_DAMP7, 2},  				MIB_INT(snmp_lakb_damp1[1][0]),  		NULL},	//Отладка
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_DAMP7, 3},  				MIB_INT(snmp_lakb_damp1[2][0]),  		NULL},	//Отладка
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_DAMP7, 4},  				MIB_INT(snmp_lakb_damp1[3][0]),  		NULL},	//Отладка
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_DAMP7, 5},  				MIB_INT(snmp_lakb_damp1[4][0]),  		NULL},	//Отладка
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_DAMP7, 6},  				MIB_INT(snmp_lakb_damp1[5][0]),  		NULL},	//Отладка
	{ MIB_OCTET_STR | MIB_ATR_RO, 13, {OID_ENTERPRISE, OID_DEVICE, DISPLAY_LAKB, DISPLAY_LAKB_DAMP7, 7},  				MIB_INT(snmp_lakb_damp1[6][0]),  		NULL},	//Отладка	*/


/*	{ MIB_INTEGER, 	13, {OID0(1,3), 6, 1, 4, 1, 130, 131, 31, 4, 2, 1, 1, 1},    MIB_INT(LPC_RTC->HOUR),    NULL},
	{ MIB_INTEGER, 	13, {OID0(1,3), 6, 1, 4, 1, 130, 131, 31, 4, 2, 1, 1, 2},    MIB_INT(LPC_RTC->YEAR),    NULL},				  
	{ MIB_INTEGER, 	13, {OID0(1,3), 6, 1, 4, 1, 130, 131, 31, 4, 2, 1, 2, 1},    MIB_INT(LPC_RTC->MIN),    NULL},
	{ MIB_INTEGER, 	13, {OID0(1,3), 6, 1, 4, 1, 130, 131, 31, 4, 2, 1, 2, 2},    MIB_INT(LPC_RTC->YEAR),    NULL},
	{ MIB_INTEGER, 	13, {OID0(1,3), 6, 1, 4, 1, 130, 131, 31, 4, 2, 1, 3, 1},     MIB_INT(LPC_RTC->SEC),    NULL},
	{ MIB_INTEGER, 	13, {OID0(1,3), 6, 1, 4, 1, 130, 131, 31, 4, 2, 1, 3, 2},    MIB_INT(LPC_RTC->MONTH),    NULL},
	{ MIB_INTEGER, 	13, {OID0(1,3), 6, 1, 4, 1, 130, 131, 31, 4, 2, 1, 4, 1},     MIB_INT(sysMainsVoltage),    NULL},
	{ MIB_INTEGER, 	13, {OID0(1,3), 6, 1, 4, 1, 130, 131, 31, 4, 2, 1, 4, 2},    MIB_INT(LPC_RTC->HOUR),    NULL},
	{ MIB_INTEGER, 	13, {OID0(1,3), 6, 1, 4, 1, 130, 131, 31, 4, 2, 1, 5, 1},     MIB_INT(sysMainsVoltage),    NULL},	    //-----------------------------------------------
	{ MIB_INTEGER, 	13, {OID0(1,3), 6, 1, 4, 1, 130, 131, 31, 4, 2, 1, 5, 2},    MIB_INT(sysServices),    NULL},
	{ MIB_INTEGER, 	13, {OID0(1,3), 6, 1, 4, 1, 130, 131, 31, 4, 2, 1, 6, 1},     MIB_INT(sysMainsVoltage),    NULL},
	{ MIB_INTEGER, 	13, {OID0(1,3), 6, 1, 4, 1, 130, 131, 31, 4, 2, 1, 6, 2},    MIB_INT(sysServices),    NULL},
	{ MIB_INTEGER, 	13, {OID0(1,3), 6, 1, 4, 1, 130, 131, 31, 4, 2, 1, 7, 1},     MIB_INT(sysMainsVoltage),    NULL},
	{ MIB_INTEGER, 	13, {OID0(1,3), 6, 1, 4, 1, 130, 131, 31, 4, 2, 1, 7, 2},    MIB_INT(TestForTableValues),    NULL},
	{ MIB_INTEGER, 	13, {OID0(1,3), 6, 1, 4, 1, 130, 131, 31, 4, 2, 1, 8, 1},     MIB_INT(sysMainsVoltage),    NULL},
	{ MIB_INTEGER, 	13, {OID0(1,3), 6, 1, 4, 1, 130, 131, 31, 4, 2, 1, 8, 2},    MIB_INT(TestForTableValues),    NULL},
	{ MIB_INTEGER, 	13, {OID0(1,3), 6, 1, 4, 1, 130, 131, 31, 4, 2, 1, 9, 1},     MIB_INT(sysMainsVoltage),    NULL},
	{ MIB_INTEGER, 	13, {OID0(1,3), 6, 1, 4, 1, 130, 131, 31, 4, 2, 1, 9, 2},    MIB_INT(TestForTableValues),    NULL},
	{ MIB_INTEGER, 	13, {OID0(1,3), 6, 1, 4, 1, 130, 131, 31, 4, 2, 1, 10, 1},     MIB_INT(sysMainsVoltage),    NULL},
	{ MIB_INTEGER, 	13, {OID0(1,3), 6, 1, 4, 1, 130, 131, 31, 4, 2, 1, 10, 2},    MIB_INT(TestForTableValues),    NULL},
	{ MIB_INTEGER, 	13, {OID0(1,3), 6, 1, 4, 1, 130, 131, 31, 4, 2, 1, 11, 1},    MIB_INT(sysMainsVoltage),     NULL},
	{ MIB_INTEGER, 	13, {OID0(1,3), 6, 1, 4, 1, 130, 131, 31, 4, 2, 1, 11, 2},    MIB_INT(TestForTableValues),    NULL},
	{ MIB_INTEGER, 	13, {OID0(1,3), 6, 1, 4, 1, 130, 131, 31, 4, 2, 1, 12, 1},     MIB_INT(sysMainsVoltage),    NULL},
	{ MIB_INTEGER, 	13, {OID0(1,3), 6, 1, 4, 1, 130, 131, 31, 4, 2, 1, 12, 2},    MIB_INT(TestForTableValues),    NULL},
	{ MIB_INTEGER, 	13, {OID0(1,3), 6, 1, 4, 1, 130, 131, 31, 4, 2, 1, 13, 1},    MIB_INT(sysMainsVoltage),    NULL},
	{ MIB_INTEGER, 	13, {OID0(1,3), 6, 1, 4, 1, 130, 131, 31, 4, 2, 1, 13, 2},    MIB_INT(TestForTableValues),    NULL},
	{ MIB_OCTET_STR, 13, {OID0(1,3), 6, 1, 4, 1, 130, 131, 31, 1, 2, 7, 4, 0},  MIB_STR("Proverka sviazi. Проверка связи."),   NULL},	*/

	};
const int snmp_mib_size = (sizeof(snmp_mib) / sizeof(MIB_ENTRY));

///*----------------------------------------------------------------------------
// *      MIB Callback Functions
// *---------------------------------------------------------------------------*/
//
//static void write_leds (int mode) {
//  /* No action on read access. */
//  if (mode == MIB_WRITE) {
//    LED_out (LedOut);
//  }
//}
//
//static void read_key (int mode) {
//  /* Read ARM Digital Input */
//  if (mode == MIB_READ) {
//    KeyIn = get_button();
//  }
//}
//
//static void upd_display (int mode) {
//  /* Update LCD Module display text. */
//  if (mode == MIB_WRITE) {
//    /* Write access. */
//    LCDupdate = __TRUE;
//  }
//}

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
