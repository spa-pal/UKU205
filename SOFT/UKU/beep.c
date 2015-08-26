#include "beep.h"
#include "main.h"
#include "LPC17XX.H"

unsigned long beep_stat_temp,beep_stat;
char beep_stat_cnt;
char beep_cnt;

//-----------------------------------------------
void beep_drv(void)
{
LPC_GPIO2->FIODIR|=(1UL<<6);
if((beep_stat_temp&0x00000001UL)&&(ZV_ON))
    	{
    	LPC_GPIO2->FIOSET|=(1UL<<6);
    	beep_cnt=6;
    	}
else LPC_GPIO2->FIOCLR|=(1UL<<6);

beep_stat_temp>>=1;
if(--beep_stat_cnt==0)
	{
	beep_stat_cnt=32;
	beep_stat_temp=beep_stat;
	}

//IO1SET|=(1UL<<27);
//FIO1SET|=(1UL<<27);
}

//-----------------------------------------------
void beep_init(long zvuk,char fl) 
{
if(fl=='O')
	{
	beep_stat_temp=zvuk;
	beep_stat=0x0L;
	beep_stat_cnt=32;
	} 
else if(fl=='A')
	{
	beep_stat_temp=zvuk;
	beep_stat=zvuk;
	beep_stat_cnt=32;
	}	 

else if(fl=='R')
	{
	beep_stat=zvuk;
	}	
		          
else if(fl=='S')
	{
	beep_stat_temp=0x0L;
	beep_stat=0x0L;
	beep_stat_cnt=32;
	}	
}
