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
/*  @ASCII : (GBK/GB2312) in Windows x64. CCSv5.5����                                                                                   */
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
    // ��ʼ��ϵͳ
    //system_clk_init();

    uart_gpio_init();
    uart_0_init();
    // ��ʼ��spi��gpio��
    spi_gpio_init();
    // ��ʼ������
    spi_init();

    // װ���������
    Load_Net_Parameters();
    // Ӳ����λW5500
    W5500_Hardware_Reset();
    // W5500��ʼ������
    W5500_Initialization();

    _EINT();
    while (1) {
        // W5500�˿ڳ�ʼ������
        W5500_Socket_Set();
        // W5500�жϴ��������
        W5500_Interrupt_Process();
        // ���Socket0���յ�����
        if((S0_Data & S_RECEIVE) == S_RECEIVE) {
            S0_Data&=~S_RECEIVE;
            // W5500���ղ����ͽ��յ�������
            Process_Socket_Data(0);
            uart_0_send_str(Tx_Buffer);
        }
        // ��ʱ�����ַ���
        else if(W5500_Send_Delay_Counter >= 3000) {
            if(S0_State == (S_INIT|S_CONN)) {
                S0_Data&=~S_TRANSMITOK;
                memcpy(Tx_Buffer, "\r\nMSP430 NETWORK SOCKET\r\n", 27);
                // ָ��Socket(0~7)�������ݴ���,�˿�0����23�ֽ�����
                Write_SOCK_Data_Buffer(0, Tx_Buffer, 23);
                uart_0_send_str(Tx_Buffer);
            }
            W5500_Send_Delay_Counter=0;
        }
        W5500_Send_Delay_Counter++;

    }
}
/*******************************************************************************
 * ������  : Delay
 * ����    : ��ʱ����(ms)
 * ����    : d:��ʱϵ������λΪ����
 * ���    : ��
 * ����    : ��
 * ˵��    : ��ʱ������Timer2��ʱ��������1����ļ�����ʵ�ֵ�
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
 * ������  : W5500_Initialization
 * ����    : W5500��ʼ������
 * ����    : ��
 * ���    : ��
 * ����ֵ  : ��
 * ˵��    : ��
 *******************************************************************************/

void W5500_Initialization(void)
{
    //��ʼ��W5500�Ĵ�������
    W5500_Init();
    //������ط�����
    Detect_Gateway();
    //ָ��Socket(0~7)��ʼ��,��ʼ���˿�0
    Socket_Init(0);
}

/*******************************************************************************
 * ������  : Load_Net_Parameters
 * ����    : װ���������
 * ����    : ��
 * ���    : ��
 * ����ֵ  : ��
 * ˵��    : ���ء����롢�����ַ������IP��ַ���˿ںš�Ŀ��IP��ַ��Ŀ�Ķ˿ںš��˿ڹ���ģʽ
 *******************************************************************************/
void Load_Net_Parameters(void)
{
    Gateway_IP[0] = 192;//�������ز���
    Gateway_IP[1] = 168;
    Gateway_IP[2] = 1;
    Gateway_IP[3] = 1;

    Sub_Mask[0]=255;//������������
    Sub_Mask[1]=255;
    Sub_Mask[2]=255;
    Sub_Mask[3]=0;

    Phy_Addr[0]=0x0c;//���������ַ
    Phy_Addr[1]=0x29;
    Phy_Addr[2]=0xab;
    Phy_Addr[3]=0x7c;
    Phy_Addr[4]=0x00;
    Phy_Addr[5]=0x01;

    IP_Addr[0]=192;//���ر���IP��ַ
    IP_Addr[1]=168;
    IP_Addr[2]=1;
    IP_Addr[3]=199;

    S0_Port[0] = 0x13;//���ض˿�0�Ķ˿ں�5000
    S0_Port[1] = 0x88;

    S0_DIP[0]=192;//���ض˿�0��Ŀ��IP��ַ
    S0_DIP[1]=168;
    S0_DIP[2]=1;
    S0_DIP[3]=100;

    S0_DPort[0] = 0x17;//���ض˿�0��Ŀ�Ķ˿ں�6000
    S0_DPort[1] = 0x70;

    S0_Mode=TCP_CLIENT;//���ض˿�0�Ĺ���ģʽ,TCP�ͻ���ģʽ
}

/*******************************************************************************
 * ������  : W5500_Socket_Set
 * ����    : W5500�˿ڳ�ʼ������
 * ����    : ��
 * ���    : ��
 * ����ֵ  : ��
 * ˵��    : �ֱ�����4���˿�,���ݶ˿ڹ���ģʽ,���˿�����TCP��������TCP�ͻ��˻�UDPģʽ.
 *           �Ӷ˿�״̬�ֽ�Socket_State�����ж϶˿ڵĹ������
 *******************************************************************************/
void W5500_Socket_Set(void)
{
    if(S0_State==0)//�˿�0��ʼ������
    {
        if(S0_Mode==TCP_SERVER)//TCP������ģʽ
        {
            if(Socket_Listen(0)==TRUE)
                S0_State=S_INIT;
            else
                S0_State=0;
        }
        else if(S0_Mode==TCP_CLIENT)//TCP�ͻ���ģʽ
        {
            if(Socket_Connect(0)==TRUE)
                S0_State=S_INIT;
            else
                S0_State=0;
        }
        else//UDPģʽ
        {
            if(Socket_UDP(0)==TRUE)
                S0_State=S_INIT|S_CONN;
            else
                S0_State=0;
        }
    }
}

/*******************************************************************************
 * ������  : Process_Socket_Data
 * ����    : W5500���ղ����ͽ��յ�������
 * ����    : s:�˿ں�
 * ���    : ��
 * ����ֵ  : ��
 * ˵��    : �������ȵ���S_rx_process()��W5500�Ķ˿ڽ������ݻ�������ȡ����,
 *           Ȼ�󽫶�ȡ�����ݴ�Rx_Buffer������Temp_Buffer���������д���
 *           ������ϣ������ݴ�Temp_Buffer������Tx_Buffer������������S_tx_process()
 *           �������ݡ�
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
    // ���ݻػ�
    rx_buffer[rx_count] = RXBUF0 & 0xFF;
    TXBUF0 = RXBUF0;

    rx_count ++;
    if (rx_count == 10) {
        Write_SOCK_Data_Buffer(0, rx_buffer, 10);
        rx_count = 0;
    }

    LPM1_EXIT;
}
