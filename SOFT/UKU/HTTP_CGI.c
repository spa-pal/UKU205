/*----------------------------------------------------------------------------
 *      RL-ARM - TCPnet
 *----------------------------------------------------------------------------
 *      Name:    HTTP_CGI.C
 *      Purpose: HTTP Server CGI Module
 *      Rev.:    V4.05
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2009 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <Net_Config.h>
#include <stdio.h>
#include <string.h>
#include "main.h"
#include "control.h"
#include "sntp.h"
#include "http_data.h"
#include "eeprom_map.h"
#include "snmp_data_file.h"
#include <LPC17xx.H>

/* ---------------------------------------------------------------------------
 * The HTTP server provides a small scripting language.
 *
 * The script language is simple and works as follows. Each script line starts
 * with a command character, either "i", "t", "c", "#" or ".".
 *   "i" - command tells the script interpreter to "include" a file from the
 *         virtual file system and output it to the web browser.
 *   "t" - command should be followed by a line of text that is to be output
 *         to the browser.
 *   "c" - command is used to call one of the C functions from the this file.
 *         It may be followed by the line of text. This text is passed to
 *         'cgi_func()' as a pointer to environment variable.
 *   "#' - command is a comment line and is ignored (the "#" denotes a comment)
 *   "." - denotes the last script line.
 *
 * --------------------------------------------------------------------------*/

/* http_demo.c */
extern U16 AD_in (U32 ch);
extern U8  get_button (void);
char psw_err;

/* at_System.c */
extern  LOCALM localm[];
#define LocM   localm[NETIF_ETH]

/* Net_Config.c */
extern struct tcp_info tcp_socket[];
extern U8 const tcp_NumSocks;
extern U8 const http_EnAuth;
extern U8       http_auth_passw[20];

extern BOOL LEDrun;
extern void LED_out (U32 val);
extern BOOL LCDupdate;
extern U8   lcd_text[2][16+1];

/* Local variables. */
static U8 P2;
static char const state[][11] = {
  "FREE",
  "CLOSED",
  "LISTEN",
  "SYN_REC",
  "SYN_SENT",
  "FINW1",
  "FINW2",
  "CLOSING",
  "LAST_ACK",
  "TWAIT",
  "CONNECT"};

/* My structure of CGI status U32 variable. This variable is private for */
/* each HTTP Session and is not altered by HTTP Server. It is only set to  */
/* zero when the cgi_func() is called for the first time.                  */
typedef struct {
  U16 xcnt;
  U16 unused;
} MY_BUF;
#define MYBUF(p)        ((MY_BUF *)p)



/*
//-----------------------------------------------
char* http_tm_src_output(char numOfSrc)
{
char buffer[100];

//char* buffer;
sprintf(buffer,"%d %d %d %d 0x%02x", bps[numOfSrc]._Uii, bps[numOfSrc]._Ii, bps[numOfSrc]._Ti, bps_status2number(numOfSrc), bps[numOfSrc]._flags_tm );

return buffer;
}*/




/*----------------------------------------------------------------------------
 * HTTP Server Common Gateway Interface Functions
 *---------------------------------------------------------------------------*/

/*--------------------------- cgi_process_var -------------------------------*/

void cgi_process_var (U8 *qs) {
  /* This function is called by HTTP server to process the Querry_String   */
  /* for the CGI Form GET method. It is called on SUBMIT from the browser. */
  /*.The Querry_String.is SPACE terminated.                                */
  U8 *var;
  int s[4];

	//web_plazma[1]++;

  var = (U8 *)alloc_mem (40);
  do {
    /* Loop through all the parameters. */
    qs = http_get_var (qs, var, 40);
    /* Check the returned string, 'qs' now points to the next. */
    if (var[0] != 0) {
      /* Returned string is non 0-length. */
      if (str_scomp (var, "ip=") == __TRUE) {
        /* My IP address parameter. */
        sscanf ((const char *)&var[3], "%d.%d.%d.%d",&s[0],&s[1],&s[2],&s[3]);
        LocM.IpAdr[0]   = s[0];
        LocM.IpAdr[1]   = s[1];
        LocM.IpAdr[2]   = s[2];
        LocM.IpAdr[3]   = s[3];
      }
      else if (str_scomp (var, "msk=") == __TRUE) {
        /* Net mask parameter. */
        sscanf ((const char *)&var[4], "%d.%d.%d.%d",&s[0],&s[1],&s[2],&s[3]);
        LocM.NetMask[0] = s[0];
        LocM.NetMask[1] = s[1];
        LocM.NetMask[2] = s[2];
        LocM.NetMask[3] = s[3];
      }
      else if (str_scomp (var, "gw=") == __TRUE) {
        /* Default gateway parameter. */
        sscanf ((const char *)&var[3], "%d.%d.%d.%d",&s[0],&s[1],&s[2],&s[3]);
        LocM.DefGW[0]   = s[0];
        LocM.DefGW[1]   = s[1];
        LocM.DefGW[2]   = s[2];
        LocM.DefGW[3]   = s[3];
      }
      else if (str_scomp (var, "pdns=") == __TRUE) {
        /* Default gateway parameter. */
        sscanf ((const char *)&var[5], "%d.%d.%d.%d",&s[0],&s[1],&s[2],&s[3]);
        LocM.PriDNS[0]  = s[0];
        LocM.PriDNS[1]  = s[1];
        LocM.PriDNS[2]  = s[2];
        LocM.PriDNS[3]  = s[3];
      }
      else if (str_scomp (var, "sdns=") == __TRUE) {
        /* Default gateway parameter. */
        sscanf ((const char *)&var[5], "%d.%d.%d.%d",&s[0],&s[1],&s[2],&s[3]);
        LocM.SecDNS[0]  = s[0];
        LocM.SecDNS[1]  = s[1];
        LocM.SecDNS[2]  = s[2];
        LocM.SecDNS[3]  = s[3];
      }
    }
  }while (qs);
  free_mem ((OS_FRAME *)var);
}

//action="/parole.cgx"
//document.location.href = "http://www.site.ru"

/*--------------------------- cgi_process_data ------------------------------*/

void cgi_process_data (U8 code, U8 *dat, U16 len) {
  /* This function is called by HTTP server to process the returned Data    */
  /* for the CGI Form POST method. It is called on SUBMIT from the browser. */
  /* Parameters:                                                            */
  /*   code  - callback context code                                        */
  /*           0 = www-url-encoded form data                                */
  /*           1 = filename for file upload (0-terminated string)           */
  /*           2 = file upload raw data                                     */
  /*           3 = end of file upload (file close requested)                */
  /*           4 = any xml encoded POST data (single or last stream)        */
  /*           5 = the same as 4, but with more xml data to follow          */
  /*               Use http_get_content_type() to check the content type    */  
  /*   dat   - pointer to POST received data                                */
  /*   len   - received data length                                         */
  U8 passw[12],retyped[12];
  U8 *var,stpassw; 
U8 *varr[3];
U8 i;

//web_plazma[2]++;
//web_plazma[3]+=len;



  switch (code) {
    case 0:
      /* Url encoded form data received. */
      break;

    default:
      /* Ignore all other codes. */
      return;
  }

  P2 = 0;
  /////LEDrun = __TRUE;
  if (len == 0) {
    /* No data or all items (radio, checkbox) are off. */
    /////LED_out (P2);
    return;
  }
  
  
stpassw = 0;
var = (U8 *)alloc_mem (40);
varr[0] = (U8 *)alloc_mem (40);
varr[1] = (U8 *)alloc_mem (150);
varr[2] = (U8 *)alloc_mem (40);

i=0;

do 
	{
    /* Parse all returned parameters. */
	if(i==1)dat = http_get_var (dat, varr[i++], 150);
    else dat = http_get_var (dat, varr[i++], 40);
	//web_plazma[0]++;
	}
while (dat);

//return;
    if (varr[0,0] != 0) 
		{
		/* Parameter found, returned string is non 0-length. */
		if (str_scomp (varr[0], "parol") == __TRUE)
			{
			char str_buff[4];
        	//web_plazma[1]++;
			
			if ((str_scomp (varr[0]+6, snmp_web_passw) == __TRUE)&&(len == 9)) 
				{
				uku_set_autorized=1;
				psw_err=0;
				}
			else
				{
				psw_err=1;
				uku_set_autorized=0;
				}
			}
		else if (str_scomp (varr[0], "param=") == __TRUE) 
			{
			if(strstr (varr[1], "value="))
				{
				
				//web_plazma[4]=22;

				short input_param;
				if(strstr (varr[0], "serno"))
					{
					sscanf ((const char *)varr[1]+6, "%d",&web_param_input);
					lc640_write_int(EE_AUSW_MAIN_NUMBER,(short)(web_param_input&0x0000ffffUL));
					lc640_write_int(EE_AUSW_MAIN_NUMBER+2,(short)((web_param_input&0xffff0000UL)>>16UL));
					}
				else if(strstr (varr[0], "place"))
					{
					char i = 0;
					str_copy(place_holder,"                                                                      ");
					str_copy(place_holder,pal_cyr_decoder(varr[1]+6));
					//str_copy(place_holder,varr[1]+6);
					/*while(place_holder[i]) 
						{
						lc640_write(EE_HTTP_LOCATION+i,place_holder[i]);
						i++;
						}*/
					for (i=0;i<70;i++)lc640_write(EE_HTTP_LOCATION+i,place_holder[i]);
					}

				else if(strstr (varr[0], "main_bps_"))
					{
					sscanf ((const char *)varr[1]+6, "%d",&web_param_input);
					input_param=(short)web_param_input;
					//web_plazma[0] = (char)web_param_input;
					gran(&input_param,1,2);
					lc640_write_int(EE_MAIN_BPS,input_param-1);

					}
				else if(strstr (varr[0], "zv_on_"))
					{
					sscanf ((const char *)varr[1]+6, "%d",&web_param_input);
					lc640_write_int(EE_ZV_ON,(short)(web_param_input&0x00000001UL));

					}
				else if(strstr (varr[0], "av_avt_"))
					{
					sscanf ((const char *)varr[1]+6, "%d",&web_param_input);
					lc640_write_int(EE_AV_OFF_AVT,(short)(web_param_input&0x00000001UL));

					}				

				else if(strstr (varr[0], "mnemo_"))
					{
					sscanf ((const char *)varr[1]+6, "%d",&web_param_input);
					if((short)(web_param_input&0x0000ffffUL)==0)lc640_write_int(EE_MNEMO_ON,0);
					else if((short)(web_param_input&0x0000ffffUL))
						{
						short tempSS=(short)(web_param_input&0x0000ffffUL);
						if((tempSS>=5)&&(tempSS<=300))
							{
							lc640_write_int(EE_MNEMO_ON,mnON);
							lc640_write_int(EE_MNEMO_TIME,tempSS);
							}
						}
				/*	else if((short)(web_param_input&0x0000ffffUL)==2)
						{
						lc640_write_int(EE_MNEMO_ON,1);
						lc640_write_int(EE_MNEMO_TIME,30);
						}
					else if((short)(web_param_input&0x0000ffffUL)==3)
						{
						lc640_write_int(EE_MNEMO_ON,1);
						lc640_write_int(EE_MNEMO_TIME,60);
						}
					else if((short)(web_param_input&0x0000ffffUL)==4)
						{
						lc640_write_int(EE_MNEMO_ON,1);
						lc640_write_int(EE_MNEMO_TIME,300);
						}  */

					}
/**				else if(strstr (varr[0], "un_max_"))
					{
					sscanf ((const char *)varr[1]+6, "%d",&web_param_input);
					lc640_write_int(EE_UMAXN,(short)(web_param_input&0x0000ffffUL));

					}		
					*/
				else if(strstr (varr[0], "un_min_"))
					{
					sscanf ((const char *)varr[1]+6, "%d",&web_param_input);
					lc640_write_int(EE_UMN,(short)(web_param_input&0x0000ffffUL));

					}
				else if(strstr (varr[0], "un_max_"))
					{
					sscanf ((const char *)varr[1]+6, "%d",&web_param_input);
					lc640_write_int(EE_UMAXN,(short)(web_param_input&0x0000ffffUL));

					}
					/*						
				else if(strstr (varr[0], "rot_t_"))
					{
					sscanf ((const char *)varr[1]+6, "%d",&web_param_input);
					lc640_write_int(EE_FORVARDBPSCHHOUR,(short)(web_param_input&0x0000ffffUL));

					}*/	
				else if(strstr (varr[0], "i_max_"))
					{
					sscanf ((const char *)varr[1]+6, "%d",&web_param_input);
					lc640_write_int(EE_IMAX,(short)(web_param_input&0x0000ffffUL));

					}
				else if(strstr (varr[0], "i_min_"))
					{
					sscanf ((const char *)varr[1]+6, "%d",&web_param_input);
					lc640_write_int(EE_IMIN,(short)(web_param_input&0x0000ffffUL));

					}
				else if(strstr (varr[0], "u_max_i_"))
					{
					sscanf ((const char *)varr[1]+6, "%d",&web_param_input);
					lc640_write_int(EE_UMAX,(short)(web_param_input&0x0000ffffUL));

					}
			/*	else if(strstr (varr[0], "u_min_i_"))
					{
					sscanf ((const char *)varr[1]+6, "%d",&web_param_input);
					DU=UB20-((short)(web_param_input&0x0000ffffUL));
					gran(&DU,50,UB20-100);
					lc640_write_int(EE_DU,DU);
					//lc640_write_int(EE_IMIN,(short)(web_param_input&0x0000ffffUL));
					} */
				else if(strstr (varr[0], "ub20_"))
					{
					sscanf ((const char *)varr[1]+6, "%d",&web_param_input);
					lc640_write_int(EE_UB20,(short)(web_param_input&0x0000ffffUL));
 					}
						
				else if(strstr (varr[0], "ub0_"))
					{
					sscanf ((const char *)varr[1]+6, "%d",&web_param_input);
					lc640_write_int(EE_UB0,(short)(web_param_input&0x0000ffffUL));
					}

				else if(strstr (varr[0], "usign_"))
					{
					sscanf ((const char *)varr[1]+6, "%d",&web_param_input);
					lc640_write_int(EE_USIGN,(short)(web_param_input&0x0000ffffUL));
					}
																																																
				else if(strstr (varr[0], "izmax_"))
					{
					sscanf ((const char *)varr[1]+6, "%d",&web_param_input);
					lc640_write_int(EE_IZMAX,(short)(web_param_input&0x0000ffffUL));
					}

			/*	else if(strstr (varr[0], "tbatsign_"))
					{
					sscanf ((const char *)varr[1]+6, "%d",&web_param_input);
					lc640_write_int(EE_TBATSIGN,(short)(web_param_input&0x0000ffffUL));
					}
					
				else if(strstr (varr[0], "tbatmax_"))
					{
					sscanf ((const char *)varr[1]+6, "%d",&web_param_input);
					lc640_write_int(EE_TBATMAX,(short)(web_param_input&0x0000ffffUL));
					}
					
				else if(strstr (varr[0], "uvz_"))
					{
					sscanf ((const char *)varr[1]+6, "%d",&web_param_input);
					lc640_write_int(EE_UVZ,(short)(web_param_input&0x0000ffffUL));
					} 
					
				else if(strstr (varr[0], "tvz_"))
					{
					sscanf ((const char *)varr[1]+6, "%d",&web_param_input);
					lc640_write_int(EE_VZ_HR,(short)(web_param_input&0x0000ffffUL));
					}
				else if(strstr (varr[0], "tsign_"))
					{
					sscanf ((const char *)varr[1]+6, "%d",&web_param_input);
					lc640_write_int(EE_TSIGN,(short)(web_param_input&0x0000ffffUL));
					} */	
					
				else if(strstr (varr[0], "tmax_"))
					{
					sscanf ((const char *)varr[1]+6, "%d",&web_param_input);
					lc640_write_int(EE_TMAX,(short)(web_param_input&0x0000ffffUL));
					}
			  /*
				else if(strstr (varr[0], "tzas_"))
					{
					sscanf ((const char *)varr[1]+6, "%d",&web_param_input);
					lc640_write_int(EE_TZAS,(short)(web_param_input&0x0000ffffUL));
					} */

				else if(strstr (varr[0], "apv1_on_"))
					{
					sscanf ((const char *)varr[1]+6, "%d",&web_param_input);
					if((web_param_input&0x0000ffffUL)==1)lc640_write_int(EE_APV_ON1,apvON);
					else lc640_write_int(EE_APV_ON1,apvOFF);
					}

				else if(strstr (varr[0], "apv2_on_"))
					{
					sscanf ((const char *)varr[1]+6, "%d",&web_param_input);
					if((web_param_input&0x0000ffffUL)==1)lc640_write_int(EE_APV_ON2,apvON);
					else lc640_write_int(EE_APV_ON2,apvOFF);
					}

				else if(strstr (varr[0], "apv2_time_"))
					{
					sscanf ((const char *)varr[1]+6, "%d",&web_param_input);
					lc640_write_int(EE_APV_ON2_TIME,(short)(web_param_input&0x0000ffffUL));
					} 
				
				else if(strstr (varr[0], "u0b_"))
					{
					sscanf ((const char *)varr[1]+6, "%d",&web_param_input);
					lc640_write_int(EE_U0B,(short)(web_param_input&0x0000ffffUL));
					}
				/*	
				else if(strstr (varr[0], "tbat_"))
					{
					sscanf ((const char *)varr[1]+6, "%d",&web_param_input);
					lc640_write_int(EE_TBAT,(short)(web_param_input&0x0000ffffUL));
					}
					
				else if(strstr (varr[0], "ikb_"))
					{
					sscanf ((const char *)varr[1]+6, "%d",&web_param_input);
					lc640_write_int(EE_IKB,(short)(web_param_input&0x0000ffffUL));
					}
					  */
				else if(strstr (varr[0], "year_"))
					{
					sscanf ((const char *)varr[1]+6, "%d",&web_param_input);
					LPC_RTC->YEAR=(short)(web_param_input&0x0000ffffUL);
					}

				else if(strstr (varr[0], "month_"))
					{
					sscanf ((const char *)varr[1]+6, "%d",&web_param_input);
					LPC_RTC->MONTH=(short)(web_param_input&0x0000ffffUL);
					}

				else if(strstr (varr[0], "day_"))
					{
					sscanf ((const char *)varr[1]+6, "%d",&web_param_input);
					LPC_RTC->DOM=(short)(web_param_input&0x0000ffffUL);
					}  

				else if(strstr (varr[0], "hour_"))
					{
					sscanf ((const char *)varr[1]+6, "%d",&web_param_input);
					LPC_RTC->HOUR=(short)(web_param_input&0x0000ffffUL);
					}
					
				else if(strstr (varr[0], "minut_"))
					{
					sscanf ((const char *)varr[1]+6, "%d",&web_param_input);
					LPC_RTC->MIN=(short)(web_param_input&0x0000ffffUL);
					}
				/*	
				else if(strstr (varr[0], "secund_"))
					{
					sscanf ((const char *)varr[1]+6, "%d",&web_param_input);
					LPC_RTC->SEC=(short)(web_param_input&0x0000ffffUL);
					}

				else if(strstr (varr[0], "sk1avsign_"))
					{
					sscanf ((const char *)varr[1]+6, "%d",&web_param_input);
					if((short)(web_param_input&0x000000ffUL))lc640_write_int(EE_SK_SIGN0,0);
					else lc640_write_int(EE_SK_SIGN0,0);
					}
				else if(strstr (varr[0], "sk2avsign_"))
					{
					sscanf ((const char *)varr[1]+6, "%d",&web_param_input);
					if((short)(web_param_input&0x000000ffUL))lc640_write_int(EE_SK_SIGN1,0);
					else lc640_write_int(EE_SK_SIGN1,0);
					}
				else if(strstr (varr[0], "sk3avsign_"))
					{
					sscanf ((const char *)varr[1]+6, "%d",&web_param_input);
					if((short)(web_param_input&0x000000ffUL))lc640_write_int(EE_SK_SIGN2,0);
					else lc640_write_int(EE_SK_SIGN2,0);
					} 
				else if(strstr (varr[0], "sk4avsign_"))
					{
					sscanf ((const char *)varr[1]+6, "%d",&web_param_input);
					if((short)(web_param_input&0x000000ffUL))lc640_write_int(EE_SK_SIGN3,0);
					else lc640_write_int(EE_SK_SIGN3,0);
					}
				else if(strstr (varr[0], "sk1lcden_"))
					{
					sscanf ((const char *)varr[1]+6, "%d",&web_param_input);
					if((short)(web_param_input&0x000000ffUL))lc640_write_int(EE_SK_LCD_EN0,0);
					else lc640_write_int(EE_SK_LCD_EN0,1);
					}
				else if(strstr (varr[0], "sk2lcden_"))
					{
					sscanf ((const char *)varr[1]+6, "%d",&web_param_input);
					if((short)(web_param_input&0x000000ffUL))lc640_write_int(EE_SK_LCD_EN1,0);
					else lc640_write_int(EE_SK_LCD_EN1,1);
					}
				else if(strstr (varr[0], "sk3lcden_"))
					{
					sscanf ((const char *)varr[1]+6, "%d",&web_param_input);
					if((short)(web_param_input&0x000000ffUL))lc640_write_int(EE_SK_LCD_EN2,0);
					else lc640_write_int(EE_SK_LCD_EN2,1);

					}
				else if(strstr (varr[0], "sk4lcden_"))
					{
					sscanf ((const char *)varr[1]+6, "%d",&web_param_input);
					if((short)(web_param_input&0x000000ffUL))lc640_write_int(EE_SK_LCD_EN3,0);
					else lc640_write_int(EE_SK_LCD_EN3,1);

					}
				else if(strstr (varr[0], "sk1zven_"))
					{
					sscanf ((const char *)varr[1]+6, "%d",&web_param_input);
					if((short)(web_param_input&0x000000ffUL))lc640_write_int(EE_SK_ZVUK_EN0,0);
					else lc640_write_int(EE_SK_ZVUK_EN0,1);
					}
				else if(strstr (varr[0], "sk2zven_"))
					{
					sscanf ((const char *)varr[1]+6, "%d",&web_param_input);
					if((short)(web_param_input&0x000000ffUL))lc640_write_int(EE_SK_ZVUK_EN1,0);
					else lc640_write_int(EE_SK_ZVUK_EN1,1);
					}
				else if(strstr (varr[0], "sk3zven_"))
					{
					sscanf ((const char *)varr[1]+6, "%d",&web_param_input);
					if((short)(web_param_input&0x000000ffUL))lc640_write_int(EE_SK_ZVUK_EN2,0);
					else lc640_write_int(EE_SK_ZVUK_EN2,1);
					}
				else if(strstr (varr[0], "sk4zven_"))
					{
					sscanf ((const char *)varr[1]+6, "%d",&web_param_input);
					if((short)(web_param_input&0x000000ffUL))lc640_write_int(EE_SK_ZVUK_EN3,0);
					else lc640_write_int(EE_SK_ZVUK_EN3,1);
					}*/	
				else if(strstr (varr[0], "sntp_gmt_"))
					{
					sscanf ((const char *)varr[1]+6, "%d",&web_param_input);
					lc640_write_int(EE_SNTP_GMT,(short)web_param_input);
					} 
				else if(strstr (varr[0], "sntp_exe_"))
					{
					time_sinc_hndl_req_cnt=5;
					Rem_IP[0]=SNTP_IP1;
					Rem_IP[1]=SNTP_IP2;
					Rem_IP[2]=SNTP_IP3;
					Rem_IP[3]=SNTP_IP4;
					sntp_requ();
					//sntp_plazma++;					
					}
				else if(strstr (varr[0], "sntp_src_"))
					{
					sscanf ((const char *)varr[1]+6, "%d",&web_param_input);
					if((short)(web_param_input&0x000000ffUL))lc640_write_int(EE_SNTP_WEB_ENABLE,0);
					else lc640_write_int(EE_SNTP_WEB_ENABLE,1);
					}
				else if(strstr (varr[0], "sntp_en_"))
					{
					sscanf ((const char *)varr[1]+6, "%d",&web_param_input);
					gran(&web_param_input,0,3);
					lc640_write_int(EE_SNTP_ENABLE,web_param_input);
					}
				else if(strstr (varr[0], "sntp_ip_"))
					{
					sscanf ((const char *)varr[1]+6, "%d",&web_param_input);
					lc640_write_int(EE_SNTP_IP4,(char)(web_param_input&0x000000ffUL));
					lc640_write_int(EE_SNTP_IP3,(char)((web_param_input&0x0000ff00UL)>>8));
					lc640_write_int(EE_SNTP_IP2,(char)((web_param_input&0x00ff0000UL)>>16));
					lc640_write_int(EE_SNTP_IP1,(char)((web_param_input&0xff000000UL)>>24));
					}	
				else if(strstr (varr[0], "numist_"))
					{
					sscanf ((const char *)varr[1]+6, "%d",&web_param_input);
					input_param=(short)web_param_input;
					gran(&input_param,1,2);
					lc640_write_int(EE_NUMIST,(short)input_param);
					} /*
				else if(strstr (varr[0], "numphase_"))
					{
					sscanf ((const char *)varr[1]+6, "%d",&web_param_input);
					if((short)(web_param_input&0x000000ffUL))lc640_write_int(EE_NUMPHASE,3);
					else lc640_write_int(EE_NUMPHASE,1);
					} */
				else if(strstr (varr[0], "numsk_"))
					{
					sscanf ((const char *)varr[1]+6, "%d",&web_param_input);
					input_param=(short)web_param_input;
					gran(&input_param,0,4);
					lc640_write_int(EE_NUMSK,input_param);
					}  
				else if(strstr (varr[0], "numdt_"))
					{
					sscanf ((const char *)varr[1]+6, "%d",&web_param_input);
					input_param=(short)web_param_input;
					gran(&input_param,0,1);
					lc640_write_int(EE_NUMDT,input_param);
					}  /*
				else if(strstr (varr[0], "nummakb_"))
					{
					sscanf ((const char *)varr[1]+6, "%d",&web_param_input);
					lc640_write_int(EE_NUMMAKB,(short)web_param_input);
					}
				else if(strstr (varr[0], "numenmv_"))
					{
					sscanf ((const char *)varr[1]+6, "%d",&web_param_input);
					lc640_write_int(EE_NUMENMV,(short)web_param_input);
					}*/																																																																																																																																																																																																	
				}

			//if(strstr(var, "pl"))web_plazma[1]++;
			//else if(strstr(var, "mi"))web_plazma[1]--;
			}
      else if (str_scomp (var, "but=but2") == __TRUE) {
        //web_plazma[4]=2;
      }
      else if (str_scomp (var, "but=but3") == __TRUE) {
        //web_plazma[4]=3;
      }
      else if (str_scomp (var, "pw=") == __TRUE) {
        /* Change password. */
        str_copy (passw, var+3);
        stpassw |= 1;
      }
      else if (str_scomp (var, "pw2=") == __TRUE) {
        /* Retyped password. */
        str_copy (retyped, var+4);
        stpassw |= 2;
      }
      else if (str_scomp (var, "lcd1=") == __TRUE) {
        /* LCD Module Line 1 text. */
        /////str_copy (lcd_text[0], var+5);
        /////LCDupdate = __TRUE;
      }
      else if (str_scomp (var, "lcd2=") == __TRUE) {
        /* LCD Module Line 2 text. */
        /////str_copy (lcd_text[1], var+5);
        /////LCDupdate = __TRUE;
			}
		}

free_mem ((OS_FRAME *)var);
free_mem ((OS_FRAME *)varr[0]);
free_mem ((OS_FRAME *)varr[1]);
free_mem ((OS_FRAME *)varr[2]);
  /////LED_out (P2);

  if (stpassw == 0x03) {
    len = strlen ((const char *)passw);
    if (mem_comp (passw, retyped, len) == __TRUE) {
      /* OK, both entered passwords the same, change it. */
      str_copy (http_auth_passw, passw);
    }
  }
}



/*--------------------------- cgi_func --------------------------------------*/

U16 cgi_func (U8 *env, U8 *buf, U16 buflen, U32 *pcgi) {
  /* This function is called by HTTP server script interpreter to make a    */
  /* formated output for 'stdout'. It returns the number of bytes written   */
  /* to the output buffer. Hi-bit of return value (len is or-ed with 0x8000)*/
  /* is a repeat flag for the system script interpreter. If this bit is set */
  /* to 1, the system will call the 'cgi_func()' again for the same script  */
  /* line with parameter 'pcgi' pointing to a 4-byte buffer. This buffer    */
  /* can be used for storing different status variables for this function.  */
  /* It is set to 0 by HTTP Server on first call and is not altered by      */
  /* HTTP server for repeated calls. This function should NEVER write more  */
  /* than 'buflen' bytes to the buffer.                                     */
  /* Parameters:                                                            */
  /*   env    - environment variable string                                 */
  /*   buf    - HTTP transmit buffer                                        */
  /*   buflen - length of this buffer (500-1400 bytes - depends on MSS)     */
  /*   pcgi   - pointer to session local buffer used for repeated loops     */
  /*            This is a U32 variable - size is 4 bytes. Value is:         */
  /*            - on 1st call = 0                                           */
  /*            - 2nd call    = as set by this function on first call       */
  TCP_INFO *tsoc;
  U32 len = 0;
  U8 id, *lang;
  static U32 adv;
  char temp;
  switch (env[0]) {
    /* Analyze the environment string. It is the script 'c' line starting */
    /* at position 2. What you write to the script file is returned here. */

	case 'a':
		// Аварии
		switch (env[1]) {
			case '0':
				switch (env[2]) {
					case '1':
						len = sprintf((char *)buf,(const char *)&env[4],"normal");
						break;
			        case '2':
			          	len = sprintf((char *)buf,(const char *)&env[4],pal_cyr_coder("Авария БПС №1"));
						break;
				}
		  		break;
		}
		break;

    case 'b':
		//Батареи
		switch (env[1]) {
			case '1':
				len = sprintf((char *)buf,(const char *)&env[3],http_tm_bat_output(0));
		  		break;
			case '2':
				len = sprintf((char *)buf,(const char *)&env[3],http_tm_bat_output(1));
		  		break;
		}
		break;

    case 'c':
      /* TCP status - file 'tcp.cgi' */
      while ((len + 150) < buflen) {
        tsoc = &tcp_socket[MYBUF(pcgi)->xcnt];
        MYBUF(pcgi)->xcnt++;
        /* 'sprintf' format string is defined here. */
        len += sprintf((char *)(buf+len),"<tr align=\"center\">");
        if (tsoc->State <= TCP_STATE_CLOSED) {
          len += sprintf ((char *)(buf+len),
                          "<td>%d</td><td>%s</td><td>-</td><td>-</td>"
                          "<td>-</td><td>-</td></tr>\r\n",
                          MYBUF(pcgi)->xcnt,state[tsoc->State]);
        }
        else if (tsoc->State == TCP_STATE_LISTEN) {
          len += sprintf ((char *)(buf+len),
                          "<td>%d</td><td>%s</td><td>-</td><td>-</td>"
                          "<td>%d</td><td>-</td></tr>\r\n",
                          MYBUF(pcgi)->xcnt,state[tsoc->State],tsoc->LocPort);
        }
        else {
          len += sprintf ((char *)(buf+len),
                          "<td>%d</td><td>%s</td><td>%d.%d.%d.%d</td>"
                          "<td>%d</td><td>%d</td><td>%d</td></tr>\r\n",
                          MYBUF(pcgi)->xcnt,state[tsoc->State],
                          tsoc->RemIpAdr[0],tsoc->RemIpAdr[1],
                          tsoc->RemIpAdr[2],tsoc->RemIpAdr[3],
                          tsoc->RemPort,tsoc->LocPort,tsoc->AliveTimer);
        }
        /* Repeat for all TCP Sockets. */
        if (MYBUF(pcgi)->xcnt == tcp_NumSocks) {
          break;
        }
      }
      if (MYBUF(pcgi)->xcnt < tcp_NumSocks) {
        /* Hi bit is a repeat flag. */
        len |= 0x8000;
      }
      break;

	case 'd':
		// Описание системы
		
		switch (env[1]) {
			case '0':
				
				switch (env[2]) {
					case '1':
						if(AUSW_MAIN==6006)			len = sprintf((char *)buf,(const char *)&env[4],pal_cyr_coder("ИБЭП220/60В-6А-1U"));
						else if(AUSW_MAIN==6012)	len = sprintf((char *)buf,(const char *)&env[4],pal_cyr_coder("ИБЭП220/60В-12А-1U"));
						else if(AUSW_MAIN==4806)	len = sprintf((char *)buf,(const char *)&env[4],pal_cyr_coder("ИБЭП220/48В-6А-1U"));
						else if(AUSW_MAIN==4812)	len = sprintf((char *)buf,(const char *)&env[4],pal_cyr_coder("ИБЭП220/48В-12А-1U"));
						else 						len = sprintf((char *)buf,(const char *)&env[4],pal_cyr_coder("модель неопределена"));
					break;
			        case '2':
			          	len = sprintf((char *)buf,(const char *)&env[4],AUSW_MAIN_NUMBER);
					break;
			        case '3':
			          	len = sprintf((char *)buf,(const char *)&env[4],pal_cyr_coder(place_holder));
					break;
			        case '4':  	//количество батарей
			          	len = sprintf((char *)buf,(const char *)&env[4],NUMBAT);
					break;
			        case '5':	//количество источников
						len = sprintf((char *)buf,(const char *)&env[4],NUMIST);
					break;
			        case '6':	//количество инверторов
			          	len = sprintf((char *)buf,(const char *)&env[4],0);
					break;
			        case '7': 	//количество байпасов
			          	len = sprintf((char *)buf,(const char *)&env[4],0);
					break;
			        case '8': 	//количество сухих контактов
			          	len = sprintf((char *)buf,(const char *)&env[4],0);
					break;
			        case '9': 	//количество внешних датчиков температуры
			          	len = sprintf((char *)buf,(const char *)&env[4],0);
					break;
				}
		  	break;
		}
	break;

    case 'e':
      /* Browser Language - file 'language.cgi' */
      lang = http_get_lang();
      if (strcmp ((const char *)lang, "en") == 0) {
        lang = "English";
      }
      else if (strcmp ((const char *)lang, "en-us") == 0) {
        lang = "English USA";
      }
      else if (strcmp ((const char *)lang, "en-gb") == 0) {
        lang = "English GB";
      }
      else if (strcmp ((const char *)lang, "de") == 0) {
        lang = "German";
      }
      else if (strcmp ((const char *)lang, "de-ch") == 0) {
        lang = "German CH";
      }
      else if (strcmp ((const char *)lang, "de-at") == 0) {
        lang = "German AT";
      }
      else if (strcmp ((const char *)lang, "fr") == 0) {
        lang = "French";
      }
      else if (strcmp ((const char *)lang, "sl") == 0) {
        lang = "Slovene";
      }
      else {
        lang = "Unknown";
      }
      len = sprintf((char *)buf,(const char *)&env[2],lang,http_get_lang());
      break;

    case 'f':
      /* LCD Module control - file 'lcd.cgi' */
      switch (env[2]) {
        case '1':
          /////len = sprintf((char *)buf,(const char *)&env[4],lcd_text[0]);
          break;
        case '2':
          /////len = sprintf((char *)buf,(const char *)&env[4],lcd_text[1]);
          break;
      }
      break;

	case 'g':
      /* AD Input - file 'ad.cgi' */
		switch (env[2]) {
        case '1':
          //adv = web_cnt_main;
          len = sprintf((char *)buf,(const char *)&env[4],adv);
          break;
        case '2':
          len = sprintf((char *)buf,(const char *)&env[4],(float)adv*3.3/1024);
          break;
        case '3':
          adv = (adv * 100) / 1024;
          len = sprintf((char *)buf,(const char *)&env[4],adv);
          break;
      }
      break;

    case 'k':
		//сухие контакты
      	switch (env[1]) {
        	case '1':
          		len = sprintf((char *)buf,(const char *)&env[3],http_tm_sk_output(0));
          		break;
     		case '2':
          		len = sprintf((char *)buf,(const char *)&env[3],http_tm_sk_output(1));
          		break;
         	case '3':
          		len = sprintf((char *)buf,(const char *)&env[3],http_tm_sk_output(2));
          		break;
     		case '4':
          		len = sprintf((char *)buf,(const char *)&env[3],http_tm_sk_output(3));
          		break;
		}
		break;

    case 'm':
      	switch (env[1]) {
        	case '1':
          		len = sprintf((char *)buf,(const char *)&env[3],web_cnt_2hz);
          		break;
     		case '2':
          		len = sprintf((char *)buf,(const char *)&env[3],web_cnt_main);
          		break;
     		case '3':
				if(psw_err)	len = sprintf((char *)buf,(const char *)&env[3],"error");
				else 	   	len = sprintf((char *)buf,(const char *)&env[3],"good");
          		break;
		}
		break;

    case 'p':
		//Первичное питание, сеть
      	switch (env[1]) {
        	case 'n':
          		len = sprintf((char *)buf,(const char *)&env[3],http_power_num_of_phases);
          		break;
     		case 'A':
          		len = sprintf((char *)buf,(const char *)&env[3],http_power_voltage_of_phase[0]);
          		break;
     		case 'B':
          		len = sprintf((char *)buf,(const char *)&env[3],http_power_voltage_of_phase[1]);
          		break;       		
     		case 'C':
          		len = sprintf((char *)buf,(const char *)&env[3],http_power_voltage_of_phase[2]);
          		break;
			case 'F':
          		len = sprintf((char *)buf,(const char *)&env[3],http_power_frequncy);
          		break;
     		case 'S':
				len = sprintf((char *)buf,(const char *)&env[3],http_power_status);
          		break;		}
		break;

    case 't':
		//Датчики температуры
		switch (env[1]) {
        	case '1':
          		len = sprintf((char *)buf,(const char *)&env[3],http_tm_dt_output(0));
          		break;
     		case '2':
          		len = sprintf((char *)buf,(const char *)&env[3],http_tm_dt_output(1));
          		break;
         	case '3':
          		len = sprintf((char *)buf,(const char *)&env[3],http_tm_dt_output(2));
          		break;
 		}
		break;

	case 'l':
		// журнал
		switch (env[1]) {
			case 'd':
          		len = sprintf((char *)buf,(const char *)&env[3],http_get_log_deep());
          		break;
			case 'n':
          		len = sprintf((char *)buf,(const char *)&env[3],log_item_cnt);
				uku_set_autorized=0;
				break;
			case '0':
				len = sprintf((char *)buf,(const char *)&env[3],http_get_log_rec(log_item_cnt));
				break;
			case 'e':
				len = sprintf((char *)buf,(const char *)&env[3],pal_cyr_coder("end"));
				if(++log_item_cnt>=http_get_log_deep())log_item_cnt=0;
				break;
		}
		
		break;

    case 'o':
		//нагрузка
		switch (env[1]) {
			case 'u':
				len = sprintf((char *)buf,(const char *)&env[3],Uload);
		  		break;
			case 'i':
				len = sprintf((char *)buf,(const char *)&env[3],iload);
		  		break;
		}
		break;

    case 's':
		/* телеметрия источников */
      	switch (env[1]) {
        	case '0':
          		switch (env[2]) {
		        	case '0':
		          		len = sprintf((char *)buf,(const char *)&env[4],http_tm_src_output(0));
		          		break;
		     		case '1':
		          		len = sprintf((char *)buf,(const char *)&env[4],http_tm_src_output(1));
		          		break;
		     		case '2':
		          		len = sprintf((char *)buf,(const char *)&env[4],http_tm_src_output(2));
		          		break;
		     		case '3':
		          		len = sprintf((char *)buf,(const char *)&env[4],http_tm_src_output(3));
		          		break;
		     		case '4':
		          		len = sprintf((char *)buf,(const char *)&env[4],http_tm_src_output(4));
		          		break;
		        	case '5':
		          		len = sprintf((char *)buf,(const char *)&env[4],http_tm_src_output(5));
		          		break;
		     		case '6':
		          		len = sprintf((char *)buf,(const char *)&env[4],http_tm_src_output(6));
		          		break;
		     		case '7':
		          		len = sprintf((char *)buf,(const char *)&env[4],http_tm_src_output(7));
		          		break;
		     		case '8':
		          		len = sprintf((char *)buf,(const char *)&env[4],http_tm_src_output(8));
		          		break;
		     		case '9':
		          		len = sprintf((char *)buf,(const char *)&env[4],http_tm_src_output(9));
		          		break;
				}
				break;
        	case '1':
          		switch (env[2]) {
		        	case '0':
		          		len = sprintf((char *)buf,(const char *)&env[4],http_tm_src_output(10));
		          		break;
		     		case '1':
		          		len = sprintf((char *)buf,(const char *)&env[4],http_tm_src_output(11));
		          		break;
		     		case '2':
		          		len = sprintf((char *)buf,(const char *)&env[4],http_tm_src_output(12));
		          		break;
		     		case '3':
		          		len = sprintf((char *)buf,(const char *)&env[4],http_tm_src_output(13));
		          		break;
		     		case '4':
		          		len = sprintf((char *)buf,(const char *)&env[4],http_tm_src_output(14));
		          		break;
		        	case '5':
		          		len = sprintf((char *)buf,(const char *)&env[4],http_tm_src_output(15));
		          		break;
		     		case '6':
		          		len = sprintf((char *)buf,(const char *)&env[4],http_tm_src_output(16));
		          		break;
		     		case '7':
		          		len = sprintf((char *)buf,(const char *)&env[4],http_tm_src_output(17));
		          		break;
		     		case '8':
		          		len = sprintf((char *)buf,(const char *)&env[4],http_tm_src_output(18));
		          		break;
		     		case '9':
		          		len = sprintf((char *)buf,(const char *)&env[4],http_tm_src_output(19));
		          		break;
				}
				break;
		}
		break;

    case 'w':
		/* телеметрия блока дополнительных входов */


      	switch (env[1]) {
		    case 'n':
          		len = sprintf((char *)buf,(const char *)&env[3],64);
          		break;
        	case '0':
          		switch (env[2]) {
/*		        	case '0':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[0]);
		          		break;
		     		case '1':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[1]);
		          		break;
		     		case '2':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[2]);
		          		break;
		     		case '3':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[3]);
		          		break;
		     		case '4':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[4]);
		          		break;
		        	case '5':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[5]);
		          		break;
		     		case '6':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[6]);
		          		break;
		     		case '7':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[7]);
		          		break;
		     		case '8':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[8]);
		          		break;
		     		case '9':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[9]);
		          		break;	*/
				}
				break;
        	case '1':
          		switch (env[2]) {
/*		        	case '0':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[10]);
		          		break;
		     		case '1':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[11]);
		          		break;
		     		case '2':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[12]);
		          		break;
		     		case '3':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[13]);
		          		break;
		     		case '4':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[14]);
		          		break;
		        	case '5':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[15]);
		          		break;
		     		case '6':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[16]);
		          		break;
		     		case '7':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[17]);
		          		break;
		     		case '8':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[18]);
		          		break;
		     		case '9':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[19]);
		          		break;	*/
				}
				break;
        	case '2':
          		switch (env[2]) {
/*		        	case '0':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[20]);
		          		break;
		     		case '1':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[21]);
		          		break;
		     		case '2':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[22]);
		          		break;
		     		case '3':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[23]);
		          		break;
		     		case '4':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[24]);
		          		break;
		        	case '5':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[25]);
		          		break;
		     		case '6':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[26]);
		          		break;
		     		case '7':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[27]);
		          		break;
		     		case '8':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[28]);
		          		break;
		     		case '9':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[29]);
		          		break;	*/
				}
				break;
        	case '3':
          		switch (env[2]) {
/*		        	case '0':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[30]);
		          		break;
		     		case '1':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[31]);
		          		break;
		     		case '2':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[32]);
		          		break;
		     		case '3':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[33]);
		          		break;
		     		case '4':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[34]);
		          		break;
		        	case '5':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[35]);
		          		break;
		     		case '6':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[36]);
		          		break;
		     		case '7':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[37]);
		          		break;
		     		case '8':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[38]);
		          		break;
		     		case '9':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[39]);
		          		break; */
				}
				break;
        	case '4':
          		switch (env[2]) {
/*		        	case '0':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[40]);
		          		break;
		     		case '1':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[41]);
		          		break;
		     		case '2':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[42]);
		          		break;
		     		case '3':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[43]);
		          		break;
		     		case '4':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[44]);
		          		break;
		        	case '5':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[45]);
		          		break;
		     		case '6':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[46]);
		          		break;
		     		case '7':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[47]);
		          		break;
		     		case '8':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[48]);
		          		break;
		     		case '9':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[49]);
		          		break; */
				}
				break;
        	case '5':
          		switch (env[2]) {
/*		        	case '0':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[50]);
		          		break;
		     		case '1':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[51]);
		          		break;
		     		case '2':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[52]);
		          		break;
		     		case '3':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[53]);
		          		break;
		     		case '4':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[54]);
		          		break;
		        	case '5':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[55]);
		          		break;
		     		case '6':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[56]);
		          		break;
		     		case '7':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[57]);
		          		break;
		     		case '8':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[58]);
		          		break;
		     		case '9':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[59]);
		          		break; */
				}
				break;
        	case '6':
          		switch (env[2]) {
/*		        	case '0':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[60]);
		          		break;
		     		case '1':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[61]);
		          		break;
		     		case '2':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[62]);
		          		break;
		     		case '3':
		          		len = sprintf((char *)buf,(const char *)&env[4],snmp_enmv_data[63]);
		          		break; */
				}
				break;

		}
		break;

    case 'y':
		/* меню установок */
      	switch (env[1]) {
        	case 'n':
          		len = sprintf((char *)buf,(const char *)&env[3],38);
          		break;
        	case '0':
          		switch (env[2]) {
		        	case '1':
		          		len = sprintf((char *)buf,(const char *)&env[4],AUSW_MAIN_NUMBER," ");
		          		break;
		     		case '2':
		          		len = sprintf((char *)buf,(const char *)&env[4],0,pal_cyr_coder(place_holder));
		          		break;
		     		case '3':
		          		len = sprintf((char *)buf,(const char *)&env[4],(ZV_ON==1)?1:0," ");
		          		break;
		     		case '4':
		          		len = sprintf((char *)buf,(const char *)&env[4],(MAIN_BPS==1)?2:1," ");
		          		break;
		     		case '5':
		          		len = sprintf((char *)buf,(const char *)&env[4],(AV_OFF_AVT==1)?1:0," ");
		          		break;
		     		case '6':
		         		len = sprintf((char *)buf,(const char *)&env[4],MNEMO_TIME,(MNEMO_ON==mnON)?"on":"off");
		          		break;
		     		case '7':
		         		len = sprintf((char *)buf,(const char *)&env[4],UMN," ");
		          		break;
		     		case '8':
		         		len = sprintf((char *)buf,(const char *)&env[4],UMAXN," ");
		          		break;		     		
					case '9':
		         		len = sprintf((char *)buf,(const char *)&env[4],UMN," ");
		          		break;																				   
				}
				break;
        	case '1':
          		switch (env[2]) {
					case '0':
		         		len = sprintf((char *)buf,(const char *)&env[4],IMAX," ");
		          		break;																				   
					case '1':
		          		len = sprintf((char *)buf,(const char *)&env[4],IMIN," ");
		          		break;
		     		case '2':
		          		len = sprintf((char *)buf,(const char *)&env[4],UMAX," ");
		          		break;
/*		     		case '3':
		          		len = sprintf((char *)buf,(const char *)&env[4],UB20-DU," ");
		          		break; */
		     		case '4':
		          		len = sprintf((char *)buf,(const char *)&env[4],UB20," ");
		          		break;
		     		case '5':
		          		len = sprintf((char *)buf,(const char *)&env[4],UB0," ");
		          		break;
		     		case '6':
		         		len = sprintf((char *)buf,(const char *)&env[4],USIGN," ");
		          		break;
		     		case '7':
		         		len = sprintf((char *)buf,(const char *)&env[4],IZMAX," ");
		          		break;
/*		     		case '8':
		         		len = sprintf((char *)buf,(const char *)&env[4],TBATSIGN," ");
		          		break;		     		
					case '9':
		         		len = sprintf((char *)buf,(const char *)&env[4],TBATMAX," ");
		          		break;*/																				   
				}
				break;
        	case '2':
          		switch (env[2]) {
/*					case '0':
		         		len = sprintf((char *)buf,(const char *)&env[4],UVZ," ");
		          		break;																				   
					case '1':
		          		len = sprintf((char *)buf,(const char *)&env[4],VZ_HR," ");
		          		break; 
		     		case '2':
		          		len = sprintf((char *)buf,(const char *)&env[4],TSIGN," ");
		          		break;	*/
		     		case '3':
		          		len = sprintf((char *)buf,(const char *)&env[4],TMAX," ");
		          		break;
		     	/*	case '4':
		          		len = sprintf((char *)buf,(const char *)&env[4],TZAS," ");
		          		break; */
		     		case '5':
		          		len = sprintf((char *)buf,(const char *)&env[4],(apv_on1==apvON)?1:0," ");
		          		break;
		     		case '6':
		         		len = sprintf((char *)buf,(const char *)&env[4],(apv_on2==apvON)?1:0," ");
		          		break;
		     		case '7':
		         		len = sprintf((char *)buf,(const char *)&env[4],apv_on2_time," ");
		          		break;
		     		case '8':
		         		len = sprintf((char *)buf,(const char *)&env[4],U0B," ");
		          		break;		     		
				/*	case '9':
		         		len = sprintf((char *)buf,(const char *)&env[4],TBAT," ");
		          		break;	*/																			   
				}
				break;
       		case '3':
          		switch (env[2]) {
					case '0':
		         		len = sprintf((char *)buf,(const char *)&env[4],IKB," ");
		          		break;																				   
					case '1':
		          		len = sprintf((char *)buf,(const char *)&env[4],LPC_RTC->YEAR," ");
		          		break;
		     		case '2':
		          		len = sprintf((char *)buf,(const char *)&env[4],LPC_RTC->MONTH," ");
		          		break;
		     		case '3':
		          		len = sprintf((char *)buf,(const char *)&env[4],LPC_RTC->DOM," ");
		          		break;
		     		case '4':
		          		len = sprintf((char *)buf,(const char *)&env[4],LPC_RTC->HOUR," ");
		          		break;
		     		case '5':
		          		len = sprintf((char *)buf,(const char *)&env[4],LPC_RTC->MIN," ");
		          		break;
		     		case '6':
						if		(SNTP_ENABLE==0)	len = sprintf((char *)buf,(const char *)&env[4],SNTP_ENABLE,pal_cyr_coder("Выключено"));
						else if	(SNTP_ENABLE==1)	len = sprintf((char *)buf,(const char *)&env[4],SNTP_ENABLE,pal_cyr_coder("Период 1 час"));
						else if	(SNTP_ENABLE==2)	len = sprintf((char *)buf,(const char *)&env[4],SNTP_ENABLE,pal_cyr_coder("Период 1 сутки"));
						else if	(SNTP_ENABLE==3)	len = sprintf((char *)buf,(const char *)&env[4],SNTP_ENABLE,pal_cyr_coder("Период 1 неделя"));
		         		else 						len = sprintf((char *)buf,(const char *)&env[4],SNTP_ENABLE,pal_cyr_coder("ВЫКЛ."));
		          		break;
/*		     		case '7':
						if(AVZ==AVZ_1)temp=1;
						else if(AVZ==AVZ_2)temp=2;
						else if(AVZ==AVZ_3)temp=3; 
						else if(AVZ==AVZ_6)temp=4;
						else if(AVZ==AVZ_12)temp=5;			
						else temp=0;
						len = sprintf((char *)buf,(const char *)&env[4],temp," ");
		          		break;
		     		case '8':
		         		if(sk_stat[0]==ssON)temp=1;
						else temp=0;
						len = sprintf((char *)buf,(const char *)&env[4],temp," ");
		          		break;		     		
					case '9':
		         		if(SK_SIGN[0]==1)temp=0;
						else temp=1;
						len = sprintf((char *)buf,(const char *)&env[4],temp," ");
		          		break;	*/																			   
				}
				break;
       		case '4':
          		switch (env[2]) {
		/*			case '0':
		         		if(sk_av_stat[0]==sasON)temp=1;
						else temp=0;
						len = sprintf((char *)buf,(const char *)&env[4],temp," ");
		          		break;																				   
		     		case '1':
		         		if(sk_stat[1]==ssON)temp=1;
						else temp=0;
						len = sprintf((char *)buf,(const char *)&env[4],temp," ");
		          		break;		     		
					case '2':
		         		if(SK_SIGN[1]==1)temp=0;
						else temp=1;
						len = sprintf((char *)buf,(const char *)&env[4],temp," ");
		          		break;																				   
					case '3':
		         		if(sk_av_stat[1]==sasON)temp=1;
						else temp=0;
						len = sprintf((char *)buf,(const char *)&env[4],temp," ");
		          		break;																				   
		     		case '4':
		         		if(sk_stat[2]==ssON)temp=1;
						else temp=0;
						len = sprintf((char *)buf,(const char *)&env[4],temp," ");
		          		break;		     		
					case '5':
		         		if(SK_SIGN[2]==1)temp=0;
						else temp=1;
						len = sprintf((char *)buf,(const char *)&env[4],temp," ");
		          		break;																				   
					case '6':
		         		if(sk_av_stat[2]==sasON)temp=1;
						else temp=0;
						len = sprintf((char *)buf,(const char *)&env[4],temp," ");
		          		break;																				   
		     		case '7':
		         		if(sk_stat[3]==ssON)temp=1;
						else temp=0;
						len = sprintf((char *)buf,(const char *)&env[4],temp," ");
		          		break;		     		
					case '8':
		         		if(SK_SIGN[3]==1)temp=0;
						else temp=1;
						len = sprintf((char *)buf,(const char *)&env[4],temp," ");
		          		break;																				   
					case '9':
		         		if(sk_av_stat[3]==sasON)temp=1;
						else temp=0;
						len = sprintf((char *)buf,(const char *)&env[4],temp," ");
		          		break;	*/																			   
				}
				break;
       		case '5':
          		switch (env[2]) {
/*					case '0':
		         		if(!SK_LCD_EN[0])temp=1;
						else temp=0;
						len = sprintf((char *)buf,(const char *)&env[4],temp," ");
						break;																				   
					case '1':
		         		if(!SK_LCD_EN[1])temp=1;
						else temp=0;
						len = sprintf((char *)buf,(const char *)&env[4],temp," ");
		          		break;
		     		case '2':
		         		if(!SK_LCD_EN[2])temp=1;
						else temp=0;
						len = sprintf((char *)buf,(const char *)&env[4],temp," ");
		          		break;
		     		case '3':
		         		if(!SK_LCD_EN[3])temp=1;
						else temp=0;
						len = sprintf((char *)buf,(const char *)&env[4],temp," ");
		          		break;
					case '4':
		         		if(!SK_ZVUK_EN[0])temp=1;
						else temp=0;
						len = sprintf((char *)buf,(const char *)&env[4],temp," ");
						break;																				   
					case '5':
		         		if(!SK_ZVUK_EN[1])temp=1;
						else temp=0;
						len = sprintf((char *)buf,(const char *)&env[4],temp," ");
		          		break;
		     		case '6':
		         		if(!SK_ZVUK_EN[2])temp=1;
						else temp=0;
						len = sprintf((char *)buf,(const char *)&env[4],temp," ");
		          		break;
		     		case '7':
		         		if(!SK_ZVUK_EN[3])temp=1;
						else temp=0;
						len = sprintf((char *)buf,(const char *)&env[4],temp," ");
		          		break;*/
		     		case '8':
						len = sprintf((char *)buf,(const char *)&env[4],(signed)SNTP_GMT," ");
		          		break;		     		
					case '9':
		         		if(lc640_read_int(EE_SNTP_WEB_ENABLE)==1)temp=0;
						else temp=1;
						len = sprintf((char *)buf,(const char *)&env[4],temp," ");
		          		break;																				   
				}
				break;

       		case '6':
          		switch (env[2]) {
					case '0':
		         		len = sprintf((char *)buf,(const char *)&env[4],http_ip_output(	(char)lc640_read_int(EE_SNTP_IP1),
																						(char)lc640_read_int(EE_SNTP_IP2),
																						(char)lc640_read_int(EE_SNTP_IP3),
																						(char)lc640_read_int(EE_SNTP_IP4))); 
						break;																				   
					case '1':
						len = sprintf((char *)buf,(const char *)&env[4],NUMIST," ");
		          		break;
		     		case '2':
					/*	if(NUMPHASE==1)temp=0;
						else temp=1;
						len = sprintf((char *)buf,(const char *)&env[4],temp," "); */
		          		break;
		     		case '3':
						len = sprintf((char *)buf,(const char *)&env[4],NUMSK," ");
		          		break;
					case '4':
						len = sprintf((char *)buf,(const char *)&env[4],NUMDT," ");
						break;																				   
					case '5':
					//	len = sprintf((char *)buf,(const char *)&env[4],NUMMAKB," ");
		          		break;
		     		case '6':
					//	len = sprintf((char *)buf,(const char *)&env[4],NUMENMV," ");
		          		break;
		     		case '7':
		         		if(!SK_ZVUK_EN[3])temp=1;
						else temp=0;
						len = sprintf((char *)buf,(const char *)&env[4],temp," ");
		          		break;
		     		case '8':
					/*	len = sprintf((char *)buf,(const char *)&env[4],(signed)SNTP_GMT," ");*/
		          		break;		     		
					case '9':
		         		if(lc640_read_int(EE_SNTP_WEB_ENABLE)==1)temp=0;
						else temp=1;
						len = sprintf((char *)buf,(const char *)&env[4],temp," "); 
		          		break;																				   
				}
				break;
		}
		break;

    case 'x':
      if(uku_set_autorized)	len = sprintf((char *)buf,(const char *)&env[1],"ON");
	  else if(psw_err)		len = sprintf((char *)buf,(const char *)&env[1],"DENIED");
	  else 					len = sprintf((char *)buf,(const char *)&env[1],"OFF");
      break;

    case '2':
      /* Button state - xml file 'button.cgx' */
	  //web_plazma[0]++;
      len = sprintf((char *)buf,"<checkbox><id>button%c</id><on>%s</on></checkbox>",
                    env[1],(/*web_cnt_main*/3 & (1 << (env[1]-'0'))) ? "true" : "false");
      break;
    case '3':
      /* Button state - xml file 'button.cgx' */
	  //web_plazma[0]++;
      len = sprintf((char *)buf,"<checkbox><id>button%c</id><on>%s</on></checkbox>",
                    env[1],(/*web_cnt_main*/ 6 & (1 << (env[1]-'0'))) ? "true" : "false");
      break;
    case 'z':
		/* конец файла */
      	len = sprintf((char *)buf,(const char *)&env[2],"end");
		uku_set_autorized=0;
   		break;
  }
  return ((U16)len);
}


/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
