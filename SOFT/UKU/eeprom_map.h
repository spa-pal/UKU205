


#define RESET_CNT			0x08

#define SECTOR_KOEF 		0x10
#define KTSRC1				SECTOR_KOEF
#define KTSRC2				SECTOR_KOEF+2
#define KUSRC1				SECTOR_KOEF+4
#define KUSRC2				SECTOR_KOEF+6
#define KISRC1				SECTOR_KOEF+8
#define KISRC2				SECTOR_KOEF+10
#define KUBAT				SECTOR_KOEF+12
#define EE_KIBAT0			SECTOR_KOEF+14
#define KIBAT1				SECTOR_KOEF+16
#define KTBAT				SECTOR_KOEF+18
#define KUNET       		SECTOR_KOEF+20
#define KFNET       		SECTOR_KOEF+22
#define EE_KUBAT_PART0		SECTOR_KOEF+24
#define EE_KUBAT_PART1		SECTOR_KOEF+26
#define EE_KUBAT_PART2		SECTOR_KOEF+28
#define EE_KUBAT_PART3		SECTOR_KOEF+30	

#define EE_MAIN_BPS SECTOR_KOEF+100
#define EE_UMAX    	EE_MAIN_BPS+2
#define EE_UBMAX   	EE_UMAX+2
#define EE_UB20    	EE_UBMAX+2
#define EE_TMAX	EE_UB20+2
#define EE_AV_OFF_AVT    EE_TMAX+2
#define EE_USIGN	EE_AV_OFF_AVT+2
#define EE_UMN		EE_USIGN+2
#define EE_ZV_ON	EE_UMN+2
#define EE_IKB		EE_ZV_ON+2
#define EE_KVZ		EE_IKB+2
#define EE_IMAX	EE_KVZ+2
#define EE_KIMAX	EE_IMAX+2
#define EE_APV_ON	EE_KIMAX+2
#define EE_IZMAX	EE_APV_ON+2
#define EE_U0B		EE_IZMAX+2
#define EE_TZAS	EE_U0B+2
#define EE_NUMIST  	EE_TZAS+2
#define EE_NUMBAT  	EE_NUMIST+2
#define KI0SRC1     EE_NUMBAT+2
#define KI0SRC2     KI0SRC1+2
#define EE_APV_ON1  KI0SRC2+2
#define EE_APV_ON2  EE_APV_ON1+2
#define EE_APV_ON2_TIME  EE_APV_ON2+2
#define KTOUT0		EE_APV_ON2_TIME+2
#define KTOUT1		KTOUT0+2
#define KTOUT2		KTOUT1+2
#define EE_AVZ_TIME	KTOUT2+2	
#define EE_HOUR_AVZ	EE_AVZ_TIME+2	
#define EE_MIN_AVZ  EE_HOUR_AVZ+2
#define EE_SEC_AVZ  EE_MIN_AVZ+2
#define EE_DATE_AVZ EE_SEC_AVZ+2
#define EE_MONTH_AVZ	EE_DATE_AVZ+2
#define EE_YEAR_AVZ EE_MONTH_AVZ+2
#define EE_AVZ		EE_YEAR_AVZ+2
#define EE_MNEMO_ON EE_AVZ+2
#define EE_MNEMO_TIME 	EE_MNEMO_ON+2
#define EE_NUMDT  	EE_MNEMO_TIME+2
#define EE_NUMSK  	EE_NUMDT+2
#define EE_IU1  	EE_NUMSK+2
#define EE_IU2		EE_IU1+2
#define EE_IU3		EE_IU2+2
#define EE_IU4		EE_IU3+2
#define EE_LBAT_STRUKT		EE_IU4+2
#define EE_LBAT_CONF_CH_COUNTER	EE_LBAT_STRUKT+2

#define SECTOR_AUSW  		SECTOR_KOEF+200
#define EE_AUSW_MAIN 		SECTOR_AUSW
#define EE_AUSW_MAIN_NUMBER 	EE_AUSW_MAIN+2 
#define EE_AUSW_UKU 		EE_AUSW_MAIN_NUMBER+4 
#define EE_AUSW_UKU_SUB 		EE_AUSW_UKU+2
#define EE_AUSW_UKU_NUMBER	EE_AUSW_UKU_SUB+2
#define EE_AUSW_DAY			EE_AUSW_UKU_NUMBER+4 
#define EE_AUSW_MONTH		EE_AUSW_DAY+2
#define EE_AUSW_YEAR		EE_AUSW_MONTH+2
#define EE_AUSW_BPS1_NUMBER	EE_AUSW_YEAR+4		
#define EE_AUSW_BPS2_NUMBER	EE_AUSW_BPS1_NUMBER+4
#define EE_AUSW_RS232		EE_AUSW_BPS2_NUMBER+4
#define EE_AUSW_PDH			EE_AUSW_RS232+2
#define EE_AUSW_SDH			EE_AUSW_PDH+2
#define EE_AUSW_ETH			EE_AUSW_SDH+2

#define SECTOR_BAT  		SECTOR_KOEF+250
#define EE_BAT_IS_ON          SECTOR_BAT
#define EE_BAT_DAY_OF_ON      SECTOR_BAT+2
#define EE_BAT_MONTH_OF_ON    SECTOR_BAT+4
#define EE_BAT_YEAR_OF_ON     SECTOR_BAT+6
#define EE_BAT_C_REAL         SECTOR_BAT+8
#define EE_BAT_RESURS         SECTOR_BAT+10
#define EE_ZAR_CNT            SECTOR_BAT+12
#define EE_ZAR_CNT_KE         SECTOR_BAT+14
#define EE_VZ_HR			SECTOR_BAT+16
#define EE_BAT_C_NOM          SECTOR_BAT+18
#define EE_TBAT          	SECTOR_BAT+20
#define KOEF_LONG	30

#define SECTOR_EXT  		SECTOR_KOEF+300
#define EE_TMAX_EXT_EN0		SECTOR_EXT
#define EE_TMAX_EXT0		SECTOR_EXT+2
#define EE_TMIN_EXT_EN0		SECTOR_EXT+4
#define EE_TMIN_EXT0		SECTOR_EXT+6
#define EE_T_EXT_REL_EN0		SECTOR_EXT+8
#define EE_T_EXT_ZVUK_EN0	SECTOR_EXT+10
#define EE_T_EXT_LCD_EN0		SECTOR_EXT+12
#define EE_T_EXT_SNMP_TRAP_EN0		SECTOR_EXT+14
#define EE_TMAX_EXT_EN1		SECTOR_EXT+16
#define EE_TMAX_EXT1		SECTOR_EXT+18
#define EE_TMIN_EXT_EN1		SECTOR_EXT+20
#define EE_TMIN_EXT1		SECTOR_EXT+22
#define EE_T_EXT_REL_EN1		SECTOR_EXT+24
#define EE_T_EXT_ZVUK_EN1	SECTOR_EXT+26
#define EE_T_EXT_LCD_EN1		SECTOR_EXT+28
#define EE_T_EXT_SNMP_TRAP_EN1		SECTOR_EXT+30
#define EE_TMAX_EXT_EN2		SECTOR_EXT+32
#define EE_TMAX_EXT2		SECTOR_EXT+34
#define EE_TMIN_EXT_EN2		SECTOR_EXT+36
#define EE_TMIN_EXT2		SECTOR_EXT+38
#define EE_T_EXT_REL_EN2		SECTOR_EXT+40
#define EE_T_EXT_ZVUK_EN2	SECTOR_EXT+42
#define EE_T_EXT_LCD_EN2		SECTOR_EXT+44
#define EE_T_EXT_SNMP_TRAP_EN2		SECTOR_EXT+46
#define EE_SK_SIGN0			SECTOR_EXT+48
#define EE_SK_REL_EN0		SECTOR_EXT+50
#define EE_SK_ZVUK_EN0		SECTOR_EXT+52
#define EE_SK_LCD_EN0		SECTOR_EXT+54
#define EE_SK_SNMP_TRAP_EN0		SECTOR_EXT+56
#define EE_SK_SIGN1			SECTOR_EXT+58
#define EE_SK_REL_EN1		SECTOR_EXT+60
#define EE_SK_ZVUK_EN1		SECTOR_EXT+62
#define EE_SK_LCD_EN1		SECTOR_EXT+64
#define EE_SK_SNMP_TRAP_EN1		SECTOR_EXT+66
#define EE_SK_SIGN2			SECTOR_EXT+68
#define EE_SK_REL_EN2		SECTOR_EXT+70
#define EE_SK_ZVUK_EN2		SECTOR_EXT+72
#define EE_SK_LCD_EN2		SECTOR_EXT+74
#define EE_SK_SNMP_TRAP_EN2		SECTOR_EXT+76
#define EE_SK_SIGN3			SECTOR_EXT+78
#define EE_SK_REL_EN3		SECTOR_EXT+80
#define EE_SK_ZVUK_EN3		SECTOR_EXT+82
#define EE_SK_LCD_EN3		SECTOR_EXT+84
#define EE_SK_SNMP_TRAP_EN3		SECTOR_EXT+86
#define EE_UBM_AV             SECTOR_EXT+88
#define EE_REL_AV1            SECTOR_EXT+90
#define EE_REL_AV2            SECTOR_EXT+92
#define EE_REL_AV3            SECTOR_EXT+94


/*
#define KE_PTR			996
#define KE_CNT			998
#define UNET_AVAR_PTR	1000
#define UNET_AVAR_CNT	1002
#define SRC1_AVAR_PTR	1004
#define SRC1_AVAR_CNT	1006
#define SRC2_AVAR_PTR	1008
#define SRC2_AVAR_CNT	1010
#define BAT_AVAR_PTR	1012
#define BAT_AVAR_CNT	1014
#define VZ_PTR			1016
#define VZ_CNT			1018
#define WRK_PTR		1020
#define WRK_CNT		1022
*/

#define SECTOR_ETH  		SECTOR_KOEF+500
#define EE_ETH_IS_ON		SECTOR_ETH
#define EE_ETH_DHCP_ON		SECTOR_ETH+2
#define EE_ETH_IP_1			SECTOR_ETH+4
#define EE_ETH_IP_2			SECTOR_ETH+6
#define EE_ETH_IP_3			SECTOR_ETH+8
#define EE_ETH_IP_4			SECTOR_ETH+10
#define EE_ETH_MASK_1		SECTOR_ETH+12
#define EE_ETH_MASK_2		SECTOR_ETH+14
#define EE_ETH_MASK_3		SECTOR_ETH+16
#define EE_ETH_MASK_4		SECTOR_ETH+18
#define EE_ETH_TRAP1_IP_1	SECTOR_ETH+20
#define EE_ETH_TRAP1_IP_2	SECTOR_ETH+22
#define EE_ETH_TRAP1_IP_3	SECTOR_ETH+24
#define EE_ETH_TRAP1_IP_4	SECTOR_ETH+26
#define EE_ETH_TRAP2_IP_1	SECTOR_ETH+28
#define EE_ETH_TRAP2_IP_2	SECTOR_ETH+30
#define EE_ETH_TRAP2_IP_3	SECTOR_ETH+32
#define EE_ETH_TRAP2_IP_4	SECTOR_ETH+34
#define EE_ETH_TRAP3_IP_1	SECTOR_ETH+36
#define EE_ETH_TRAP3_IP_2	SECTOR_ETH+38
#define EE_ETH_TRAP3_IP_3	SECTOR_ETH+40
#define EE_ETH_TRAP3_IP_4	SECTOR_ETH+42
#define EE_ETH_TRAP4_IP_1	SECTOR_ETH+44
#define EE_ETH_TRAP4_IP_2	SECTOR_ETH+46
#define EE_ETH_TRAP4_IP_3	SECTOR_ETH+48
#define EE_ETH_TRAP4_IP_4	SECTOR_ETH+50
#define EE_ETH_TRAP5_IP_1	SECTOR_ETH+52
#define EE_ETH_TRAP5_IP_2	SECTOR_ETH+54
#define EE_ETH_TRAP5_IP_3	SECTOR_ETH+56
#define EE_ETH_TRAP5_IP_4	SECTOR_ETH+58
#define EE_ETH_SNMP_PORT_READ	SECTOR_ETH+60
#define EE_ETH_SNMP_PORT_WRITE	SECTOR_ETH+62
#define EE_ETH_GW_1			SECTOR_ETH+64
#define EE_ETH_GW_2			SECTOR_ETH+66
#define EE_ETH_GW_3			SECTOR_ETH+68
#define EE_ETH_GW_4			SECTOR_ETH+70
#define EE_MODBUS_ADRESS		SECTOR_ETH+72
#define EE_MODBUS_BAUDRATE	SECTOR_ETH+74

#define SECTOR_LOCATION  	SECTOR_ETH+200
#define EE_LOCATION			SECTOR_LOCATION
#define SECTOR_COMMUNITY  	SECTOR_ETH+270
#define EE_COMMUNITY		SECTOR_COMMUNITY
#define SECTOR_MODEL  		SECTOR_LOCATION+100	//916
#define EE_MODEL			SECTOR_MODEL

#define EVENT_LOG		1024 
//массив данных журнала событий 32*64=2048
// Структура данных журнала событий:
// Байт 0 - род устройства:
//					'B' - батарея
//					'S' - бпсы
//					'P' - питающая сеть
//					'I' - инверторы
//					'U' - УКУ
//					'T' - Внешний датчик температуры
//					'L' - Внешний логический вход
// Байт 1 - порядковый номер устройства(начинается с нуля)
// Байт 2 - род события:
//					'A' - авария (для питающей сети) 
//					'L' - авария связи (для БПСов и инверторов)
//					'C' - авария отсутствия(для батареи и БПСов) 
//					'U' - авария завышенного напряжения (для БПСов и инверторов) 
//					'u' - авария заниженного напряжения (для БПСов и инверторов) 
//					'T' - авария по температуре (для БПСов и инверторов) 
//					'R' - перезагрузка или включение, только для УКУ
// Байт 3 - год возникновения события
// Байт 4 - месяц возникновения события
// Байт 5 - день возникновения события
// Байт 6 - час возникновения события
// Байт 7 - минута возникновения события
// Байт 8 - секунда возникновения события



#define PTR_EVENT_LOG	EVENT_LOG+1024+512+1024 
// указатель на события(показывает на последнее записанное) 
#define CNT_EVENT_LOG	PTR_EVENT_LOG+2 
// колличество событий (не более 64) 


extern const short ptr_kt_src[2];
extern const short ptr_ku_src[2];
extern const short ptr_ki_src[2];
extern const short ptr_ki0_src[2];
extern const short ptr_ku_bat;
extern const short ptr_ki_bat0;
extern const short ptr_ki_bat1;
extern const short ptr_kt_bat;
extern const short ptr_ku_net;
extern const short ptr_kf_net;

extern const short ADR_TMAX_EXT_EN[3];
extern const short ADR_TMAX_EXT[3];
extern const short ADR_TMIN_EXT_EN[3];
extern const short ADR_TMIN_EXT[3];
extern const short ADR_T_EXT_REL_EN[3];
extern const short ADR_T_EXT_ZVUK_EN[3];
extern const short ADR_T_EXT_LCD_EN[3];
extern const short ADR_T_EXT_SNMP_TRAP_EN[3];

extern const short ADR_SK_SIGN[4];
extern const short ADR_SK_REL_EN[4];
extern const short ADR_SK_ZVUK_EN[4];
extern const short ADR_SK_LCD_EN[4];
extern const short ADR_SK_SNMP_TRAP_EN[4];
