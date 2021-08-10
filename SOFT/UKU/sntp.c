
#include <LPC17xx.H>
#include "rtl.h"
#include "sntp.h"
#include "main.h"
#include "eeprom_map.h"

U8 socket_udp;
U16 udp_callback_cnt,udp_callback_cnt1;
U8 Rem_IP[4] = {SNTP_IP1,SNTP_IP2,SNTP_IP3,SNTP_IP4};
const int NTP_PACKET_SIZE = 48;
U16 udp_callback_plazma[12];
U16 day_of_year;
U32 full_days_since_2000_01_01;
U32 curr_days_since_2000_01_01;
U32 sec_after_2000_01_01;
U16 day_after_this_month;
const U16 days_of_month[]={0,31,28,31,30,31,30,31,31,30,31,30,31};
U16	this_year;
long tempL;
U32 day_after_this_year;
U16 this_month;
U16 day_of_month;
U32 sec_in_this_day;
U16 hour_in_this_day;
U16 min_in_this_hour;
U16 sec_in_this_min;
U16 time_sinc_hndl_req_cnt;
U32 time_sinc_hndl_main_cnt=60;

char sntp_plazma;


//-----------------------------------------------
void procrec (U8 *buf) {
/*  switch (buf[0]) {
    case BLINKLED:
      LED_out (buf[1]);
      break;
  }	*/
}

//-----------------------------------------------
U16 udp_callback (U8 soc, U8 *rip, U16 rport, U8 *buf, U16 len) 
{
rip  = rip;
rport= rport;
len  = len;

udp_callback_cnt++;

if (soc == socket_udp) 
	{
	long tempL;
	long year_cnt,day_cnt;

	udp_callback_cnt1++;
	udp_callback_plazma[0]=rip[0];
	udp_callback_plazma[1]=rip[1];
	udp_callback_plazma[2]=rip[2];
	udp_callback_plazma[3]=rip[3];
	udp_callback_plazma[4]=rport;
	udp_callback_plazma[5]=buf[40];
	udp_callback_plazma[6]=buf[41];
	udp_callback_plazma[7]=buf[42];
	udp_callback_plazma[8]=buf[43];
	tempL=buf[40];
	tempL<<=8;
	tempL|=buf[41];
	tempL<<=8;
	tempL|=buf[42];
	tempL<<=8;
	tempL|=buf[43];



	udp_callback_cnt1++;




	tempL-=3155673600L;

	tempL+=(3600L*SNTP_GMT);

	sec_after_2000_01_01=tempL;

	tempL/=86400L;

		//tempL=6575;

		//full_days_since_2000_01_01=(U32)tempL;
	full_days_since_2000_01_01=sec_after_2000_01_01/86400L;
		
	curr_days_since_2000_01_01=full_days_since_2000_01_01+1;

	day_after_this_year=0;
	for(this_year=0;this_year<100;this_year++)
		{
		long temptempL;
		temptempL=day_after_this_year;
		day_after_this_year+=365;
		if((this_year%4)==0)day_after_this_year++;

		if(curr_days_since_2000_01_01<=day_after_this_year)
			{
			day_after_this_year=temptempL;

			day_of_year=curr_days_since_2000_01_01-day_after_this_year;
			break;
			}
		}

	day_after_this_month=0;
	for(this_month=1;this_month<13;this_month++)
		{
		long temptempL;
		temptempL=day_after_this_month;
		day_after_this_month+=days_of_month[this_month];
		if(((this_year%4)==0)&&(this_month==2))day_after_this_month++;

		if(day_of_year<=day_after_this_month)
			{
			day_after_this_month=temptempL;
			day_of_month=day_of_year-day_after_this_month;
			break;
			}
		}

	sec_in_this_day=sec_after_2000_01_01-(full_days_since_2000_01_01*86400L);
	hour_in_this_day=(short)(sec_in_this_day/3600L);
	min_in_this_hour=(short)((sec_in_this_day%3600L)/60L);
	sec_in_this_min=(short)((sec_in_this_day%3600L)%60L);

	udp_callback_plazma[10]=day_of_year;
		//udp_callback_plazma[9]=year_cnt;

	if(time_sinc_hndl_req_cnt)
		{
		LPC_RTC->HOUR=hour_in_this_day;
		LPC_RTC->MIN=min_in_this_hour;
		LPC_RTC->SEC=sec_in_this_min;
		LPC_RTC->YEAR=this_year;
		LPC_RTC->MONTH=this_month;
		LPC_RTC->DOM=day_of_month;
		}
	}

}


//-----------------------------------------------
void sntp_requ (void)
{
U8 *sendbuf;
U8 p2;

if (socket_udp != 0) 
	{
    /* Start Connection */
    sendbuf = udp_get_buf (NTP_PACKET_SIZE);

	memset(sendbuf,NTP_PACKET_SIZE,0);

	sendbuf[0] = 0xE3;   // LI, Version, Mode
	sendbuf[1] = 0;     // Stratum, or type of clock
	sendbuf[2] = 6;     // Polling Interval
	sendbuf[3] = 0xEC;  // Peer Clock Precision
		  // 8 bytes of zero for Root Delay & Root Dispersion
	sendbuf[12]  = 49;
	sendbuf[13]  = 0x4E;
	sendbuf[14]  = 49;
	sendbuf[15]  = 52;

	udp_send (socket_udp, Rem_IP, 123, sendbuf, NTP_PACKET_SIZE);
	}
}

//-----------------------------------------------
void time_sinc_hndl(void)
{
if(time_sinc_hndl_req_cnt)time_sinc_hndl_req_cnt--;

if((SNTP_ENABLE)&&(ETH_IS_ON))
	{
	if(time_sinc_hndl_main_cnt)
		{
		time_sinc_hndl_main_cnt--;
		if(!time_sinc_hndl_main_cnt)
			{
			time_sinc_hndl_req_cnt=5;

			if(lc640_read_int(EE_SNTP_WEB_ENABLE)==1)
				{
				Rem_IP[0]=SNTP_IP1;
				Rem_IP[1]=SNTP_IP2;
				Rem_IP[2]=SNTP_IP3;
				Rem_IP[3]=SNTP_IP4;
				}
			else
				{
				Rem_IP[0]=(char)lc640_read_int(EE_SNTP_IP1);
				Rem_IP[1]=(char)lc640_read_int(EE_SNTP_IP2);
				Rem_IP[2]=(char)lc640_read_int(EE_SNTP_IP3);
				Rem_IP[3]=(char)lc640_read_int(EE_SNTP_IP4);
				}
			sntp_requ();

			if(SNTP_ENABLE==1)time_sinc_hndl_main_cnt=3600L;
			else if(SNTP_ENABLE==2)time_sinc_hndl_main_cnt=86400L;
			else if(SNTP_ENABLE==3)time_sinc_hndl_main_cnt=604800L;
			}
		}
	}
}