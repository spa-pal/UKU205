
extern char plazma_modbus_tcp[20];
extern char modbus_tcp_plazma[20];

U16 tcp_callback (U8 soc, U8 evt, U8 *ptr, U16 par);

extern char modbus_tcp_func;
extern char modbus_tcp_unit;
extern short modbus_tcp_rx_arg0;
extern short modbus_tcp_rx_arg1;

#define MODBUS_TCP_PROT	1

extern char* modbus_tcp_out_ptr;

extern U8 socket_tcp;
extern U8 tcp_connect_stat;

U16 tcp_callback (U8 soc, U8 evt, U8 *ptr, U16 par);

