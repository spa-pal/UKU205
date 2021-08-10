extern char snmp_community[10];
extern char snmp_web_passw[4];

//Информация об устройстве
extern signed short snmp_device_code;
extern signed 	   	snmp_sernum;
extern signed short snmp_sernum_lsb;
extern signed short snmp_sernum_msb;
extern char 	   	snmp_location[100];
extern signed short snmp_numofbat;
extern signed short snmp_numofbps;
extern signed short snmp_numofevents;
extern char 	   	snmp_model[30];		//Модель ИБЭПа

//Состояние первичной сети
extern signed short snmp_mains_power_voltage;
extern signed short snmp_mains_power_frequency;
extern signed short snmp_mains_power_status;
extern signed short snmp_mains_power_alarm;

//Состояние нагрузки
extern signed short snmp_load_voltage;
extern signed short snmp_load_current;

//Состояние БПСов
extern signed short snmp_bps_number[2];
extern signed short snmp_bps_voltage[2];
extern signed short snmp_bps_current[2];
extern signed short snmp_bps_temperature[2];
extern signed short snmp_bps_stat[2];

//Состояние Батареи
extern signed short snmp_bat_number;
extern signed short snmp_bat_voltage;
extern signed short snmp_bat_current;
extern signed short snmp_bat_temperature;
extern signed short snmp_bat_capacity;
extern signed short snmp_bat_charge;
extern signed short snmp_bat_status;

extern signed short snmp_bat_cell1_voltage, snmp_bat_cell2_voltage, snmp_bat_cell3_voltage, snmp_bat_cell4_voltage, snmp_bat_cell5_voltage;
extern signed short snmp_bat_cell1_imbalance, snmp_bat_cell2_imbalance, snmp_bat_cell3_imbalance, snmp_bat_cell4_imbalance, snmp_bat_cell5_imbalance;
extern signed short snmp_bat_cell1_imbalance_stat, snmp_bat_cell2_imbalance_stat, snmp_bat_cell3_imbalance_stat, snmp_bat_cell4_imbalance_stat, snmp_bat_cell5_imbalance_stat;


//Состояние предохранителей
extern signed short snmp_fuse_av_stat[3];
extern signed short snmp_fuse_number[3];

//Состояние сухих контактов
extern signed short snmp_sk_av_stat[4];
extern signed short snmp_sk_number[4];

//Спецфункции
extern signed short snmp_spc_stat;
extern char snmp_spc_trap_message[100];
extern signed short snmp_spc_trap_value;
extern signed short snmp_spc_trap_value_0,snmp_spc_trap_value_1,snmp_spc_trap_value_2;

//Команды
extern signed short snmp_command;
extern signed short snmp_command_parametr;

//Журнал аварий
extern char snmp_log[64][128];

//Установочные параметры
extern signed short snmp_main_bps;
extern signed short snmp_zv_en;
extern signed short snmp_alarm_auto_disable;
extern signed short snmp_bat_test_time;
extern signed short snmp_u_max;
extern signed short snmp_u_0_grad;
extern signed short snmp_u_20_grad;
extern signed short snmp_u_sign;
extern signed short snmp_u_min_power;
extern signed short snmp_u_withouth_bat;
extern signed short snmp_control_current;
extern signed short snmp_max_charge_current;
extern signed short snmp_max_current;
extern signed short snmp_max_current_koef;
extern signed short snmp_max_current_koef;
extern signed short snmp_up_charge_koef;
extern signed short snmp_powerup_psu_timeout;
extern signed short snmp_max_temperature;
 
extern char temp_str[20];

extern char snmp_trap_send_i,snmp_trap_send_ii;
extern unsigned short obj[10];
extern unsigned char temp_ip[4];
 
//-----------------------------------------------
void snmp_data (void);
void snmp_sernum_write (int mode); 
void snmp_location_write (int mode);
void snmp_model_write (int mode); 
void snmp_command_execute (int mode);
void event2snmp(char num);
void snmp_main_bps_write (int mode);
void snmp_zv_on_write (int mode);
void snmp_alarm_auto_disable_write (int mode);
void snmp_bat_test_time_write (int mode);
void snmp_u_max_write (int mode);
void snmp_u_0_grad_write (int mode);
void snmp_u_20_grad_write (int mode);
void snmp_u_sign_write (int mode);
void snmp_u_min_power_write (int mode);
void snmp_u_withouth_bat_write (int mode);
void snmp_control_current_write (int mode);
void snmp_max_charge_current_write (int mode);
void snmp_max_current_write (int mode);
void snmp_max_current_koef_write (int mode);
void snmp_up_charge_koef_write (int mode);
void snmp_powerup_psu_timeout_write (int mode);
void snmp_max_temperature_write (int mode);
void event2snmp(char num);
void snmp_set_primary (int mode);
void snmp_disable (int mode);
void snmp_unlock (int mode);
void snmp_parallel_switch (int mode);
void snmp_vz_start (int mode);
void snmp_ke_start (int mode);
void snmp_spc_stop (int mode);
void snmp_trap_send(char* str, signed short in0, signed short in1, signed short in2);

 
