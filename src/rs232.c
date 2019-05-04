/*
 * rs232.c
 *
 *  Created on: 2019-3-4
 *      Author: lifim
 */
#include <msp430.h>
#include "project.h"
#include "rs232.h"

uint32_t rx_count = 0;
uint8_t rx_buffer[80];

void    uart_gpio_init(void)
{

    P3DIR |= BIT4 + BIT5 + BIT6 + BIT7;
    P3OUT |= BIT4 + BIT5 + BIT6 + BIT7;
    P3SEL |= 0xF0;
    // 1111 0000
    //

}

void    uart_0_init( void )
{
    // baud rate: 9600
    // 8bit
    // 1位停止位
    // 无奇偶校验

    ME1 |= UTXE0 + URXE0;                     // 使能USART0的发送和接受
    UCTL0 |= CHAR;                            // 选择8位字符
    UTCTL0 |= SSEL0;                          // UCLK = ACLK
    UBR00 = 0x03;                             // 波特率9600
    UBR10 = 0x00;                             //
    UMCTL0 = 0x4A;                            // Modulation
    UCTL0 &= ~SWRST;                          // 初始化UART状态机
    IE1 |= URXIE0;                            // 使能USART0的接收中断
}

void uart_0_send_str( unsigned char *ptr )
{
    while( (*ptr != '\0') ) {

        while ( !( IFG1 & UTXIFG0 ) );              // TX缓存空闲？
        TXBUF0 = *ptr++;                                // 发送数据
    }
}

void    uart_0_put_char( unsigned char ch )
{
    while ( !( IFG1 & UTXIFG0 ) );              // TX缓存空闲？
    TXBUF0 = ch;                                // 发送数据
}


