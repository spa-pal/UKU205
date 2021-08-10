#include "http_data.h"
#include "control.h"
#include "eeprom_map.h"
#include "25lc640.h"
#include "common_func.h"
#include "main.h"
#include "stdio.h"
//#include "avar_hndl.h"

//Телеметрия сети
char http_power_num_of_phases;
short http_power_voltage_of_phase[3];
short http_power_frequncy;
char http_power_status;
char http_output_buff[70];
const char hex_alfa[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
char log_item_cnt=0;
char pal_cyr_coder_output[200];
char http_tm_output_buff[200];

//-----------------------------------------------
char* pal_cyr_decoder(char* input) 
{
char* output;
short i=0,ii=0;

output = pal_cyr_coder_output;
	
while (input[i])
	{
	if(input[i]=='^')
		{
		i++;
		if(input[i]=='X')
			{
			i++;
			if(input[i]=='A') output[ii++]='Ш';
			else if(input[i]=='E') output[ii++]='Ё';
			else if(input[i]=='C') output[ii++]='Ж';
			else if(input[i]=='D') output[ii++]='Щ';
			else if(input[i]=='B') output[ii++]='Ъ';
			else if(input[i]=='F') output[ii++]='Ы';
			else if(input[i]=='G') output[ii++]='Ь';
			else if(input[i]=='H') output[ii++]='Э';
			else if(input[i]=='a') output[ii++]='ш';
			else if(input[i]=='e') output[ii++]='ё';
			else if(input[i]=='c') output[ii++]='ж';
			else if(input[i]=='d') output[ii++]='щ';
			else if(input[i]=='b') output[ii++]='ъ';
			else if(input[i]=='f') output[ii++]='ы';
			else if(input[i]=='g') output[ii++]='ь';
			else if(input[i]=='y') output[ii++]='э';
			else if(input[i]=='i') output[ii++]='°';
			else if(input[i]=='j') output[ii++]='#';
			}
		else if(input[i]=='A') output[ii++]='А';
		else if(input[i]=='B') output[ii++]='Б';
		else if(input[i]=='C') output[ii++]='Ц';
		else if(input[i]=='D') output[ii++]='Д';
		else if(input[i]=='E') output[ii++]='Е';
		else if(input[i]=='F') output[ii++]='Ф';
		else if(input[i]=='G') output[ii++]='Г';
		else if(input[i]=='H') output[ii++]='Х';
		else if(input[i]=='I') output[ii++]='И';
		else if(input[i]=='J') output[ii++]='Й';
		else if(input[i]=='K') output[ii++]='К';
		else if(input[i]=='L') output[ii++]='Л';
		else if(input[i]=='M') output[ii++]='М';
		else if(input[i]=='N') output[ii++]='Н';
		else if(input[i]=='O') output[ii++]='О';
		else if(input[i]=='P') output[ii++]='П';
		else if(input[i]=='Q') output[ii++]='Я';
		else if(input[i]=='R') output[ii++]='Р';
		else if(input[i]=='S') output[ii++]='С';
		else if(input[i]=='T') output[ii++]='Т';
		else if(input[i]=='U') output[ii++]='У';
		else if(input[i]=='V') output[ii++]='Ю';
		else if(input[i]=='W') output[ii++]='В';
		else if(input[i]=='Y') output[ii++]='Ч';
		else if(input[i]=='Z') output[ii++]='З';
		else if(input[i]=='a') output[ii++]='а';
		else if(input[i]=='b') output[ii++]='б';
		else if(input[i]=='c') output[ii++]='ц';
		else if(input[i]=='d') output[ii++]='д';
		else if(input[i]=='e') output[ii++]='е';
		else if(input[i]=='f') output[ii++]='ф';
		else if(input[i]=='g') output[ii++]='г';
		else if(input[i]=='h') output[ii++]='х';
		else if(input[i]=='i') output[ii++]='и';
		else if(input[i]=='j') output[ii++]='й';
		else if(input[i]=='k') output[ii++]='к';
		else if(input[i]=='l') output[ii++]='л';
		else if(input[i]=='m') output[ii++]='м';
		else if(input[i]=='n') output[ii++]='н';
		else if(input[i]=='o') output[ii++]='о';
		else if(input[i]=='p') output[ii++]='п';
		else if(input[i]=='q') output[ii++]='я';
		else if(input[i]=='r') output[ii++]='р';
		else if(input[i]=='s') output[ii++]='с';
		else if(input[i]=='t') output[ii++]='т';
		else if(input[i]=='u') output[ii++]='у';
		else if(input[i]=='v') output[ii++]='ю';
		else if(input[i]=='w') output[ii++]='в';
		else if(input[i]=='y') output[ii++]='ч';
		else if(input[i]=='z') output[ii++]='з';
		i++;
		}
	else 
		{
		output[ii++]=input[i++];
		}
	}
output[ii]=0;
return output;
}

//-----------------------------------------------
char* pal_cyr_coder(char* in)
{
char* output;
short i=0,ii=0;
output = pal_cyr_coder_output;

while(in[i])
	{
	if(in[i]=='А')
		{
		output[ii++]='^';
		output[ii++]='A';
		i++;
		}
	else if(in[i]=='Б')
		{
		output[ii++]='^';
		output[ii++]='B';
		i++;
		}
	else if(in[i]=='В')
		{
		output[ii++]='^';
		output[ii++]='W';
		i++;
		}
	else if(in[i]=='Г')
		{
		output[ii++]='^';
		output[ii++]='G';
		i++;
		}
	else if(in[i]=='Д')
		{
		output[ii++]='^';
		output[ii++]='D';
		i++;
		}
	else if(in[i]=='Е')
		{
		output[ii++]='^';
		output[ii++]='E';
		i++;
		}
	else if(in[i]=='Ё')
		{
		output[ii++]='^';
		output[ii++]='X';
		output[ii++]='E';
		i++;
		}
	else if(in[i]=='Ж')
		{
		output[ii++]='^';
		output[ii++]='X';
		output[ii++]='C';
		i++;
		}
	else if(in[i]=='З')
		{
		output[ii++]='^';
		output[ii++]='Z';
		i++;
		}
	else if(in[i]=='И')
		{
		output[ii++]='^';
		output[ii++]='I';
		i++;
		}
	else if(in[i]=='Й')
		{
		output[ii++]='^';
		output[ii++]='J';
		i++;
		}
	else if(in[i]=='К')
		{
		output[ii++]='^';
		output[ii++]='K';
		i++;
		}
	else if(in[i]=='Л')
		{
		output[ii++]='^';
		output[ii++]='L';
		i++;
		}
	else if(in[i]=='М')
		{
		output[ii++]='^';
		output[ii++]='M';
		i++;
		}
	else if(in[i]=='Н')
		{
		output[ii++]='^';
		output[ii++]='N';
		i++;
		}
	else if(in[i]=='О')
		{
		output[ii++]='^';
		output[ii++]='O';
		i++;
		}
	else if(in[i]=='П')
		{
		output[ii++]='^';
		output[ii++]='P';
		i++;
		}
	else if(in[i]=='Р')
		{
		output[ii++]='^';
		output[ii++]='R';
		i++;
		}
	else if(in[i]=='С')
		{
		output[ii++]='^';
		output[ii++]='S';
		i++;
		}
	else if(in[i]=='Т')
		{
		output[ii++]='^';
		output[ii++]='T';
		i++;
		}
	else if(in[i]=='У')
		{
		output[ii++]='^';
		output[ii++]='U';
		i++;
		}
	else if(in[i]=='Ф')
		{
		output[ii++]='^';
		output[ii++]='F';
		i++;
		}
	else if(in[i]=='Х')
		{
		output[ii++]='^';
		output[ii++]='H';
		i++;
		}
	else if(in[i]=='Ц')
		{
		output[ii++]='^';
		output[ii++]='C';
		i++;
		}
	else if(in[i]=='Ч')
		{
		output[ii++]='^';
		output[ii++]='Y';
		i++;
		}
	else if(in[i]=='Ш')
		{
		output[ii++]='^';
		output[ii++]='X';
		output[ii++]='A';
		i++;
		}
	else if(in[i]=='Щ')
		{
		output[ii++]='^';
		output[ii++]='X';
		output[ii++]='D';
		i++;
		}
	else if(in[i]=='Ъ')
		{
		output[ii++]='^';
		output[ii++]='X';
		output[ii++]='B';
		i++;
		}
	else if(in[i]=='Ы')
		{
		output[ii++]='^';
		output[ii++]='X';
		output[ii++]='F';
		i++;
		}
	else if(in[i]=='Ь')
		{
		output[ii++]='^';
		output[ii++]='X';
		output[ii++]='G';
		i++;
		}
	else if(in[i]=='Э')
		{
		output[ii++]='^';
		output[ii++]='X';
		output[ii++]='H';
		i++;
		}
	else if(in[i]=='Ю')
		{
		output[ii++]='^';
		output[ii++]='V';
		i++;
		}
	else if(in[i]=='Я')
		{
		output[ii++]='^';
		output[ii++]='Q';
		i++;
		}
	else if(in[i]=='а')
		{
		output[ii++]='^';
		output[ii++]='a';
		i++;
		}
	else if(in[i]=='б')
		{
		output[ii++]='^';
		output[ii++]='b';
		i++;
		}
	else if(in[i]=='в')
		{
		output[ii++]='^';
		output[ii++]='w';
		i++;
		}
	else if(in[i]=='г')
		{
		output[ii++]='^';
		output[ii++]='g';
		i++;
		}
	else if(in[i]=='д')
		{
		output[ii++]='^';
		output[ii++]='d';
		i++;
		}
	else if(in[i]=='е')
		{
		output[ii++]='^';
		output[ii++]='e';
		i++;
		}
	else if(in[i]=='ё')
		{
		output[ii++]='^';
		output[ii++]='X';
		output[ii++]='e';
		i++;
		}
	else if(in[i]=='ж')
		{
		output[ii++]='^';
		output[ii++]='X';
		output[ii++]='c';
		i++;
		}
	else if(in[i]=='з')
		{
		output[ii++]='^';
		output[ii++]='z';
		i++;
		}
	else if(in[i]=='и')
		{
		output[ii++]='^';
		output[ii++]='i';
		i++;
		}
	else if(in[i]=='й')
		{
		output[ii++]='^';
		output[ii++]='j';
		i++;
		}
	else if(in[i]=='к')
		{
		output[ii++]='^';
		output[ii++]='k';
		i++;
		}
	else if(in[i]=='л')
		{
		output[ii++]='^';
		output[ii++]='l';
		i++;
		}
	else if(in[i]=='м')
		{
		output[ii++]='^';
		output[ii++]='m';
		i++;
		}
	else if(in[i]=='н')
		{
		output[ii++]='^';
		output[ii++]='n';
		i++;
		}
	else if(in[i]=='о')
		{
		output[ii++]='^';
		output[ii++]='o';
		i++;
		}
	else if(in[i]=='п')
		{
		output[ii++]='^';
		output[ii++]='p';
		i++;
		}
	else if(in[i]=='р')
		{
		output[ii++]='^';
		output[ii++]='r';
		i++;
		}
	else if(in[i]=='с')
		{
		output[ii++]='^';
		output[ii++]='s';
		i++;
		}
	else if(in[i]=='т')
		{
		output[ii++]='^';
		output[ii++]='t';
		i++;
		}
	else if(in[i]=='у')
		{
		output[ii++]='^';
		output[ii++]='u';
		i++;
		}
	else if(in[i]=='ф')
		{
		output[ii++]='^';
		output[ii++]='f';
		i++;
		}
	else if(in[i]=='х')
		{
		output[ii++]='^';
		output[ii++]='h';
		i++;
		}
	else if(in[i]=='ц')
		{
		output[ii++]='^';
		output[ii++]='c';
		i++;
		}
	else if(in[i]=='ч')
		{
		output[ii++]='^';
		output[ii++]='y';
		i++;
		}
	else if(in[i]=='ш')
		{
		output[ii++]='^';
		output[ii++]='X';
		output[ii++]='a';
		i++;
		}
	else if(in[i]=='щ')
		{
		output[ii++]='^';
		output[ii++]='X';
		output[ii++]='d';
		i++;
		}
	else if(in[i]=='ъ')
		{
		output[ii++]='^';
		output[ii++]='X';
		output[ii++]='b';
		i++;
		}
	else if(in[i]=='ы')
		{
		output[ii++]='^';
		output[ii++]='X';
		output[ii++]='f';
		i++;
		}
	else if(in[i]=='ь')
		{
		output[ii++]='^';
		output[ii++]='X';
		output[ii++]='g';
		i++;
		}
	else if(in[i]=='э')
		{
		output[ii++]='^';
		output[ii++]='X';
		output[ii++]='h';
		i++;
		}
	else if(in[i]=='ю')
		{
		output[ii++]='^';
		output[ii++]='v';
		i++;
		}
	else if(in[i]=='я')
		{
		output[ii++]='^';
		output[ii++]='q';
		i++;
		}
	else if(in[i]=='°')
		{
		output[ii++]='^';
		output[ii++]='X';
		output[ii++]='i';
		i++;
		}
	else if(in[i]=='№')
		{
		output[ii++]='^';
		output[ii++]='X';
		output[ii++]='j';
		i++;
		}
	else
		{
		output[ii++]=in[i++];
		}
	}

/*while(in[i])
	{
	output[ii++]=in[i++];
	}*/

output[ii++]=0;	
/*
for(i=0;i<4;i++)
	{
	output[ii++]=in[i++];
	}  

output[0]='0';
output[1]='1';
output[2]='2';
output[3]='3';
output[4]='4';
output[5]='5';
output[6]='6';
output[7]='7';
output[8]='8';
output[9]='9';
output[10]='a';
output[11]='b';
output[12]='c';
output[13]='d';
output[14]='e';
output[15]='f';
output[16]='g';
output[17]='h';
output[18]='i';
output[19]='j';
output[20]=0;  */

return output;
}

//-----------------------------------------------
void http_data(void)
{
http_power_num_of_phases=1;
if((http_power_num_of_phases!=1)&&(http_power_num_of_phases!=3)) http_power_num_of_phases=0;
http_power_voltage_of_phase[0]=Unet;
//http_power_voltage_of_phase[1]=net_Ub;
//http_power_voltage_of_phase[2]=net_Uc;
http_power_frequncy = fnet;
http_power_status=0;
if(St&0x01)http_power_status=1;
//http_power_status=net_av;
};

//-----------------------------------------------
short http_get_log_deep(void)
{
return lc640_read_int(CNT_EVENT_LOG);
};

//-----------------------------------------------
char* http_get_log_rec(char num)
{
char i;
unsigned int tempii;
char buff[40];

for (i=0;i<40;i++) buff[i]=0;

tempii=lc640_read_int(PTR_EVENT_LOG);
tempii=ptr_carry(tempii,64,-1*((signed)num));
tempii*=32;
tempii+=EVENT_LOG;

lc640_read_long_ptr(tempii,buff);
lc640_read_long_ptr(tempii+4,buff+4);
lc640_read_long_ptr(tempii+8,buff+8);
lc640_read_long_ptr(tempii+12,buff+12);
lc640_read_long_ptr(tempii+16,buff+16);
lc640_read_long_ptr(tempii+20,buff+20);
lc640_read_long_ptr(tempii+24,buff+24);
lc640_read_long_ptr(tempii+28,buff+28);

for (i=0;i<32;i++)
	{
	http_output_buff[i*2]=hex_alfa[buff[i]/16];
	http_output_buff[(i*2)+1]=hex_alfa[buff[i]%16];
	}
http_output_buff[64]=0;

return http_output_buff;
}

//-----------------------------------------------
char* http_tm_dt_output(char numOfDt)
{
//char buffer[100];
sprintf(http_tm_output_buff,"%d %d", 29/*t_ext[numOfDt]*/, 0/*ND_EXT[numOfDt]*/);
/*http_tm_output_buff[0]='a';
http_tm_output_buff[1]='b';
http_tm_output_buff[2]='c';
http_tm_output_buff[3]='d';
http_tm_output_buff[4]='e';
http_tm_output_buff[5]=0;*/
return http_tm_output_buff;
//sprintf(buffer,"%d %d", 29/*t_ext[numOfDt]*/, 0/*ND_EXT[numOfDt]*/);

//return buffer;
}

//-----------------------------------------------
char* http_tm_dt_output_(char numOfDt)
{
//char buffer[100];

sprintf(http_tm_output_buff,"%d %d", 29/*t_ext[numOfDt]*/, 0/*ND_EXT[numOfDt]*/);
/*http_tm_output_buff[0]='a';
http_tm_output_buff[1]='b';
http_tm_output_buff[2]='c';
http_tm_output_buff[3]='d';
http_tm_output_buff[4]='e';
http_tm_output_buff[5]=0;*/
return http_tm_output_buff;
}

//-----------------------------------------------
char* http_tm_sk_output(char numOfSk)
{
char buffer[100];
char temp1=0;
char temp2=0;
/*
if(sk_stat[numOfSk]==ssON)temp1=1;
if(sk_av_stat[numOfSk]==sasON)temp2=1; */

sprintf(buffer,"%d %d", temp1, temp2);

return buffer;
}

//-----------------------------------------------
char http_bps_status2number(char number)
{
//return number+spirit_wrk_cnt;
/*if((bps[number]._state==bsWRK)&&(!bps[number]._flags_tm)) 		return 1;
if((bps[number]._state==bsRDY)) 								return 2;
if((bps[number]._state==bsWRK)&&(bps[number]._flags_tm&0x08)) 	return 3;
if((bps[number]._state==bsBL)) 									return 4;
if((bps[number]._state==bsAPV)) 								return 5;
if((bps[number]._av&(1<<0))) 									return 6;
if((bps[number]._av&(1<<2))) 									return 7;
if((bps[number]._av&(1<<1))) 									return 8;
if((bps[number]._av&(1<<3))) 									return 9;
if((bps[number]._state==bsOFF_AV_NET)) 							return 10; */
}

//-----------------------------------------------
char* http_tm_src_output(char numOfSrc)
{
//char buffer[100]={"123 456 789"};

sprintf(http_tm_output_buff,"%d %d %d "/*%d 0x%02x*/, /*123*/Us[numOfSrc], /*456*/Is[numOfSrc], /*789*/t_i[numOfSrc]/*, http_bps_status2number(numOfSrc), bps[numOfSrc]._flags_tm*/ );

return http_tm_output_buff;
}

//-----------------------------------------------
char* http_ip_output(char ip1, char ip2, char ip3, char ip4)
{
//char buffer[100];

sprintf(http_tm_output_buff,"%d.%d.%d.%d", ip1, ip2, ip3, ip4);

return http_tm_output_buff;
}

//-----------------------------------------------
char* http_tm_bat_output(char numOfBat)
{
//char buffer[300];
char* batstat="abcdef";

short batison=0;
short batcreal=-1;
short batubm=-1;
short batnd;
if(BAT_IS_ON==bisON)batison=1;
if(BAT_C_REAL!=0x5555)batcreal=BAT_C_REAL;
else batcreal=-BAT_C_NOM*10;
batubm=0;
batnd=0;
if(NDB)batnd=1;

if(!bit_minus)			batstat=pal_cyr_coder("заряжается");
else 					batstat=pal_cyr_coder("разряжается");
if(St&0x02)				batstat=pal_cyr_coder("Авария цепи батареи!!!");
//if(bat[numOfBat]._av&2)batstat=pal_cyr_coder("Авария средней точки батареи!!!"); */
//batstat=pal_cyr_coder("Авария средней точки батареи!!!");

sprintf(http_tm_output_buff,"%d, %d, %d, %d, %d, %d, %d, %d, %d, %s", batison, Ubat, Ibat, t_b,
 		batnd, batcreal, zar_percent, 0,batubm, batstat);

//sprintf(buffer,"%d, %d, %d", 34, 56, 78);
return http_tm_output_buff;
}

/*

 char iii;
char dt[4],dt_[4],dt__[4];
    
		


     
lc640_read_long_ptr(tempii,dt);
lc640_read_long_ptr(tempii+8,dt_);
lc640_read_long_ptr(tempii+12,dt__);
*/

void demo_avar_vrite(void)
{

}

