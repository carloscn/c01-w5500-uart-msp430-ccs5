/*
 * spi.c
 *
 *  Created on: 2019-3-2
 *      Author: lifim
 */

#include <msp430.h>
#include "spi.h"


void spi_gpio_init(void)
{
    // Set cs & reset pin is p3.0
    P3SEL &= ~(BIT0 + BIT1 + BIT2 + BIT3);

    P2SEL &= ~(BIT7 + BIT8);
    // Setup P3.1/2/3 for SPI mode
    P3DIR |= BIT0 + BIT1 + BIT3;
    P2DIR |= BIT7 + BIT8;
    P3DIR &= ~(BIT2);
    P3OUT &= ~(BIT0 + BIT1 + BIT3);
    P2OUT &= ~BIT7;


}

void spi_init(void)
{
#if 0
    U0CTL |= SWRST;
    U0CTL |= CHAR + SYNC + MM;               // 8-bit, SPI, Master
    U0TCTL = 0x00;
    U0TCTL = CKPH + SSEL0 + STC;           //时钟源为ACLK,3线模式，上升沿数据有效
    U0BR0 = 0x07;                          // 波特率为4681
    U0BR1 = 0x00;
    U0MCTL = 0x00;
    ME1 = USPIE0;                        //SPI0模块允许
    U0CTL &= ~SWRST;
    //IE1 |= URXIE0;                      //接收中断允许

#endif
}

inline void spi_cs_low(void)
{
    P3OUT |= BIT0;
    _NOP();
    _NOP();
}

inline void spi_cs_high(void)
{
    P3OUT &= ~BIT0;
    _NOP();
    _NOP();
}

inline void spi_write_byte(unsigned char ch)
{
    while ((IFG1 & UTXIFG0) == 0);
    TXBUF0 = ch;                             //发送命令

}

inline uint8_t spi_read_byte(void)
{
    while ((IFG1 & URXIFG0) == 0);
    return RXBUF0;
}


void WIZ_CS(uint8_t val)
{
    val?spi_cs_high():spi_cs_low();
}

uint8_t SPI2_SendByte(uint8_t byte)
{
    spi_write_byte(byte);
    return spi_read_byte();

}
