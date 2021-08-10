
#include "snmp_data_file.h" 
#include <lpc17xx.h>
#include "modbus.h"
//#include "LPC17xx.H"
#include "main.h"
#include "control.h"
#include <string.h>

#include "eeprom_map.h"
#include "rtl.h"
#include "modbus_tcp.h"
#include "25lc640.h"
#include "uart0.h"
#include "curr_version.h"

#define MODBUS_RTU_PROT	0

extern int  mem_copy (void *dp, void *sp, int len);

unsigned char modbus_buf[20];
short modbus_crc16;
char modbus_timeout_cnt;
char bMODBUS_TIMEOUT;
unsigned char modbus_rx_buffer[30];	//Буфер, куда складывает принимаемые даннные обработчик прерывания по приему УАРТа 
unsigned char modbus_an_buffer[30];    	//Буфер, куда они потом копируются для анализа
unsigned char modbus_rx_buffer_ptr;	//Указатель на текущую позицию принимающего буфера
unsigned char modbus_rx_counter;		//Количество принятых байт, используется при анализе целостности посылки и при расшифровке

short modbus_modbus_adress_eq;
short modbus_modbus4f_cnt;

short modbus_plazma;				//Отладка
short modbus_plazma1;				//Отладка
short modbus_plazma2;				//Отладка
short modbus_plazma3;				//Отладка
short modbus_plazma_p;				//Отладка
short modbus_plazma_pp;				//Отладка

unsigned short modbus_rx_arg0;		//встроенный в посылку первый аргумент
unsigned short modbus_rx_arg1;		//встроенный в посылку второй аргумент
unsigned short modbus_rx_arg2;		//встроенный в посылку третий аргумент
unsigned short modbus_rx_arg3;		//встроенный в посылку четвертый аргумент

char modbus_tx_buff[100];

//char modbus_registers[200];

//static const char foo[] = "I wish I'd read K&R, and other tomes more diligently";



/*modbus_registers[3]=(char)(bps_I%256);
modbus_registers[4]=(char)(net_U/256);					//Рег3   	напряжение сети питания, 1В
modbus_registers[5]=(char)(net_U%256);
modbus_registers[6]=(char)(net_F/256);					//Рег4   	частота сети питания, 0.1Гц
modbus_registers[7]=(char)(net_F%256);
modbus_registers[8]=(char)(net_Ua/256);					//Рег5	напряжение сети питания фаза A, 1В	
modbus_registers[9]=(char)(net_Ua%256);		 	
modbus_registers[10]=(char)(net_Ub/256);				//Рег6	напряжение сети питания фаза B, 1В
modbus_registers[11]=(char)(net_Ub%256);
modbus_registers[12]=(char)(net_Uc/256);				//Рег7	напряжение сети питания фаза C, 1В
modbus_registers[13]=(char)(net_Uc%256);
modbus_registers[14]=(char)(bat[0]._Ub/256);				//Рег8	напряжение батареи №1, 0.1В
modbus_registers[15]=(char)(bat[0]._Ub%256);
modbus_registers[16]=(char)(bat[0]._Ib/256);				//Рег9   	ток батареи №1, 0.01А
modbus_registers[17]=(char)(bat[0]._Ib%256);
modbus_registers[18]=(char)(bat[0]._Tb/256);				//Рег10	температура батареи №1, 1Гц
modbus_registers[19]=(char)(bat[0]._Tb%256);
modbus_registers[20]=(char)(bat[0]._zar/256);			//Рег11	заряд батареи №1, %
modbus_registers[21]=(char)(bat[0]._zar%256);
modbus_registers[22]=(char)(bat[0]._Ubm/256);			//Рег12	напряжение средней точки батареи №1, 0.1В
modbus_registers[23]=(char)(bat[0]._Ubm%256);
modbus_registers[24]=(char)(bat[0]._dUbm/256);			//Рег13	ошибка средней точки батареи №1, %
modbus_registers[25]=(char)(bat[0]._dUbm%256);
modbus_registers[26]=(char)(BAT_C_REAL[0]/256);			//Рег14	Реальная емкость батареи №1, 0.1А*ч, если 0x5555 то не измерялась
modbus_registers[27]=(char)(BAT_C_REAL[0]%256);
modbus_registers[28]=(char)(bat[1]._Ub/256);				//Рег15	напряжение батареи №1, 0.1В
modbus_registers[29]=(char)(bat[1]._Ub%256);
modbus_registers[30]=(char)(bat[1]._Ib/256);				//Рег16   	ток батареи №1, 0.01А
modbus_registers[31]=(char)(bat[1]._Ib%256);
modbus_registers[32]=(char)(bat[1]._Tb/256);				//Рег17	температура батареи №1, 1Гц
modbus_registers[33]=(char)(bat[1]._Tb%256);
modbus_registers[34]=(char)(bat[1]._zar/256);			//Рег18	заряд батареи №1, %
modbus_registers[35]=(char)(bat[1]._zar%256);
modbus_registers[36]=(char)(bat[1]._Ubm/256);			//Рег19	напряжение средней точки батареи №1, 0.1В
modbus_registers[37]=(char)(bat[1]._Ubm%256);
modbus_registers[38]=(char)(bat[1]._dUbm/256);			//Рег20	ошибка средней точки батареи №1, %
modbus_registers[39]=(char)(bat[1]._dUbm%256);
modbus_registers[40]=(char)(BAT_C_REAL[1]/256);			//Рег21	Реальная емкость батареи №1, 0.1А*ч, если 0x5555 то не измерялась
modbus_registers[41]=(char)(BAT_C_REAL[1]%256);
modbus_registers[42]=(char)(bps[0]._Uii/256);			//Рег22	Выходное напряжение выпрямителя №1, 0.1В
modbus_registers[43]=(char)(bps[0]._Uii%256);
modbus_registers[44]=(char)(bps[0]._Ii/256);				//Рег23	Выходной ток выпрямителя №1, 0.1А
modbus_registers[45]=(char)(bps[0]._Ii%256);
modbus_registers[46]=(char)(bps[0]._Ti/256);				//Рег24	Температура радиатора выпрямителя №1, 1гЦ
modbus_registers[47]=(char)(bps[0]._Ti%256);
modbus_registers[48]=(char)(bps[0]._av/256);				//Рег25	Байт флагов выпрямителя №1, 0x01 - перегрев, 0x02 завышено Uвых, 0x04 занижено Uвых, 0x08 - отсутствует связь с выпрямителем
modbus_registers[49]=(char)(bps[0]._av%256);
modbus_registers[50]=(char)(bps[1]._Uii/256);			//Рег26	Выходное напряжение выпрямителя №2, 0.1В
modbus_registers[51]=(char)(bps[1]._Uii%256);
modbus_registers[52]=(char)(bps[1]._Ii/256);				//Рег27	Выходной ток выпрямителя №2, 0.1А
modbus_registers[53]=(char)(bps[1]._Ii%256);
modbus_registers[54]=(char)(bps[1]._Ti/256);				//Рег28	Температура радиатора выпрямителя №2, 1гЦ
modbus_registers[55]=(char)(bps[1]._Ti%256);
modbus_registers[56]=(char)(bps[1]._av/256);				//Рег29	Байт флагов выпрямителя №2, 0x01 - перегрев, 0x02 завышено Uвых, 0x04 занижено Uвых, 0x08 - отсутствует связь с выпрямителем
modbus_registers[57]=(char)(bps[1]._av%256);
modbus_registers[58]=(char)(bps[2]._Uii/256);			//Рег30	Выходное напряжение выпрямителя №3, 0.1В
modbus_registers[59]=(char)(bps[2]._Uii%256);
modbus_registers[60]=(char)(bps[2]._Ii/256);				//Рег31	Выходной ток выпрямителя №3, 0.1А
modbus_registers[61]=(char)(bps[2]._Ii%256);
modbus_registers[62]=(char)(bps[2]._Ti/256);				//Рег32	Температура радиатора выпрямителя №3, 1гЦ
modbus_registers[63]=(char)(bps[2]._Ti%256);
modbus_registers[64]=(char)(bps[2]._av/256);				//Рег33	Байт флагов выпрямителя №3, 0x01 - перегрев, 0x02 завышено Uвых, 0x04 занижено Uвых, 0x08 - отсутствует связь с выпрямителем
modbus_registers[65]=(char)(bps[2]._av%256);
modbus_registers[66]=(char)(bps[3]._Uii/256);			//Рег34	Выходное напряжение выпрямителя №4, 0.1В
modbus_registers[67]=(char)(bps[3]._Uii%256);
modbus_registers[68]=(char)(bps[3]._Ii/256);				//Рег35	Выходной ток выпрямителя №4, 0.1А
modbus_registers[69]=(char)(bps[3]._Ii%256);
modbus_registers[70]=(char)(bps[3]._Ti/256);				//Рег36	Температура радиатора выпрямителя №4, 1гЦ
modbus_registers[71]=(char)(bps[3]._Ti%256);
modbus_registers[72]=(char)(bps[3]._av/256);				//Рег37	Байт флагов выпрямителя №4, 0x01 - перегрев, 0x02 завышено Uвых, 0x04 занижено Uвых, 0x08 - отсутствует связь с выпрямителем
modbus_registers[73]=(char)(bps[3]._av%256);
modbus_registers[74]=(char)(bps[4]._Uii/256);			//Рег38	Выходное напряжение выпрямителя №5, 0.1В
modbus_registers[75]=(char)(bps[4]._Uii%256);
modbus_registers[76]=(char)(bps[4]._Ii/256);				//Рег39	Выходной ток выпрямителя №5, 0.1А
modbus_registers[77]=(char)(bps[4]._Ii%256);
modbus_registers[78]=(char)(bps[4]._Ti/256);				//Рег40	Температура радиатора выпрямителя №5, 1гЦ
modbus_registers[79]=(char)(bps[4]._Ti%256);
modbus_registers[80]=(char)(bps[4]._av/256);				//Рег41	Байт флагов выпрямителя №5, 0x01 - перегрев, 0x02 завышено Uвых, 0x04 занижено Uвых, 0x08 - отсутствует связь с выпрямителем
modbus_registers[81]=(char)(bps[4]._av%256);
modbus_registers[82]=(char)(bps[5]._Uii/256);			//Рег42	Выходное напряжение выпрямителя №6, 0.1В
modbus_registers[83]=(char)(bps[5]._Uii%256);
modbus_registers[84]=(char)(bps[5]._Ii/256);				//Рег43	Выходной ток выпрямителя №6, 0.1А
modbus_registers[85]=(char)(bps[5]._Ii%256);
modbus_registers[86]=(char)(bps[5]._Ti/256);				//Рег44	Температура радиатора выпрямителя №6, 1гЦ
modbus_registers[87]=(char)(bps[5]._Ti%256);
modbus_registers[88]=(char)(bps[5]._av/256);				//Рег45	Байт флагов выпрямителя №6, 0x01 - перегрев, 0x02 завышено Uвых, 0x04 занижено Uвых, 0x08 - отсутствует связь с выпрямителем
modbus_registers[89]=(char)(bps[5]._av%256);
modbus_registers[90]=(char)(bps[6]._Uii/256);			//Рег46	Выходное напряжение выпрямителя №7, 0.1В
modbus_registers[91]=(char)(bps[6]._Uii%256);
modbus_registers[92]=(char)(bps[6]._Ii/256);				//Рег47	Выходной ток выпрямителя №7, 0.1А
modbus_registers[93]=(char)(bps[6]._Ii%256);
modbus_registers[94]=(char)(bps[6]._Ti/256);				//Рег48	Температура радиатора выпрямителя №7, 1гЦ
modbus_registers[95]=(char)(bps[6]._Ti%256);
modbus_registers[96]=(char)(bps[6]._av/256);				//Рег49	Байт флагов выпрямителя №7, 0x01 - перегрев, 0x02 завышено Uвых, 0x04 занижено Uвых, 0x08 - отсутствует связь с выпрямителем
modbus_registers[97]=(char)(bps[6]._av%256);
modbus_registers[98]=(char)(bps[7]._Uii/256);			//Рег50	Выходное напряжение выпрямителя №8, 0.1В
modbus_registers[99]=(char)(bps[7]._Uii%256);
modbus_registers[100]=(char)(bps[7]._Ii/256);			//Рег51	Выходной ток выпрямителя №8, 0.1А
modbus_registers[101]=(char)(bps[7]._Ii%256);
modbus_registers[102]=(char)(bps[7]._Ti/256);			//Рег52	Температура радиатора выпрямителя №8, 1гЦ
modbus_registers[103]=(char)(bps[7]._Ti%256);
modbus_registers[104]=(char)(bps[7]._av/256);			//Рег53	Байт флагов выпрямителя №8, 0x01 - перегрев, 0x02 завышено Uвых, 0x04 занижено Uвых, 0x08 - отсутствует связь с выпрямителем
modbus_registers[105]=(char)(bps[7]._av%256);
modbus_registers[106]=(char)(bps_U/256);					//Рег54   	напряжение выпрямителей, 0.1В
modbus_registers[107]=(char)(bps_U%256);
tempS=0;
if(speedChIsOn) tempS=1;
modbus_registers[108]=(char)(tempS/256);					//Рег55   	Ускоренный заряд включенность, (1 - вкл, 0 - Выкл)
modbus_registers[109]=(char)(tempS%256);
tempS=0;
if(spc_stat==spcVZ) tempS=1;
modbus_registers[110]=(char)(tempS/256);					//Рег56   	Выравнивающий заряд включенность, (1 - вкл, 0 - Выкл)
modbus_registers[111]=(char)(tempS%256);
modbus_registers[112]=(char)(uout_av/256);					//Рег57   Контроль выходного напряжения, (0 - норма, 1 - завышено, 2 - занижено)
modbus_registers[113]=(char)(uout_av%256);

tempS=t_ext[0];
if(ND_EXT[0])tempS=-1000;
modbus_registers[400]=(char)(tempS/256);				//Рег201	Внешний датчик температуры №1
modbus_registers[401]=(char)(tempS%256);
tempS=t_ext[1];
if(ND_EXT[1])tempS=-1000;
modbus_registers[402]=(char)(tempS/256);				//Рег202	Внешний датчик температуры №2
modbus_registers[403]=(char)(tempS%256);
tempS=t_ext[2];
if(ND_EXT[2])tempS=-1000;
modbus_registers[404]=(char)(tempS/256);				//Рег203	Внешний датчик температуры №3
modbus_registers[405]=(char)(tempS%256);
tempS=t_ext[3];
if(ND_EXT[3])tempS=-1000;
modbus_registers[406]=(char)(tempS/256);				//Рег204	Внешний датчик температуры №4
modbus_registers[407]=(char)(tempS%256);

tempS=0;
if(sk_stat[0]==ssON) tempS|=0x0001;
if(sk_av_stat[0]==sasON) tempS|=0x0002;
modbus_registers[420]=(char)(tempS/256);				//Рег211	Состояние  сухого контакта №1, (нулевой бит - физическое состояние, 1 - замкнут, 0 - разомкнут, первый бит - аварийность, 1 - авария, 0 - норма)
modbus_registers[421]=(char)(tempS%256);
tempS=0;
if(sk_stat[1]==ssON) tempS|=0x0001;
if(sk_av_stat[1]==sasON) tempS|=0x0002;
modbus_registers[422]=(char)(tempS/256);				//Рег212	Состояние  сухого контакта №1, (нулевой бит - физическое состояние, 1 - замкнут, 0 - разомкнут, первый бит - аварийность, 1 - авария, 0 - норма)
modbus_registers[423]=(char)(tempS%256);
tempS=0;
if(sk_stat[2]==ssON) tempS|=0x0001;
if(sk_av_stat[2]==sasON) tempS|=0x0002;
modbus_registers[424]=(char)(tempS/256);				//Рег213	Состояние  сухого контакта №1, (нулевой бит - физическое состояние, 1 - замкнут, 0 - разомкнут, первый бит - аварийность, 1 - авария, 0 - норма)
modbus_registers[425]=(char)(tempS%256);
tempS=0;
if(sk_stat[3]==ssON) tempS|=0x0001;
if(sk_av_stat[3]==sasON) tempS|=0x0002;
modbus_registers[426]=(char)(tempS/256);				//Рег214	Состояние  сухого контакта №1, (нулевой бит - физическое состояние, 1 - замкнут, 0 - разомкнут, первый бит - аварийность, 1 - авария, 0 - норма)
modbus_registers[427]=(char)(tempS%256);

if(prot==MODBUS_RTU_PROT)
	{
	modbus_tx_buff[0]=adr;
	modbus_tx_buff[1]=func;

	modbus_tx_buff[2]=(char)(reg_quantity*2);

	mem_copy((char*)&modbus_tx_buff[3],(char*)&modbus_registers[(reg_adr-1)*2],reg_quantity*2);

	crc_temp=CRC16_2(modbus_tx_buff,(reg_quantity*2)+3);

	modbus_tx_buff[3+(reg_quantity*2)]=crc_temp%256;
	modbus_tx_buff[4+(reg_quantity*2)]=crc_temp/256;

	for (i=0;i<(5+(reg_quantity*2));i++)
		{
		putchar0(modbus_tx_buff[i]);
		}
	for (i=0;i<(5+(reg_quantity*2));i++)
		{
		putchar_sc16is700(modbus_tx_buff[i]);
		}
	}
else if(prot==MODBUS_TCP_PROT)
	{
	modbus_tcp_out_ptr=(char*)&modbus_registers[(reg_adr-1)*2];
	} */

	//   	};


//-----------------------------------------------
unsigned short CRC16_2(char* buf, short len)
{
unsigned short crc = 0xFFFF;
short pos;
short i;

for (pos = 0; pos < len; pos++)
  	{
    	crc ^= (unsigned short)buf[pos];          // XOR byte into least sig. byte of crc

    	for ( i = 8; i != 0; i--) 
		{    // Loop over each bit
      	if ((crc & 0x0001) != 0) 
			{      // If the LSB is set
        		crc >>= 1;                    // Shift right and XOR 0xA001
        		crc ^= 0xA001;
      		}
      	else  crc >>= 1;                    // Just shift right
    		}
  	}
  // Note, this number has low and high bytes swapped, so use it accordingly (or swap bytes)
return crc;
}


//-----------------------------------------------
void modbus_in(void)
{
short crc16_calculated;		//вычисляемая из принятых данных CRC
short crc16_incapsulated;	//встроеннная в посылку CRC
unsigned short modbus_rx_arg0;		//встроенный в посылку первый аргумент
unsigned short modbus_rx_arg1;		//встроенный в посылку второй аргумент
//unsigned short modbus_rx_arg2;		//встроенный в посылку третий аргумент
//unsigned short modbus_rx_arg3;		//встроенный в посылку четвертый аргумент
unsigned char modbus_func;			//встроенный в посылку код функции
char i_cnt, j_cnt; //o_2
char i;

mem_copy(modbus_an_buffer,modbus_rx_buffer,modbus_rx_buffer_ptr);
modbus_rx_counter=modbus_rx_buffer_ptr;
modbus_rx_buffer_ptr=0;
bMODBUS_TIMEOUT=0;
	
crc16_calculated  = CRC16_2((char*)modbus_an_buffer, modbus_rx_counter-2);
crc16_incapsulated = *((short*)&modbus_an_buffer[modbus_rx_counter-2]);

modbus_plazma1=modbus_rx_counter;
modbus_plazma2=crc16_calculated;
modbus_plazma3=crc16_incapsulated;

modbus_func=modbus_an_buffer[1];
modbus_rx_arg0=(((unsigned short)modbus_an_buffer[2])*((unsigned short)256))+((unsigned short)modbus_an_buffer[3]);
modbus_rx_arg1=(((unsigned short)modbus_an_buffer[4])*((unsigned short)256))+((unsigned short)modbus_an_buffer[5]);
//modbus_rx_arg2=(((unsigned short)modbus_an_buffer[6])*((unsigned short)256))+((unsigned short)modbus_an_buffer[7]);
//modbus_rx_arg3=(((unsigned short)modbus_an_buffer[8])*((unsigned short)256))+((unsigned short)modbus_an_buffer[9]);

//#define IPS_CURR_AVG_MODBUS_ADRESS	1

/*
if(modbus_an_buffer[0]=='r')
	{
	//pvlk=1;
	if(modbus_an_buffer[1]=='e')
		{
//		pvlk=2;
		if(modbus_an_buffer[2]=='a')
			{
//			pvlk=3;
			if(modbus_an_buffer[3]=='d')
				{
//				pvlk=4;
				if(modbus_an_buffer[6]==crc_87(modbus_an_buffer,6))
					{
//					pvlk=5;
					if(modbus_an_buffer[7]==crc_95(modbus_an_buffer,6))
						{
//						pvlk=6;	

							{
							unsigned short ptr;
							unsigned long data1,data2;
							char temp_out[20];
//							pvlk++;
							ptr=modbus_an_buffer[4]+(modbus_an_buffer[5]*256U);
							data1=lc640_read_long(ptr);
							data2=lc640_read_long(ptr+4);
							temp_out[0]='r';
							temp_out[1]='e';
							temp_out[2]='a';
							temp_out[3]='d';
							temp_out[4]=*((char*)&ptr);
							temp_out[5]=*(((char*)&ptr)+1);	
							temp_out[6]=*((char*)&data1);
							temp_out[7]=*(((char*)&data1)+1);		
							temp_out[8]=*(((char*)&data1)+2);	
							temp_out[9]=*(((char*)&data1)+3);		
							temp_out[10]=*((char*)&data2);
							temp_out[11]=*(((char*)&data2)+1);		
							temp_out[12]=*(((char*)&data2)+2);	
							temp_out[13]=*(((char*)&data2)+3);	
							temp_out[14]=crc_87(temp_out,14);	
							temp_out[15]=crc_95(temp_out,14);			
							
							temp_out[17]=0;
							for (i=0;i<16;i++)
								{
								putchar_sc16is700(temp_out[i]);
								temp_out[17]^=temp_out[i];
								}
							putchar_sc16is700(16);
							putchar_sc16is700(temp_out[17]^16);
							putchar_sc16is700(0x0a);
							}
						}
					}
				}
			} 
		}	 
	} 

if(modbus_an_buffer[0]=='w')
	{
//	pvlk=1;
	if(modbus_an_buffer[1]=='r')
		{
//		pvlk=2;
		if(modbus_an_buffer[2]=='i')
			{
//			pvlk=3;
			if(modbus_an_buffer[3]=='t')
				{
//				pvlk=4;
				if(modbus_an_buffer[4]=='e')
					{
//					pvlk=5;
					if(modbus_an_buffer[15]==crc_87(modbus_an_buffer,15))
						{
//						pvlk=6;
						if(modbus_an_buffer[16]==crc_95(modbus_an_buffer,15))

							{
							unsigned short ptr;
							unsigned long data1,data2;
							char temp_out[20];
//							pvlk=7;
							ptr=modbus_an_buffer[5]+(modbus_an_buffer[6]*256U);
							*((char*)&data1)=modbus_an_buffer[7];
							*(((char*)&data1)+1)=modbus_an_buffer[8];
							*(((char*)&data1)+2)=modbus_an_buffer[9];
							*(((char*)&data1)+3)=modbus_an_buffer[10];
							*((char*)&data2)=modbus_an_buffer[11];
							*(((char*)&data2)+1)=modbus_an_buffer[12];
							*(((char*)&data2)+2)=modbus_an_buffer[13];
							*(((char*)&data2)+3)=modbus_an_buffer[14];	
							lc640_write_long(ptr,data1);
							lc640_write_long(ptr+4,data2);
							
							//data1=lc640_read_long(ptr);
							//data2=lc640_read_long(ptr+4);
							temp_out[0]='w';
							temp_out[1]='r';
							temp_out[2]='i';
							temp_out[3]='t';
							temp_out[4]='e';
							temp_out[5]=*((char*)&ptr);
							temp_out[6]=*(((char*)&ptr)+1);	
						
							temp_out[7]=crc_87(temp_out,7);	
							temp_out[8]=crc_95(temp_out,7);			
							
							temp_out[10]=0;
							for (i=0;i<9;i++)
								{
								putchar_sc16is700(temp_out[i]);
								temp_out[10]^=temp_out[i];
								}
							putchar_sc16is700(9);
							putchar_sc16is700(temp_out[10]^9);
							putchar_sc16is700(0x0a);
							}
						}
					}
				}
		   	}
		}
	}
 */
if(crc16_calculated==crc16_incapsulated)
	{
//	ica_plazma[4]++;
 	if(modbus_an_buffer[0]==MODBUS_ADRESS)	  //o_12_e
		{
		modbus_modbus_adress_eq++;
		if(modbus_func==3)		//чтение произвольного кол-ва регистров хранения
			{
			modbus_plazma++;
			modbus_hold_registers_transmit(MODBUS_ADRESS,modbus_func,modbus_rx_arg0,modbus_rx_arg1,MODBUS_RTU_PROT);
			}

		if(modbus_func==4)		//чтение произвольного кол-ва регистров	входов
			{
			modbus_input_registers_transmit(MODBUS_ADRESS,modbus_func,modbus_rx_arg0,modbus_rx_arg1,MODBUS_RTU_PROT);
			modbus_modbus4f_cnt++;
			}

		else if(modbus_func==6) 	//запись регистров хранения
			{
			if(modbus_rx_arg0==11)		//Установка времени 
				{
				LPC_RTC->YEAR=(uint16_t)modbus_rx_arg1;
				}
			if(modbus_rx_arg0==12)		//Установка времени 
				{
				LPC_RTC->MONTH=(uint16_t)modbus_rx_arg1;
				}
			if(modbus_rx_arg0==13)		//Установка времени 
				{
				LPC_RTC->DOM=(uint16_t)modbus_rx_arg1;
				}
			if(modbus_rx_arg0==14)		//Установка времени 
				{
				LPC_RTC->HOUR=(uint16_t)modbus_rx_arg1;
				}
			if(modbus_rx_arg0==15)		//Установка времени 
				{
				LPC_RTC->MIN=(uint16_t)modbus_rx_arg1;
				}
			if(modbus_rx_arg0==16)		//Установка времени 
				{
				LPC_RTC->SEC=(uint16_t)modbus_rx_arg1;
				}
			if(modbus_rx_arg0==20)		//ток стабилизации для режима стабилизации тока
				{
				if((modbus_rx_arg1>=0)&&(modbus_rx_arg1<=18))
				lc640_write_int(EE_NUMIST,modbus_rx_arg1);  
				}
			if(modbus_rx_arg0==21)		//ток стабилизации для режима стабилизации тока
				{
				//if((modbus_rx_arg1==0)||(modbus_rx_arg1==1))
//				lc640_write_int(EE_PAR,modbus_rx_arg1);  
				}
			if(modbus_rx_arg0==22)		//ток стабилизации для режима стабилизации тока
				{
				if((modbus_rx_arg1==0)||(modbus_rx_arg1==1))
				lc640_write_int(EE_ZV_ON,modbus_rx_arg1);  
				}
			if(modbus_rx_arg0==23)		//ток стабилизации для режима стабилизации тока
				{
				//if((modbus_rx_arg1==0)||(modbus_rx_arg1==1))
				//lc640_write_int(EE_TERMOKOMP,modbus_rx_arg1);  
				}
			if(modbus_rx_arg0==24)		//ток стабилизации для режима стабилизации тока
				{
				if(/*(modbus_rx_arg1>=0)||*/(modbus_rx_arg1<=20))
				lc640_write_int(EE_UBM_AV,modbus_rx_arg1);  
				}


			if(modbus_rx_arg0==30)		//напряжение стабилизации для режима стабилизации напряжения
				{
				if((modbus_rx_arg1>0)&&(modbus_rx_arg1<5))modbus_rx_arg1=0;
				else if(modbus_rx_arg1>=60)TBAT=60;
				else TBAT=modbus_rx_arg1;
				lc640_write_int(EE_TBAT,TBAT);

				//main_kb_cnt=(TBAT*60)-20;
	     		}
/*			if(modbus_rx_arg0==31)		//
				{
				lc640_write_int(EE_UMAX,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==32)		//
				{
//				lc640_write_int(EE_DU,UB20-modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==33)		//
				{
				lc640_write_int(EE_UB0,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==34)		//
				{
				lc640_write_int(EE_UB20,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==35)		//
				{
				lc640_write_int(EE_USIGN,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==36)		//
				{
				lc640_write_int(EE_UMN,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==37)		//
				{
				lc640_write_int(EE_U0B,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==38)		//
				{
				lc640_write_int(EE_IKB,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==39)		//
				{
				lc640_write_int(EE_IZMAX,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==40)		//
				{
				lc640_write_int(EE_IMAX,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==41)		//
				{
				lc640_write_int(EE_IMIN,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==42)		//
				{
				lc640_write_int(EE_UVZ,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==43)		//
				{
				if((modbus_rx_arg1>=0)&&(modbus_rx_arg1<=3))lc640_write_int(EE_TZAS,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==44)		//
				{
				lc640_write_int(EE_TMAX,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==45)		//
				{
				lc640_write_int(EE_TSIGN,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==46)		//
				{
//				lc640_write_int(EE_TBATMAX,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==47)		//
				{
//				lc640_write_int(EE_TBATSIGN,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==48)		//
				{
//				lc640_write_int(EE_SPEED_CHRG_CURR,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==49)		//
				{
//				lc640_write_int(EE_SPEED_CHRG_VOLT,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==50)		//
				{
//				lc640_write_int(EE_SPEED_CHRG_TIME,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==51)		//
				{
//				lc640_write_int(EE_U_OUT_KONTR_MAX,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==52)		//
				{
//				lc640_write_int(EE_U_OUT_KONTR_MIN,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==53)		//
				{
//				if((modbus_rx_arg1>=5)&&(modbus_rx_arg1<=100))lc640_write_int(EE_U_OUT_KONTR_DELAY,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==54)		//
				{
				lc640_write_int(EE_UB0,modbus_rx_arg1);
				lc640_write_int(EE_UB20,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==55)		//
				{
				lc640_write_int(EE_UMAXN,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==56)		
				{
				if(modbus_rx_arg1<=3) lc640_write_int(EE_SNTP_ENABLE,modbus_rx_arg1);
	     		}						
			if(modbus_rx_arg0==57)		
				{
				signed short www=(signed short)modbus_rx_arg1;
				if(www>=-12 && www<=13) lc640_write_int(EE_SNTP_GMT,modbus_rx_arg1);
	     		}						
			if(modbus_rx_arg0==58)		
				{
				if(modbus_rx_arg1<=255) lc640_write_int(EE_SNTP_IP1,modbus_rx_arg1);
	     		}						
			if(modbus_rx_arg0==59)		
				{
				if(modbus_rx_arg1<=255) lc640_write_int(EE_SNTP_IP2,modbus_rx_arg1);
	     		}						
			if(modbus_rx_arg0==60)		
				{
				if(modbus_rx_arg1<=255) lc640_write_int(EE_SNTP_IP3,modbus_rx_arg1);
	     		}						
			if(modbus_rx_arg0==61)		
				{
				if(modbus_rx_arg1<=255) lc640_write_int(EE_SNTP_IP4,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==62)		
				{
				#ifdef UKU_220_IPS_TERMOKOMPENSAT
				if(modbus_rx_arg1<=2) lc640_write_int(EE_NUMBAT,modbus_rx_arg1);
				#else
				if(modbus_rx_arg1<=1) lc640_write_int(EE_NUMBAT,modbus_rx_arg1);
				#endif
	     		}
			if(modbus_rx_arg0==63)		
				{
				if(modbus_rx_arg1<=3) lc640_write_int(EE_NUMDT,modbus_rx_arg1);
	     		}	
			if(modbus_rx_arg0==64)		
				{
//				if(modbus_rx_arg1==0 || modbus_rx_arg1==2 || modbus_rx_arg1==4) lc640_write_int(EE_NUMMAKB,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==65)		
				{
//				if(modbus_rx_arg1<=4) lc640_write_int(EE_NUMSK,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==66)		
				{
//				if(modbus_rx_arg1==0 || modbus_rx_arg1==1) lc640_write_int(EE_NUM_RKI,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==67)		
				{
//				if(modbus_rx_arg1==0 || modbus_rx_arg1==1) lc640_write_int(EE_NUM_NET_IN,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==68)		
				{
//				if(modbus_rx_arg1==0 || modbus_rx_arg1==1) lc640_write_int(EE_NUMBDR,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==69)		
				{
//				if(modbus_rx_arg1<9) lc640_write_int(EE_NUMENMV,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==70)		
				{
//				if(modbus_rx_arg1>=1 && modbus_rx_arg1<=200) lc640_write_int(EE_BAT_C_POINT_NUM_ELEM,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==71)		
				{
//				if(modbus_rx_arg1>=10 && modbus_rx_arg1<=25000) lc640_write_int(EE_BAT_C_POINT_20,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==72)		
				{
//				if(modbus_rx_arg1>=10 && modbus_rx_arg1<=21000) lc640_write_int(EE_BAT_C_POINT_10,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==73)		
				{
//				if(modbus_rx_arg1>=10 && modbus_rx_arg1<=20000) lc640_write_int(EE_BAT_C_POINT_5,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==74)		
				{
//				if(modbus_rx_arg1>=10 && modbus_rx_arg1<=18000) lc640_write_int(EE_BAT_C_POINT_3,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==75)		
				{
//				if(modbus_rx_arg1>=10 && modbus_rx_arg1<=16000) lc640_write_int(EE_BAT_C_POINT_1,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==76)		
				{
//				if(modbus_rx_arg1>=10 && modbus_rx_arg1<=13000) lc640_write_int(EE_BAT_C_POINT_1_2,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==77)		
				{
//				if(modbus_rx_arg1>=10 && modbus_rx_arg1<=8000) lc640_write_int(EE_BAT_C_POINT_1_6,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==78)		
				{
//				if(modbus_rx_arg1>=10 && modbus_rx_arg1<=10000) lc640_write_int(EE_BAT_U_END_20,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==79)		
				{
//				if(modbus_rx_arg1>=10 && modbus_rx_arg1<=10000) lc640_write_int(EE_BAT_U_END_10,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==80)		
				{
//				if(modbus_rx_arg1>=10 && modbus_rx_arg1<=10000) lc640_write_int(EE_BAT_U_END_5,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==81)		
				{
//				if(modbus_rx_arg1>=10 && modbus_rx_arg1<=10000) lc640_write_int(EE_BAT_U_END_3,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==82)		
				{
//				if(modbus_rx_arg1>=10 && modbus_rx_arg1<=10000) lc640_write_int(EE_BAT_U_END_1,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==83)		
				{
//				if(modbus_rx_arg1>=10 && modbus_rx_arg1<=10000) lc640_write_int(EE_BAT_U_END_1_2,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==84)		
				{
//				if(modbus_rx_arg1>=10 && modbus_rx_arg1<=10000) lc640_write_int(EE_BAT_U_END_1_6,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==85)		
				{
//				if(modbus_rx_arg1>=10 && modbus_rx_arg1<=100) lc640_write_int(EE_BAT_K_OLD,modbus_rx_arg1);
	     		}		
			if(modbus_rx_arg0==86)		
				{
//				if(modbus_rx_arg1>=15 && modbus_rx_arg1<=250) lc640_write_int(EE_UVENTOFF,modbus_rx_arg1);
	     		}			
			if(modbus_rx_arg0==87)		
				{
//				if(modbus_rx_arg1>=10 && modbus_rx_arg1<=2000) lc640_write_int(EE_IMAX_VZ,modbus_rx_arg1);
	     		}  
			if(modbus_rx_arg0==88)		
				{
//				if(modbus_rx_arg1<=72) lc640_write_int(EE_VZ_HR,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==89)		
				{
//				if(modbus_rx_arg1==0 || modbus_rx_arg1==1) lc640_write_int(EE_VZ_CH_VENT_BLOK,modbus_rx_arg1);
	     		} 
			if(modbus_rx_arg0==90)		
				{
//				if(modbus_rx_arg1==0 || modbus_rx_arg1==1) lc640_write_int(EE_SPEED_CHRG_AVT_EN,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==91)		
				{
//				if(modbus_rx_arg1>0 && modbus_rx_arg1<=100) lc640_write_int(EE_SPEED_CHRG_D_U,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==92)		
				{
//				if(modbus_rx_arg1==0 || modbus_rx_arg1==2) lc640_write_int(EE_SPEED_CHRG_BLOCK_SRC,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==93)		
				{
//				if(modbus_rx_arg1==0 || modbus_rx_arg1==1) lc640_write_int(EE_SPEED_CHRG_BLOCK_LOG,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==94)		
				{								
//				if(modbus_rx_arg1==0 || modbus_rx_arg1==1) lc640_write_int(EE_SP_CH_VENT_BLOK,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==95)		
				{								
//				if(modbus_rx_arg1>=UB20 && modbus_rx_arg1<=2600) lc640_write_int(EE_UZ_U,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==96)		
				{
//				if(modbus_rx_arg1>=10 && modbus_rx_arg1<=10000) lc640_write_int(EE_UZ_IMAX,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==97)		
				{
//				if(modbus_rx_arg1>=1 && modbus_rx_arg1<=72) lc640_write_int(EE_UZ_T,modbus_rx_arg1);
	     		}					  
			if(modbus_rx_arg0==98)		
				{
//				if(modbus_rx_arg1>=UB20 && modbus_rx_arg1<=3000) lc640_write_int(EE_FZ_U1,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==99)		
				{
//				if(modbus_rx_arg1>=10 && modbus_rx_arg1<=1000) lc640_write_int(EE_FZ_IMAX1,modbus_rx_arg1);
	     		}
			// 2 регистра внизу
			if(modbus_rx_arg0==102)		
				{
//				if(modbus_rx_arg1>=1 && modbus_rx_arg1<=10) lc640_write_int(EE_FZ_T1,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==103)		
				{
//				if(modbus_rx_arg1>=10 && modbus_rx_arg1<=1000) lc640_write_int(EE_FZ_ISW12,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==104)		
				{
//				if(modbus_rx_arg1>=UB20 && modbus_rx_arg1<=3000) lc640_write_int(EE_FZ_U2,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==105)		
				{
//				if(modbus_rx_arg1>=10 && modbus_rx_arg1<=1000) lc640_write_int(EE_FZ_IMAX2,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==106)		
				{
//				if(modbus_rx_arg1>=1 && modbus_rx_arg1<=10) lc640_write_int(EE_FZ_T2,modbus_rx_arg1);
	     		}	 
			if(modbus_rx_arg0==107)		
				{
				if(modbus_rx_arg1==0 || modbus_rx_arg1==1) lc640_write_int(EE_AV_OFF_AVT,modbus_rx_arg1);
	     		}				
		   	if(modbus_rx_arg0==108)		
				{
				if(modbus_rx_arg1==0 || modbus_rx_arg1==1) lc640_write_int(EE_APV_ON1,modbus_rx_arg1);
	     		}
		  	if(modbus_rx_arg0==109)		
				{
				if(modbus_rx_arg1==0 || modbus_rx_arg1==1) lc640_write_int(EE_APV_ON2,modbus_rx_arg1);
	     		}		   
		 	if(modbus_rx_arg0==110)		
				{
				if(modbus_rx_arg1>=1 && modbus_rx_arg1<=24) lc640_write_int(EE_APV_ON2_TIME,modbus_rx_arg1);
	     		}  
			if(modbus_rx_arg0==111)		
				{
				if(modbus_rx_arg1==0)	   lc640_write_int(EE_SK_SIGN0,0xFFFF);
				else if(modbus_rx_arg1==1) lc640_write_int(EE_SK_SIGN0,0);
	     		} 	   
			if(modbus_rx_arg0==112)		
				{
				if(modbus_rx_arg1==0)	   lc640_write_int(EE_SK_ZVUK_EN0,0xFFFF);
				else if(modbus_rx_arg1==1) lc640_write_int(EE_SK_ZVUK_EN0,0);
	     		}
			if(modbus_rx_arg0==113)		
				{
				if(modbus_rx_arg1==0)	   lc640_write_int(EE_SK_LCD_EN0,0xFFFF);
				else if(modbus_rx_arg1==1) lc640_write_int(EE_SK_LCD_EN0,0);
	     		}
			if(modbus_rx_arg0==114)		
				{
				if(modbus_rx_arg1==0)	   lc640_write_int(EE_SK_SIGN1,0xFFFF);
				else if(modbus_rx_arg1==1) lc640_write_int(EE_SK_SIGN1,0);
	     		} 	   
			if(modbus_rx_arg0==115)		
				{
				if(modbus_rx_arg1==0)	   lc640_write_int(EE_SK_ZVUK_EN1,0xFFFF);
				else if(modbus_rx_arg1==1) lc640_write_int(EE_SK_ZVUK_EN1,0);
	     		}
			if(modbus_rx_arg0==116)		
				{
				if(modbus_rx_arg1==0)	   lc640_write_int(EE_SK_LCD_EN1,0xFFFF);
				else if(modbus_rx_arg1==1) lc640_write_int(EE_SK_LCD_EN1,0);
	     		}
			if(modbus_rx_arg0==117)		
				{
				if(modbus_rx_arg1==0)	   lc640_write_int(EE_SK_SIGN2,0xFFFF);
				else if(modbus_rx_arg1==1) lc640_write_int(EE_SK_SIGN2,0);
	     		} 	   
			if(modbus_rx_arg0==118)		
				{
				if(modbus_rx_arg1==0)	   lc640_write_int(EE_SK_ZVUK_EN2,0xFFFF);
				else if(modbus_rx_arg1==1) lc640_write_int(EE_SK_ZVUK_EN2,0);
	     		}
			if(modbus_rx_arg0==119)		
				{
				if(modbus_rx_arg1==0)	   lc640_write_int(EE_SK_LCD_EN2,0xFFFF);
				else if(modbus_rx_arg1==1) lc640_write_int(EE_SK_LCD_EN2,0);
	     		}
			if(modbus_rx_arg0==120)		
				{
				if(modbus_rx_arg1==0)	   lc640_write_int(EE_SK_SIGN3,0xFFFF);
				else if(modbus_rx_arg1==1) lc640_write_int(EE_SK_SIGN3,0);
	     		} 	   
			if(modbus_rx_arg0==121)		
				{
				if(modbus_rx_arg1==0)	   lc640_write_int(EE_SK_ZVUK_EN3,0xFFFF);
				else if(modbus_rx_arg1==1) lc640_write_int(EE_SK_ZVUK_EN3,0);
	     		}
			if(modbus_rx_arg0==122)		
				{
				if(modbus_rx_arg1==0)	   lc640_write_int(EE_SK_LCD_EN3,0xFFFF);
				else if(modbus_rx_arg1==1) lc640_write_int(EE_SK_LCD_EN3,0);
	     		}		  
		   	if(modbus_rx_arg0==123)		
				{
//				if(modbus_rx_arg1==0 || modbus_rx_arg1==1) lc640_write_int(EE_TERMOKOMP,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==124)		
				{
				if(modbus_rx_arg1<=500) lc640_write_int(EE_FORVARDBPSCHHOUR,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==125)		
				{
				if(modbus_rx_arg1==0 || modbus_rx_arg1==1) lc640_write_int(EE_DOP_RELE_FUNC,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==126)		
				{
				//if(modbus_rx_arg1<=2) lc640_write_int(EE_IPS_BLOCK_SRC,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==127)		
				{
				//if(modbus_rx_arg1==0 || modbus_rx_arg1==1) lc640_write_int(EE_IPS_BLOCK_LOG,modbus_rx_arg1);
	     		} 
		   	if(modbus_rx_arg0==128)		
				{
				if(modbus_rx_arg1>0 && modbus_rx_arg1<=100) lc640_write_int(EE_MODBUS_ADRESS,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==129)		
				{
				if(modbus_rx_arg1==120 || modbus_rx_arg1==240 || modbus_rx_arg1==480 || modbus_rx_arg1==960 || modbus_rx_arg1==1920 
				|| modbus_rx_arg1==3840 || modbus_rx_arg1==5760 || modbus_rx_arg1==11520){ 
					//lc640_write_int(EE_MODBUS_BAUDRATE,modbus_rx_arg1);
					//MODBUS_BAUDRATE=modbus_rx_arg1;
					}
	     		} 	
			if(modbus_rx_arg0==130)		
				{
				if(modbus_rx_arg1==0 || modbus_rx_arg1==1) lc640_write_int(EE_ETH_IS_ON,modbus_rx_arg1);
	     		}	 
			if(modbus_rx_arg0==131)		
				{
				if(modbus_rx_arg1==0 || modbus_rx_arg1==1) lc640_write_int(EE_ETH_DHCP_ON,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==132)  //IP адрес
				{
				if(modbus_rx_arg1<=255) lc640_write_int(EE_ETH_IP_1,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==133)		
				{
				if(modbus_rx_arg1<=255) lc640_write_int(EE_ETH_IP_2,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==134)		
				{
				if(modbus_rx_arg1<=255) lc640_write_int(EE_ETH_IP_3,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==135)		
				{
				if(modbus_rx_arg1<=255) lc640_write_int(EE_ETH_IP_4,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==136)	//маска подсети	
				{
				if(modbus_rx_arg1<=255) lc640_write_int(EE_ETH_MASK_1,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==137)		
				{
				if(modbus_rx_arg1<=255) lc640_write_int(EE_ETH_MASK_2,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==138)		
				{
				if(modbus_rx_arg1<=255) lc640_write_int(EE_ETH_MASK_3,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==139)		
				{
				if(modbus_rx_arg1<=255) lc640_write_int(EE_ETH_MASK_4,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==140)	//шлюз
				{
				if(modbus_rx_arg1<=255) lc640_write_int(EE_ETH_GW_1,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==141)		
				{
				if(modbus_rx_arg1<=255) lc640_write_int(EE_ETH_GW_2,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==142)		
				{
				if(modbus_rx_arg1<=255) lc640_write_int(EE_ETH_GW_3,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==143)		
				{
				if(modbus_rx_arg1<=255) lc640_write_int(EE_ETH_GW_4,modbus_rx_arg1);
	     		}	 
			if(modbus_rx_arg0==144)		
				{
				lc640_write_int(EE_ETH_SNMP_PORT_READ,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==145)		
				{
				lc640_write_int(EE_ETH_SNMP_PORT_WRITE,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==146)		
				{
				if(modbus_rx_arg1<=255) lc640_write_int(EE_COMMUNITY,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==147)		
				{
				if(modbus_rx_arg1<=255) lc640_write_int(EE_COMMUNITY+2,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==148)		
				{
				if(modbus_rx_arg1<=255) lc640_write_int(EE_COMMUNITY+4,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==149)		
				{
				if(modbus_rx_arg1<=255) lc640_write_int(EE_COMMUNITY+6,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==150)		
				{
				if(modbus_rx_arg1<=255) lc640_write_int(EE_COMMUNITY+8,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==151)		
				{
				if(modbus_rx_arg1<=255) lc640_write_int(EE_COMMUNITY+10,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==152)		
				{
				if(modbus_rx_arg1<=255) lc640_write_int(EE_COMMUNITY+12,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==153)		
				{
				if(modbus_rx_arg1<=255) lc640_write_int(EE_COMMUNITY+14,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==154)  //TRAP1 IP адрес
				{
				if(modbus_rx_arg1<=255) lc640_write_int(EE_ETH_TRAP1_IP_1,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==155)		
				{
				if(modbus_rx_arg1<=255) lc640_write_int(EE_ETH_TRAP1_IP_2,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==156)		
				{
				if(modbus_rx_arg1<=255) lc640_write_int(EE_ETH_TRAP1_IP_3,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==157)		
				{
				if(modbus_rx_arg1<=255) lc640_write_int(EE_ETH_TRAP1_IP_4,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==158)  //TRAP2 IP адрес
				{
				if(modbus_rx_arg1<=255) lc640_write_int(EE_ETH_TRAP2_IP_1,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==159)		
				{
				if(modbus_rx_arg1<=255) lc640_write_int(EE_ETH_TRAP2_IP_2,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==160)		
				{
				if(modbus_rx_arg1<=255) lc640_write_int(EE_ETH_TRAP2_IP_3,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==161)		
				{
				if(modbus_rx_arg1<=255) lc640_write_int(EE_ETH_TRAP2_IP_4,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==162)  //TRAP3 IP адрес
				{
				if(modbus_rx_arg1<=255) lc640_write_int(EE_ETH_TRAP3_IP_1,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==163)		
				{
				if(modbus_rx_arg1<=255) lc640_write_int(EE_ETH_TRAP3_IP_2,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==164)		
				{
				if(modbus_rx_arg1<=255) lc640_write_int(EE_ETH_TRAP3_IP_3,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==165)		
				{
				if(modbus_rx_arg1<=255) lc640_write_int(EE_ETH_TRAP3_IP_4,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==166)  //TRAP4 IP адрес
				{
				if(modbus_rx_arg1<=255) lc640_write_int(EE_ETH_TRAP4_IP_1,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==167)		
				{
				if(modbus_rx_arg1<=255) lc640_write_int(EE_ETH_TRAP4_IP_2,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==168)		
				{
				if(modbus_rx_arg1<=255) lc640_write_int(EE_ETH_TRAP4_IP_3,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==169)		
				{
				if(modbus_rx_arg1<=255) lc640_write_int(EE_ETH_TRAP4_IP_4,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==170)  //TRAP5 IP адрес
				{
				if(modbus_rx_arg1<=255) lc640_write_int(EE_ETH_TRAP5_IP_1,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==171)		
				{
				if(modbus_rx_arg1<=255) lc640_write_int(EE_ETH_TRAP5_IP_2,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==172)		
				{
				if(modbus_rx_arg1<=255) lc640_write_int(EE_ETH_TRAP5_IP_3,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==173)		
				{
				if(modbus_rx_arg1<=255) lc640_write_int(EE_ETH_TRAP5_IP_4,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==174)		
				{
				if(modbus_rx_arg1<=255) lc640_write_int(EE_WEB_PASSWORD,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==175)		
				{
				if(modbus_rx_arg1<=255) lc640_write_int(EE_WEB_PASSWORD+2,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==176)		
				{
				if(modbus_rx_arg1<=255) lc640_write_int(EE_WEB_PASSWORD+4,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==177 && modbus_rx_arg1>0) bRESET_INT_WDT=1;// перегрузка УКУ, инициализировать интернет    
			if(modbus_rx_arg0==178)		
				{
				modbus_rx_arg1/=10;
//				if(modbus_rx_arg1<=6000) lc640_write_int(EE_TVENTMAX,modbus_rx_arg1);
	     		} 
		   	if(modbus_rx_arg0==179)		
				{
//				if(modbus_rx_arg1<=2) lc640_write_int(EE_ICA_EN,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==180)		
				{
				//if(modbus_rx_arg1<=2) lc640_write_int(EE_ICA_CH,modbus_rx_arg1);
	     		}
/			if(modbus_rx_arg0==185)		
				{
				if(modbus_rx_arg1>0 && modbus_rx_arg1<=254) lc640_write_int(EE_ICA_MODBUS_TCP_UNIT_ID,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==186)		
				{
				//if(modbus_rx_arg1>0 && modbus_rx_arg1<=254) lc640_write_int(EE_ICA_MODBUS_ADDRESS,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==187)		
				{
				//if(modbus_rx_arg1>=10 && modbus_rx_arg1<=100) lc640_write_int(EE_PWM_START,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==188)		
				{
				//if(modbus_rx_arg1>=1 && modbus_rx_arg1<=3) lc640_write_int(EE_KB_ALGORITM,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==189)		
				{
				//if(modbus_rx_arg1>=1 && modbus_rx_arg1<=5) lc640_write_int(EE_REG_SPEED,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==190)		
				{
				if(modbus_rx_arg1==0 || modbus_rx_arg1==1) lc640_write_int(EE_SMART_SPC,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==191)		
				{
				//if(modbus_rx_arg1==1 || modbus_rx_arg1==3) lc640_write_int(EE_NUMPHASE,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==192)		
				{
				//if(modbus_rx_arg1>=10 && modbus_rx_arg1<=100) lc640_write_int(EE_TVENTON,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==193)		
				{
				//if(modbus_rx_arg1>=10 && modbus_rx_arg1<=100) lc640_write_int(EE_TVENTOFF,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==194)		
				{
				if(modbus_rx_arg1<=2) lc640_write_int(EE_RELEVENTSIGN,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==195)		
				{
				if(modbus_rx_arg1<=2) lc640_write_int(EE_NPN_OUT,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==196)		
				{
				if(modbus_rx_arg1>=100 && modbus_rx_arg1<=2500) lc640_write_int(EE_UONPN,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==197)		
				{
				if(modbus_rx_arg1>=100 && modbus_rx_arg1<=2500) lc640_write_int(EE_UVNPN,modbus_rx_arg1);
	     		}
			if(modbus_rx_arg0==198)		
				{
				if(modbus_rx_arg1>=10 && modbus_rx_arg1<=60) lc640_write_int(EE_TZNPN,modbus_rx_arg1);
	     		}		
	/*		if(modbus_rx_arg0==199)		
				{
				if(modbus_rx_arg1<=50) lc640_write_int(EE_UBM_AV,modbus_rx_arg1);
	     		} */
				//o_10_s  */
	

 /*
modbus_registers[500]=(char)(UKUFSO_IBEP_SN>>8);				
modbus_registers[501]=(char)(UKUFSO_IBEP_SN); 
modbus_registers[502]=(char)(UKUFSO_IBEP_SN>>24);				
modbus_registers[503]=(char)(UKUFSO_IBEP_SN>>16); 
modbus_registers[504]=(char)(UKUFSO_IBEP_START_DATE_YEAR>>8);	
modbus_registers[505]=(char)(UKUFSO_IBEP_START_DATE_YEAR); 
modbus_registers[506]=(char)(UKUFSO_IBEP_START_DATE_MONTH);		
modbus_registers[507]=(char)(UKUFSO_IBEP_START_DATE_DAY); 

modbus_registers[560]=(char)(UKUFSO_BPS1_SN>>8);				
modbus_registers[561]=(char)(UKUFSO_BPS1_SN); 
modbus_registers[562]=(char)(UKUFSO_BPS1_SN>>24);				
modbus_registers[563]=(char)(UKUFSO_BPS1_SN>>16); 
modbus_registers[564]=(char)(UKUFSO_BPS1_START_DATE_YEAR>>8);	
modbus_registers[565]=(char)(UKUFSO_BPS1_START_DATE_YEAR); 
modbus_registers[566]=(char)(UKUFSO_BPS1_START_DATE_MONTH);		
modbus_registers[567]=(char)(UKUFSO_BPS1_START_DATE_DAY);
modbus_registers[568]=(char)(UKUFSO_BPS2_SN>>8);				
modbus_registers[569]=(char)(UKUFSO_BPS2_SN); 
modbus_registers[570]=(char)(UKUFSO_BPS2_SN>>24);				
modbus_registers[571]=(char)(UKUFSO_BPS2_SN>>16); 
modbus_registers[572]=(char)(UKUFSO_BPS2_START_DATE_YEAR>>8);	
modbus_registers[573]=(char)(UKUFSO_BPS2_START_DATE_YEAR); 
modbus_registers[574]=(char)(UKUFSO_BPS2_START_DATE_MONTH);		
modbus_registers[575]=(char)(UKUFSO_BPS2_START_DATE_DAY);
modbus_registers[576]=(char)(UKUFSO_BPS3_SN>>8);				
modbus_registers[577]=(char)(UKUFSO_BPS3_SN); 
modbus_registers[578]=(char)(UKUFSO_BPS3_SN>>24);				
modbus_registers[579]=(char)(UKUFSO_BPS3_SN>>16); 
modbus_registers[580]=(char)(UKUFSO_BPS3_START_DATE_YEAR>>8);	
modbus_registers[581]=(char)(UKUFSO_BPS3_START_DATE_YEAR); 
modbus_registers[582]=(char)(UKUFSO_BPS3_START_DATE_MONTH);		
modbus_registers[583]=(char)(UKUFSO_BPS3_START_DATE_DAY);
modbus_registers[584]=(char)(UKUFSO_BPS4_SN>>8);				
modbus_registers[585]=(char)(UKUFSO_BPS4_SN); 
modbus_registers[586]=(char)(UKUFSO_BPS4_SN>>24);				
modbus_registers[587]=(char)(UKUFSO_BPS4_SN>>16); 
modbus_registers[588]=(char)(UKUFSO_BPS4_START_DATE_YEAR>>8);	
modbus_registers[589]=(char)(UKUFSO_BPS4_START_DATE_YEAR); 
modbus_registers[590]=(char)(UKUFSO_BPS4_START_DATE_MONTH);		
modbus_registers[591]=(char)(UKUFSO_BPS4_START_DATE_DAY);
 


	*/
//#endif

			if(modbus_rx_arg0==19)		//вкл/выкл источника напр.
				{
	/*			if(modbus_rx_arg1==1)
					{
					if(work_stat!=wsPS)
						{
						work_stat=wsPS;
						time_proc=0;
						time_proc_remain=T_PROC_PS;
						restart_on_PS();
						lc640_write_int(EE_MAIN_MENU_MODE,mmmIN);
						}
					}
				if(modbus_rx_arg1==0)
					{
					if(work_stat==wsPS)
						{
						work_stat=wsOFF;
						restart_off();
						}
					} */
				}
			if(modbus_rx_arg0==20)		//вкл/выкл источника тока
				{
/*				if(modbus_rx_arg1==1)
					{
					if(work_stat!=wsGS)
						{
						work_stat=wsGS;
						time_proc=0;
						time_proc_remain=T_PROC_GS;
						lc640_write_int(EE_MAIN_MENU_MODE,mmmIT);
						}
					}
				if(modbus_rx_arg1==0)
					{
					if(work_stat==wsGS)
						{
						work_stat=wsOFF;
						restart_off();
						}
					}*/
				}
			//modbus_hold_register_transmit(MODBUS_ADRESS,modbus_func,modbus_rx_arg0);

///
//

			//modbus_hold_registers_transmit(MODBUS_ADRESS,modbus_func,modbus_rx_arg0,1,MODBUS_RTU_PROT);
				{
			/*if(prot==MODBUS_RTU_PROT)
				{*/
				mem_copy(modbus_tx_buff,modbus_rx_buffer,8);
	
				for (i=0;i<(8);i++)
					{
					putchar0(modbus_tx_buff[i]);
					}

	
			/*	}
			else if(prot==MODBUS_TCP_PROT)
				{
				modbus_tcp_out_ptr=(char*)&modbus_registers[(reg_adr-1)*2];
				}*/
				}
			}
/*			else if(modbus_func==1) {	
				if(modbus_an_buffer[2]==8){
				  for(i_cnt=0;i_cnt<8;i_cnt++) {
				  	for(j_cnt=0;j_cnt<8;j_cnt++){
					   snmp_enmv_data[i_cnt*8+j_cnt]=(modbus_an_buffer[3+i_cnt]>>j_cnt)&0x01;
					}
					enmv_data[i_cnt]=modbus_an_buffer[3+i_cnt];				   
				  }
				  enmv_on=0;
				  enmv_puts_en=1;
				}
			}*/ 

		} 
	
	}


}
/*
//-----------------------------------------------
void modbus_register_transmit(unsigned char adr,unsigned char func,unsigned short reg_adr)
{
char modbus_registers[150];
//char modbus_tx_buff[50];
unsigned short crc_temp;
char i;


modbus_registers[0]=(char)(load_U/256);					//Рег1
modbus_registers[1]=(char)(load_U%256);
modbus_registers[2]=(char)(load_I/256);					//Рег2
modbus_registers[3]=(char)(load_I%256);
modbus_registers[4]=(char)(Ib_ips_termokompensat/256);		//Рег3
modbus_registers[5]=(char)(Ib_ips_termokompensat%256);
modbus_registers[6]=(char)(t_ext[0]/256);				//Рег4
modbus_registers[7]=(char)(t_ext[0]%256);
modbus_registers[8]=(char)(net_Ua/256);					//Рег5
modbus_registers[9]=(char)(net_Ua%256);		 	
modbus_registers[10]=(char)(net_Ub/256);				//Рег6
modbus_registers[11]=(char)(net_Ub%256);
modbus_registers[12]=(char)(net_Uc/256);				//Рег7
modbus_registers[13]=(char)(net_Uc%256);
modbus_registers[14]=(char)(net_F3/256);				//Рег8
modbus_registers[15]=(char)(net_F3%256);
modbus_registers[16]=(char)(load_I/256);				//Рег9
modbus_registers[17]=(char)(load_I%256);
modbus_registers[18]=(char)(load_I/256);				//Рег10
modbus_registers[19]=(char)(load_I%256);
modbus_registers[20]=(char)((LPC_RTC->YEAR)/256);			//Рег11
modbus_registers[21]=(char)((LPC_RTC->YEAR)%256);
modbus_registers[22]=(char)((LPC_RTC->MONTH)/256);		//Рег12
modbus_registers[23]=(char)((LPC_RTC->MONTH)%256);
modbus_registers[24]=(char)((LPC_RTC->DOM)/256);			//Рег13
modbus_registers[25]=(char)((LPC_RTC->DOM)%256);
modbus_registers[26]=(char)((LPC_RTC->HOUR)/256);			//Рег14
modbus_registers[27]=(char)((LPC_RTC->HOUR)%256);
modbus_registers[28]=(char)((LPC_RTC->MIN)/256);			//Рег15
modbus_registers[29]=(char)((LPC_RTC->MIN)%256);
modbus_registers[30]=(char)((LPC_RTC->SEC)/256);			//Рег16
modbus_registers[31]=(char)((LPC_RTC->SEC)%256);
modbus_registers[32]=(char)(load_I/256);				//Рег17
modbus_registers[33]=(char)(load_I%256);
modbus_registers[34]=(char)(load_I/256);				//Рег18
modbus_registers[35]=(char)(load_I%256);
modbus_registers[36]=(char)(load_I/256);		//Рег19
modbus_registers[37]=(char)(load_I%256);
modbus_registers[38]=(char)(NUMIST/256);		//Рег20
modbus_registers[39]=(char)(NUMIST%256);
modbus_registers[40]=(char)(PAR/256);		//Рег21
modbus_registers[41]=(char)(PAR%256);
modbus_registers[42]=(char)(ZV_ON/256);		//Рег22
modbus_registers[43]=(char)(ZV_ON%256);
modbus_registers[44]=(char)(TERMOKOMPENS/256);		//Рег23
modbus_registers[45]=(char)(TERMOKOMPENS%256);
modbus_registers[46]=(char)(UBM_AV/256);		//Рег24
modbus_registers[47]=(char)(UBM_AV%256);
modbus_registers[48]=(char)(load_I/256);		//Рег25
modbus_registers[49]=(char)(load_I%256);
modbus_registers[50]=(char)(load_I/256);		//Рег26
modbus_registers[51]=(char)(load_I%256);
modbus_registers[52]=(char)(load_I/256);		//Рег27
modbus_registers[53]=(char)(load_I%256);
modbus_registers[54]=(char)(load_I/256);		//Рег28
modbus_registers[55]=(char)(load_I%256);
modbus_registers[56]=(char)(load_I/256);		//Рег29
modbus_registers[57]=(char)(load_I%256);
modbus_registers[58]=(char)(TBAT/256);			//Рег30
modbus_registers[59]=(char)(TBAT%256);
modbus_registers[60]=(char)(UMAX/256);			//Рег31
modbus_registers[61]=(char)(UMAX%256);
modbus_registers[62]=(char)((UB20-DU)/256);		//Рег32
modbus_registers[63]=(char)((UB20-DU)%256);
modbus_registers[64]=(char)(UB0/256);			//Рег33
modbus_registers[65]=(char)(UB0%256);
modbus_registers[66]=(char)(UB20/256);			//Рег34
modbus_registers[67]=(char)(UB20%256);
modbus_registers[68]=(char)(USIGN/256);		//Рег35
modbus_registers[69]=(char)(USIGN%256);
modbus_registers[70]=(char)(UMN/256);		//Рег36
modbus_registers[71]=(char)(UMN%256);
modbus_registers[72]=(char)(U0B/256);		//Рег37
modbus_registers[73]=(char)(U0B%256);
modbus_registers[74]=(char)(IKB/256);		//Рег38
modbus_registers[75]=(char)(IKB%256);
modbus_registers[76]=(char)(IZMAX/256);		//Рег39
modbus_registers[77]=(char)(IZMAX%256);
modbus_registers[78]=(char)(IMAX/256);		//Рег40
modbus_registers[79]=(char)(IMAX%256);
modbus_registers[80]=(char)(IMIN/256);		//Рег41
modbus_registers[81]=(char)(IMIN%256);
modbus_registers[82]=(char)(UVZ/256);		//Рег42
modbus_registers[83]=(char)(UVZ%256);
modbus_registers[84]=(char)(TZAS/256);		//Рег43
modbus_registers[85]=(char)(TZAS%256);
modbus_registers[86]=(char)(TMAX/256);		//Рег44
modbus_registers[87]=(char)(TMAX%256);
modbus_registers[88]=(char)(TSIGN/256);		//Рег45
modbus_registers[89]=(char)(TSIGN%256);
modbus_registers[90]=(char)(TBATMAX/256);		//Рег46
modbus_registers[91]=(char)(TBATMAX%256);
modbus_registers[92]=(char)(TBATSIGN/256);		//Рег47
modbus_registers[93]=(char)(TBATSIGN%256);
modbus_registers[94]=(char)(load_I/256);		//Рег48
modbus_registers[95]=(char)(load_I%256);
modbus_registers[96]=(char)(load_I/256);		//Рег49
modbus_registers[97]=(char)(load_I%256);
modbus_registers[98]=(char)(load_I/256);		//Рег50
modbus_registers[99]=(char)(load_I%256);

modbus_tx_buff[0]=adr;
modbus_tx_buff[1]=func;
modbus_tx_buff[2]=(char)(reg_adr/256);
modbus_tx_buff[3]=(char)(reg_adr%256);
//modbus_tx_buff[4]=(char)(reg_quantity/256);
//modbus_tx_buff[5]=(char)(reg_quantity%256);


mem_copy((char*)&modbus_tx_buff[4],(char*)&modbus_registers[(reg_adr-1)*2],2);

crc_temp=CRC16_2(modbus_tx_buff,6);

modbus_tx_buff[6]=crc_temp%256;
modbus_tx_buff[7]=crc_temp/256;

for (i=0;i<8;i++)
	{
	putchar0(modbus_tx_buff[i]);
	}
for (i=0;i<8;i++)
	{
	putchar_sc16is700(modbus_tx_buff[i]);
	}
}*/
/*//-----------------------------------------------
void modbus_registers_transmit(unsigned char adr,unsigned char func,unsigned short reg_adr,unsigned short reg_quantity)
{
char modbus_registers[100];
//char modbus_tx_buff[100];
unsigned short crc_temp;
char i;


modbus_registers[0]=(char)(load_U/256);					//Рег1
modbus_registers[1]=(char)(load_U%256);
modbus_registers[2]=(char)(load_I/256);					//Рег2
modbus_registers[3]=(char)(load_I%256);
modbus_registers[4]=(char)(Ib_ips_termokompensat/256);		//Рег3
modbus_registers[5]=(char)(Ib_ips_termokompensat%256);
modbus_registers[6]=(char)(t_ext[0]/256);				//Рег4
modbus_registers[7]=(char)(t_ext[0]%256);
modbus_registers[8]=(char)(net_Ua/256);					//Рег5
modbus_registers[9]=(char)(net_Ua%256);		 	
modbus_registers[10]=(char)(net_Ub/256);				//Рег6
modbus_registers[11]=(char)(net_Ub%256);
modbus_registers[12]=(char)(net_Uc/256);				//Рег7
modbus_registers[13]=(char)(net_Uc%256);
modbus_registers[14]=(char)(net_F3/256);				//Рег8
modbus_registers[15]=(char)(net_F3%256);
modbus_registers[16]=(char)(load_I/256);				//Рег9
modbus_registers[17]=(char)(load_I%256);
modbus_registers[18]=(char)(load_I/256);				//Рег10
modbus_registers[19]=(char)(load_I%256);
modbus_registers[20]=(char)((LPC_RTC->YEAR)/256);			//Рег11
modbus_registers[21]=(char)((LPC_RTC->YEAR)%256);
modbus_registers[22]=(char)((LPC_RTC->MONTH)/256);		//Рег12
modbus_registers[23]=(char)((LPC_RTC->MONTH)%256);
modbus_registers[24]=(char)((LPC_RTC->DOM)/256);			//Рег13
modbus_registers[25]=(char)((LPC_RTC->DOM)%256);
modbus_registers[26]=(char)((LPC_RTC->HOUR)/256);			//Рег14
modbus_registers[27]=(char)((LPC_RTC->HOUR)%256);
modbus_registers[28]=(char)((LPC_RTC->MIN)/256);			//Рег15
modbus_registers[29]=(char)((LPC_RTC->MIN)%256);
modbus_registers[30]=(char)((LPC_RTC->SEC)/256);			//Рег16
modbus_registers[31]=(char)((LPC_RTC->SEC)%256);
modbus_registers[32]=(char)(load_I/256);				//Рег17
modbus_registers[33]=(char)(load_I%256);
modbus_registers[34]=(char)(load_I/256);				//Рег18
modbus_registers[35]=(char)(load_I%256);
modbus_registers[36]=(char)(load_I/256);		//Рег19
modbus_registers[37]=(char)(load_I%256);
modbus_registers[38]=(char)(NUMIST/256);		//Рег20
modbus_registers[39]=(char)(NUMIST%256);
modbus_registers[40]=(char)(PAR/256);		//Рег21
modbus_registers[41]=(char)(PAR%256);
modbus_registers[42]=(char)(ZV_ON/256);		//Рег22
modbus_registers[43]=(char)(ZV_ON%256);
modbus_registers[44]=(char)(TERMOKOMPENS/256);		//Рег23
modbus_registers[45]=(char)(TERMOKOMPENS%256);
modbus_registers[46]=(char)(UBM_AV/256);		//Рег24
modbus_registers[47]=(char)(UBM_AV%256);
modbus_registers[48]=(char)(load_I/256);		//Рег25
modbus_registers[49]=(char)(load_I%256);
modbus_registers[50]=(char)(load_I/256);		//Рег26
modbus_registers[51]=(char)(load_I%256);
modbus_registers[52]=(char)(load_I/256);		//Рег27
modbus_registers[53]=(char)(load_I%256);
modbus_registers[54]=(char)(load_I/256);		//Рег28
modbus_registers[55]=(char)(load_I%256);
modbus_registers[56]=(char)(load_I/256);		//Рег29
modbus_registers[57]=(char)(load_I%256);
modbus_registers[58]=(char)(TBAT/256);			//Рег30
modbus_registers[59]=(char)(TBAT%256);
modbus_registers[60]=(char)(UMAX/256);			//Рег31
modbus_registers[61]=(char)(UMAX%256);
modbus_registers[62]=(char)((UB20-DU)/256);		//Рег32
modbus_registers[63]=(char)((UB20-DU)%256);
modbus_registers[64]=(char)(UB0/256);			//Рег33
modbus_registers[65]=(char)(UB0%256);
modbus_registers[66]=(char)(UB20/256);			//Рег34
modbus_registers[67]=(char)(UB20%256);
modbus_registers[68]=(char)(USIGN/256);		//Рег35
modbus_registers[69]=(char)(USIGN%256);
modbus_registers[70]=(char)(UMN/256);		//Рег36
modbus_registers[71]=(char)(UMN%256);
modbus_registers[72]=(char)(U0B/256);		//Рег37
modbus_registers[73]=(char)(U0B%256);
modbus_registers[74]=(char)(IKB/256);		//Рег38
modbus_registers[75]=(char)(IKB%256);
modbus_registers[76]=(char)(IZMAX/256);		//Рег39
modbus_registers[77]=(char)(IZMAX%256);
modbus_registers[78]=(char)(IMAX/256);		//Рег40
modbus_registers[79]=(char)(IMAX%256);
modbus_registers[80]=(char)(IMIN/256);		//Рег41
modbus_registers[81]=(char)(IMIN%256);
modbus_registers[82]=(char)(UVZ/256);		//Рег42
modbus_registers[83]=(char)(UVZ%256);
modbus_registers[84]=(char)(TZAS/256);		//Рег43
modbus_registers[85]=(char)(TZAS%256);
modbus_registers[86]=(char)(TMAX/256);		//Рег44
modbus_registers[87]=(char)(TMAX%256);
modbus_registers[88]=(char)(TSIGN/256);		//Рег45
modbus_registers[89]=(char)(TSIGN%256);
modbus_registers[90]=(char)(TBATMAX/256);		//Рег46
modbus_registers[91]=(char)(TBATMAX%256);
modbus_registers[92]=(char)(TBATSIGN/256);		//Рег47
modbus_registers[93]=(char)(TBATSIGN%256);
modbus_registers[94]=(char)(load_I/256);		//Рег48
modbus_registers[95]=(char)(load_I%256);
modbus_registers[96]=(char)(load_I/256);		//Рег49
modbus_registers[97]=(char)(load_I%256);
modbus_registers[98]=(char)(load_I/256);		//Рег50
modbus_registers[99]=(char)(load_I%256);




modbus_tx_buff[0]=adr;
modbus_tx_buff[1]=func;
//modbus_tx_buff[2]=(char)(reg_adr/256);
//modbus_tx_buff[3]=(char)(reg_adr%256);
modbus_tx_buff[2]=(char)(reg_quantity*2);
//modbus_tx_buff[5]=(char)(reg_quantity%256);


mem_copy((char*)&modbus_tx_buff[3],(char*)&modbus_registers[(reg_adr-1)*2],reg_quantity*2);

crc_temp=CRC16_2(modbus_tx_buff,(reg_quantity*2)+3);

modbus_tx_buff[3+(reg_quantity*2)]=crc_temp%256;
modbus_tx_buff[4+(reg_quantity*2)]=crc_temp/256;

//int2lcdyx(reg_quantity,0,10,0);

for (i=0;i<15;i++)
	{
	putchar0(modbus_tx_buff[i]);
	} 
for (i=0;i<15;i++)
	{
	putchar_sc16is700(modbus_tx_buff[i]);
	}
}*/
/*
//-----------------------------------------------
void modbus_hold_register_transmit(unsigned char adr,unsigned char func,unsigned short reg_adr)
{
char modbus_registers[150];
//char modbus_tx_buff[150];
unsigned short crc_temp;
char i;

modbus_registers[20]=(char)((LPC_RTC->YEAR)/256);			//Рег11  Время, год
modbus_registers[21]=(char)((LPC_RTC->YEAR)%256);
modbus_registers[22]=(char)((LPC_RTC->MONTH)/256);		//Рег12  Время, месяц
modbus_registers[23]=(char)((LPC_RTC->MONTH)%256);
modbus_registers[24]=(char)((LPC_RTC->DOM)/256);			//Рег13  Время, день месяца
modbus_registers[25]=(char)((LPC_RTC->DOM)%256);
modbus_registers[26]=(char)((LPC_RTC->HOUR)/256);			//Рег14  Время, час
modbus_registers[27]=(char)((LPC_RTC->HOUR)%256);
modbus_registers[28]=(char)((LPC_RTC->MIN)/256);			//Рег15  Время, минуты
modbus_registers[29]=(char)((LPC_RTC->MIN)%256);
modbus_registers[30]=(char)((LPC_RTC->SEC)/256);			//Рег16  Время, секунды
modbus_registers[31]=(char)((LPC_RTC->SEC)%256);
modbus_registers[38]=(char)(NUMIST/256);				//Рег20  Количество выпрямителей в структуре
modbus_registers[39]=(char)(NUMIST%256);
modbus_registers[40]=(char)(PAR/256);					//Рег21  Параллельная работа выпрямителей вкл./выкл.
modbus_registers[41]=(char)(PAR%256);
modbus_registers[42]=(char)(ZV_ON/256);					//Рег22  Звуковая аварийная сигнализация вкл./выкл.
modbus_registers[43]=(char)(ZV_ON%256);
modbus_registers[46]=(char)(UBM_AV/256);				//Рег24  Аварийный уровень отклонения напряжения средней точки батареи, %
modbus_registers[47]=(char)(UBM_AV%256);
modbus_registers[58]=(char)(TBAT/256);					//Рег30  Период проверки цепи батареи, минут.
modbus_registers[59]=(char)(TBAT%256);
modbus_registers[60]=(char)(UMAX/256);					//Рег31  Максимальное (аварийное) напряжение выпрямителей, 0.1В
modbus_registers[61]=(char)(UMAX%256);
modbus_registers[62]=(char)((UB20-DU)/256);				//Рег32  Минимальное (аварийное) напряжение выпрямителей, 0.1В
modbus_registers[63]=(char)((UB20-DU)%256);
modbus_registers[64]=(char)(UB0/256);					//Рег33  Напряжение содержания батареи при 0гЦ, 0.1В
modbus_registers[65]=(char)(UB0%256);
modbus_registers[66]=(char)(UB20/256);					//Рег34  Напряжение содержания батареи при 20гЦ, 0.1В
modbus_registers[67]=(char)(UB20%256);
modbus_registers[68]=(char)(USIGN/256);					//Рег35  Минимальное (сигнальное) напряжение батареи, 1В
modbus_registers[69]=(char)(USIGN%256);
modbus_registers[70]=(char)(UMN/256);					//Рег36  Минимальное (аварийное) напряжение питающей сети, 1В
modbus_registers[71]=(char)(UMN%256);
modbus_registers[72]=(char)(U0B/256);					//Рег37  Рабочее напряжение при невведенных батареях, 0.1В
modbus_registers[73]=(char)(U0B%256);
modbus_registers[74]=(char)(IKB/256);					//Рег38  Ток контроля наличия батареи, 0.1а
modbus_registers[75]=(char)(IKB%256);
modbus_registers[76]=(char)(IZMAX/256);					//Рег39  Ток заряда батареи максимальный, 0.1А
modbus_registers[77]=(char)(IZMAX%256);
modbus_registers[78]=(char)(IMAX/256);					//Рег40  Ток переключения на большее кол-во выпрямителей, 0.1А
modbus_registers[79]=(char)(IMAX%256);
modbus_registers[80]=(char)(IMIN/256);					//Рег41  Ток переключения на меньшее кол-во выпрямителей, 0.1А
modbus_registers[81]=(char)(IMIN%256);
modbus_registers[82]=(char)(UVZ/256);					//Рег42  Напряжение выравнивающего заряда, 0.1В
modbus_registers[83]=(char)(UVZ%256);
modbus_registers[84]=(char)(TZAS/256);					//Рег43  Время задержки включения выпрямителей, сек
modbus_registers[85]=(char)(TZAS%256);
modbus_registers[86]=(char)(TMAX/256);					//Рег44  Температура выпрямителей аварийная, 1гЦ
modbus_registers[87]=(char)(TMAX%256);
modbus_registers[88]=(char)(TSIGN/256);					//Рег45  Температура выпрямителей сигнальная, 1гЦ
modbus_registers[89]=(char)(TSIGN%256);
modbus_registers[90]=(char)(TBATMAX/256);				//Рег46  Температура батареи аварийная, 1гЦ
modbus_registers[91]=(char)(TBATMAX%256);
modbus_registers[92]=(char)(TBATSIGN/256);				//Рег47  Температура батареи сигнальная, 1гЦ
modbus_registers[93]=(char)(TBATSIGN%256);
modbus_registers[94]=(char)(speedChrgCurr/256);					//Рег48  Ток ускоренного заряда, 0.1А
modbus_registers[95]=(char)(speedChrgCurr%256);
modbus_registers[96]=(char)(speedChrgVolt/256);				//Рег49	 Напряжение ускоренного заряда, 0.1В 
modbus_registers[97]=(char)(speedChrgVolt%256);
modbus_registers[98]=(char)(speedChrgTimeInHour/256);				//Рег50	 Время ускоренного заряда, 1ч
modbus_registers[99]=(char)(speedChrgTimeInHour%256);


modbus_tx_buff[0]=adr;
modbus_tx_buff[1]=func;
modbus_tx_buff[2]=(char)(reg_adr/256);
modbus_tx_buff[3]=(char)(reg_adr%256);

mem_copy((char*)&modbus_tx_buff[4],(char*)&modbus_registers[(reg_adr-1)*2],2);

crc_temp=CRC16_2(modbus_tx_buff,6);

modbus_tx_buff[6]=crc_temp%256;
modbus_tx_buff[7]=crc_temp/256;

for (i=0;i<8;i++)
	{
	putchar0(modbus_tx_buff[i]);
	}
for (i=0;i<8;i++)
	{
	putchar_sc16is700(modbus_tx_buff[i]);
	}
}
*/

//-----------------------------------------------
void modbus_hold_registers_transmit(unsigned char adr,unsigned char func,unsigned short reg_adr,unsigned short reg_quantity, char prot)
{
signed char modbus_registers[800];	//o_10
//char modbus_tx_buff[150];
unsigned short crc_temp;
char i;

modbus_registers[20]=(char)((LPC_RTC->YEAR)>>8);			//Рег11  Время, год
modbus_registers[21]=(char)((LPC_RTC->YEAR));
modbus_registers[22]=(char)((LPC_RTC->MONTH)>>8);		//Рег12  Время, месяц
modbus_registers[23]=(char)((LPC_RTC->MONTH));
modbus_registers[24]=(char)((LPC_RTC->DOM)>>8);			//Рег13  Время, день месяца
modbus_registers[25]=(char)((LPC_RTC->DOM));
modbus_registers[26]=(char)((LPC_RTC->HOUR)>>8);			//Рег14  Время, час
modbus_registers[27]=(char)((LPC_RTC->HOUR));
modbus_registers[28]=(char)((LPC_RTC->MIN)>>8);			//Рег15  Время, минуты
modbus_registers[29]=(char)((LPC_RTC->MIN));
modbus_registers[30]=(char)((LPC_RTC->SEC)>>8);			//Рег16  Время, секунды
modbus_registers[31]=(char)((LPC_RTC->SEC));
modbus_registers[38]=(char)(NUMIST>>8);				//Рег20  Количество выпрямителей в структуре
modbus_registers[39]=(char)(NUMIST);
//modbus_registers[40]=(char)(PAR>>8);					//Рег21  Параллельная работа выпрямителей вкл./выкл.
//modbus_registers[41]=(char)(PAR);
modbus_registers[42]=(char)(ZV_ON>>8);					//Рег22  Звуковая аварийная сигнализация вкл./выкл.
modbus_registers[43]=(char)(ZV_ON);
modbus_registers[46]=(char)(UBM_AV>>8);				//Рег24  Аварийный уровень отклонения напряжения средней точки батареи, %
modbus_registers[47]=(char)(UBM_AV);
modbus_registers[58]=(char)(TBAT>>8);					//Рег30  Период проверки цепи батареи, минут.
modbus_registers[59]=(char)(TBAT);
modbus_registers[60]=(char)(UMAX>>8);					//Рег31  Максимальное (аварийное) напряжение выпрямителей, 0.1В
modbus_registers[61]=(char)(UMAX);
//modbus_registers[62]=(char)((UB20-DU)>>8);				//Рег32  Минимальное (аварийное) напряжение выпрямителей, 0.1В
//modbus_registers[63]=(char)((UB20-DU));
modbus_registers[64]=(char)(UB0>>8);					//Рег33  Напряжение содержания батареи при 0гЦ, 0.1В
modbus_registers[65]=(char)(UB0);
modbus_registers[66]=(char)(UB20>>8);					//Рег34  Напряжение содержания батареи при 20гЦ, 0.1В
modbus_registers[67]=(char)(UB20);
modbus_registers[68]=(char)(USIGN>>8);					//Рег35  Минимальное (сигнальное) напряжение батареи, 1В
modbus_registers[69]=(char)(USIGN);
modbus_registers[70]=(char)(UMN>>8);					//Рег36  Минимальное (аварийное) напряжение питающей сети, 1В
modbus_registers[71]=(char)(UMN);
modbus_registers[72]=(char)(U0B>>8);					//Рег37  Рабочее напряжение при невведенных батареях, 0.1В
modbus_registers[73]=(char)(U0B);
modbus_registers[74]=(char)(IKB>>8);					//Рег38  Ток контроля наличия батареи, 0.1а
modbus_registers[75]=(char)(IKB);
modbus_registers[76]=(char)(IZMAX>>8);					//Рег39  Ток заряда батареи максимальный, 0.1А
modbus_registers[77]=(char)(IZMAX);
modbus_registers[78]=(char)(IMAX>>8);					//Рег40  Ток переключения на большее кол-во выпрямителей, 0.1А
modbus_registers[79]=(char)(IMAX);
modbus_registers[80]=(char)(IMIN>>8);					//Рег41  Ток переключения на меньшее кол-во выпрямителей, 0.1А
modbus_registers[81]=(char)(IMIN);
//modbus_registers[82]=(char)(UVZ>>8);					//Рег42  Напряжение выравнивающего заряда, 0.1В
//modbus_registers[83]=(char)(UVZ);
modbus_registers[84]=(char)(TZAS>>8);					//Рег43  Время задержки включения выпрямителей, сек
modbus_registers[85]=(char)(TZAS);
/*modbus_registers[86]=(char)(TMAX>>8);					//Рег44  Температура выпрямителей аварийная, 1гЦ
modbus_registers[87]=(char)(TMAX);
modbus_registers[88]=(char)(TSIGN>>8);					//Рег45  Температура выпрямителей сигнальная, 1гЦ  
modbus_registers[89]=(char)(TSIGN);																		 */
//modbus_registers[90]=(char)(TBATMAX>>8);				//Рег46  Температура батареи аварийная, 1гЦ
//modbus_registers[91]=(char)(TBATMAX);
//modbus_registers[92]=(char)(TBATSIGN>>8);				//Рег47  Температура батареи сигнальная, 1гЦ
//modbus_registers[93]=(char)(TBATSIGN);
//modbus_registers[98]=(char)(speedChrgTimeInHour>>8);	//Рег50	 Время ускоренного заряда, 1ч
//modbus_registers[99]=(char)(speedChrgTimeInHour);
//modbus_registers[100]=(char)(U_OUT_KONTR_MAX>>8);		//Рег51	 Контроль выходного напряжения, Umax, 0.1В
//modbus_registers[101]=(char)(U_OUT_KONTR_MAX);
//modbus_registers[102]=(char)(U_OUT_KONTR_MIN>>8);		//Рег52	 Контроль выходного напряжения, Umin, 0.1В
//modbus_registers[103]=(char)(U_OUT_KONTR_MIN);
//modbus_registers[104]=(char)(U_OUT_KONTR_DELAY>>8);		//Рег53	 Контроль выходного напряжения, Tзадержки, 1сек.
//modbus_registers[105]=(char)(U_OUT_KONTR_DELAY);
modbus_registers[106]=(char)(UB0>>8);					//Рег54	 Установка выходного напряжения для ИПС без батареи(СГЕП-ГАЗПРОМ)
modbus_registers[107]=(char)(UB0);
modbus_registers[108]=(char)(UMAXN>>8);					//Рег55  Максимальное (аварийное) напряжение питающей сети, 1В
modbus_registers[109]=(char)(UMAXN);

modbus_registers[110]=0;								//Рег56  период синхронизации времени, 0-выкл, 1-1ч, 2-1сут., 3-1-нед.
modbus_registers[111]=(char)(SNTP_ENABLE);				
modbus_registers[112]=(char)(SNTP_GMT>>8);				//Рег57  Часовой пояс, от -12 до +13
modbus_registers[113]=(char)(SNTP_GMT);					
modbus_registers[114]=0;								//Рег58  1 число IP
modbus_registers[115]=(char)(lc640_read_int(EE_SNTP_IP1));					
modbus_registers[116]=0;								//Рег59  2 число IP
modbus_registers[117]=(char)(lc640_read_int(EE_SNTP_IP2));					
modbus_registers[118]=0;								//Рег60  3 число IP
modbus_registers[119]=(char)(lc640_read_int(EE_SNTP_IP3));					
modbus_registers[120]=0;								//Рег61  4 число IP
modbus_registers[121]=(char)(lc640_read_int(EE_SNTP_IP4));
modbus_registers[122]=0;								//Рег62  количество АКБ
modbus_registers[123]=(char)(NUMBAT);
modbus_registers[124]=0;								//Рег63  количество датчиков температуры
modbus_registers[125]=(char)(NUMDT);	
//modbus_registers[126]=0;								//Рег64  количество мониторов АКБ 0,2,4
//modbus_registers[127]=(char)(NUMMAKB);	
modbus_registers[128]=0;								//Рег65  количество СК
modbus_registers[129]=(char)(NUMSK);
//modbus_registers[130]=0;								//Рег66  количество РКИ
//modbus_registers[131]=(char)(num_rki);	
//modbus_registers[132]=0;								//Рег67  количество сетевых вводов
//modbus_registers[133]=(char)(num_net_in);
//modbus_registers[134]=0;								//Рег68  количество доп. реле
//modbus_registers[135]=(char)(NUMBDR);
//modbus_registers[136]=0;								//Рег69  количество ЭНМВ
//modbus_registers[137]=(char)(NUMENMV);
//modbus_registers[170]=(char)(UVENTOFF>>8);			   	//Рег86  Uоткл. вентилятора АКБ, 1В	 15-250
//modbus_registers[171]=(char)(UVENTOFF);
//modbus_registers[172]=(char)(IMAX_VZ>>8);				//Рег87  макс. ток выравн. заряда , 0,1А   10-2000
//modbus_registers[173]=(char)(IMAX_VZ);
modbus_registers[174]=(char)(VZ_HR>>8);					//Рег88  время выравн. заряда, если =0,  то 0,5 часа. max=72ч, 1ч
modbus_registers[175]=(char)(VZ_HR);
//modbus_registers[176]=0;								//Рег89  блокирование ВЗ вентиляцией СК1, 1-вкл
//modbus_registers[177]=(char)(VZ_CH_VENT_BLOK);
/*modbus_registers[178]=0;								//Рег90  автом. УЗ, 1-вкл
modbus_registers[179]=(char)(speedChrgAvtEn);
modbus_registers[180]=(char)(speedChrgDU>>8);			//Рег91  dU УЗ, 1В	  1-100
modbus_registers[181]=(char)(speedChrgDU);
modbus_registers[182]=0;								//Рег92  блокирование УЗ  0-выкл, 2-CK2
modbus_registers[183]=(char)(speedChrgBlckSrc);
modbus_registers[184]=0;								//Рег93  сигнал блокирование УЗ  1-замкн, 0-разомкн.
modbus_registers[185]=(char)(speedChrgBlckLog);
modbus_registers[186]=0;								//Рег94  блокирование УЗ вентиляцией СК1 1-вкл, 0-выкл.
modbus_registers[187]=(char)(SP_CH_VENT_BLOK); */
//modbus_registers[188]=(char)(UZ_U>>8);					//Рег95  Напряжение уравнительного заряда, 0,1В	  Uб20*10-2600
//modbus_registers[189]=(char)(UZ_U);
//modbus_registers[190]=(char)(UZ_IMAX>>8);				//Рег96  Ток уравнительного заряда, 0,1А  10-10000
//modbus_registers[191]=(char)(UZ_IMAX);
//modbus_registers[192]=(char)(UZ_T>>8);					//Рег97  Время работы уравнительного заряда, 1ч	 1-72
//modbus_registers[193]=(char)(UZ_T);
//modbus_registers[194]=(char)(FZ_U1>>8);					//Рег98  Напряжение формовочного заряда 1, 0,1В	  Uб20*10-3000
//modbus_registers[195]=(char)(FZ_U1);
//modbus_registers[196]=(char)(FZ_IMAX1>>8);				//Рег99  Ток формовочного заряда 1, 0,1А   10-1000
//modbus_registers[197]=(char)(FZ_IMAX1);
// здесь 2 регистра для выравнивания токов
//modbus_registers[202]=(char)(FZ_T1>>8);					//Рег102  Время работы формовочного заряда 1, 1ч 1-10
//modbus_registers[203]=(char)(FZ_T1);
//modbus_registers[204]=(char)(FZ_ISW12>>8);				//Рег103  Ток переключения ФЗ, 0,1А	  10-1000
//modbus_registers[205]=(char)(FZ_ISW12);
//modbus_registers[206]=(char)(FZ_U2>>8);					//Рег104  Напряжение формовочного заряда 2, 0,1В   Uб20*10-3000
//modbus_registers[207]=(char)(FZ_U2);
//modbus_registers[208]=(char)(FZ_IMAX2>>8);				//Рег105  Ток формовочного заряда 2, 0,1А	10-1000
//modbus_registers[209]=(char)(FZ_IMAX2);
//modbus_registers[210]=(char)(FZ_T2>>8);					//Рег106  Время работы формовочного заряда 2, 1ч  1-10
//modbus_registers[211]=(char)(FZ_T2);
modbus_registers[212]=0;								//Рег107  отключение аварийного сигнала 0-ручн., 1-автом.
modbus_registers[213]=(char)(AV_OFF_AVT);
modbus_registers[214]=0;								//Рег108  АПВ 1й уровень 0-выкл., 1-вкл.
//modbus_registers[215]=(char)(APV_ON1);
modbus_registers[216]=0;								//Рег109  АПВ 2й уровень 0-выкл., 1-вкл.
//modbus_registers[217]=(char)(APV_ON2);
modbus_registers[218]=0;								//Рег110  АПВ 2й уровень период, 1ч , 1-24ч
//modbus_registers[219]=(char)(APV_ON2_TIME);
if(SK_SIGN[0]==0){
	modbus_registers[220]=0;								//Рег111  СК1 авария: 1-замкнун, 0-разомкнут
	modbus_registers[221]=1;
}else{
	modbus_registers[220]=0;								//Рег111  СК1 авария: 1-замкнун, 0-разомкнут
	modbus_registers[221]=0;
}
if(SK_ZVUK_EN[0]==0){
	modbus_registers[222]=0;								//Рег112  СК1 авария звук: 0-выкл, 1-вкл
	modbus_registers[223]=1;
}else{
	modbus_registers[222]=0;								//Рег112  СК1 авария звук: 0-выкл, 1-вкл
	modbus_registers[223]=0;
}
if(SK_LCD_EN[0]==0){
	modbus_registers[224]=0;								//Рег113  СК1 авария ЛСД: 0-выкл, 1-вкл
	modbus_registers[225]=1;
}else{
	modbus_registers[224]=0;								//Рег113  СК1 авария ЛСД: 0-выкл, 1-вкл
	modbus_registers[225]=0;
}
if(SK_SIGN[1]==0){
	modbus_registers[226]=0;								//Рег114  СК2 авария: 1-замкнун, 0-разомкнут
	modbus_registers[227]=1;
}else{
	modbus_registers[226]=0;								//Рег114  СК2 авария: 1-замкнун, 0-разомкнут
	modbus_registers[227]=0;
}
if(SK_ZVUK_EN[1]==0){
	modbus_registers[228]=0;								//Рег115  СК2 авария звук: 0-выкл, 1-вкл
	modbus_registers[229]=1;
}else{
	modbus_registers[228]=0;								//Рег115  СК2 авария звук: 0-выкл, 1-вкл
	modbus_registers[229]=0;										   
}
if(SK_LCD_EN[1]==0){
	modbus_registers[230]=0;								//Рег116  СК2 авария ЛСД: 0-выкл, 1-вкл
	modbus_registers[231]=1;
}else{
	modbus_registers[230]=0;								//Рег116  СК2 авария ЛСД: 0-выкл, 1-вкл
	modbus_registers[231]=0;
}

if(SK_SIGN[2]==0){
	modbus_registers[232]=0;								//Рег117  СК3 авария: 1-замкнун, 0-разомкнут
	modbus_registers[233]=1;
}else{
	modbus_registers[232]=0;								//Рег117  СК3 авария: 1-замкнун, 0-разомкнут
	modbus_registers[233]=0;
}
if(SK_ZVUK_EN[2]==0){
	modbus_registers[234]=0;								//Рег118  СК3 авария звук: 0-выкл, 1-вкл
	modbus_registers[235]=1;
}else{
	modbus_registers[234]=0;								//Рег118  СК3 авария звук: 0-выкл, 1-вкл
	modbus_registers[235]=0;
}
if(SK_LCD_EN[2]==0){
	modbus_registers[236]=0;								//Рег119  СК3 авария ЛСД: 0-выкл, 1-вкл
	modbus_registers[237]=1;
}else{
	modbus_registers[236]=0;								//Рег119  СК3 авария ЛСД: 0-выкл, 1-вкл
	modbus_registers[237]=0;
}
if(SK_SIGN[3]==0){
	modbus_registers[238]=0;								//Рег120  СК4 авария: 1-замкнун, 0-разомкнут
	modbus_registers[239]=1;
}else{
	modbus_registers[238]=0;								//Рег120  СК4 авария: 1-замкнун, 0-разомкнут
	modbus_registers[239]=0;
}
if(SK_ZVUK_EN[3]==0){
	modbus_registers[240]=0;								//Рег121  СК4 авария звук: 0-выкл, 1-вкл
	modbus_registers[241]=1;
}else{
	modbus_registers[240]=0;								//Рег121  СК4 авария звук: 0-выкл, 1-вкл
	modbus_registers[241]=0;										   
}
if(SK_LCD_EN[3]==0){
	modbus_registers[242]=0;								//Рег122  СК4 авария ЛСД: 0-выкл, 1-вкл
	modbus_registers[243]=1;
}else{
	modbus_registers[242]=0;								//Рег122  СК4 авария ЛСД: 0-выкл, 1-вкл
	modbus_registers[243]=0;
}
//modbus_registers[244]=0;									//Рег123  термокомпенсация 0-выкл, 1-вкл
//modbus_registers[245]=(char)(TERMOKOMPENS);
//modbus_registers[246]=(char)(FORVARDBPSCHHOUR>>8);			//Рег124  Время ротации БПС, 1ч, 0-500ч, 0-выкл
//modbus_registers[247]=(char)(FORVARDBPSCHHOUR);
//modbus_registers[248]=0;									//Рег125  назначение доп реле 0-УЗ или ВЗ, 1-разряж.АКБ
//modbus_registers[249]=(char)(DOP_RELE_FUNC);
//modbus_registers[250]=0;									//Рег126  блокировка ИПС, 0-выкл, 1-СК1, 2-СК2
//modbus_registers[251]=(char)(ipsBlckSrc);
//modbus_registers[252]=0;									//Рег127  сигнал блокирования, 0-разомкн. 1-замкн.
//modbus_registers[253]=(char)(ipsBlckLog);
modbus_registers[254]=0;									//Рег128  адрес модбас	1-100
modbus_registers[255]=(char)(MODBUS_ADRESS);		
//modbus_registers[256]=(char)(MODBUS_BAUDRATE>>8);			//Рег129  скорость/10 
//modbus_registers[257]=(char)(MODBUS_BAUDRATE);
modbus_registers[258]=0;									//Рег130  интернет, 0-вкл. 1-выкл
modbus_registers[259]=(char)(ETH_IS_ON);
modbus_registers[260]=0;									//Рег131  интернет DHCP, 0-вкл. 1-выкл.
modbus_registers[261]=(char)(ETH_DHCP_ON);
modbus_registers[262]=0;									//Рег132  интернет IP число 1
modbus_registers[263]=(char)(ETH_IP_1);
modbus_registers[264]=0;									//Рег133  интернет IP число 2
modbus_registers[265]=(char)(ETH_IP_2);
modbus_registers[266]=0;									//Рег134  интернет IP число 3
modbus_registers[267]=(char)(ETH_IP_3);
modbus_registers[268]=0;									//Рег135  интернет IP число 4
modbus_registers[269]=(char)(ETH_IP_4);
modbus_registers[270]=0;									//Рег136  маска сети число 1
modbus_registers[271]=(char)(ETH_MASK_1);
modbus_registers[272]=0;									//Рег137  маска сети число 2
modbus_registers[273]=(char)(ETH_MASK_2);
modbus_registers[274]=0;									//Рег138  маска сети число 3
modbus_registers[275]=(char)(ETH_MASK_3);
modbus_registers[276]=0;									//Рег139  маска сети число 4
modbus_registers[277]=(char)(ETH_MASK_4);
modbus_registers[278]=0;									//Рег140  шлюз число 1
modbus_registers[279]=(char)(ETH_GW_1);
modbus_registers[280]=0;									//Рег141  шлюз число 2
modbus_registers[281]=(char)(ETH_GW_2);
modbus_registers[282]=0;									//Рег142  шлюз число 3
modbus_registers[283]=(char)(ETH_GW_3);
modbus_registers[284]=0;									//Рег143  шлюз число 4
modbus_registers[285]=(char)(ETH_GW_4);
modbus_registers[286]=(char)(ETH_SNMP_PORT_READ>>8);		//Рег144  порт чтения 
modbus_registers[287]=(char)(ETH_SNMP_PORT_READ);
modbus_registers[288]=(char)(ETH_SNMP_PORT_WRITE>>8);		//Рег145  порт записи 
modbus_registers[289]=(char)(ETH_SNMP_PORT_WRITE);
modbus_registers[290]=0;									//Рег146  пароль знак 1
modbus_registers[291]=(char)(snmp_community[0]);
modbus_registers[292]=0;									//Рег147  пароль знак 2
modbus_registers[293]=(char)(snmp_community[1]);
modbus_registers[294]=0;									//Рег148  пароль знак 3
modbus_registers[295]=(char)(snmp_community[2]);
modbus_registers[296]=0;									//Рег149  пароль знак 4
modbus_registers[297]=(char)(snmp_community[3]);
modbus_registers[298]=0;									//Рег150  пароль знак 5
modbus_registers[299]=(char)(snmp_community[4]);
modbus_registers[300]=0;									//Рег151  пароль знак 6
modbus_registers[301]=(char)(snmp_community[5]);
modbus_registers[302]=0;									//Рег152  пароль знак 7
modbus_registers[303]=(char)(snmp_community[6]);
modbus_registers[304]=0;									//Рег153  пароль знак 7
modbus_registers[305]=(char)(snmp_community[7]);
modbus_registers[306]=0;									//Рег154  TRAP1 IP число 1
modbus_registers[307]=(char)(ETH_TRAP1_IP_1);
modbus_registers[308]=0;									//Рег155  TRAP1 IP число 2
modbus_registers[309]=(char)(ETH_TRAP1_IP_2);
modbus_registers[310]=0;									//Рег156  TRAP1 IP число 3
modbus_registers[311]=(char)(ETH_TRAP1_IP_3);
modbus_registers[312]=0;									//Рег157  TRAP1 IP число 4
modbus_registers[313]=(char)(ETH_TRAP1_IP_4);
modbus_registers[314]=0;									//Рег158  TRAP2 IP число 1
modbus_registers[315]=(char)(ETH_TRAP2_IP_1);
modbus_registers[316]=0;									//Рег159  TRAP2 IP число 2
modbus_registers[317]=(char)(ETH_TRAP2_IP_2);
modbus_registers[318]=0;									//Рег160  TRAP2 IP число 3
modbus_registers[319]=(char)(ETH_TRAP2_IP_3);
modbus_registers[320]=0;									//Рег161  TRAP2 IP число 4
modbus_registers[321]=(char)(ETH_TRAP2_IP_4);
modbus_registers[322]=0;									//Рег162  TRAP3 IP число 1
modbus_registers[323]=(char)(ETH_TRAP3_IP_1);
modbus_registers[324]=0;									//Рег163  TRAP3 IP число 2
modbus_registers[325]=(char)(ETH_TRAP3_IP_2);
modbus_registers[326]=0;									//Рег164  TRAP3 IP число 3
modbus_registers[327]=(char)(ETH_TRAP3_IP_3);
modbus_registers[328]=0;									//Рег165  TRAP3 IP число 4
modbus_registers[329]=(char)(ETH_TRAP3_IP_4);
modbus_registers[330]=0;									//Рег166  TRAP4 IP число 1
modbus_registers[331]=(char)(ETH_TRAP4_IP_1);
modbus_registers[332]=0;									//Рег167  TRAP4 IP число 2
modbus_registers[333]=(char)(ETH_TRAP4_IP_2);
modbus_registers[334]=0;									//Рег168  TRAP4 IP число 3
modbus_registers[335]=(char)(ETH_TRAP4_IP_3);
modbus_registers[336]=0;									//Рег169  TRAP4 IP число 4
modbus_registers[337]=(char)(ETH_TRAP4_IP_4);
modbus_registers[338]=0;									//Рег170  TRAP5 IP число 1
modbus_registers[339]=(char)(ETH_TRAP5_IP_1);
modbus_registers[340]=0;									//Рег171  TRAP5 IP число 2
modbus_registers[341]=(char)(ETH_TRAP5_IP_2);
modbus_registers[342]=0;									//Рег172  TRAP5 IP число 3
modbus_registers[343]=(char)(ETH_TRAP5_IP_3);
modbus_registers[344]=0;									//Рег173  TRAP5 IP число 4
modbus_registers[345]=(char)(ETH_TRAP5_IP_4);
modbus_registers[346]=0;									//Рег174  пароль знак 1
modbus_registers[347]=(char)(snmp_web_passw[0]);
modbus_registers[348]=0;									//Рег175  пароль знак 2
modbus_registers[349]=(char)(snmp_web_passw[1]);
modbus_registers[350]=0;									//Рег176  пароль знак 3
modbus_registers[351]=(char)(snmp_web_passw[2]);
modbus_registers[352]=0;									//Рег177  перезагрузка УКУ
modbus_registers[353]=0;  /*
modbus_registers[354]=(char)((TVENTMAX*10)>>8);				//Рег178  порог ресурса вентилятора 
modbus_registers[355]=(char)(TVENTMAX*10); */
/*modbus_registers[356]=0;							//Рег179  выравн.токов, 0-ведомый, 1-включено, 2-внешн.упр.
modbus_registers[357]=(char)(ICA_EN);
modbus_registers[358]=0;							//Рег180  выравн.токов, 0-MODBUS-RTU, 1-MODBUS-TCP, 2-RS485-2
modbus_registers[359]=(char)(ICA_CH); */
//modbus_registers[360]=0;									//Рег181  IP ведомого число 1
//modbus_registers[361]=(char)(ICA_MODBUS_TCP_IP1);
//modbus_registers[362]=0;									//Рег182  IP ведомого число 2
//modbus_registers[363]=(char)(ICA_MODBUS_TCP_IP2);
//modbus_registers[364]=0;									//Рег183  IP ведомого число 3
//modbus_registers[365]=(char)(ICA_MODBUS_TCP_IP3);
//modbus_registers[366]=0;									//Рег184  IP ведомого число 4
//modbus_registers[367]=(char)(ICA_MODBUS_TCP_IP4);
//modbus_registers[368]=0;									//Рег185  адрес ведомого MODBUS-TCP	 1-254
//modbus_registers[369]=(char)(ICA_MODBUS_TCP_UNIT_ID);
//modbus_registers[370]=0;									//Рег186  адрес ведомого  MODBUS-RTU  1-254
//modbus_registers[371]=(char)(ICA_MODBUS_ADDRESS);
/*modbus_registers[372]=0;									//Рег187  стартовый ШИМ, 1%, 10-100%
modbus_registers[373]=(char)(PWM_START);
modbus_registers[374]=0;									//Рег188  проверка цепи АКБ, 1-1ступень, 2-2, 3-3
modbus_registers[375]=(char)(KB_ALGORITM);
modbus_registers[376]=0;									//Рег189  скорость регулирования, 1-стандарт, 2-с/2, 3-с/3, 4-c/4, 5-c/5
modbus_registers[377]=(char)(REG_SPEED);
modbus_registers[378]=0;									//Рег190  спецзаряды, 0-запрещены, 1-разрешены
modbus_registers[379]=(char)(SMART_SPC); */
//для ИБЭП
/*modbus_registers[380]=0;									//Рег191  фазность сети, 1-1ф, 3-3ф
modbus_registers[381]=(char)(NUMPHASE);
modbus_registers[382]=0;									//Рег192  Твент.включения
modbus_registers[383]=(char)(TVENTON);
modbus_registers[384]=0;									//Рег193  Твент.выключения
modbus_registers[385]=(char)(TVENTOFF);
modbus_registers[386]=0;									//Рег194  датчик вент. 0-Tакб, 1-Твнешн., 2-Тбпс
modbus_registers[387]=(char)(RELEVENTSIGN);
modbus_registers[388]=0;									//Рег195  откл. НПН 0-выкл, 1-реле вент., 2-реле авар.АКБ2
modbus_registers[389]=(char)(NPN_OUT);
modbus_registers[390]=(char)(UONPN>>8);						//Рег196  Uоткл НПН 0,1В, 10,0-250,0В
modbus_registers[391]=(char)(UONPN);
modbus_registers[392]=(char)(UVNPN>>8);						//Рег197  Uвкл НПН 0,1В, 10,0-250,0В
modbus_registers[393]=(char)(UVNPN);
modbus_registers[394]=0;									//Рег198  задержка откл. НПН, 1с, 10-60	 
modbus_registers[395]=(char)(TZNPN); */
/*modbus_registers[396]=0;									//Рег199  контроль средн. точки, 1%, 0-50
modbus_registers[397]=(char)(UBM_AV);  */
//o_8_e	   		   	   



if(prot==MODBUS_RTU_PROT)
	{
	modbus_tx_buff[0]=adr;
	modbus_tx_buff[1]=func;
	modbus_tx_buff[2]=(char)(reg_quantity*2);
	mem_copy((char*)&modbus_tx_buff[3],(char*)&modbus_registers[(reg_adr-1)*2],reg_quantity*2);
	
	crc_temp=CRC16_2(modbus_tx_buff,(reg_quantity*2)+3);

	modbus_tx_buff[3+(reg_quantity*2)]=(char)crc_temp;
	modbus_tx_buff[4+(reg_quantity*2)]=crc_temp>>8;

	for (i=0;i<(5+(reg_quantity*2));i++)
		{
		putchar0(modbus_tx_buff[i]);
		}

/*	for (i=0;i<(5+(reg_quantity*2));i++)
		{
		putchar_sc16is700(modbus_tx_buff[i]);
		}*/
	}
else if(prot==MODBUS_TCP_PROT)
	{
	modbus_tcp_out_ptr=(char*)&modbus_registers[(reg_adr-1)*2];
	}
}

//-----------------------------------------------
void modbus_input_registers_transmit(unsigned char adr,unsigned char func,unsigned short reg_adr,unsigned short reg_quantity, char prot)
{
signed char modbus_registers[700];	 //o_10
//char modbus_tx_buff[200];
unsigned short crc_temp;
char i;
short tempS;

//tempS=(MODBUS_INPUT_REGS[0]);
//bps_I=bps_I_phantom;
/*
#if defined UKU_6U || defined UKU_220_V2 //o_10  в UKU_220_V2 не отображались U и I у UKU_6U-отображались 
modbus_registers[0]=(signed char)(load_U>>8);					//Рег1   	напряжение выходной шины, 0.1В
modbus_registers[1]=(signed char)(load_U);
modbus_registers[2]=(signed char)(load_I>>8);					//Рег2   	ток выпрямителей, 0.1А
modbus_registers[3]=(signed char)(load_I);
#else
modbus_registers[0]=(signed char)(out_U>>8);					//Рег1   	напряжение выходной шины, 0.1В
modbus_registers[1]=(signed char)(out_U);
modbus_registers[2]=(signed char)(bps_I>>8);					//Рег2   	ток выпрямителей, 0.1А
modbus_registers[3]=(signed char)(bps_I);
#endif*/
/*
modbus_registers[0]=(signed char)(out_U>>8);					//Рег1   	напряжение выходной шины, 0.1В
modbus_registers[1]=(signed char)(out_U);
modbus_registers[2]=(signed char)(bps_I>>8);					//Рег2   	ток выпрямителей, 0.1А
modbus_registers[3]=(signed char)(bps_I);
*/
/*
modbus_registers[4]=(signed char)(net_U>>8);					//Рег3   	напряжение сети питания, 1В
modbus_registers[5]=(signed char)(net_U);
modbus_registers[6]=(signed char)(net_F>>8);					//Рег4   	частота сети питания, 0.1Гц
modbus_registers[7]=(signed char)(net_F);
modbus_registers[8]=(signed char)(net_Ua>>8);					//Рег5	напряжение сети питания фаза A, 1В	
modbus_registers[9]=(signed char)(net_Ua);		 	
modbus_registers[10]=(signed char)(net_Ub>>8);				//Рег6	напряжение сети питания фаза B, 1В
modbus_registers[11]=(signed char)(net_Ub);
modbus_registers[12]=(signed char)(net_Uc>>8);				//Рег7	напряжение сети питания фаза C, 1В
modbus_registers[13]=(signed char)(net_Uc);
modbus_registers[14]=(signed char)(bat[0]._Ub>>8);				//Рег8	напряжение батареи №1, 0.1В
modbus_registers[15]=(signed char)(bat[0]._Ub);
modbus_registers[16]=(signed char)(bat[0]._Ib>>8);				//Рег9   	ток батареи №1, 0.01А
modbus_registers[17]=(signed char)(bat[0]._Ib);
*/
/*
modbus_registers[18]=(signed char)(bat[0]._Tb>>8);				//Рег10	температура батареи №1, 1Гц
modbus_registers[19]=(signed char)(bat[0]._Tb);

modbus_registers[20]=(signed char)(bat[0]._zar>>8);			//Рег11	заряд батареи №1, %
modbus_registers[21]=(signed char)(bat[0]._zar);

modbus_registers[22]=(signed char)(bat[0]._Ubm>>8);			//Рег12	напряжение средней точки батареи №1, 0.1В
modbus_registers[23]=(signed char)(bat[0]._Ubm);
modbus_registers[24]=(signed char)(bat[0]._dUbm>>8);			//Рег13	ошибка средней точки батареи №1, %
modbus_registers[25]=(signed char)(bat[0]._dUbm);
modbus_registers[26]=(signed char)(BAT_C_REAL[0]>>8);			//Рег14	Реальная емкость батареи №1, 0.1А*ч, если 0x5555 то не измерялась
modbus_registers[27]=(signed char)(BAT_C_REAL[0]);
modbus_registers[28]=(signed char)(bat[1]._Ub>>8);				//Рег15	напряжение батареи №1, 0.1В
modbus_registers[29]=(signed char)(bat[1]._Ub);
modbus_registers[30]=(signed char)(bat[1]._Ib>>8);				//Рег16   	ток батареи №1, 0.01А
modbus_registers[31]=(signed char)(bat[1]._Ib);
modbus_registers[32]=(signed char)(bat[1]._Tb>>8);				//Рег17	температура батареи №1, 1Гц
modbus_registers[33]=(signed char)(bat[1]._Tb);
modbus_registers[34]=(signed char)(bat[1]._zar>>8);			//Рег18	заряд батареи №1, %
modbus_registers[35]=(signed char)(bat[1]._zar);
modbus_registers[36]=(signed char)(bat[1]._Ubm>>8);			//Рег19	напряжение средней точки батареи №1, 0.1В
modbus_registers[37]=(signed char)(bat[1]._Ubm);
modbus_registers[38]=(signed char)(bat[1]._dUbm>>8);			//Рег20	ошибка средней точки батареи №1, %
modbus_registers[39]=(signed char)(bat[1]._dUbm);
modbus_registers[40]=(signed char)(BAT_C_REAL[1]>>8);			//Рег21	Реальная емкость батареи №1, 0.1А*ч, если 0x5555 то не измерялась
modbus_registers[41]=(signed char)(BAT_C_REAL[1]);
modbus_registers[42]=(signed char)(bps[0]._Uii>>8);			//Рег22	Выходное напряжение выпрямителя №1, 0.1В
modbus_registers[43]=(signed char)(bps[0]._Uii);
modbus_registers[44]=(signed char)(bps[0]._Ii>>8);				//Рег23	Выходной ток выпрямителя №1, 0.1А
modbus_registers[45]=(signed char)(bps[0]._Ii);
modbus_registers[46]=(signed char)(bps[0]._Ti>>8);				//Рег24	Температура радиатора выпрямителя №1, 1гЦ
modbus_registers[47]=(signed char)(bps[0]._Ti);
modbus_registers[48]=(signed char)(bps[0]._av>>8);				//Рег25	Байт флагов выпрямителя №1, 0x01 - перегрев, 0x02 завышено Uвых, 0x04 занижено Uвых, 0x08 - отсутствует связь с выпрямителем
modbus_registers[49]=(signed char)(bps[0]._av);
modbus_registers[50]=(signed char)(bps[1]._Uii>>8);			//Рег26	Выходное напряжение выпрямителя №2, 0.1В
modbus_registers[51]=(signed char)(bps[1]._Uii);
modbus_registers[52]=(signed char)(bps[1]._Ii>>8);				//Рег27	Выходной ток выпрямителя №2, 0.1А
modbus_registers[53]=(signed char)(bps[1]._Ii);
modbus_registers[54]=(signed char)(bps[1]._Ti>>8);				//Рег28	Температура радиатора выпрямителя №2, 1гЦ
modbus_registers[55]=(signed char)(bps[1]._Ti);
modbus_registers[56]=(signed char)(bps[1]._av>>8);				//Рег29	Байт флагов выпрямителя №2, 0x01 - перегрев, 0x02 завышено Uвых, 0x04 занижено Uвых, 0x08 - отсутствует связь с выпрямителем
modbus_registers[57]=(signed char)(bps[1]._av);
modbus_registers[58]=(signed char)(bps[2]._Uii>>8);			//Рег30	Выходное напряжение выпрямителя №3, 0.1В
modbus_registers[59]=(signed char)(bps[2]._Uii);
modbus_registers[60]=(signed char)(bps[2]._Ii>>8);				//Рег31	Выходной ток выпрямителя №3, 0.1А
modbus_registers[61]=(signed char)(bps[2]._Ii);
modbus_registers[62]=(signed char)(bps[2]._Ti>>8);				//Рег32	Температура радиатора выпрямителя №3, 1гЦ
modbus_registers[63]=(signed char)(bps[2]._Ti);
modbus_registers[64]=(signed char)(bps[2]._av>>8);				//Рег33	Байт флагов выпрямителя №3, 0x01 - перегрев, 0x02 завышено Uвых, 0x04 занижено Uвых, 0x08 - отсутствует связь с выпрямителем
modbus_registers[65]=(signed char)(bps[2]._av);
modbus_registers[66]=(signed char)(bps[3]._Uii>>8);			//Рег34	Выходное напряжение выпрямителя №4, 0.1В
modbus_registers[67]=(signed char)(bps[3]._Uii);
modbus_registers[68]=(signed char)(bps[3]._Ii>>8);				//Рег35	Выходной ток выпрямителя №4, 0.1А
modbus_registers[69]=(signed char)(bps[3]._Ii);
modbus_registers[70]=(signed char)(bps[3]._Ti>>8);				//Рег36	Температура радиатора выпрямителя №4, 1гЦ
modbus_registers[71]=(signed char)(bps[3]._Ti);
modbus_registers[72]=(signed char)(bps[3]._av>>8);				//Рег37	Байт флагов выпрямителя №4, 0x01 - перегрев, 0x02 завышено Uвых, 0x04 занижено Uвых, 0x08 - отсутствует связь с выпрямителем
modbus_registers[73]=(signed char)(bps[3]._av);
modbus_registers[74]=(signed char)(bps[4]._Uii>>8);			//Рег38	Выходное напряжение выпрямителя №5, 0.1В
modbus_registers[75]=(signed char)(bps[4]._Uii);
modbus_registers[76]=(signed char)(bps[4]._Ii>>8);				//Рег39	Выходной ток выпрямителя №5, 0.1А
modbus_registers[77]=(signed char)(bps[4]._Ii);
modbus_registers[78]=(signed char)(bps[4]._Ti>>8);				//Рег40	Температура радиатора выпрямителя №5, 1гЦ
modbus_registers[79]=(signed char)(bps[4]._Ti);
modbus_registers[80]=(signed char)(bps[4]._av>>8);				//Рег41	Байт флагов выпрямителя №5, 0x01 - перегрев, 0x02 завышено Uвых, 0x04 занижено Uвых, 0x08 - отсутствует связь с выпрямителем
modbus_registers[81]=(signed char)(bps[4]._av);
modbus_registers[82]=(signed char)(bps[5]._Uii>>8);			//Рег42	Выходное напряжение выпрямителя №6, 0.1В
modbus_registers[83]=(signed char)(bps[5]._Uii);
modbus_registers[84]=(signed char)(bps[5]._Ii>>8);				//Рег43	Выходной ток выпрямителя №6, 0.1А
modbus_registers[85]=(signed char)(bps[5]._Ii);
modbus_registers[86]=(signed char)(bps[5]._Ti>>8);				//Рег44	Температура радиатора выпрямителя №6, 1гЦ
modbus_registers[87]=(signed char)(bps[5]._Ti);
modbus_registers[88]=(signed char)(bps[5]._av>>8);				//Рег45	Байт флагов выпрямителя №6, 0x01 - перегрев, 0x02 завышено Uвых, 0x04 занижено Uвых, 0x08 - отсутствует связь с выпрямителем
modbus_registers[89]=(signed char)(bps[5]._av);
modbus_registers[90]=(signed char)(bps[6]._Uii>>8);			//Рег46	Выходное напряжение выпрямителя №7, 0.1В
modbus_registers[91]=(signed char)(bps[6]._Uii);
modbus_registers[92]=(signed char)(bps[6]._Ii>>8);				//Рег47	Выходной ток выпрямителя №7, 0.1А
modbus_registers[93]=(signed char)(bps[6]._Ii);
modbus_registers[94]=(signed char)(bps[6]._Ti>>8);				//Рег48	Температура радиатора выпрямителя №7, 1гЦ
modbus_registers[95]=(signed char)(bps[6]._Ti);
modbus_registers[96]=(signed char)(bps[6]._av>>8);				//Рег49	Байт флагов выпрямителя №7, 0x01 - перегрев, 0x02 завышено Uвых, 0x04 занижено Uвых, 0x08 - отсутствует связь с выпрямителем
modbus_registers[97]=(signed char)(bps[6]._av);
modbus_registers[98]=(signed char)(bps[7]._Uii>>8);			//Рег50	Выходное напряжение выпрямителя №8, 0.1В
modbus_registers[99]=(signed char)(bps[7]._Uii);
modbus_registers[100]=(signed char)(bps[7]._Ii>>8);			//Рег51	Выходной ток выпрямителя №8, 0.1А
modbus_registers[101]=(signed char)(bps[7]._Ii);
modbus_registers[102]=(signed char)(bps[7]._Ti>>8);			//Рег52	Температура радиатора выпрямителя №8, 1гЦ
modbus_registers[103]=(signed char)(bps[7]._Ti);
modbus_registers[104]=(signed char)(bps[7]._av>>8);			//Рег53	Байт флагов выпрямителя №8, 0x01 - перегрев, 0x02 завышено Uвых, 0x04 занижено Uвых, 0x08 - отсутствует связь с выпрямителем
modbus_registers[105]=(signed char)(bps[7]._av);
modbus_registers[106]=(signed char)(bps_U>>8);					//Рег54   	напряжение выпрямителей, 0.1В
modbus_registers[107]=(signed char)(bps_U);
tempS=0;
if((speedChIsOn)||(sp_ch_stat==scsWRK)) tempS=1;
modbus_registers[108]=(signed char)(tempS>>8);					//Рег55   	Ускоренный заряд включенность, (1 - вкл, 0 - Выкл)
modbus_registers[109]=(signed char)(tempS);
tempS=0;
if(spc_stat==spcVZ) tempS=1;
modbus_registers[110]=(signed char)(tempS>>8);					//Рег56   	Выравнивающий заряд включенность, (1 - вкл, 0 - Выкл)
modbus_registers[111]=(signed char)(tempS);
modbus_registers[112]=(signed char)(uout_av>>8);					//Рег57   Контроль выходного напряжения, (0 - норма, 1 - завышено, 2 - занижено)
modbus_registers[113]=(signed char)(uout_av);*/
/*
tempS=0;													 //Рег60	Регистр флагов состояния системы
if(bat_ips._av)			tempS|=(1<<0);						 // Бит 0	Авария батареи
if(avar_stat&0x0001)   	tempS|=(1<<1);						 //	Бит 1	Авария питающей сети 
if(avar_stat&(1<<(3+0)))tempS|=(1<<2);						 //	Бит 2	Авария выпрямителя №1
if(avar_stat&(1<<(3+1)))tempS|=(1<<3);						 //	Бит 3	Авария выпрямителя №2
if(avar_stat&(1<<(3+2)))tempS|=(1<<4);						 //	Бит 4	Авария выпрямителя №3
if(avar_stat&(1<<(3+3)))tempS|=(1<<5);						 	//	Бит 5	Авария выпрямителя №4
if(avar_stat&(1<<(3+4)))tempS|=(1<<6);						 	//	Бит 6	Авария выпрямителя №5
if(avar_stat&(1<<(3+5)))tempS|=(1<<7);						 	//	Бит 7	Авария выпрямителя №6
if(avar_stat&(1<<(3+6)))tempS|=(1<<8);						 	//	Бит 8	Авария выпрямителя №7
if(avar_stat&(1<<(3+6)))tempS|=(1<<9);						 	//	Бит 8	Авария выпрямителя №8
*/
/*
tempS=0;
tempS=avar_stat;
#ifdef UKU_ZVU
if(bat_ips._av)			tempS|=(1<<1);
else 					tempS&=~(1<<1);
#endif */
//Рег60	Регистр флагов состояния системы
// 	Бит 0	Авария питающей сети 
//	Бит 1	Авария батареи №1(Авария батареи для ЗВУ)
//	Бит 2	Авария батареи №2
//	Бит 3	Авария выпрямителя №1
//	Бит 4	Авария выпрямителя №2
//	Бит 5	Авария выпрямителя №3
//	Бит 6	Авария выпрямителя №4
//	Бит 7	Авария выпрямителя №5
//	Бит 8	Авария выпрямителя №6
//	Бит 9	Авария выпрямителя №7
//	Бит 10	Авария выпрямителя №8

//modbus_registers[118]=(signed char)(tempS>>8);
//modbus_registers[119]=(signed char)(tempS);
/*
modbus_registers[120]=(signed char)(volta_short>>8);		//Рег61   	напряжение счетчика, 0.1В
modbus_registers[121]=(signed char)(volta_short);
modbus_registers[122]=(signed char)(curr_short>>8);			//Рег62  	ток счетчика, 0.01А
modbus_registers[123]=(signed char)(curr_short);
modbus_registers[124]=(signed char)(power_int>>8);			//Рег63   	мощность счетчика, 1Вт
modbus_registers[125]=(signed char)(power_int);
*/

modbus_registers[138]=(signed char)(HARDVARE_VERSION>>8);	//Рег 70  	аппаратная версия
modbus_registers[139]=(signed char)(HARDVARE_VERSION);
modbus_registers[140]=(signed char)(SOFT_VERSION>>8);		//Рег 71  	версия ПО
modbus_registers[141]=(signed char)(SOFT_VERSION);
modbus_registers[142]=(signed char)(BUILD>>8);				//Рег 72  	номер компиляции ПО
modbus_registers[143]=(signed char)(BUILD);
modbus_registers[144]=(signed char)(BUILD_YEAR>>8);			//Рег 73  	год	компиляции ПО
modbus_registers[145]=(signed char)(BUILD_YEAR);
modbus_registers[146]=(signed char)(BUILD_MONTH>>8);		//Рег 74  	месяц компиляции ПО
modbus_registers[147]=(signed char)(BUILD_MONTH);
modbus_registers[148]=(signed char)(BUILD_DAY>>8);			//Рег 75  	день компиляции ПО
modbus_registers[149]=(signed char)(BUILD_DAY);
modbus_registers[150]=(signed char)(AUSW_MAIN_NUMBER>>8); 	//?aa 76 caaianeie iiia?
modbus_registers[151]=(signed char)(AUSW_MAIN_NUMBER);
modbus_registers[152]=(signed char)(AUSW_MAIN_NUMBER>>24);			//Рег 77  	caaianeie iiia?
modbus_registers[153]=(signed char)(AUSW_MAIN_NUMBER>>16);
//tempS=cntrl_stat_old;
//if(	(main_kb_cnt==(TBAT*60)-21) || (main_kb_cnt==(TBAT*60)-20) || (main_kb_cnt==(TBAT*60)-19)) tempS=((short)TBAT)|0x4000;
//tempS=0x800f;
//modbus_registers[198]=(signed char)(tempS>>8);				//???100	????????? ???
//modbus_registers[199]=(signed char)(tempS);
/*
modbus_registers[200]=(signed char)(bps[8]._Uii>>8);		//Рег101	Выходное напряжение выпрямителя №9, 0.1В
modbus_registers[201]=(signed char)(bps[8]._Uii);
modbus_registers[202]=(signed char)(bps[8]._Ii>>8);			//Рег102	Выходной ток выпрямителя №9, 0.1А
modbus_registers[203]=(signed char)(bps[8]._Ii);
modbus_registers[204]=(signed char)(bps[8]._Ti>>8);			//Рег103	Температура радиатора выпрямителя №9, 1гЦ
modbus_registers[205]=(signed char)(bps[8]._Ti);
modbus_registers[206]=(signed char)(bps[8]._av>>8);			//Рег104	Байт флагов выпрямителя №9, 0x01 - перегрев, 0x02 завышено Uвых, 0x04 занижено Uвых, 0x08 - отсутствует связь с выпрямителем
modbus_registers[207]=(signed char)(bps[8]._av);
modbus_registers[208]=(signed char)(bps[9]._Uii>>8);		//Рег101	Выходное напряжение выпрямителя №9, 0.1В
modbus_registers[209]=(signed char)(bps[9]._Uii);
modbus_registers[210]=(signed char)(bps[9]._Ii>>8);			//Рег102	Выходной ток выпрямителя №9, 0.1А
modbus_registers[211]=(signed char)(bps[9]._Ii);
modbus_registers[212]=(signed char)(bps[9]._Ti>>8);			//Рег103	Температура радиатора выпрямителя №9, 1гЦ
modbus_registers[213]=(signed char)(bps[9]._Ti);
modbus_registers[214]=(signed char)(bps[9]._av>>8);			//Рег104	Байт флагов выпрямителя №9, 0x01 - перегрев, 0x02 завышено Uвых, 0x04 занижено Uвых, 0x08 - отсутствует связь с выпрямителем
modbus_registers[215]=(signed char)(bps[9]._av);
modbus_registers[216]=(signed char)(bps[10]._Uii>>8);		//Рег101	Выходное напряжение выпрямителя №9, 0.1В
modbus_registers[217]=(signed char)(bps[10]._Uii);
modbus_registers[218]=(signed char)(bps[10]._Ii>>8);			//Рег102	Выходной ток выпрямителя №9, 0.1А
modbus_registers[219]=(signed char)(bps[10]._Ii);
modbus_registers[220]=(signed char)(bps[10]._Ti>>8);			//Рег103	Температура радиатора выпрямителя №9, 1гЦ
modbus_registers[221]=(signed char)(bps[10]._Ti);
modbus_registers[222]=(signed char)(bps[10]._av>>8);			//Рег104	Байт флагов выпрямителя №9, 0x01 - перегрев, 0x02 завышено Uвых, 0x04 занижено Uвых, 0x08 - отсутствует связь с выпрямителем
modbus_registers[223]=(signed char)(bps[10]._av);
modbus_registers[224]=(signed char)(bps[11]._Uii>>8);		//Рег101	Выходное напряжение выпрямителя №9, 0.1В
modbus_registers[225]=(signed char)(bps[11]._Uii);
modbus_registers[226]=(signed char)(bps[11]._Ii>>8);			//Рег102	Выходной ток выпрямителя №9, 0.1А
modbus_registers[227]=(signed char)(bps[11]._Ii);
modbus_registers[228]=(signed char)(bps[11]._Ti>>8);			//Рег103	Температура радиатора выпрямителя №9, 1гЦ
modbus_registers[229]=(signed char)(bps[11]._Ti);
modbus_registers[230]=(signed char)(bps[11]._av>>8);			//Рег104	Байт флагов выпрямителя №9, 0x01 - перегрев, 0x02 завышено Uвых, 0x04 занижено Uвых, 0x08 - отсутствует связь с выпрямителем
modbus_registers[231]=(signed char)(bps[11]._av);
modbus_registers[232]=(signed char)(bps[12]._Uii>>8);		//Рег101	Выходное напряжение выпрямителя №9, 0.1В
modbus_registers[233]=(signed char)(bps[12]._Uii);
modbus_registers[234]=(signed char)(bps[12]._Ii>>8);			//Рег102	Выходной ток выпрямителя №9, 0.1А
modbus_registers[235]=(signed char)(bps[12]._Ii);
modbus_registers[236]=(signed char)(bps[12]._Ti>>8);			//Рег103	Температура радиатора выпрямителя №9, 1гЦ
modbus_registers[237]=(signed char)(bps[12]._Ti);
modbus_registers[238]=(signed char)(bps[12]._av>>8);			//Рег104	Байт флагов выпрямителя №9, 0x01 - перегрев, 0x02 завышено Uвых, 0x04 занижено Uвых, 0x08 - отсутствует связь с выпрямителем
modbus_registers[239]=(signed char)(bps[12]._av);
modbus_registers[240]=(signed char)(bps[13]._Uii>>8);		//Рег101	Выходное напряжение выпрямителя №9, 0.1В
modbus_registers[241]=(signed char)(bps[13]._Uii);
modbus_registers[242]=(signed char)(bps[13]._Ii>>8);			//Рег102	Выходной ток выпрямителя №9, 0.1А
modbus_registers[243]=(signed char)(bps[13]._Ii);
modbus_registers[244]=(signed char)(bps[13]._Ti>>8);			//Рег103	Температура радиатора выпрямителя №9, 1гЦ
modbus_registers[245]=(signed char)(bps[13]._Ti);
modbus_registers[246]=(signed char)(bps[13]._av>>8);			//Рег104	Байт флагов выпрямителя №9, 0x01 - перегрев, 0x02 завышено Uвых, 0x04 занижено Uвых, 0x08 - отсутствует связь с выпрямителем
modbus_registers[247]=(signed char)(bps[13]._av);
modbus_registers[248]=(signed char)(bps[14]._Uii>>8);		//Рег101	Выходное напряжение выпрямителя №9, 0.1В
modbus_registers[249]=(signed char)(bps[14]._Uii);
modbus_registers[250]=(signed char)(bps[14]._Ii>>8);			//Рег102	Выходной ток выпрямителя №9, 0.1А
modbus_registers[251]=(signed char)(bps[14]._Ii);
modbus_registers[252]=(signed char)(bps[14]._Ti>>8);			//Рег103	Температура радиатора выпрямителя №9, 1гЦ
modbus_registers[253]=(signed char)(bps[14]._Ti);
modbus_registers[254]=(signed char)(bps[14]._av>>8);			//Рег104	Байт флагов выпрямителя №9, 0x01 - перегрев, 0x02 завышено Uвых, 0x04 занижено Uвых, 0x08 - отсутствует связь с выпрямителем
modbus_registers[255]=(signed char)(bps[14]._av);
modbus_registers[256]=(signed char)(bps[15]._Uii>>8);		//Рег101	Выходное напряжение выпрямителя №9, 0.1В
modbus_registers[257]=(signed char)(bps[15]._Uii);
modbus_registers[258]=(signed char)(bps[15]._Ii>>8);			//Рег102	Выходной ток выпрямителя №9, 0.1А
modbus_registers[259]=(signed char)(bps[15]._Ii);
modbus_registers[260]=(signed char)(bps[15]._Ti>>8);			//Рег103	Температура радиатора выпрямителя №9, 1гЦ
modbus_registers[261]=(signed char)(bps[15]._Ti);
modbus_registers[262]=(signed char)(bps[15]._av>>8);			//Рег104	Байт флагов выпрямителя №9, 0x01 - перегрев, 0x02 завышено Uвых, 0x04 занижено Uвых, 0x08 - отсутствует связь с выпрямителем
modbus_registers[263]=(signed char)(bps[15]._av);
modbus_registers[264]=(signed char)(bps[16]._Uii>>8);		//Рег101	Выходное напряжение выпрямителя №9, 0.1В
modbus_registers[265]=(signed char)(bps[16]._Uii);
modbus_registers[266]=(signed char)(bps[16]._Ii>>8);			//Рег102	Выходной ток выпрямителя №9, 0.1А
modbus_registers[267]=(signed char)(bps[16]._Ii);
modbus_registers[268]=(signed char)(bps[16]._Ti>>8);			//Рег103	Температура радиатора выпрямителя №9, 1гЦ
modbus_registers[269]=(signed char)(bps[16]._Ti);
modbus_registers[270]=(signed char)(bps[16]._av>>8);			//Рег104	Байт флагов выпрямителя №9, 0x01 - перегрев, 0x02 завышено Uвых, 0x04 занижено Uвых, 0x08 - отсутствует связь с выпрямителем
modbus_registers[271]=(signed char)(bps[16]._av);
modbus_registers[272]=(signed char)(bps[17]._Uii>>8);		//Рег101	Выходное напряжение выпрямителя №9, 0.1В
modbus_registers[273]=(signed char)(bps[17]._Uii);
modbus_registers[274]=(signed char)(bps[17]._Ii>>8);			//Рег102	Выходной ток выпрямителя №9, 0.1А
modbus_registers[275]=(signed char)(bps[17]._Ii);
modbus_registers[276]=(signed char)(bps[17]._Ti>>8);			//Рег103	Температура радиатора выпрямителя №9, 1гЦ
modbus_registers[277]=(signed char)(bps[17]._Ti);
modbus_registers[278]=(signed char)(bps[17]._av>>8);			//Рег104	Байт флагов выпрямителя №9, 0x01 - перегрев, 0x02 завышено Uвых, 0x04 занижено Uвых, 0x08 - отсутствует связь с выпрямителем
modbus_registers[279]=(signed char)(bps[17]._av);
modbus_registers[280]=(signed char)(bps[18]._Uii>>8);		//Рег101	Выходное напряжение выпрямителя №9, 0.1В
modbus_registers[281]=(signed char)(bps[18]._Uii);
modbus_registers[282]=(signed char)(bps[18]._Ii>>8);			//Рег102	Выходной ток выпрямителя №9, 0.1А
modbus_registers[283]=(signed char)(bps[18]._Ii);
modbus_registers[284]=(signed char)(bps[18]._Ti>>8);			//Рег103	Температура радиатора выпрямителя №9, 1гЦ
modbus_registers[285]=(signed char)(bps[18]._Ti);
modbus_registers[286]=(signed char)(bps[18]._av>>8);			//Рег104	Байт флагов выпрямителя №9, 0x01 - перегрев, 0x02 завышено Uвых, 0x04 занижено Uвых, 0x08 - отсутствует связь с выпрямителем
modbus_registers[287]=(signed char)(bps[18]._av);
modbus_registers[288]=(signed char)(bps[19]._Uii>>8);		//Рег101	Выходное напряжение выпрямителя №9, 0.1В
modbus_registers[289]=(signed char)(bps[19]._Uii);
modbus_registers[290]=(signed char)(bps[19]._Ii>>8);			//Рег102	Выходной ток выпрямителя №9, 0.1А
modbus_registers[291]=(signed char)(bps[19]._Ii);
modbus_registers[292]=(signed char)(bps[19]._Ti>>8);			//Рег103	Температура радиатора выпрямителя №9, 1гЦ
modbus_registers[293]=(signed char)(bps[19]._Ti);
modbus_registers[294]=(signed char)(bps[19]._av>>8);			//Рег104	Байт флагов выпрямителя №9, 0x01 - перегрев, 0x02 завышено Uвых, 0x04 занижено Uвых, 0x08 - отсутствует связь с выпрямителем
modbus_registers[295]=(signed char)(bps[19]._av);
modbus_registers[296]=(signed char)(bps[20]._Uii>>8);		//Рег101	Выходное напряжение выпрямителя №9, 0.1В
modbus_registers[297]=(signed char)(bps[20]._Uii);
modbus_registers[298]=(signed char)(bps[20]._Ii>>8);			//Рег102	Выходной ток выпрямителя №9, 0.1А
modbus_registers[299]=(signed char)(bps[20]._Ii);
modbus_registers[300]=(signed char)(bps[20]._Ti>>8);			//Рег103	Температура радиатора выпрямителя №9, 1гЦ
modbus_registers[301]=(signed char)(bps[20]._Ti);
modbus_registers[302]=(signed char)(bps[20]._av>>8);			//Рег104	Байт флагов выпрямителя №9, 0x01 - перегрев, 0x02 завышено Uвых, 0x04 занижено Uвых, 0x08 - отсутствует связь с выпрямителем
modbus_registers[303]=(signed char)(bps[20]._av);
modbus_registers[304]=(signed char)(bps[21]._Uii>>8);		//Рег101	Выходное напряжение выпрямителя №9, 0.1В
modbus_registers[305]=(signed char)(bps[21]._Uii);
modbus_registers[306]=(signed char)(bps[21]._Ii>>8);			//Рег102	Выходной ток выпрямителя №9, 0.1А
modbus_registers[307]=(signed char)(bps[21]._Ii);
modbus_registers[308]=(signed char)(bps[21]._Ti>>8);			//Рег103	Температура радиатора выпрямителя №9, 1гЦ
modbus_registers[309]=(signed char)(bps[21]._Ti);
modbus_registers[310]=(signed char)(bps[21]._av>>8);			//Рег104	Байт флагов выпрямителя №9, 0x01 - перегрев, 0x02 завышено Uвых, 0x04 занижено Uвых, 0x08 - отсутствует связь с выпрямителем
modbus_registers[311]=(signed char)(bps[21]._av);
modbus_registers[312]=(signed char)(bps[22]._Uii>>8);		//Рег101	Выходное напряжение выпрямителя №9, 0.1В
modbus_registers[313]=(signed char)(bps[22]._Uii);
modbus_registers[314]=(signed char)(bps[22]._Ii>>8);			//Рег102	Выходной ток выпрямителя №9, 0.1А
modbus_registers[315]=(signed char)(bps[22]._Ii);
modbus_registers[316]=(signed char)(bps[22]._Ti>>8);			//Рег103	Температура радиатора выпрямителя №9, 1гЦ
modbus_registers[317]=(signed char)(bps[22]._Ti);
modbus_registers[318]=(signed char)(bps[22]._av>>8);			//Рег104	Байт флагов выпрямителя №9, 0x01 - перегрев, 0x02 завышено Uвых, 0x04 занижено Uвых, 0x08 - отсутствует связь с выпрямителем
modbus_registers[319]=(signed char)(bps[22]._av);
modbus_registers[320]=(signed char)(bps[23]._Uii>>8);		//Рег101	Выходное напряжение выпрямителя №9, 0.1В
modbus_registers[321]=(signed char)(bps[23]._Uii);
modbus_registers[322]=(signed char)(bps[23]._Ii>>8);			//Рег102	Выходной ток выпрямителя №9, 0.1А
modbus_registers[323]=(signed char)(bps[23]._Ii);
modbus_registers[324]=(signed char)(bps[23]._Ti>>8);			//Рег103	Температура радиатора выпрямителя №9, 1гЦ
modbus_registers[325]=(signed char)(bps[23]._Ti);
modbus_registers[326]=(signed char)(bps[23]._av>>8);			//Рег104	Байт флагов выпрямителя №9, 0x01 - перегрев, 0x02 завышено Uвых, 0x04 занижено Uвых, 0x08 - отсутствует связь с выпрямителем
modbus_registers[327]=(signed char)(bps[23]._av);
modbus_registers[328]=(signed char)(bps[24]._Uii>>8);		//Рег101	Выходное напряжение выпрямителя №9, 0.1В
modbus_registers[329]=(signed char)(bps[24]._Uii);
modbus_registers[330]=(signed char)(bps[24]._Ii>>8);			//Рег102	Выходной ток выпрямителя №9, 0.1А
modbus_registers[331]=(signed char)(bps[24]._Ii);
modbus_registers[332]=(signed char)(bps[24]._Ti>>8);			//Рег103	Температура радиатора выпрямителя №9, 1гЦ
modbus_registers[333]=(signed char)(bps[24]._Ti);
modbus_registers[334]=(signed char)(bps[24]._av>>8);			//Рег104	Байт флагов выпрямителя №9, 0x01 - перегрев, 0x02 завышено Uвых, 0x04 занижено Uвых, 0x08 - отсутствует связь с выпрямителем
modbus_registers[335]=(signed char)(bps[24]._av);
modbus_registers[336]=(signed char)(bps[25]._Uii>>8);		//Рег101	Выходное напряжение выпрямителя №9, 0.1В
modbus_registers[337]=(signed char)(bps[25]._Uii);
modbus_registers[338]=(signed char)(bps[25]._Ii>>8);			//Рег102	Выходной ток выпрямителя №9, 0.1А
modbus_registers[339]=(signed char)(bps[25]._Ii);
modbus_registers[340]=(signed char)(bps[25]._Ti>>8);			//Рег103	Температура радиатора выпрямителя №9, 1гЦ
modbus_registers[341]=(signed char)(bps[25]._Ti);
modbus_registers[342]=(signed char)(bps[25]._av>>8);			//Рег104	Байт флагов выпрямителя №9, 0x01 - перегрев, 0x02 завышено Uвых, 0x04 занижено Uвых, 0x08 - отсутствует связь с выпрямителем
modbus_registers[343]=(signed char)(bps[25]._av);
modbus_registers[344]=(signed char)(bps[26]._Uii>>8);		//Рег101	Выходное напряжение выпрямителя №9, 0.1В
modbus_registers[345]=(signed char)(bps[26]._Uii);
modbus_registers[346]=(signed char)(bps[26]._Ii>>8);			//Рег102	Выходной ток выпрямителя №9, 0.1А
modbus_registers[347]=(signed char)(bps[26]._Ii);
modbus_registers[348]=(signed char)(bps[26]._Ti>>8);			//Рег103	Температура радиатора выпрямителя №9, 1гЦ
modbus_registers[349]=(signed char)(bps[26]._Ti);
modbus_registers[350]=(signed char)(bps[26]._av>>8);			//Рег104	Байт флагов выпрямителя №9, 0x01 - перегрев, 0x02 завышено Uвых, 0x04 занижено Uвых, 0x08 - отсутствует связь с выпрямителем
modbus_registers[351]=(signed char)(bps[26]._av);
modbus_registers[352]=(signed char)(bps[27]._Uii>>8);		//Рег101	Выходное напряжение выпрямителя №9, 0.1В
modbus_registers[353]=(signed char)(bps[27]._Uii);
modbus_registers[354]=(signed char)(bps[27]._Ii>>8);			//Рег102	Выходной ток выпрямителя №9, 0.1А
modbus_registers[355]=(signed char)(bps[27]._Ii);
modbus_registers[356]=(signed char)(bps[27]._Ti>>8);			//Рег103	Температура радиатора выпрямителя №9, 1гЦ
modbus_registers[357]=(signed char)(bps[27]._Ti);
modbus_registers[358]=(signed char)(bps[27]._av>>8);			//Рег104	Байт флагов выпрямителя №9, 0x01 - перегрев, 0x02 завышено Uвых, 0x04 занижено Uвых, 0x08 - отсутствует связь с выпрямителем
modbus_registers[359]=(signed char)(bps[27]._av);
modbus_registers[360]=(signed char)(bps[28]._Uii>>8);		//Рег101	Выходное напряжение выпрямителя №9, 0.1В
modbus_registers[361]=(signed char)(bps[28]._Uii);
modbus_registers[362]=(signed char)(bps[28]._Ii>>8);			//Рег102	Выходной ток выпрямителя №9, 0.1А
modbus_registers[363]=(signed char)(bps[28]._Ii);
modbus_registers[364]=(signed char)(bps[28]._Ti>>8);			//Рег103	Температура радиатора выпрямителя №9, 1гЦ
modbus_registers[365]=(signed char)(bps[28]._Ti);
modbus_registers[366]=(signed char)(bps[28]._av>>8);			//Рег104	Байт флагов выпрямителя №9, 0x01 - перегрев, 0x02 завышено Uвых, 0x04 занижено Uвых, 0x08 - отсутствует связь с выпрямителем
modbus_registers[367]=(signed char)(bps[28]._av);
modbus_registers[368]=(signed char)(bps[29]._Uii>>8);		//Рег101	Выходное напряжение выпрямителя №9, 0.1В
modbus_registers[369]=(signed char)(bps[29]._Uii);
modbus_registers[370]=(signed char)(bps[29]._Ii>>8);			//Рег102	Выходной ток выпрямителя №9, 0.1А
modbus_registers[371]=(signed char)(bps[29]._Ii);
modbus_registers[372]=(signed char)(bps[29]._Ti>>8);			//Рег103	Температура радиатора выпрямителя №9, 1гЦ
modbus_registers[373]=(signed char)(bps[29]._Ti);
modbus_registers[374]=(signed char)(bps[29]._av>>8);			//Рег104	Байт флагов выпрямителя №9, 0x01 - перегрев, 0x02 завышено Uвых, 0x04 занижено Uвых, 0x08 - отсутствует связь с выпрямителем
modbus_registers[375]=(signed char)(bps[29]._av);
modbus_registers[376]=(signed char)(bps[30]._Uii>>8);		//Рег101	Выходное напряжение выпрямителя №9, 0.1В
modbus_registers[377]=(signed char)(bps[30]._Uii);
modbus_registers[378]=(signed char)(bps[30]._Ii>>8);			//Рег102	Выходной ток выпрямителя №9, 0.1А
modbus_registers[379]=(signed char)(bps[30]._Ii);
modbus_registers[380]=(signed char)(bps[30]._Ti>>8);			//Рег103	Температура радиатора выпрямителя №9, 1гЦ
modbus_registers[381]=(signed char)(bps[30]._Ti);
modbus_registers[382]=(signed char)(bps[30]._av>>8);			//Рег104	Байт флагов выпрямителя №9, 0x01 - перегрев, 0x02 завышено Uвых, 0x04 занижено Uвых, 0x08 - отсутствует связь с выпрямителем
modbus_registers[383]=(signed char)(bps[30]._av);
modbus_registers[384]=(signed char)(bps[31]._Uii>>8);		//Рег101	Выходное напряжение выпрямителя №9, 0.1В
modbus_registers[385]=(signed char)(bps[31]._Uii);
modbus_registers[386]=(signed char)(bps[31]._Ii>>8);			//Рег102	Выходной ток выпрямителя №9, 0.1А
modbus_registers[387]=(signed char)(bps[31]._Ii);
modbus_registers[388]=(signed char)(bps[31]._Ti>>8);			//Рег103	Температура радиатора выпрямителя №9, 1гЦ
modbus_registers[389]=(signed char)(bps[31]._Ti);
modbus_registers[390]=(signed char)(bps[31]._av>>8);			//Рег104	Байт флагов выпрямителя №9, 0x01 - перегрев, 0x02 завышено Uвых, 0x04 занижено Uвых, 0x08 - отсутствует связь с выпрямителем
modbus_registers[391]=(signed char)(bps[31]._av);
modbus_registers[392]=(signed char)(bps[32]._Uii>>8);		//Рег101	Выходное напряжение выпрямителя №9, 0.1В
modbus_registers[393]=(signed char)(bps[32]._Uii);
modbus_registers[394]=(signed char)(bps[32]._Ii>>8);			//Рег102	Выходной ток выпрямителя №9, 0.1А
modbus_registers[395]=(signed char)(bps[32]._Ii);
modbus_registers[396]=(signed char)(bps[32]._Ti>>8);			//Рег103	Температура радиатора выпрямителя №9, 1гЦ
modbus_registers[397]=(signed char)(bps[32]._Ti);
modbus_registers[398]=(signed char)(bps[32]._av>>8);			//Рег104	Байт флагов выпрямителя №9, 0x01 - перегрев, 0x02 завышено Uвых, 0x04 занижено Uвых, 0x08 - отсутствует связь с выпрямителем
modbus_registers[399]=(signed char)(bps[32]._av);*/




/*

tempS=t_ext[0];
if(ND_EXT[0])tempS=-1000;
modbus_registers[400]=(signed char)(tempS>>8);				//Рег201	Внешний датчик температуры №1
modbus_registers[401]=(signed char)(tempS);
tempS=t_ext[1];
if(ND_EXT[1])tempS=-1000;
modbus_registers[402]=(signed char)(tempS>>8);				//Рег202	Внешний датчик температуры №2
modbus_registers[403]=(signed char)(tempS);
tempS=t_ext[2];
if(ND_EXT[2])tempS=-1000;
modbus_registers[404]=(signed char)(tempS>>8);				//Рег203	Внешний датчик температуры №3
modbus_registers[405]=(signed char)(tempS);	*/
/*tempS=t_ext[3];
if(ND_EXT[3])tempS=-1000;
modbus_registers[406]=(signed char)(tempS>>8);				//Рег204	Внешний датчик температуры №4
modbus_registers[407]=(signed char)(tempS);   */

/*
modbus_registers[406]=(signed char)(bat_hndl_t_razr_min>>8);
modbus_registers[407]=(signed char)(bat_hndl_t_razr_min);  */
/*
tempS=0;
if(sk_stat[0]==ssON) tempS|=0x0001;
if(sk_av_stat[0]==sasON) tempS|=0x0002;
modbus_registers[420]=(signed char)(tempS>>8);				//Рег211	Состояние  сухого контакта №1, (нулевой бит - физическое состояние, 1 - замкнут, 0 - разомкнут, первый бит - аварийность, 1 - авария, 0 - норма)
modbus_registers[421]=(signed char)(tempS);
tempS=0;
if(sk_stat[1]==ssON) tempS|=0x0001;
if(sk_av_stat[1]==sasON) tempS|=0x0002;
modbus_registers[422]=(signed char)(tempS>>8);				//Рег212	Состояние  сухого контакта №1, (нулевой бит - физическое состояние, 1 - замкнут, 0 - разомкнут, первый бит - аварийность, 1 - авария, 0 - норма)
modbus_registers[423]=(signed char)(tempS);
tempS=0;
if(sk_stat[2]==ssON) tempS|=0x0001;
if(sk_av_stat[2]==sasON) tempS|=0x0002;
modbus_registers[424]=(signed char)(tempS>>8);				//Рег213	Состояние  сухого контакта №1, (нулевой бит - физическое состояние, 1 - замкнут, 0 - разомкнут, первый бит - аварийность, 1 - авария, 0 - норма)
modbus_registers[425]=(signed char)(tempS);
tempS=0;
if(sk_stat[3]==ssON) tempS|=0x0001;
if(sk_av_stat[3]==sasON) tempS|=0x0002;
modbus_registers[426]=(signed char)(tempS>>8);				//Рег214	Состояние  сухого контакта №1, (нулевой бит - физическое состояние, 1 - замкнут, 0 - разомкнут, первый бит - аварийность, 1 - авария, 0 - норма)
modbus_registers[427]=(signed char)(tempS);
*/

//modbus_registers[430]=(signed char)(tempS>>8);				//Рег216	Байт статуса батареи №2(0x01 - авария цепи батареи, 0x02 - авария средней точки батареи)
//modbus_registers[431]=(signed char)(tempS);

//tempS=bat_hndl_t_razr_min;
//modbus_registers[432]=(signed char)(tempS>>8);				//Рег217	Остаточное время работы батареи в минутах
//modbus_registers[433]=(signed char)(tempS);

//modbus_registers[434]=(signed char)(snmp_bat_flag[0]>>8);	//Рег218 //  //флаги АКБ №1
//modbus_registers[435]=(signed char)(snmp_bat_flag[0]);	
//modbus_registers[436]=(signed char)(snmp_bat_flag[1]>>8);	//Рег219 //  //флаги АКБ №2
//modbus_registers[437]=(signed char)(snmp_bat_flag[1]);
/*
Бит 0- равен 1, если напряжение на АКБ ниже уставки Uсигн., иначе равен нулю.
Бит 1- равен 1, если показание датчика температуры АКБ выше уставки t бат.сигн., иначе равен нулю. 
Бит 2- равен 1, если показание датчика температуры АКБ выше уставки t бат.мах., иначе равен нулю.
Бит 3- равен 1, если ток АКБ меньше нуля (АКБ разряжается), иначе равен нулю.
Бит 4- равен 1, если включена функция контроля емкости АКБ, иначе равен нулю.
Бит 5- равен 1, если включен выравнивающий заряд АКБ, иначе равен нулю.
Бит 6- равен 1, если режим выравнивающего заряда заблокирован.
Бит 7- равен 1, если включен ускоренный заряд АКБ, иначе равен нулю.
Бит 8- равен 1, если режим ускоренного заряда заблокирован.
Бит 9- равен 1, если включен уравнительный заряд АКБ, иначе равен нулю.
Бит 10- равен 1, если режим уравнительного заряда заблокирован.
Бит 11- равен 1, если включен формовочный заряд АКБ, иначе равен нулю.
Бит 12- равен 1, если режим формовочного заряда заблокирован.
*/
/*
modbus_registers[438]=(signed char)(bps[0]._vent_resurs>>8);	//Рег220 ресурс вентилятора БПС1
modbus_registers[439]=(signed char)(bps[0]._vent_resurs);
modbus_registers[440]=(signed char)(bps[1]._vent_resurs>>8);	//Рег221 ресурс вентилятора БПС2
modbus_registers[441]=(signed char)(bps[1]._vent_resurs);
modbus_registers[442]=(signed char)(bps[2]._vent_resurs>>8);	//Рег222 ресурс вентилятора БПС3
modbus_registers[443]=(signed char)(bps[2]._vent_resurs);
modbus_registers[444]=(signed char)(bps[3]._vent_resurs>>8);	//Рег223 ресурс вентилятора БПС4
modbus_registers[445]=(signed char)(bps[3]._vent_resurs);
modbus_registers[446]=(signed char)(bps[4]._vent_resurs>>8);	//Рег224 ресурс вентилятора БПС5
modbus_registers[447]=(signed char)(bps[4]._vent_resurs);
modbus_registers[448]=(signed char)(bps[5]._vent_resurs>>8);	//Рег225 ресурс вентилятора БПС6
modbus_registers[449]=(signed char)(bps[5]._vent_resurs);
modbus_registers[450]=(signed char)(bps[6]._vent_resurs>>8);	//Рег226 ресурс вентилятора БПС7
modbus_registers[451]=(signed char)(bps[6]._vent_resurs);
modbus_registers[452]=(signed char)(bps[7]._vent_resurs>>8);	//Рег227 ресурс вентилятора БПС8
modbus_registers[453]=(signed char)(bps[7]._vent_resurs); */
			

if(prot==MODBUS_RTU_PROT)
	{
	modbus_tx_buff[0]=adr;
	modbus_tx_buff[1]=func;

	modbus_tx_buff[2]=(char)(reg_quantity*2);

	mem_copy((signed char*)&modbus_tx_buff[3],(signed char*)&modbus_registers[(reg_adr-1)*2],reg_quantity*2);

	crc_temp=CRC16_2(modbus_tx_buff,(reg_quantity*2)+3);

	modbus_tx_buff[3+(reg_quantity*2)]=(char)crc_temp;
	modbus_tx_buff[4+(reg_quantity*2)]=crc_temp>>8;

	for (i=0;i<(5+(reg_quantity*2));i++)
		{
		putchar0(modbus_tx_buff[i]);
		}
/*	for (i=0;i<(5+(reg_quantity*2));i++)
		{
		putchar_sc16is700(modbus_tx_buff[i]);
		}*/
	}
else if(prot==MODBUS_TCP_PROT)
	{
	mem_copy((signed char*)modbus_tx_buff,(signed char*)&modbus_registers[(reg_adr-1)*2],reg_quantity*2);
	modbus_tcp_out_ptr=(signed char*)modbus_tx_buff;
	}
}

