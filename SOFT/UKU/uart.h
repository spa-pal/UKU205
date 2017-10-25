/*****************************************************************************
 *   uart.h:  Header file for NXP LPC17xx Family Microprocessors
 *
 *   Copyright(C) 2009, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2009.05.27  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#ifndef __UART_H 
#define __UART_H

#define IER_RBR		0x01
#define IER_THRE	0x02
#define IER_RLS		0x04

#define IIR_PEND	0x01
#define IIR_RLS		0x03
#define IIR_RDA		0x02
#define IIR_CTI		0x06
#define IIR_THRE	0x01

#define LSR_RDR		0x01
#define LSR_OE		0x02
#define LSR_PE		0x04
#define LSR_FE		0x08
#define LSR_BI		0x10
#define LSR_THRE	0x20
#define LSR_TEMT	0x40
#define LSR_RXFE	0x80

#define BUFSIZE		0x40

#define PT	(UIB0[1]&0x80)
#define C_D	(UIB0[1]&0x40)
#define FR	(UIB0[1]&0x20)

#define RX_BUFFER_SIZE0 64
#define TX_BUFFER_SIZE0 64

#define END 	0x0A

extern char bRXIN0;
extern char UIB0[30];
extern char flag0;
extern char rx_buffer0[RX_BUFFER_SIZE0];
extern char tx_buffer0[TX_BUFFER_SIZE0];
extern unsigned char rx_wr_index0,rx_rd_index0,rx_counter0;
extern unsigned char tx_wr_index0,tx_rd_index0,tx_counter0;
extern char rx_buffer_overflow0;
extern char plazma_uart0;
extern char memo_out[50];
extern char data_rs[50];
extern char data_rs0[50];
extern const char Table87[];
extern const char Table95[]; 

uint32_t UARTInit( uint32_t portNum, uint32_t Baudrate );
void UART0_IRQHandler( void );
void UART1_IRQHandler( void );
void UARTSend( uint32_t portNum, uint8_t *BufferPtr, uint32_t Length );
void uart_in_an0(void);
char crc_87(char* ptr,char num);
char crc_95(char* ptr,char num);
void uart_out_adr0 (char *ptr, char len);


#endif /* end __UART_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
