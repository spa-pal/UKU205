#include "eeprom_map.h"



const short ptr_kt_src[2]={KTSRC1,KTSRC2};
const short ptr_ku_src[2]={KUSRC1,KUSRC2};
const short ptr_ki_src[2]={KISRC1,KISRC2};
const short ptr_ki0_src[2]={KI0SRC1,KI0SRC2};
const short ptr_ku_bat=KUBAT;
const short ptr_ki_bat0=EE_KIBAT0;
const short ptr_ki_bat1=KIBAT1;
const short ptr_kt_bat=KTBAT;
const short ptr_ku_net=KUNET;
const short ptr_kf_net=KFNET;

const short ADR_TMAX_EXT_EN[3]={EE_TMAX_EXT_EN0,EE_TMAX_EXT_EN1,EE_TMAX_EXT_EN2};
const short ADR_TMAX_EXT[3]={EE_TMAX_EXT0,EE_TMAX_EXT1,EE_TMAX_EXT2};
const short ADR_TMIN_EXT_EN[3]={EE_TMIN_EXT_EN0,EE_TMIN_EXT_EN1,EE_TMIN_EXT_EN2};
const short ADR_TMIN_EXT[3]={EE_TMIN_EXT0,EE_TMIN_EXT1,EE_TMIN_EXT2};
const short ADR_T_EXT_REL_EN[3]={EE_T_EXT_REL_EN0,EE_T_EXT_REL_EN1,EE_T_EXT_REL_EN2};
const short ADR_T_EXT_ZVUK_EN[3]={EE_T_EXT_ZVUK_EN0,EE_T_EXT_ZVUK_EN1,EE_T_EXT_ZVUK_EN2};
const short ADR_T_EXT_LCD_EN[3]={EE_T_EXT_LCD_EN0,EE_T_EXT_LCD_EN1,EE_T_EXT_LCD_EN2};
const short ADR_T_EXT_SNMP_TRAP_EN[3]={EE_T_EXT_SNMP_TRAP_EN0,EE_T_EXT_SNMP_TRAP_EN1,EE_T_EXT_SNMP_TRAP_EN2};

const short ADR_SK_SIGN[4]={EE_SK_SIGN0,EE_SK_SIGN1,EE_SK_SIGN2,EE_SK_SIGN3};
const short ADR_SK_REL_EN[4]={EE_SK_REL_EN0,EE_SK_REL_EN1,EE_SK_REL_EN2,EE_SK_REL_EN3};
const short ADR_SK_ZVUK_EN[4]={EE_SK_ZVUK_EN0,EE_SK_ZVUK_EN1,EE_SK_ZVUK_EN2,EE_SK_ZVUK_EN3};
const short ADR_SK_LCD_EN[4]={EE_SK_LCD_EN0,EE_SK_LCD_EN1,EE_SK_LCD_EN2,EE_SK_LCD_EN3};
const short ADR_SK_SNMP_TRAP_EN[4]={EE_SK_SNMP_TRAP_EN0,EE_SK_SNMP_TRAP_EN1,EE_SK_SNMP_TRAP_EN2,EE_SK_SNMP_TRAP_EN3};
