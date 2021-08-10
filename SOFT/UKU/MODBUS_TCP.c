
#include <lpc17xx.h>
#include <rtl.h>
#include "main.h"
#include "modbus_tcp.h"
#include "eeprom_map.h"
#include "25lc640.h"

char plazma_modbus_tcp[20];
char modbus_tcp_plazma[20];

char modbus_tcp_func;
char modbus_tcp_unit;
short modbus_tcp_rx_arg0;
short modbus_tcp_rx_arg1;

//#define MODBUS_TCP_PROT	1

char* modbus_tcp_out_ptr;

U8 socket_tcp;
U8 tcp_connect_stat;

/*--------------------------- TCP socket ------------------------------------*/

U16 tcp_callback (U8 soc, U8 evt, U8 *ptr, U16 par) 
{
/* This function is called by the TCP module on TCP event */
/* Check the 'Net_Config.h' for possible events.          */
par = par;

if (soc != socket_tcp) {
	return (0);
}

modbus_tcp_plazma[0]++;

switch (evt) 
	{
    case TCP_EVT_DATA:
    /* TCP data frame has arrived, data is located at *par1, */
    /* data length is par2. Allocate buffer to send reply.   */
    //procrec(ptr);
	
	plazma_modbus_tcp[1]=ptr[0];
	plazma_modbus_tcp[2]=ptr[1];
	plazma_modbus_tcp[3]=ptr[2];
	plazma_modbus_tcp[4]=ptr[3];
	plazma_modbus_tcp[5]=ptr[4];
	plazma_modbus_tcp[6]=ptr[5];
	plazma_modbus_tcp[7]=ptr[6];
	plazma_modbus_tcp[8]=ptr[7];
	plazma_modbus_tcp[9]=ptr[8];
	plazma_modbus_tcp[10]=ptr[9];
	plazma_modbus_tcp[11]=ptr[10];
	  //plazma_modbus_tcp[4]=ptr[3];
	  //plazma_modbus_tcp[5]=par;
	  //plazma_modbus_tcp[6]=ptr[5];

   	
	modbus_tcp_func=ptr[7];
	modbus_tcp_unit=ptr[6];

	modbus_tcp_rx_arg0=(((unsigned short)ptr[8])*((unsigned short)256))+((unsigned short)ptr[9]);
	modbus_tcp_rx_arg1=(((unsigned short)ptr[10])*((unsigned short)256))+((unsigned short)ptr[11]);

	if(modbus_tcp_unit==MODBUS_ADRESS)
		{
		char modbus_tcp_tx_buff[200];

		if(modbus_tcp_func==3)		//чтение произвольного кол-ва регистров хранения
			{
			U8 *sendbuf;
			

			modbus_hold_registers_transmit(MODBUS_ADRESS,modbus_tcp_func,modbus_tcp_rx_arg0,modbus_tcp_rx_arg1,MODBUS_TCP_PROT);

			sendbuf = tcp_get_buf((modbus_tcp_rx_arg1*2)+9);
			sendbuf[0]=ptr[0];
			sendbuf[1]=ptr[1];
			sendbuf[2]=ptr[2];
			sendbuf[3]=ptr[3];
			sendbuf[4]=((modbus_tcp_rx_arg1*2)+3)/256;
			sendbuf[5]=((modbus_tcp_rx_arg1*2)+3)%256;;
			sendbuf[6]=modbus_tcp_unit;
			sendbuf[7]=modbus_tcp_func;
			sendbuf[8]=(U8)(modbus_tcp_rx_arg1*2);
			mem_copy((char*)&sendbuf[9],modbus_tcp_out_ptr,(modbus_tcp_rx_arg1*2));
			//sendbuf[9]=3;
			//sendbuf[10]=4;
          	tcp_send (socket_tcp, sendbuf, ((modbus_tcp_rx_arg1*2)+9));
			//
			
			//modbus_tcp_tx_buff[4]=0;
			//modbus_tcp_tx_buff[5]=5;
			//modbus_tcp_tx_buff[6]=1;//modbus_tcp_unit;
			//modbus_tcp_tx_buff[7]=3;//modbus_tcp_func;
			//modbus_tcp_tx_buff[8]=2;
			//mem_copy((char*)&modbus_tcp_tx_buff[9],modbus_tcp_out_ptr,2);
			//modbus_tcp_tx_buff[9]=2;
			//modbus_tcp_tx_buff[10]=3;
			//tcp_send (socket_tcp, modbus_tcp_tx_buff, 11);


			}

		if(modbus_tcp_func==4)		//чтение произвольного кол-ва регистров	входов
			{
			U8 *sendbuf;

			modbus_input_registers_transmit(MODBUS_ADRESS,modbus_tcp_func,modbus_tcp_rx_arg0,modbus_tcp_rx_arg1,MODBUS_TCP_PROT);

			sendbuf = tcp_get_buf((modbus_tcp_rx_arg1*2)+9);
			sendbuf[0]=ptr[0];
			sendbuf[1]=ptr[1];
			sendbuf[2]=ptr[2];
			sendbuf[3]=ptr[3];
			sendbuf[4]=((modbus_tcp_rx_arg1*2)+3)/256;
			sendbuf[5]=((modbus_tcp_rx_arg1*2)+3)%256;;
			sendbuf[6]=modbus_tcp_unit;
			sendbuf[7]=modbus_tcp_func;
			sendbuf[8]=(U8)(modbus_tcp_rx_arg1*2);
			mem_copy((char*)&sendbuf[9],modbus_tcp_out_ptr,(modbus_tcp_rx_arg1*2));
			//sendbuf[9]=3;
			//sendbuf[10]=4;
          	tcp_send (socket_tcp, sendbuf, ((modbus_tcp_rx_arg1*2)+9));
			//
			
			//modbus_tcp_tx_buff[4]=0;
			//modbus_tcp_tx_buff[5]=5;
			//modbus_tcp_tx_buff[6]=1;//modbus_tcp_unit;
			//modbus_tcp_tx_buff[7]=3;//modbus_tcp_func;
			//modbus_tcp_tx_buff[8]=2;
			//mem_copy((char*)&modbus_tcp_tx_buff[9],modbus_tcp_out_ptr,2);
			//modbus_tcp_tx_buff[9]=2;
			//modbus_tcp_tx_buff[10]=3;
			//tcp_send (socket_tcp, modbus_tcp_tx_buff, 11);
			}

		else if(modbus_tcp_func==6) 	//запись регистров хранения
			{
			U8 *sendbuf;

			
			if(modbus_tcp_rx_arg0==11)		//Установка времени 
				{
				LPC_RTC->YEAR=(uint16_t)modbus_tcp_rx_arg1;
				}
			if(modbus_tcp_rx_arg0==12)		//Установка времени 
				{
				LPC_RTC->MONTH=(uint16_t)modbus_tcp_rx_arg1;
				}
			if(modbus_tcp_rx_arg0==13)		//Установка времени 
				{
				LPC_RTC->DOM=(uint16_t)modbus_tcp_rx_arg1;
				}
			if(modbus_tcp_rx_arg0==14)		//Установка времени 
				{
				plazma_modbus_tcp[0]++;
				LPC_RTC->HOUR=(uint16_t)modbus_tcp_rx_arg1;
				}
			if(modbus_tcp_rx_arg0==15)		//Установка времени 
				{
				LPC_RTC->MIN=(uint16_t)modbus_tcp_rx_arg1;
				}
				if(modbus_tcp_rx_arg0==16)		//Установка времени 
					{
					LPC_RTC->SEC=(uint16_t)modbus_tcp_rx_arg1;
					}
				if(modbus_tcp_rx_arg0==20)		//ток стабилизации для режима стабилизации тока
					{
					if((modbus_tcp_rx_arg1>0)&&(modbus_tcp_rx_arg1<=18))
					lc640_write_int(EE_NUMIST,modbus_tcp_rx_arg1);  
					}
				if(modbus_tcp_rx_arg0==21)		//ток стабилизации для режима стабилизации тока
					{
					//if((modbus_tcp_rx_arg1==0)||(modbus_tcp_rx_arg1==1))
					//lc640_write_int(EE_PAR,modbus_tcp_rx_arg1);  
					}
				if(modbus_tcp_rx_arg0==22)		//ток стабилизации для режима стабилизации тока
					{
					if((modbus_tcp_rx_arg1==0)||(modbus_tcp_rx_arg1==1))
					lc640_write_int(EE_ZV_ON,modbus_tcp_rx_arg1);  
					}
				if(modbus_tcp_rx_arg0==23)		//ток стабилизации для режима стабилизации тока
					{
					//if((modbus_tcp_rx_arg1==0)||(modbus_tcp_rx_arg1==1))
					//lc640_write_int(EE_TERMOKOMP,modbus_tcp_rx_arg1);  
					}
				if(modbus_tcp_rx_arg0==24)		//ток стабилизации для режима стабилизации тока
					{
					if((modbus_tcp_rx_arg1>=0)||(modbus_tcp_rx_arg1<=20))
					lc640_write_int(EE_UBM_AV,modbus_tcp_rx_arg1);  
					}
	
	
				if(modbus_tcp_rx_arg0==30)		//напряжение стабилизации для режима стабилизации напряжения
					{
					if(modbus_tcp_rx_arg1<0)TBAT=0;
					else if((modbus_tcp_rx_arg1>0)&&(modbus_tcp_rx_arg1<=5))modbus_tcp_rx_arg1=0;
					else if(modbus_tcp_rx_arg1>=60)TBAT=60;
					else TBAT=modbus_tcp_rx_arg1;
					lc640_write_int(EE_TBAT,TBAT);
		     		}
				if(modbus_tcp_rx_arg0==31)		//
					{
					lc640_write_int(EE_UMAX,modbus_tcp_rx_arg1);
		     		}
				if(modbus_tcp_rx_arg0==32)		//
					{
					//lc640_write_int(EE_DU,UB20-modbus_tcp_rx_arg1);
		     		}
				if(modbus_tcp_rx_arg0==33)		//
					{
					lc640_write_int(EE_UB0,modbus_tcp_rx_arg1);
		     		}
				if(modbus_tcp_rx_arg0==34)		//
					{
					lc640_write_int(EE_UB20,modbus_tcp_rx_arg1);
		     		}
				if(modbus_tcp_rx_arg0==35)		//
					{
					lc640_write_int(EE_USIGN,modbus_tcp_rx_arg1);
		     		}
				if(modbus_tcp_rx_arg0==36)		//
					{
					lc640_write_int(EE_UMN,modbus_tcp_rx_arg1);
		     		}
				if(modbus_tcp_rx_arg0==37)		//
					{
					lc640_write_int(EE_U0B,modbus_tcp_rx_arg1);
		     		}
				if(modbus_tcp_rx_arg0==38)		//
					{
					lc640_write_int(EE_IKB,modbus_tcp_rx_arg1);
		     		}
				if(modbus_tcp_rx_arg0==39)		//
					{
					lc640_write_int(EE_IZMAX,modbus_tcp_rx_arg1);
		     		}
				if(modbus_tcp_rx_arg0==40)		//
					{
					lc640_write_int(EE_IMAX,modbus_tcp_rx_arg1);
		     		}
				if(modbus_tcp_rx_arg0==41)		//
					{
					lc640_write_int(EE_IMIN,modbus_tcp_rx_arg1);
		     		}
				if(modbus_tcp_rx_arg0==42)		//
					{
					//lc640_write_int(EE_UVZ,modbus_tcp_rx_arg1);
		     		}
				if(modbus_tcp_rx_arg0==43)		//
					{
					lc640_write_int(EE_TZAS,modbus_tcp_rx_arg1);
		     		}
				if(modbus_tcp_rx_arg0==44)		//
					{
					lc640_write_int(EE_TMAX,modbus_tcp_rx_arg1);
		     		}
				if(modbus_tcp_rx_arg0==45)		//
					{
					//lc640_write_int(EE_TSIGN,modbus_tcp_rx_arg1);
		     		}
				if(modbus_tcp_rx_arg0==46)		//
					{
				//	lc640_write_int(EE_TBATMAX,modbus_tcp_rx_arg1);
		     		}
				if(modbus_tcp_rx_arg0==47)		//
					{
				//	lc640_write_int(EE_TBATSIGN,modbus_tcp_rx_arg1);
		     		}
				if(modbus_tcp_rx_arg0==48)		//
					{
				//	lc640_write_int(EE_SPEED_CHRG_CURR,modbus_tcp_rx_arg1);
		     		}
				if(modbus_tcp_rx_arg0==49)		//
					{
				//	lc640_write_int(EE_SPEED_CHRG_VOLT,modbus_tcp_rx_arg1);
		     		}
				if(modbus_tcp_rx_arg0==50)		//
					{
				//	lc640_write_int(EE_SPEED_CHRG_TIME,modbus_tcp_rx_arg1);
		     		}
				if(modbus_tcp_rx_arg0==51)		//
					{
				//	lc640_write_int(EE_U_OUT_KONTR_MAX,modbus_tcp_rx_arg1);
		     		}
				if(modbus_tcp_rx_arg0==52)		//
					{
				//	lc640_write_int(EE_U_OUT_KONTR_MIN,modbus_tcp_rx_arg1);
		     		}
				if(modbus_tcp_rx_arg0==53)		//
					{
				//	lc640_write_int(EE_U_OUT_KONTR_DELAY,modbus_tcp_rx_arg1);
		     		}
		//o_10_s	 забыл в прошлый раз про modbus tcp
			if(modbus_tcp_rx_arg0==54)		//
				{
				lc640_write_int(EE_UB0,modbus_tcp_rx_arg1);
				lc640_write_int(EE_UB20,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==55)		//
				{
				lc640_write_int(EE_UMAXN,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==56)		
				{
				if(modbus_tcp_rx_arg1<=3) lc640_write_int(EE_SNTP_ENABLE,modbus_tcp_rx_arg1);
	     		}						
			if(modbus_tcp_rx_arg0==57)		
				{
				signed short www=(signed short)modbus_tcp_rx_arg1;
				if(www>=-12 && www<=13) lc640_write_int(EE_SNTP_GMT,modbus_tcp_rx_arg1);
	     		}						
			if(modbus_tcp_rx_arg0==58)		
				{
				if(modbus_tcp_rx_arg1<=255) lc640_write_int(EE_SNTP_IP1,modbus_tcp_rx_arg1);
	     		}						
			if(modbus_tcp_rx_arg0==59)		
				{
				if(modbus_tcp_rx_arg1<=255) lc640_write_int(EE_SNTP_IP2,modbus_tcp_rx_arg1);
	     		}						
			if(modbus_tcp_rx_arg0==60)		
				{
				if(modbus_tcp_rx_arg1<=255) lc640_write_int(EE_SNTP_IP3,modbus_tcp_rx_arg1);
	     		}						
			if(modbus_tcp_rx_arg0==61)		
				{
				if(modbus_tcp_rx_arg1<=255) lc640_write_int(EE_SNTP_IP4,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==62)		
				{
				#ifdef UKU_220_IPS_TERMOKOMPENSAT
				if(modbus_tcp_rx_arg1<=2) lc640_write_int(EE_NUMBAT,modbus_tcp_rx_arg1);
				#else
				if(modbus_tcp_rx_arg1<=1) lc640_write_int(EE_NUMBAT,modbus_tcp_rx_arg1);
				#endif
	     		}
			if(modbus_tcp_rx_arg0==63)		
				{
				if(modbus_tcp_rx_arg1<=3) lc640_write_int(EE_NUMDT,modbus_tcp_rx_arg1);
	     		}	
			if(modbus_tcp_rx_arg0==64)		
				{
				//if(modbus_tcp_rx_arg1==0 || modbus_tcp_rx_arg1==2 || modbus_tcp_rx_arg1==4) lc640_write_int(EE_NUMMAKB,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==65)		
				{
				//if(modbus_tcp_rx_arg1<=4) lc640_write_int(EE_NUMSK,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==66)		
				{
				//if(modbus_tcp_rx_arg1==0 || modbus_tcp_rx_arg1==1) lc640_write_int(EE_NUM_RKI,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==67)		
				{
				//if(modbus_tcp_rx_arg1==0 || modbus_tcp_rx_arg1==1) lc640_write_int(EE_NUM_NET_IN,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==68)		
				{
			//	if(modbus_tcp_rx_arg1==0 || modbus_tcp_rx_arg1==1) lc640_write_int(EE_NUMBDR,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==69)		
				{
				//if(modbus_tcp_rx_arg1>0 && modbus_tcp_rx_arg1<9) lc640_write_int(EE_NUMENMV,modbus_tcp_rx_arg1); //o_12
	     		}
		
			if(modbus_tcp_rx_arg0==86)		
				{
				//if(modbus_tcp_rx_arg1>=15 && modbus_tcp_rx_arg1<=250) lc640_write_int(EE_UVENTOFF,modbus_tcp_rx_arg1);
	     		}			
			if(modbus_tcp_rx_arg0==87)		
				{
				//if(modbus_tcp_rx_arg1>=10 && modbus_tcp_rx_arg1<=2000) lc640_write_int(EE_IMAX_VZ,modbus_tcp_rx_arg1);
	     		}  
			if(modbus_tcp_rx_arg0==88)		
				{
				if(modbus_tcp_rx_arg1<=72) lc640_write_int(EE_VZ_HR,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==89)		
				{
				//if(modbus_tcp_rx_arg1==0 || modbus_tcp_rx_arg1==1) lc640_write_int(EE_VZ_CH_VENT_BLOK,modbus_tcp_rx_arg1);
	     		} 
			if(modbus_tcp_rx_arg0==90)		
				{
				//if(modbus_tcp_rx_arg1==0 || modbus_tcp_rx_arg1==1) lc640_write_int(EE_SPEED_CHRG_AVT_EN,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==91)		
				{
				//if(modbus_tcp_rx_arg1>0 && modbus_tcp_rx_arg1<=100) lc640_write_int(EE_SPEED_CHRG_D_U,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==92)		
				{
				//if(modbus_tcp_rx_arg1==0 || modbus_tcp_rx_arg1==2) lc640_write_int(EE_SPEED_CHRG_BLOCK_SRC,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==93)		
				{
				//if(modbus_tcp_rx_arg1==0 || modbus_tcp_rx_arg1==1) lc640_write_int(EE_SPEED_CHRG_BLOCK_LOG,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==94)		
				{								
				//if(modbus_tcp_rx_arg1==0 || modbus_tcp_rx_arg1==1) lc640_write_int(EE_SP_CH_VENT_BLOK,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==95)		
				{								
				//if(modbus_tcp_rx_arg1>=UB20 && modbus_tcp_rx_arg1<=2600) lc640_write_int(EE_UZ_U,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==96)		
				{
				//if(modbus_tcp_rx_arg1>=10 && modbus_tcp_rx_arg1<=10000) lc640_write_int(EE_UZ_IMAX,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==97)		
				{
				//if(modbus_tcp_rx_arg1>=1 && modbus_tcp_rx_arg1<=72) lc640_write_int(EE_UZ_T,modbus_tcp_rx_arg1);
	     		}					  
			if(modbus_tcp_rx_arg0==98)		
				{
				//if(modbus_tcp_rx_arg1>=UB20 && modbus_tcp_rx_arg1<=3000) lc640_write_int(EE_FZ_U1,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==99)		
				{
				//if(modbus_tcp_rx_arg1>=10 && modbus_tcp_rx_arg1<=1000) lc640_write_int(EE_FZ_IMAX1,modbus_tcp_rx_arg1);
	     		}
			// 2 регистра внизу
			if(modbus_tcp_rx_arg0==102)		
				{
				//if(modbus_tcp_rx_arg1>=1 && modbus_tcp_rx_arg1<=10) lc640_write_int(EE_FZ_T1,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==103)		
				{
				//if(modbus_tcp_rx_arg1>=10 && modbus_tcp_rx_arg1<=1000) lc640_write_int(EE_FZ_ISW12,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==104)		
				{
				//if(modbus_tcp_rx_arg1>=UB20 && modbus_tcp_rx_arg1<=3000) lc640_write_int(EE_FZ_U2,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==105)		
				{
				//if(modbus_tcp_rx_arg1>=10 && modbus_tcp_rx_arg1<=1000) lc640_write_int(EE_FZ_IMAX2,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==106)		
				{
				//if(modbus_tcp_rx_arg1>=1 && modbus_tcp_rx_arg1<=10) lc640_write_int(EE_FZ_T2,modbus_tcp_rx_arg1);
	     		}	 
			if(modbus_tcp_rx_arg0==107)		
				{
				if(modbus_tcp_rx_arg1==0 || modbus_tcp_rx_arg1==1) lc640_write_int(EE_AV_OFF_AVT,modbus_tcp_rx_arg1);
	     		}				
		   	if(modbus_tcp_rx_arg0==108)		
				{
				if(modbus_tcp_rx_arg1==0 || modbus_tcp_rx_arg1==1) lc640_write_int(EE_APV_ON1,modbus_tcp_rx_arg1);
	     		}
		  	if(modbus_tcp_rx_arg0==109)		
				{
				if(modbus_tcp_rx_arg1==0 || modbus_tcp_rx_arg1==1) lc640_write_int(EE_APV_ON2,modbus_tcp_rx_arg1);
	     		}		   
		 	if(modbus_tcp_rx_arg0==110)		
				{
				if(modbus_tcp_rx_arg1>=1 && modbus_tcp_rx_arg1<=24) lc640_write_int(EE_APV_ON2_TIME,modbus_tcp_rx_arg1);
	     		}  
			if(modbus_tcp_rx_arg0==111)		
				{
				if(modbus_tcp_rx_arg1==0)	   lc640_write_int(EE_SK_SIGN0,0xFFFF);
				else if(modbus_tcp_rx_arg1==1) lc640_write_int(EE_SK_SIGN0,0);
	     		} 	   
			if(modbus_tcp_rx_arg0==112)		
				{
				if(modbus_tcp_rx_arg1==0)	   lc640_write_int(EE_SK_ZVUK_EN0,0xFFFF);
				else if(modbus_tcp_rx_arg1==1) lc640_write_int(EE_SK_ZVUK_EN0,0);
	     		}
			if(modbus_tcp_rx_arg0==113)		
				{
				if(modbus_tcp_rx_arg1==0)	   lc640_write_int(EE_SK_LCD_EN0,0xFFFF);
				else if(modbus_tcp_rx_arg1==1) lc640_write_int(EE_SK_LCD_EN0,0);
	     		}
			if(modbus_tcp_rx_arg0==114)		
				{
				if(modbus_tcp_rx_arg1==0)	   lc640_write_int(EE_SK_SIGN1,0xFFFF);
				else if(modbus_tcp_rx_arg1==1) lc640_write_int(EE_SK_SIGN1,0);
	     		} 	   
			if(modbus_tcp_rx_arg0==115)		
				{
				if(modbus_tcp_rx_arg1==0)	   lc640_write_int(EE_SK_ZVUK_EN1,0xFFFF);
				else if(modbus_tcp_rx_arg1==1) lc640_write_int(EE_SK_ZVUK_EN1,0);
	     		}
			if(modbus_tcp_rx_arg0==116)		
				{
				if(modbus_tcp_rx_arg1==0)	   lc640_write_int(EE_SK_LCD_EN1,0xFFFF);
				else if(modbus_tcp_rx_arg1==1) lc640_write_int(EE_SK_LCD_EN1,0);
	     		}
			if(modbus_tcp_rx_arg0==117)		
				{
				if(modbus_tcp_rx_arg1==0)	   lc640_write_int(EE_SK_SIGN2,0xFFFF);
				else if(modbus_tcp_rx_arg1==1) lc640_write_int(EE_SK_SIGN2,0);
	     		} 	   
			if(modbus_tcp_rx_arg0==118)		
				{
				if(modbus_tcp_rx_arg1==0)	   lc640_write_int(EE_SK_ZVUK_EN2,0xFFFF);
				else if(modbus_tcp_rx_arg1==1) lc640_write_int(EE_SK_ZVUK_EN2,0);
	     		}
			if(modbus_tcp_rx_arg0==119)		
				{
				if(modbus_tcp_rx_arg1==0)	   lc640_write_int(EE_SK_LCD_EN2,0xFFFF);
				else if(modbus_tcp_rx_arg1==1) lc640_write_int(EE_SK_LCD_EN2,0);
	     		}
			if(modbus_tcp_rx_arg0==120)		
				{
				if(modbus_tcp_rx_arg1==0)	   lc640_write_int(EE_SK_SIGN3,0xFFFF);
				else if(modbus_tcp_rx_arg1==1) lc640_write_int(EE_SK_SIGN3,0);
	     		} 	   
			if(modbus_tcp_rx_arg0==121)		
				{
				if(modbus_tcp_rx_arg1==0)	   lc640_write_int(EE_SK_ZVUK_EN3,0xFFFF);
				else if(modbus_tcp_rx_arg1==1) lc640_write_int(EE_SK_ZVUK_EN3,0);
	     		}
			if(modbus_tcp_rx_arg0==122)		
				{
				if(modbus_tcp_rx_arg1==0)	   lc640_write_int(EE_SK_LCD_EN3,0xFFFF);
				else if(modbus_tcp_rx_arg1==1) lc640_write_int(EE_SK_LCD_EN3,0);
	     		}		  
		   	if(modbus_tcp_rx_arg0==123)		
				{
				//if(modbus_tcp_rx_arg1==0 || modbus_tcp_rx_arg1==1) lc640_write_int(EE_TERMOKOMP,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==124)		
				{
				//if(modbus_tcp_rx_arg1<=500) lc640_write_int(EE_FORVARDBPSCHHOUR,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==125)		
				{
				//if(modbus_tcp_rx_arg1==0 || modbus_tcp_rx_arg1==1) lc640_write_int(EE_DOP_RELE_FUNC,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==126)		
				{
				//if(modbus_tcp_rx_arg1<=2) lc640_write_int(EE_IPS_BLOCK_SRC,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==127)		
				{
				//if(modbus_tcp_rx_arg1==0 || modbus_tcp_rx_arg1==1) lc640_write_int(EE_IPS_BLOCK_LOG,modbus_tcp_rx_arg1);
	     		} 
		   	if(modbus_tcp_rx_arg0==128)		
				{
				if(modbus_tcp_rx_arg1>0 && modbus_tcp_rx_arg1<=100) lc640_write_int(EE_MODBUS_ADRESS,modbus_tcp_rx_arg1);
	     		}
/*			if(modbus_tcp_rx_arg0==129)		
				{
				if(modbus_tcp_rx_arg1==120 || modbus_tcp_rx_arg1==240 || modbus_tcp_rx_arg1==480 || modbus_tcp_rx_arg1==960 || modbus_tcp_rx_arg1==1920 
				|| modbus_tcp_rx_arg1==3840 || modbus_tcp_rx_arg1==5760 || modbus_tcp_rx_arg1==11520){ 
					lc640_write_int(EE_MODBUS_BAUDRATE,modbus_tcp_rx_arg1);
					MODBUS_BAUDRATE=modbus_tcp_rx_arg1;
					}
	     		} */	
			if(modbus_tcp_rx_arg0==130)		
				{
				if(modbus_tcp_rx_arg1==0 || modbus_tcp_rx_arg1==1) lc640_write_int(EE_ETH_IS_ON,modbus_tcp_rx_arg1);
	     		}	 
			if(modbus_tcp_rx_arg0==131)		
				{
				if(modbus_tcp_rx_arg1==0 || modbus_tcp_rx_arg1==1) lc640_write_int(EE_ETH_DHCP_ON,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==132)  //IP адрес
				{
				if(modbus_tcp_rx_arg1<=255) lc640_write_int(EE_ETH_IP_1,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==133)		
				{
				if(modbus_tcp_rx_arg1<=255) lc640_write_int(EE_ETH_IP_2,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==134)		
				{
				if(modbus_tcp_rx_arg1<=255) lc640_write_int(EE_ETH_IP_3,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==135)		
				{
				if(modbus_tcp_rx_arg1<=255) lc640_write_int(EE_ETH_IP_4,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==136)	//маска подсети	
				{
				if(modbus_tcp_rx_arg1<=255) lc640_write_int(EE_ETH_MASK_1,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==137)		
				{
				if(modbus_tcp_rx_arg1<=255) lc640_write_int(EE_ETH_MASK_2,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==138)		
				{
				if(modbus_tcp_rx_arg1<=255) lc640_write_int(EE_ETH_MASK_3,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==139)		
				{
				if(modbus_tcp_rx_arg1<=255) lc640_write_int(EE_ETH_MASK_4,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==140)	//шлюз
				{
				if(modbus_tcp_rx_arg1<=255) lc640_write_int(EE_ETH_GW_1,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==141)		
				{
				if(modbus_tcp_rx_arg1<=255) lc640_write_int(EE_ETH_GW_2,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==142)		
				{
				if(modbus_tcp_rx_arg1<=255) lc640_write_int(EE_ETH_GW_3,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==143)		
				{
				if(modbus_tcp_rx_arg1<=255) lc640_write_int(EE_ETH_GW_4,modbus_tcp_rx_arg1);
	     		}	 
			if(modbus_tcp_rx_arg0==144)		
				{
				lc640_write_int(EE_ETH_SNMP_PORT_READ,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==145)		
				{
				lc640_write_int(EE_ETH_SNMP_PORT_WRITE,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==146)		
				{
				if(modbus_tcp_rx_arg1<=255) lc640_write_int(EE_COMMUNITY,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==147)		
				{
				if(modbus_tcp_rx_arg1<=255) lc640_write_int(EE_COMMUNITY+2,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==148)		
				{
				if(modbus_tcp_rx_arg1<=255) lc640_write_int(EE_COMMUNITY+4,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==149)		
				{
				if(modbus_tcp_rx_arg1<=255) lc640_write_int(EE_COMMUNITY+6,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==150)		
				{
				if(modbus_tcp_rx_arg1<=255) lc640_write_int(EE_COMMUNITY+8,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==151)		
				{
				if(modbus_tcp_rx_arg1<=255) lc640_write_int(EE_COMMUNITY+10,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==152)		
				{
				if(modbus_tcp_rx_arg1<=255) lc640_write_int(EE_COMMUNITY+12,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==153)		
				{
				if(modbus_tcp_rx_arg1<=255) lc640_write_int(EE_COMMUNITY+14,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==154)  //TRAP1 IP адрес
				{
				if(modbus_tcp_rx_arg1<=255) lc640_write_int(EE_ETH_TRAP1_IP_1,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==155)		
				{
				if(modbus_tcp_rx_arg1<=255) lc640_write_int(EE_ETH_TRAP1_IP_2,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==156)		
				{
				if(modbus_tcp_rx_arg1<=255) lc640_write_int(EE_ETH_TRAP1_IP_3,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==157)		
				{
				if(modbus_tcp_rx_arg1<=255) lc640_write_int(EE_ETH_TRAP1_IP_4,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==158)  //TRAP2 IP адрес
				{
				if(modbus_tcp_rx_arg1<=255) lc640_write_int(EE_ETH_TRAP2_IP_1,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==159)		
				{
				if(modbus_tcp_rx_arg1<=255) lc640_write_int(EE_ETH_TRAP2_IP_2,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==160)		
				{
				if(modbus_tcp_rx_arg1<=255) lc640_write_int(EE_ETH_TRAP2_IP_3,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==161)		
				{
				if(modbus_tcp_rx_arg1<=255) lc640_write_int(EE_ETH_TRAP2_IP_4,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==162)  //TRAP3 IP адрес
				{
				if(modbus_tcp_rx_arg1<=255) lc640_write_int(EE_ETH_TRAP3_IP_1,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==163)		
				{
				if(modbus_tcp_rx_arg1<=255) lc640_write_int(EE_ETH_TRAP3_IP_2,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==164)		
				{
				if(modbus_tcp_rx_arg1<=255) lc640_write_int(EE_ETH_TRAP3_IP_3,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==165)		
				{
				if(modbus_tcp_rx_arg1<=255) lc640_write_int(EE_ETH_TRAP3_IP_4,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==166)  //TRAP4 IP адрес
				{
				if(modbus_tcp_rx_arg1<=255) lc640_write_int(EE_ETH_TRAP4_IP_1,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==167)		
				{
				if(modbus_tcp_rx_arg1<=255) lc640_write_int(EE_ETH_TRAP4_IP_2,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==168)		
				{
				if(modbus_tcp_rx_arg1<=255) lc640_write_int(EE_ETH_TRAP4_IP_3,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==169)		
				{
				if(modbus_tcp_rx_arg1<=255) lc640_write_int(EE_ETH_TRAP4_IP_4,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==170)  //TRAP5 IP адрес
				{
				if(modbus_tcp_rx_arg1<=255) lc640_write_int(EE_ETH_TRAP5_IP_1,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==171)		
				{
				if(modbus_tcp_rx_arg1<=255) lc640_write_int(EE_ETH_TRAP5_IP_2,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==172)		
				{
				if(modbus_tcp_rx_arg1<=255) lc640_write_int(EE_ETH_TRAP5_IP_3,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==173)		
				{
				if(modbus_tcp_rx_arg1<=255) lc640_write_int(EE_ETH_TRAP5_IP_4,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==174)		
				{
				if(modbus_tcp_rx_arg1<=255) lc640_write_int(EE_WEB_PASSWORD,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==175)		
				{
				if(modbus_tcp_rx_arg1<=255) lc640_write_int(EE_WEB_PASSWORD+2,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==176)		
				{
				if(modbus_tcp_rx_arg1<=255) lc640_write_int(EE_WEB_PASSWORD+4,modbus_tcp_rx_arg1);
	     		}
			//if(modbus_tcp_rx_arg0==177 && modbus_tcp_rx_arg1>0) bRESET_INT_WDT=1;// перегрузка УКУ, инициализировать интернет    
			if(modbus_tcp_rx_arg0==178)		
				{
				modbus_tcp_rx_arg1/=10;
				//if(modbus_tcp_rx_arg1<=6000) lc640_write_int(EE_TVENTMAX,modbus_tcp_rx_arg1);
	     		} 
		   	if(modbus_tcp_rx_arg0==179)		
				{
				//if(modbus_tcp_rx_arg1<=2) lc640_write_int(EE_ICA_EN,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==180)		
				{
				//if(modbus_tcp_rx_arg1<=2) lc640_write_int(EE_ICA_CH,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==181)  //IP адрес второго ИПС
				{
				//if(modbus_tcp_rx_arg1<=255) lc640_write_int(EE_ICA_MODBUS_TCP_IP1,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==182)		
				{
				//if(modbus_tcp_rx_arg1<=255) lc640_write_int(EE_ICA_MODBUS_TCP_IP2,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==183)		
				{
				//if(modbus_tcp_rx_arg1<=255) lc640_write_int(EE_ICA_MODBUS_TCP_IP3,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==184)		
				{
				//if(modbus_tcp_rx_arg1<=255) lc640_write_int(EE_ICA_MODBUS_TCP_IP4,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==185)		
				{
				//if(modbus_tcp_rx_arg1>0 && modbus_tcp_rx_arg1<=254) lc640_write_int(EE_ICA_MODBUS_TCP_UNIT_ID,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==186)		
				{
				//if(modbus_tcp_rx_arg1>0 && modbus_tcp_rx_arg1<=254) lc640_write_int(EE_ICA_MODBUS_ADDRESS,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==187)		
				{
				//if(modbus_tcp_rx_arg1>=10 && modbus_tcp_rx_arg1<=100) lc640_write_int(EE_PWM_START,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==188)		
				{
				//if(modbus_tcp_rx_arg1>=1 && modbus_tcp_rx_arg1<=3) lc640_write_int(EE_KB_ALGORITM,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==189)		
				{
				//if(modbus_tcp_rx_arg1>=1 && modbus_tcp_rx_arg1<=5) lc640_write_int(EE_REG_SPEED,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==190)		
				{
				//if(modbus_tcp_rx_arg1==0 || modbus_tcp_rx_arg1==1) lc640_write_int(EE_SMART_SPC,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==191)		
				{
				//if(modbus_tcp_rx_arg1==1 || modbus_tcp_rx_arg1==3) lc640_write_int(EE_NUMPHASE,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==192)		
				{
				//if(modbus_tcp_rx_arg1>=10 && modbus_tcp_rx_arg1<=100) lc640_write_int(EE_TVENTON,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==193)		
				{
				//if(modbus_tcp_rx_arg1>=10 && modbus_tcp_rx_arg1<=100) lc640_write_int(EE_TVENTOFF,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==194)		
				{
				//if(modbus_tcp_rx_arg1<=2) lc640_write_int(EE_RELEVENTSIGN,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==195)		
				{
				//if(modbus_tcp_rx_arg1<=2) lc640_write_int(EE_NPN_OUT,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==196)		
				{
				//if(modbus_tcp_rx_arg1>=100 && modbus_tcp_rx_arg1<=2500) lc640_write_int(EE_UONPN,modbus_tcp_rx_arg1);
	     		}
			if(modbus_tcp_rx_arg0==197)		
				{
				//if(modbus_tcp_rx_arg1>=100 && modbus_tcp_rx_arg1<=2500) lc640_write_int(EE_UVNPN,modbus_tcp_rx_arg1);
	     		}
/*			if(modbus_tcp_rx_arg0==198)		
				{
				if(modbus_tcp_rx_arg1>=10 && modbus_tcp_rx_arg1<=60) lc640_write_int(EE_TZNPN,modbus_tcp_rx_arg1);
	     		}		

			if(modbus_tcp_rx_arg0==200)		
				{
				if(modbus_tcp_rx_arg1==1 ) command_rki=36;
				else if(modbus_tcp_rx_arg1==10 ) command_rki=37;
				else if(modbus_tcp_rx_arg1==-1 ) command_rki=38; //-1
				else if(modbus_tcp_rx_arg1==-10 ) command_rki=39;
	     		}
			if(modbus_tcp_rx_arg0==201)		
				{
				if(modbus_tcp_rx_arg1==1 ) command_rki=40;
				else if(modbus_tcp_rx_arg1==10 ) command_rki=41;
				else if(modbus_tcp_rx_arg1==-1 ) command_rki=42;
				else if(modbus_tcp_rx_arg1==-10 ) command_rki=43;
	     		}
			if(modbus_tcp_rx_arg0==202)		
				{
				if(modbus_tcp_rx_arg1==1 ) command_rki=14;
				else if(modbus_tcp_rx_arg1==-1 ) command_rki=13;
	     		}
			if(modbus_tcp_rx_arg0==203)		
				{
				if(modbus_tcp_rx_arg1==1 ) command_rki=20;
				else if(modbus_tcp_rx_arg1==5 ) command_rki=22;
				else if(modbus_tcp_rx_arg1==0xFFFF ) command_rki=19;
				else if(modbus_tcp_rx_arg1==0xFFFB ) command_rki=21;
	     		}
			if(modbus_tcp_rx_arg0==204)		
				{
				if(modbus_tcp_rx_arg1==1 ) command_rki=24;
				else if(modbus_tcp_rx_arg1==5 ) command_rki=26;
				else if(modbus_tcp_rx_arg1==-1 ) command_rki=23;
				else if(modbus_tcp_rx_arg1==-5 ) command_rki=25;
	     		}
			if(modbus_tcp_rx_arg0==205)		
				{
				if(modbus_tcp_rx_arg1==1 ) command_rki=28;
				else if(modbus_tcp_rx_arg1==5 ) command_rki=30;
				else if(modbus_tcp_rx_arg1==-1 ) command_rki=27;
				else if(modbus_tcp_rx_arg1==-5 ) command_rki=29;
	     		}
			if(modbus_tcp_rx_arg0==206)		
				{
				if(modbus_tcp_rx_arg1==1 ) command_rki=16;
				else if(modbus_tcp_rx_arg1==10 ) command_rki=18;
				else if(modbus_tcp_rx_arg1==-1 ) command_rki=15;
				else if(modbus_tcp_rx_arg1==-10 ) command_rki=17;
	     		}*/


		//o_10_e
	
				if(modbus_tcp_rx_arg0==19)		//вкл/выкл источника напр.
					{
		/*			if(modbus_tcp_rx_arg1==1)
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
					if(modbus_tcp_rx_arg1==0)
						{
						if(work_stat==wsPS)
							{
							work_stat=wsOFF;
							restart_off();
							}
						} */
					}
				if(modbus_tcp_rx_arg0==20)		//вкл/выкл источника тока
					{
	/*				if(modbus_tcp_rx_arg1==1)
						{
						if(work_stat!=wsGS)
							{
							work_stat=wsGS;
							time_proc=0;
							time_proc_remain=T_PROC_GS;
							lc640_write_int(EE_MAIN_MENU_MODE,mmmIT);
							}
						}
					if(modbus_tcp_rx_arg1==0)
						{
						if(work_stat==wsGS)
							{
							work_stat=wsOFF;
							restart_off();
							}
						}*/
					}
				modbus_hold_registers_transmit(MODBUS_ADRESS,modbus_tcp_func,modbus_tcp_rx_arg0,1,MODBUS_TCP_PROT);
	
				sendbuf = tcp_get_buf(11);
				sendbuf[0]=ptr[0];
				sendbuf[1]=ptr[1];
				sendbuf[2]=ptr[2];
				sendbuf[3]=ptr[3];
				sendbuf[4]=0;
				sendbuf[5]=6;
				sendbuf[6]=modbus_tcp_unit;
				sendbuf[7]=modbus_tcp_func;
				sendbuf[8]=modbus_tcp_rx_arg0/256;
				sendbuf[9]=modbus_tcp_rx_arg0%256;
				mem_copy((char*)&sendbuf[10],modbus_tcp_out_ptr,2);
				//sendbuf[9]=3;
				//sendbuf[10]=4;
	          	tcp_send (socket_tcp, sendbuf, 12);
				//
				
				//modbus_tcp_tx_buff[4]=0;
				//modbus_tcp_tx_buff[5]=5;
				//modbus_tcp_tx_buff[6]=1;//modbus_tcp_unit;
				//modbus_tcp_tx_buff[7]=3;//modbus_tcp_func;
				//modbus_tcp_tx_buff[8]=2;
				//mem_copy((char*)&modbus_tcp_tx_buff[9],modbus_tcp_out_ptr,2);
				//modbus_tcp_tx_buff[9]=2;
				//modbus_tcp_tx_buff[10]=3;
				//tcp_send (socket_tcp, modbus_tcp_tx_buff, 11);






				}


			
			} 
      	break;

    	case TCP_EVT_CONREQ:
      		/* Remote peer requested connect, accept it */
			//ica_plazma[5]++;
      		return (1);

    	case TCP_EVT_CONNECT:
      		/* The TCP socket is connected */
			tcp_connect_stat=1;
			//ica_plazma[6]++;
      		return (1);

    	case TCP_EVT_CLOSE: 
      		/* The TCP socket is connected */
			tcp_connect_stat=0;
			//ica_plazma[7]++;
      		return (1);
  		}
  	return (0);
}