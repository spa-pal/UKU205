
#define LC640_CS 13
#define CS_ON       LPC_GPIO2->FIODIR|=(1UL<<LC640_CS);LPC_GPIO2->FIOCLR|=(1UL<<LC640_CS);
#define CS_OFF      LPC_GPIO2->FIODIR|=(1UL<<LC640_CS);LPC_GPIO2->FIOSET|=(1UL<<LC640_CS);



char spi1(char in);
void spi1_config(void);
void spi1_unconfig(void);
void lc640_wren(void);
char lc640_rdsr(void);
int lc640_read(int ADR);
int lc640_read_int(int ADR);
long lc640_read_long(int ADR);
void lc640_read_long_ptr(int ADR,char* out_ptr);
void lc640_read_str(int ADR, char* ram_ptr, char num);
char lc640_write(int ADR,char in);
char lc640_write_int(short ADR,short in);
char lc640_write_long(int ADR,long in);
char lc640_write_long_ptr(int ADR,char* in);

