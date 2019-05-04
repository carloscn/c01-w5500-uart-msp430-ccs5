/**
 * \brief   This project about ds18b20 on common channels DS18B20 .
 *
 * \License  THIS FILE IS PART OF MULTIBEANS PROJECT ;
 *           all of the files  - The core part of the project;
 *           THIS PROGRAM IS FREE SOFTWARE, JUST NEED GPL 3.0 LICENSE;
 *           YOU SHOULD HAVE RECEIVED A COPY OF WTFPL LICENSE, IF NOT,
 *           MULTIBEANS WILL TAKE APPROPRIATE MEASURES.
 *
 *                ________________     ___           _________________
 *               |    __    __    |   |   |         |______     ______|
 *               |   |  |  |  |   |   |   |                |   |
 *               |   |  |__|  |   |   |   |________        |   |
 *               |___|        |___|   |____________|       |___|
 *
 *                               MULTIBEANS ORG.
 *                     Homepage: http://www.mltbns.com/
 *
 *           * You can download the license on our Github. ->
 *           * -> https://github.com/multibeans  <-
 *           * Copyright (c) 2014-2019 MULTIBEANS ORG. http://www.mltbns.com/
 *           * Copyright (c) 2019 Wei Haochen(Carlos Wei: # weihaochen@mltbns.com).
 *
 *  \note    void.
 ****************************************************************************/
/*                                                                          */
/*  @File       : main.c                                                    */
/*  @Revision   : Ver 1.0.                                                  */
/*  @Date       : 2019.03.01 Realse.                                        */
/*  @Belong     : PROJECT.                                                  */
/*  @GitHub     :                                                           */
/*  @ASCII : (GBK/GB2312) in Windows x64. CCSv5.5以上                                                                                   */
/****************************************************************************/
/*  @Attention:                                                             */
/*  ---------------------------------------------------------------------   */
/*  |    Data    |  Behavior |     Offer     |          Content         |   */
/*  |------------|-----------|---------------|--------------------------|   */
/*  | 2019.03.01 |   create  |Carlos Wei(M)  | add all device driver.   |   */
/*  ---------------------------------------------------------------------   */
/*                                                            MULTIBEANS.   */
/****************************************************************************/
#include <msp430.h>
#include "project.h"
#include "main.h"


int main(void) {

    unsigned int W5500_Send_Delay_Counter =0;

    // Stop watchdog timer
    WDTCTL = WDTPW | WDTHOLD;
    // 初始化系统
    //system_clk_init();

    uart_gpio_init();
    uart_0_init();
    // 初始化spi的gpio口
    spi_gpio_init();
    // 初始化串口
    spi_init();

    // 装载网络参数
    Load_Net_Parameters();
    // 硬件复位W5500
    W5500_Hardware_Reset();
    // W5500初始货配置
    W5500_Initialization();

    _EINT();
    while (1) {
        // W5500端口初始化配置
        W5500_Socket_Set();
        // W5500中断处理程序框架
        W5500_Interrupt_Process();
        // 如果Socket0接收到数据
        if((S0_Data & S_RECEIVE) == S_RECEIVE) {
            S0_Data&=~S_RECEIVE;
            // W5500接收并发送接收到的数据
            Process_Socket_Data(0);
            uart_0_send_str(Tx_Buffer);
        }
        // 定时发送字符串
        else if(W5500_Send_Delay_Counter >= 3000) {
            if(S0_State == (S_INIT|S_CONN)) {
                S0_Data&=~S_TRANSMITOK;
                memcpy(Tx_Buffer, "\r\nMSP430 NETWORK SOCKET\r\n", 27);
                // 指定Socket(0~7)发送数据处理,端口0发送23字节数据
                Write_SOCK_Data_Buffer(0, Tx_Buffer, 23);
                uart_0_send_str(Tx_Buffer);
            }
            W5500_Send_Delay_Counter=0;
        }
        W5500_Send_Delay_Counter++;

    }
}
/*******************************************************************************
 * 函数名  : Delay
 * 描述    : 延时函数(ms)
 * 输入    : d:延时系数，单位为毫秒
 * 输出    : 无
 * 返回    : 无
 * 说明    : 延时是利用Timer2定时器产生的1毫秒的计数来实现的
 *******************************************************************************/
void Delay(unsigned int  x)
{
    unsigned int i,j;

    for(j=0;j<5;j++)
        for(i=0;i<x;i++);
}

void system_clk_init(void)
{
    uint16_t i;
    // XT2on
    BCSCTL1 &= ~XT2OFF;
    do {
        // Clear OSCFault flag
        IFG1 &= ~OFIFG;
        // Time for flag to set
        for (i = 0xFF; i > 0; i--);
    }
    // OSCFault flag still set
    while ((IFG1 & OFIFG));
    // MCLK = SMCLK = XT2 (safe)
    BCSCTL2 |= SELM_2 + SELS;
}
/*******************************************************************************
 * 函数名  : W5500_Initialization
 * 描述    : W5500初始货配置
 * 输入    : 无
 * 输出    : 无
 * 返回值  : 无
 * 说明    : 无
 *******************************************************************************/

void W5500_Initialization(void)
{
    //初始化W5500寄存器函数
    W5500_Init();
    //检查网关服务器
    Detect_Gateway();
    //指定Socket(0~7)初始化,初始化端口0
    Socket_Init(0);
}

/*******************************************************************************
 * 函数名  : Load_Net_Parameters
 * 描述    : 装载网络参数
 * 输入    : 无
 * 输出    : 无
 * 返回值  : 无
 * 说明    : 网关、掩码、物理地址、本机IP地址、端口号、目的IP地址、目的端口号、端口工作模式
 *******************************************************************************/
void Load_Net_Parameters(void)
{
    Gateway_IP[0] = 192;//加载网关参数
    Gateway_IP[1] = 168;
    Gateway_IP[2] = 1;
    Gateway_IP[3] = 1;

    Sub_Mask[0]=255;//加载子网掩码
    Sub_Mask[1]=255;
    Sub_Mask[2]=255;
    Sub_Mask[3]=0;

    Phy_Addr[0]=0x0c;//加载物理地址
    Phy_Addr[1]=0x29;
    Phy_Addr[2]=0xab;
    Phy_Addr[3]=0x7c;
    Phy_Addr[4]=0x00;
    Phy_Addr[5]=0x01;

    IP_Addr[0]=192;//加载本机IP地址
    IP_Addr[1]=168;
    IP_Addr[2]=1;
    IP_Addr[3]=199;

    S0_Port[0] = 0x13;//加载端口0的端口号5000
    S0_Port[1] = 0x88;

    S0_DIP[0]=192;//加载端口0的目的IP地址
    S0_DIP[1]=168;
    S0_DIP[2]=1;
    S0_DIP[3]=100;

    S0_DPort[0] = 0x17;//加载端口0的目的端口号6000
    S0_DPort[1] = 0x70;

    S0_Mode=TCP_CLIENT;//加载端口0的工作模式,TCP客户端模式
}

/*******************************************************************************
 * 函数名  : W5500_Socket_Set
 * 描述    : W5500端口初始化配置
 * 输入    : 无
 * 输出    : 无
 * 返回值  : 无
 * 说明    : 分别设置4个端口,根据端口工作模式,将端口置于TCP服务器、TCP客户端或UDP模式.
 *           从端口状态字节Socket_State可以判断端口的工作情况
 *******************************************************************************/
void W5500_Socket_Set(void)
{
    if(S0_State==0)//端口0初始化配置
    {
        if(S0_Mode==TCP_SERVER)//TCP服务器模式
        {
            if(Socket_Listen(0)==TRUE)
                S0_State=S_INIT;
            else
                S0_State=0;
        }
        else if(S0_Mode==TCP_CLIENT)//TCP客户端模式
        {
            if(Socket_Connect(0)==TRUE)
                S0_State=S_INIT;
            else
                S0_State=0;
        }
        else//UDP模式
        {
            if(Socket_UDP(0)==TRUE)
                S0_State=S_INIT|S_CONN;
            else
                S0_State=0;
        }
    }
}

/*******************************************************************************
 * 函数名  : Process_Socket_Data
 * 描述    : W5500接收并发送接收到的数据
 * 输入    : s:端口号
 * 输出    : 无
 * 返回值  : 无
 * 说明    : 本过程先调用S_rx_process()从W5500的端口接收数据缓冲区读取数据,
 *           然后将读取的数据从Rx_Buffer拷贝到Temp_Buffer缓冲区进行处理。
 *           处理完毕，将数据从Temp_Buffer拷贝到Tx_Buffer缓冲区。调用S_tx_process()
 *           发送数据。
 *******************************************************************************/
void Process_Socket_Data(SOCKET s)
{
    unsigned short size;
    size=Read_SOCK_Data_Buffer(s, Rx_Buffer);
    memcpy(Tx_Buffer, Rx_Buffer, size);
    Write_SOCK_Data_Buffer(s, Tx_Buffer, size);
#if 0
    if (rx_count != 0) {
        Write_SOCK_Data_Buffer(s, rx_buffer, size);
        rx_count =0;
    }
#endif
}

#pragma vector = USART0RX_VECTOR
__interrupt void UART0_RXISR(void)
{
    // 数据回环
    rx_buffer[rx_count] = RXBUF0 & 0xFF;
    TXBUF0 = RXBUF0;

    rx_count ++;
    if (rx_count == 10) {
        Write_SOCK_Data_Buffer(0, rx_buffer, 10);
        rx_count = 0;
    }

    LPM1_EXIT;
}
