/**************************************************************************
DO NOT CHANGE ANYTHING BELOW
***************************************************************************/ 

// Define CAN SFR address bases 
//#define CAN_REG_BASE                    (0xE0000000)
//#define ACCEPTANCE_FILTER_RAM_BASE      (CAN_REG_BASE + 0x00038000)
//#define ACCEPTANCE_FILTER_REGISTER_BASE (CAN_REG_BASE + 0x0003C000)
//#define CENTRAL_CAN_REGISTER_BASE       (CAN_REG_BASE + 0x00040000)              

// Common CAN bit rates
#define   CANBitrate125k_12MHz          0x001C001D
#define   CANBitrate125k_60MHz          0x001C001D
#define   CANBitrate250k_12MHz          0x001C000E
#define   BITRATE125K25MHZ              0x00140018
/* Acceptance filter mode in AFMR register */
#define ACCF_OFF				0x01
#define ACCF_BYPASS				0x02
#define ACCF_ON					0x00
#define ACCF_FULLCAN			0x04

    /* exact-width unsigned integer types */
typedef unsigned          char uint8_t;
typedef unsigned short     int uint16_t;
typedef unsigned           int uint32_t;
typedef unsigned       __int64 uint64_t;

/**************************************************************************
USER DEFINABLE PARAMETERS
***************************************************************************/ 

// Maximum number of CAN interfaces supported by this driver (1 to 4)
// So far values 3 and 4 were not tested
// The example code in "main" uses CAN ports 1 and 2 (MAX_CANPORTS 2)
#define MAX_CANPORTS 2

// Maximum number of total FullCAN Filters for ALL CAN interfaces
#define MAX_FILTERS 2

// Counts number of filters (CAN message objects) used so far
extern short volatile gCANFilter;
extern char ptr_can1_tx_wr,ptr_can1_tx_rd;
extern long can1_info[8];
extern long can1_id[8];
extern long can1_data[8];
extern long can1_datb[8];

//extern unsigned short rotor_can[6];

// Type definition to hold a FullCAN message
// Compatible to FullCAN Mode Stored Messages in LPC User Manual
typedef struct
{
  unsigned int Dat1; // Bits  0..10: CAN Message ID
                     // Bits 13..15: CAN interface number (1..4)
                     // Bits 16..19: DLC - Data Length Counter
                     // Bits 24..25: Semaphore bits
  unsigned int DatA; // CAN Message Data Bytes 0-3
  unsigned int DatB; // CAN Message Data Bytes 4-7
} FULLCAN_MSG;


// FullCAN Message List
extern FULLCAN_MSG volatile gFullCANList[MAX_FILTERS];

extern char bR;
extern char RXBUFF[40],TXBUFF[40];
extern char bIN;
extern char bd_dumm[25];
extern char bd[25];
extern char TX_len;
extern char bOUT;
extern char can_tx_cnt;
extern char bOUT_FREE;
extern char rotor_rotor_rotor[2];

char CRC1_in(void);
char CRC2_in(void);
char CRC1_out(void);
char CRC2_out(void);
void can1_out_adr(char* ptr,char num);
void can_in_an(void);
void can_isr_rx1 (void); 
void can_isr_tx1 (void);
short can1_init (unsigned int can_btr);
#define ACCF_IDEN_NUM			1
#define EXP_STD_ID				0x0e9
short FullCAN_SetFilter  (
                         unsigned short can_port, // CAN interface number
                         unsigned int CANID // 11-bit CAN ID
                         );

// Type definition to hold a CAN message
typedef struct
{
	uint32_t Frame; // Bits 16..19: DLC - Data Length Counter
					// Bit 30: Set if this is a RTR message
					// Bit 31: Set if this is a 29-bit ID message
	uint32_t MsgID;	// CAN Message ID (11-bit or 29-bit)
	uint32_t DataA;	// CAN Message Data Bytes 0-3
	uint32_t DataB;	// CAN Message Data Bytes 4-7
} CAN_MSG;





