#ifndef _LCD_AGM1232_UKU203_H_

#define _LCD_AGM1232_UKU203_H_

//P0
#define A0	29
#define E1	30
//P1
#define E2	26
#define RW	27

#define D0	18
#define D1	D0+1
#define D2	D1+1
#define D3	D2+1
#define D4	D3+1
#define D5	D4+1
#define D6	D5+1
#define D7	D6+1

//#define RES	26


#define _RESET_				226
#define _DISPLAY_ON_             	175
#define _DISPLAY_OFF_            	174
#define _SET_PAGE_               	184
#define _SET_COLUMN_             	0
#define _SET_RAW_             	0xc0
#define _SET_DISPLAY_START_LINE_ 	192
#define Max_Col		60
#define Max_page 		3   


void lcd1_chk(void);
void lcd1_wr(char in);
void lcd2_chk(void);
void lcd2_wr(char in);
char data1_wr(char in);
void data2_wr(char in);
void lcd_set_page(char in);
void lcd_set_col(char in);
void lcd_set_raw(char in);
void lcd_init(void);
void status(void);
void delay(void);
void ltstrobe(char in);
void lcd_init_(void);
void lcd_clear(void);
void lcd_on(void);
void lcd_off(void);
void lcd_out(char* adr);

#endif
