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
    // 1λֹͣλ
    // ����żУ��

    ME1 |= UTXE0 + URXE0;                     // ʹ��USART0�ķ��ͺͽ���
    UCTL0 |= CHAR;                            // ѡ��8λ�ַ�
    UTCTL0 |= SSEL0;                          // UCLK = ACLK
    UBR00 = 0x03;                             // ������9600
    UBR10 = 0x00;                             //
    UMCTL0 = 0x4A;                            // Modulation
    UCTL0 &= ~SWRST;                          // ��ʼ��UART״̬��
    IE1 |= URXIE0;                            // ʹ��USART0�Ľ����ж�
}

void uart_0_send_str( unsigned char *ptr )
{
    while( (*ptr != '\0') ) {

        while ( !( IFG1 & UTXIFG0 ) );              // TX������У�
        TXBUF0 = *ptr++;                                // ��������
    }
}

void    uart_0_put_char( unsigned char ch )
{
    while ( !( IFG1 & UTXIFG0 ) );              // TX������У�
    TXBUF0 = ch;                                // ��������
}


