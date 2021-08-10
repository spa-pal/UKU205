#include "memo.h"
#include "25lc640.h"
#include "LPC17xx.H"
#include "main.h"
#include "eeprom_map.h"
#include "snmp_data_file.h"

//-----------------------------------------------
void memo_read (void)
{

char i;

Ktsrc[0]=lc640_read_int(KTSRC1);

Ktsrc[1]=lc640_read_int(KTSRC2);
Kusrc[0]=lc640_read_int(KUSRC1);
Kusrc[1]=lc640_read_int(KUSRC2);
Kisrc[0]=lc640_read_int(KISRC1);
Kisrc[1]=lc640_read_int(KISRC2);
Ki0src[0]=lc640_read_int(KI0SRC1);
Ki0src[1]=lc640_read_int(KI0SRC2);

Kubat=lc640_read_int(KUBAT);
Kibat0=lc640_read_int(EE_KIBAT0);
Kibat1=lc640_read_int(KIBAT1);
Ktbat=lc640_read_int(KTBAT);
Kunet=lc640_read_int(KUNET);
Kubat_part[0]=lc640_read_int(EE_KUBAT_PART0);
Kubat_part[1]=lc640_read_int(EE_KUBAT_PART1);
Kubat_part[2]=lc640_read_int(EE_KUBAT_PART2);
Kubat_part[3]=lc640_read_int(EE_KUBAT_PART3);

Ktout[0]=lc640_read_int(KTOUT0);
Ktout[1]=lc640_read_int(KTOUT1);
Ktout[2]=lc640_read_int(KTOUT2);
	
MAIN_BPS=lc640_read_int(EE_MAIN_BPS);
UMAX=lc640_read_int(EE_UMAX);
UB0=lc640_read_int(EE_UB0);
UB20=lc640_read_int(EE_UB20);
TMAX=lc640_read_int(EE_TMAX);
//C_BAT=lc640_read_int(EE_C_BAT);
USIGN=lc640_read_int(EE_USIGN);
UMN=lc640_read_int(EE_UMN);
ZV_ON=lc640_read_int(EE_ZV_ON);
IKB=lc640_read_int(EE_IKB);
KVZ=lc640_read_int(EE_KVZ);
IMAX=lc640_read_int(EE_IMAX);
KIMAX=lc640_read_int(EE_KIMAX);
IMIN=lc640_read_int(EE_IMIN);
APV_ON=lc640_read_int(EE_APV_ON);
IZMAX=lc640_read_int(EE_IZMAX);
U0B=lc640_read_int(EE_U0B);	 
TZAS=lc640_read_int(EE_TZAS);
NUMIST=lc640_read_int(EE_NUMIST);
AV_OFF_AVT=lc640_read_int(EE_AV_OFF_AVT);
MNEMO_ON=lc640_read_int(EE_MNEMO_ON);
MNEMO_TIME=lc640_read_int(EE_MNEMO_TIME);
UBM_AV=lc640_read_int(EE_UBM_AV);
REL_AV[0]=lc640_read_int(EE_REL_AV1);
REL_AV[1]=lc640_read_int(EE_REL_AV2);
REL_AV[2]=lc640_read_int(EE_REL_AV3);

BAT_IS_ON=lc640_read_int(EE_BAT_IS_ON);
if(BAT_IS_ON==bisON)NUMBAT=1;
else NUMBAT=0; 
BAT_DAY_OF_ON=lc640_read_int(EE_BAT_DAY_OF_ON);
BAT_MONTH_OF_ON=lc640_read_int(EE_BAT_MONTH_OF_ON);
BAT_YEAR_OF_ON=lc640_read_int(EE_BAT_YEAR_OF_ON);
BAT_C_REAL=lc640_read_int(EE_BAT_C_REAL);
BAT_C_NOM=lc640_read_int(EE_BAT_C_NOM);
BAT_RESURS=lc640_read_int(EE_BAT_RESURS);
apv_on1=lc640_read_int(EE_APV_ON1);
apv_on2=lc640_read_int(EE_APV_ON2);
apv_on2_time=lc640_read_int(EE_APV_ON2_TIME);
VZ_HR=lc640_read_int(EE_VZ_HR);
TBAT=lc640_read_int(EE_TBAT);
NUMDT=lc640_read_int(EE_NUMDT);
NUMSK=lc640_read_int(EE_NUMSK);

ETH_IS_ON=lc640_read_int(EE_ETH_IS_ON);
ETH_DHCP_ON=lc640_read_int(EE_ETH_DHCP_ON);
ETH_IP_1=lc640_read_int(EE_ETH_IP_1);
ETH_IP_2=lc640_read_int(EE_ETH_IP_2);
ETH_IP_3=lc640_read_int(EE_ETH_IP_3);
ETH_IP_4=lc640_read_int(EE_ETH_IP_4);
ETH_MASK_1=lc640_read_int(EE_ETH_MASK_1);
ETH_MASK_2=lc640_read_int(EE_ETH_MASK_2);
ETH_MASK_3=lc640_read_int(EE_ETH_MASK_3);
ETH_MASK_4=lc640_read_int(EE_ETH_MASK_4);
ETH_GW_1=lc640_read_int(EE_ETH_GW_1);
ETH_GW_2=lc640_read_int(EE_ETH_GW_2);
ETH_GW_3=lc640_read_int(EE_ETH_GW_3);
ETH_GW_4=lc640_read_int(EE_ETH_GW_4);
ETH_TRAP1_IP_1=lc640_read_int(EE_ETH_TRAP1_IP_1);
ETH_TRAP1_IP_2=lc640_read_int(EE_ETH_TRAP1_IP_2);
ETH_TRAP1_IP_3=lc640_read_int(EE_ETH_TRAP1_IP_3);
ETH_TRAP1_IP_4=lc640_read_int(EE_ETH_TRAP1_IP_4);
ETH_TRAP2_IP_1=lc640_read_int(EE_ETH_TRAP2_IP_1);
ETH_TRAP2_IP_2=lc640_read_int(EE_ETH_TRAP2_IP_2);
ETH_TRAP2_IP_3=lc640_read_int(EE_ETH_TRAP2_IP_3);
ETH_TRAP2_IP_4=lc640_read_int(EE_ETH_TRAP2_IP_4);
ETH_TRAP3_IP_1=lc640_read_int(EE_ETH_TRAP3_IP_1);
ETH_TRAP3_IP_2=lc640_read_int(EE_ETH_TRAP3_IP_2);
ETH_TRAP3_IP_3=lc640_read_int(EE_ETH_TRAP3_IP_3);
ETH_TRAP3_IP_4=lc640_read_int(EE_ETH_TRAP3_IP_4);
ETH_TRAP4_IP_1=lc640_read_int(EE_ETH_TRAP4_IP_1);
ETH_TRAP4_IP_2=lc640_read_int(EE_ETH_TRAP4_IP_2);
ETH_TRAP4_IP_3=lc640_read_int(EE_ETH_TRAP4_IP_3);
ETH_TRAP4_IP_4=lc640_read_int(EE_ETH_TRAP4_IP_4);
ETH_TRAP5_IP_1=lc640_read_int(EE_ETH_TRAP5_IP_1);
ETH_TRAP5_IP_2=lc640_read_int(EE_ETH_TRAP5_IP_2);
ETH_TRAP5_IP_3=lc640_read_int(EE_ETH_TRAP5_IP_3);
ETH_TRAP5_IP_4=lc640_read_int(EE_ETH_TRAP5_IP_4);
ETH_SNMP_PORT_READ=lc640_read_int(EE_ETH_SNMP_PORT_READ);
ETH_SNMP_PORT_WRITE=lc640_read_int(EE_ETH_SNMP_PORT_WRITE);

if((ind==iMn)||(ind==iAusw)||(ind==iAusw_set))
	{
	AUSW_MAIN=lc640_read_int(EE_AUSW_MAIN);
	AUSW_MAIN_NUMBER=lc640_read_long(EE_AUSW_MAIN_NUMBER);
	AUSW_DAY=lc640_read_int(EE_AUSW_DAY);
	AUSW_MONTH=lc640_read_int(EE_AUSW_MONTH);
	AUSW_YEAR=lc640_read_int(EE_AUSW_YEAR);
	AUSW_BPS1_NUMBER=lc640_read_long(EE_AUSW_BPS1_NUMBER);
	AUSW_BPS2_NUMBER=lc640_read_long(EE_AUSW_BPS2_NUMBER);
	AUSW_RS232=lc640_read_int(EE_AUSW_RS232);
	AUSW_PDH=lc640_read_int(EE_AUSW_PDH);
	AUSW_SDH=lc640_read_int(EE_AUSW_SDH);
	AUSW_ETH=lc640_read_int(EE_AUSW_ETH);
	AUSW_UKU=lc640_read_int(EE_AUSW_UKU);
	AUSW_UKU_SUB=lc640_read_int(EE_AUSW_UKU_SUB);
	AUSW_UKU_NUMBER=lc640_read_long(EE_AUSW_UKU_NUMBER);			
	}

TMAX_EXT_EN[0]=lc640_read_int(EE_TMAX_EXT_EN0);
TMAX_EXT[0]=lc640_read_int(EE_TMAX_EXT0);
TMIN_EXT_EN[0]=lc640_read_int(EE_TMIN_EXT_EN0);
TMIN_EXT[0]=lc640_read_int(EE_TMIN_EXT0);	
T_EXT_REL_EN[0]=lc640_read_int(EE_T_EXT_REL_EN0);
T_EXT_ZVUK_EN[0]=lc640_read_int(EE_T_EXT_ZVUK_EN0);
T_EXT_LCD_EN[0]=lc640_read_int(EE_T_EXT_LCD_EN0);
T_EXT_SNMP_TRAP_EN[0]=lc640_read_int(EE_T_EXT_SNMP_TRAP_EN0);

SK_SIGN[0]=lc640_read_int(EE_SK_SIGN0);
SK_REL_EN[0]=lc640_read_int(EE_SK_REL_EN0);
SK_ZVUK_EN[0]=lc640_read_int(EE_SK_ZVUK_EN0);
SK_LCD_EN[0]=lc640_read_int(EE_SK_LCD_EN0);	
SK_SNMP_TRAP_EN[0]=lc640_read_int(EE_SK_SNMP_TRAP_EN0);
SK_SIGN[1]=lc640_read_int(EE_SK_SIGN1);
SK_REL_EN[1]=lc640_read_int(EE_SK_REL_EN1);
SK_ZVUK_EN[1]=lc640_read_int(EE_SK_ZVUK_EN1);
SK_LCD_EN[1]=lc640_read_int(EE_SK_LCD_EN1);	
SK_SNMP_TRAP_EN[1]=lc640_read_int(EE_SK_SNMP_TRAP_EN1);
SK_SIGN[2]=lc640_read_int(EE_SK_SIGN2);
SK_REL_EN[2]=lc640_read_int(EE_SK_REL_EN2);
SK_ZVUK_EN[2]=lc640_read_int(EE_SK_ZVUK_EN2);
SK_LCD_EN[2]=lc640_read_int(EE_SK_LCD_EN2);	
SK_SNMP_TRAP_EN[2]=lc640_read_int(EE_SK_SNMP_TRAP_EN2);
SK_SIGN[3]=lc640_read_int(EE_SK_SIGN3);
SK_REL_EN[3]=lc640_read_int(EE_SK_REL_EN3);
SK_ZVUK_EN[3]=lc640_read_int(EE_SK_ZVUK_EN3);
SK_LCD_EN[3]=lc640_read_int(EE_SK_LCD_EN3);	
SK_SNMP_TRAP_EN[3]=lc640_read_int(EE_SK_SNMP_TRAP_EN3);


AVZ=(char)(lc640_read_int(EE_AVZ));
AVZ_TIME=lc640_read_int(EE_AVZ_TIME);
HOUR_AVZ=lc640_read_int(EE_HOUR_AVZ);
MIN_AVZ=lc640_read_int(EE_MIN_AVZ);
SEC_AVZ=lc640_read_int(EE_SEC_AVZ);
DATE_AVZ=lc640_read_int(EE_DATE_AVZ);
MONTH_AVZ=lc640_read_int(EE_MONTH_AVZ);
if(!((MONTH_AVZ>0)&&(MONTH_AVZ<13)))MONTH_AVZ=0;
YEAR_AVZ=lc640_read_int(EE_YEAR_AVZ); 
SNTP_ENABLE=lc640_read_int(EE_SNTP_ENABLE);
SNTP_GMT=lc640_read_int(EE_SNTP_GMT);
MODBUS_ADRESS=lc640_read_int(EE_MODBUS_ADRESS);

for(i=0;i<30;i++)
	{
	snmp_model[i]=lc640_read(EE_MODEL+i);
	}
for(i=0;i<64;i++)
	{
	snmp_location[i]=lc640_read(EE_LOCATION+i);
	}
for(i=0;i<10;i++)
	{
	snmp_community[i]=lc640_read(EE_COMMUNITY+(i*2));
	}
for(i=0;i<30;i++)
	{
	snmp_model[i]=lc640_read(EE_MODEL+i);
	} 
for(i=0;i<3;i++)
	{
	snmp_web_passw[i]=lc640_read(EE_WEB_PASSWORD+(i*2));
	snmp_web_passw[3]=0;
	}
}


