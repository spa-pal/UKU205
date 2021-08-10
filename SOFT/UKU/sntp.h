
#define PORT_NUM 1001

#define SNTP_IP1            88
#define SNTP_IP2            147
#define SNTP_IP3            254
#define SNTP_IP4            230
//#define GMT 7L


extern U8 socket_udp;
extern U16 udp_callback_cnt,udp_callback_cnt1;
extern U8 Rem_IP[4];
extern const int NTP_PACKET_SIZE;
extern U16 udp_callback_plazma[12];
extern U16 day_of_year;
extern U32 full_days_since_2000_01_01;
extern U32 curr_days_since_2000_01_01;
extern U32 sec_after_2000_01_01;
extern U16 day_after_this_month;
extern const U16 days_of_month[];
extern U16	this_year;
extern long tempL;
extern U32 day_after_this_year;
extern U16 this_month;
extern U16 day_of_month;
extern U32 sec_in_this_day;
extern U16 hour_in_this_day;
extern U16 min_in_this_hour;
extern U16 sec_in_this_min;
extern U16 time_sinc_hndl_req_cnt;
extern U32 time_sinc_hndl_main_cnt;

extern char sntp_plazma;

U16 udp_callback (U8 soc, U8 *rip, U16 rport, U8 *buf, U16 len);
void sntp_requ (void);
void time_sinc_hndl(void);


