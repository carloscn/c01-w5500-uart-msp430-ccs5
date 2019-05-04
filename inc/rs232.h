/*
 * rs232.h
 *
 *  Created on: 2019-3-4
 *      Author: lifim
 */
#ifndef RS232_H_
#define RS232_H_


extern void    uart_gpio_init(void);
extern void    uart_0_init( void );
extern void    uart_0_send_str( unsigned char *ptr );
extern void    uart_0_put_char( unsigned char ch );
extern uint8_t rx_buffer[80];
extern uint32_t rx_count;
#endif
