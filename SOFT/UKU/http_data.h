//Телеметрия сети
extern char http_power_num_of_phases;
extern short http_power_voltage_of_phase[3];
extern short http_power_frequncy;
extern char http_power_status;
extern char http_output_buff[70];
extern char log_item_cnt;
extern char http_string_of_model[50];
extern char* http_model;
extern char http_tm_output_buff[200];

//-----------------------------------------------
void http_data(void);
//-----------------------------------------------
short http_get_log_deep(void);
//-----------------------------------------------
char* http_get_log_rec(char num);
//-----------------------------------------------
char* http_tm_dt_output(char numOfDt);
//-----------------------------------------------
char* http_tm_dt_output_(char numOfDt);
//-----------------------------------------------
char* http_tm_sk_output(char numOfSk);
//-----------------------------------------------
char* http_tm_src_output(char numOfSrc);
//-----------------------------------------------
char* http_ip_output(char ip1, char ip2, char ip3, char ip4);
//-----------------------------------------------
char* http_tm_bat_output(char numOfSrc);
//-----------------------------------------------
char* pal_cyr_decoder(char* input);

