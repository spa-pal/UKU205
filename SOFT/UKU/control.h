
//-----------------------------------------------
//АПВ
extern unsigned main_apv_cnt,hour_apv_cnt[2],reset_apv_cnt[2];
extern unsigned short apv_cnt_sec[2],apv_cnt[2];
extern char apv_cnt_1;
extern short adc_buff_out_[3];

//-----------------------------------------------
//Управление источниками
extern char cnt_src[2];
extern signed short cntrl_stat,old_cntrl_stat,cntrl_stat1,cntrl_stat2;
extern signed short u_necc,u_necc_,u_necc_up,u_necc_dn;
extern char cnt_blok;
typedef enum {iiB,iiM} enum_ibs;
extern enum_ibs Ibs,Ibso;
extern char bmm_cnt,bmp_cnt;
extern char bS;
extern signed char num_necc;
extern signed char cnt_num_necc;
extern char tzas_cnt;
extern char bp_tumbler[2];

//-----------------------------------------------
//Выравнивание токов
extern short avg_main_cnt;
extern short i_avg_max,i_avg_min; 
extern short avg;
extern char bAVG;
extern char avg_cnt;  
extern char avg_num; 
extern signed short x_; 
extern char avg_cnt_3;

//-----------------------------------------------
//Контроль наличия батареи
extern char kb_cnt[2];
extern char kb_full_ver;
extern signed short bat_ver_cnt;
extern signed short ibat_,ibat__;


//-----------------------------------------------
//Спецфункции
typedef enum {spc_OFF=0,spc_KE=33,spc_VZ=5}enum_spc_stat;
extern enum_spc_stat spc_stat;
extern unsigned long cnt_vz_sec,cnt_vz_sec_;
extern unsigned long ke_date[2];
extern char bAVZ;

//-----------------------------------------------
//Математика
extern char bit_minus;

//-----------------------------------------------
//Звук
extern char cnt_beep;

//-----------------------------------------------
//Контроль заряда
extern char sign_U,sign_I;
extern char superviser_cnt;

//-----------------------------------------------
//Контроль предохранителей
extern const char FUSE_CONST[3];
extern char fuse_av_cnt[3];
extern char fuse_av_stat[3];
extern char fuse_av_stat_old[3];

//-----------------------------------------------
//Контроль реле аварий
extern char rele_av_stat[3];

//-----------------------------------------------
//Контроль сухих контактов
extern char sk_stat_cnt[4];
extern const char SK_CONST[4];

void apv_drv(void);
void avg_hndl(void);
void beep_hndl(void);
void cntrl_drv(void);
void cntrl_hndl(void);
void zar_drv(void);
void spc_drv(void);
void cntrl_blok_hndl(void);
void ext_drv(void);
void rs232_transmit_of_temper(char in1,char in2,short in3);
void rs232_transmit_of_sk(char in1,char in2);
void bat_drv(void);
void kb_drv(void);
void matemat(void);
void out_out(void);
void rele_drv(void);
void samokalibr_drv(void);
void time_drv(void);
void u_necc_hndl(void);
void kz_on(void);
void kz_off(void);
void bp_on(char in);
void bp_off(char in);
void kb_start(char in);
void unet_drv(void);
void src_drv(char in);
void blok_src_reset(char in);
void apv2_hndl(char in);
void adc_drv(void);
char ke_start(void);
void next_date_hndl(char period);
void def_set(int umax__,int ub0__,int ub20__,int usign__,int imax__,int uob__,int numi);
char ubat_old_drv(void);
void avz_hndl(void);
void mnemo_hndl(void);
void zar_superviser_drv(void);
void zar_superviser_start(void);


