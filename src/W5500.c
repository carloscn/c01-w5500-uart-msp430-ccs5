/**********************************************************************************
 * �ļ���  ��W5500.c
 * ����    ��W5500 ����������         
 **********************************************************************************/

#include "W5500.h"	

/***************----- ��������������� -----***************/
unsigned char Gateway_IP[4];//����IP��ַ 
unsigned char Sub_Mask[4];	//�������� 
unsigned char Phy_Addr[6];	//������ַ(MAC) 
unsigned char IP_Addr[4];	//����IP��ַ 

unsigned char S0_Port[2];	//�˿�0�Ķ˿ں�
unsigned char S0_DIP[4];	//�˿�0Ŀ��IP��ַ 
unsigned char S0_DPort[2];	//�˿�0Ŀ�Ķ˿ں�

unsigned char UDP_DIPR[4];	//UDP(�㲥)ģʽ,Ŀ������IP��ַ
unsigned char UDP_DPORT[2];	//UDP(�㲥)ģʽ,Ŀ�������˿ں�

/***************----- �˿ڵ�����ģʽ -----***************/
unsigned char S0_Mode =3;	//�˿�0������ģʽ,0:TCP������ģʽ,1:TCP�ͻ���ģʽ,2:UDP(�㲥)ģʽ

#define TCP_SERVER	0x00	//TCP������ģʽ
#define TCP_CLIENT	0x01	//TCP�ͻ���ģʽ 
#define UDP_MODE	0x02	//UDP(�㲥)ģʽ 

/***************----- �˿ڵ�����״̬ -----***************/
unsigned char S0_State =0;	//�˿�0״̬��¼,1:�˿���ɳ�ʼ��,2�˿��������(����������������) 
#define S_INIT		0x01	//�˿���ɳ�ʼ�� 
#define S_CONN		0x02	//�˿��������,���������������� 

/***************----- �˿��շ����ݵ�״̬ -----***************/
unsigned char S0_Data;		//�˿�0���պͷ������ݵ�״̬,1:�˿ڽ��յ�����,2:�˿ڷ���������� 
#define S_RECEIVE	 0x01	//�˿ڽ��յ�һ�����ݰ� 
#define S_TRANSMITOK 0x02	//�˿ڷ���һ�����ݰ���� 

/***************----- �˿����ݻ����� -----***************/
unsigned char Rx_Buffer[80];	//�˿ڽ������ݻ�����
unsigned char Tx_Buffer[80];	//�˿ڷ������ݻ�����

unsigned char W5500_Interrupt;	//W5500�жϱ�־(0:���ж�,1:���ж�)

// P3.0 CS
// P3.1 SIMO0
// P3.2 SOMI0
// P3.3 CLK
// P2.7 RESET
/*
#define  W5500_RST  0  //����W5500��RST����
#define  W5500_INT  1  //����W5500��INT����
#define  W5500_SCS  2  //����W5500��CS����
#define  W5500_SCLK  3 //����W5500��SCLK����
#define  W5500_MISO  4 //����W5500��MISO����
#define  W5500_MOSI  5 //����W5500��MOSI����
 */
void io_set(int num)
{
    switch(num) {

    case W5500_RST:
        P2OUT |= BIT7;
        break;

    case W5500_INT:
        P2OUT |= BIT8;
        break;

    case W5500_SCS:
        P3OUT |= BIT0;
        break;

    case W5500_SCLK:
        P3OUT |= BIT3;
        break;

    case W5500_MOSI:
        P3OUT |= BIT1;
        break;

    }
}

void io_low(int num)
{
    switch(num) {

    case W5500_RST:
        P2OUT &= ~BIT7;
        break;

    case W5500_INT:
        P2OUT &= ~BIT8;
        break;

    case W5500_SCS:
        P3OUT &= ~BIT0;
        break;

    case W5500_SCLK:
        P3OUT &= ~BIT3;
        break;

    case W5500_MOSI:
        P3OUT &= ~BIT1;
        break;
    }
}


unsigned char io_read(int num)
{
    unsigned char c;

    switch(num) {


    case W5500_MISO:
        c = P3IN & BIT2;
        break;

    }
    if (c != 0)
        return 1;
    else
        return 0;
}
/*******************************************************************************
 * ������  : SPI_ReadByte
 * ����    : ��ȡһ���ֽ�SPI���ص�����
 * ����    : ��
 * ���    : ��
 * ����ֵ  : ��ȡ���ļĴ�������
 * ˵��    : ��
 *******************************************************************************/
unsigned char SPI_Read_Byte(void)
{
    unsigned char i,rByte=0;

    io_low(W5500_SCLK);
    for(i=0;i<8;i++)
    {
        io_set(W5500_SCLK);
        rByte<<=1;
        rByte|=io_read(W5500_MISO);
        io_low(W5500_SCLK);
    }
    return rByte;
}

/*******************************************************************************
 * ������  : SPI_SendByte
 * ����    : SPI����һ���ֽ�����
 * ����    : dt:�����͵�����
 * ���    : ��
 * ����ֵ  : ��
 * ˵��    : ��
 *******************************************************************************/
void SPI_Send_Byte(unsigned char dt)
{
    unsigned char i;

    for(i=0;i<8;i++)
    {
        io_low(W5500_SCLK);
        if((dt<<i)&0x80)
            io_set(W5500_MOSI);
        else
            io_low(W5500_MOSI);
        io_set(W5500_SCLK);
    }
    io_low(W5500_SCLK);
}

/*******************************************************************************
 * ������  : SPI_Send_Short
 * ����    : SPI����2���ֽ�����(16λ)
 * ����    : dat:�����͵�16λ����
 * ���    : ��
 * ����ֵ  : ��
 * ˵��    : ��
 *******************************************************************************/
void SPI_Send_Short(unsigned short dt)
{
    SPI_Send_Byte((unsigned char)(dt/256));	//д���ݸ�λ
    SPI_Send_Byte(dt);						//д���ݵ�λ
}

/*******************************************************************************
 * ������  : Write_W5500_1Byte
 * ����    : ͨ��SPI��ָ����ַ�Ĵ���д1���ֽ�����
 * ����    : reg:16λ�Ĵ�����ַ,dat:��д�������
 * ���    : ��
 * ����ֵ  : ��
 * ˵��    : ��
 *******************************************************************************/
void Write_W5500_1Byte(unsigned short reg, unsigned char dat)
{
    //��W5500��SCSΪ�͵�ƽ
    io_low(W5500_SCS);

    SPI_Send_Short(reg);//ͨ��SPIд16λ�Ĵ�����ַ
    SPI_Send_Byte(FDM1|RWB_WRITE|COMMON_R);//ͨ��SPIд�����ֽ�,1���ֽ����ݳ���,д����,ѡ��ͨ�üĴ���
    SPI_Send_Byte(dat);//д1���ֽ�����
    io_set(W5500_SCS);
    //��W5500��SCSΪ�ߵ�ƽ
}

/*******************************************************************************
 * ������  : Write_W5500_2Byte
 * ����    : ͨ��SPI��ָ����ַ�Ĵ���д2���ֽ�����
 * ����    : reg:16λ�Ĵ�����ַ,dat:16λ��д�������(2���ֽ�)
 * ���    : ��
 * ����ֵ  : ��
 * ˵��    : ��
 *******************************************************************************/
void Write_W5500_2Byte(unsigned short reg, unsigned short dat)
{
    //��W5500��SCSΪ�͵�ƽ
    io_low(W5500_SCS);

    SPI_Send_Short(reg);//ͨ��SPIд16λ�Ĵ�����ַ
    SPI_Send_Byte(FDM2|RWB_WRITE|COMMON_R);//ͨ��SPIд�����ֽ�,2���ֽ����ݳ���,д����,ѡ��ͨ�üĴ���
    SPI_Send_Short(dat);//д16λ����
    io_set(W5500_SCS);
    //��W5500��SCSΪ�ߵ�ƽ
}

/*******************************************************************************
 * ������  : Write_W5500_nByte
 * ����    : ͨ��SPI��ָ����ַ�Ĵ���дn���ֽ�����
 * ����    : reg:16λ�Ĵ�����ַ,*dat_ptr:��д�����ݻ�����ָ��,size:��д������ݳ���
 * ���    : ��
 * ����ֵ  : ��
 * ˵��    : ��
 *******************************************************************************/
void Write_W5500_nByte(unsigned short reg, unsigned char *dat_ptr, unsigned short size)
{
    unsigned short i;

    //��W5500��SCSΪ�͵�ƽ
    io_low(W5500_SCS);

    SPI_Send_Short(reg);//ͨ��SPIд16λ�Ĵ�����ַ
    SPI_Send_Byte(VDM|RWB_WRITE|COMMON_R);//ͨ��SPIд�����ֽ�,N���ֽ����ݳ���,д����,ѡ��ͨ�üĴ���

    for(i=0;i<size;i++)//ѭ������������size���ֽ�����д��W5500
    {
        SPI_Send_Byte(*dat_ptr++);//дһ���ֽ�����
    }
    io_set(W5500_SCS);
    //��W5500��SCSΪ�ߵ�ƽ
}

/*******************************************************************************
 * ������  : Write_W5500_SOCK_1Byte
 * ����    : ͨ��SPI��ָ���˿ڼĴ���д1���ֽ�����
 * ����    : s:�˿ں�,reg:16λ�Ĵ�����ַ,dat:��д�������
 * ���    : ��
 * ����ֵ  : ��
 * ˵��    : ��
 *******************************************************************************/
void Write_W5500_SOCK_1Byte(SOCKET s, unsigned short reg, unsigned char dat)
{
    //��W5500��SCSΪ�͵�ƽ
    io_low(W5500_SCS);

    SPI_Send_Short(reg);//ͨ��SPIд16λ�Ĵ�����ַ
    SPI_Send_Byte(FDM1|RWB_WRITE|(s*0x20+0x08));//ͨ��SPIд�����ֽ�,1���ֽ����ݳ���,д����,ѡ��˿�s�ļĴ���
    SPI_Send_Byte(dat);//д1���ֽ�����
    io_set(W5500_SCS);
    //��W5500��SCSΪ�ߵ�ƽ
}

/*******************************************************************************
 * ������  : Write_W5500_SOCK_2Byte
 * ����    : ͨ��SPI��ָ���˿ڼĴ���д2���ֽ�����
 * ����    : s:�˿ں�,reg:16λ�Ĵ�����ַ,dat:16λ��д�������(2���ֽ�)
 * ���    : ��
 * ����ֵ  : ��
 * ˵��    : ��
 *******************************************************************************/
void Write_W5500_SOCK_2Byte(SOCKET s, unsigned short reg, unsigned short dat)
{
    //��W5500��SCSΪ�͵�ƽ
    io_low(W5500_SCS);
    SPI_Send_Short(reg);//ͨ��SPIд16λ�Ĵ�����ַ
    SPI_Send_Byte(FDM2|RWB_WRITE|(s*0x20+0x08));//ͨ��SPIд�����ֽ�,2���ֽ����ݳ���,д����,ѡ��˿�s�ļĴ���
    SPI_Send_Short(dat);//д16λ����

    io_set(W5500_SCS);
    //��W5500��SCSΪ�ߵ�ƽ
}

/*******************************************************************************
 * ������  : Write_W5500_SOCK_4Byte
 * ����    : ͨ��SPI��ָ���˿ڼĴ���д4���ֽ�����
 * ����    : s:�˿ں�,reg:16λ�Ĵ�����ַ,*dat_ptr:��д���4���ֽڻ�����ָ��
 * ���    : ��
 * ����ֵ  : ��
 * ˵��    : ��
 *******************************************************************************/
void Write_W5500_SOCK_4Byte(SOCKET s, unsigned short reg, unsigned char *dat_ptr)
{
    //��W5500��SCSΪ�͵�ƽ
    io_low(W5500_SCS);
    SPI_Send_Short(reg);//ͨ��SPIд16λ�Ĵ�����ַ
    SPI_Send_Byte(FDM4|RWB_WRITE|(s*0x20+0x08));//ͨ��SPIд�����ֽ�,4���ֽ����ݳ���,д����,ѡ��˿�s�ļĴ���

    SPI_Send_Byte(*dat_ptr++);//д��1���ֽ�����
    SPI_Send_Byte(*dat_ptr++);//д��2���ֽ�����
    SPI_Send_Byte(*dat_ptr++);//д��3���ֽ�����
    SPI_Send_Byte(*dat_ptr++);//д��4���ֽ�����
    io_set(W5500_SCS);
    //��W5500��SCSΪ�ߵ�ƽ
}

/*******************************************************************************
 * ������  : Read_W5500_1Byte
 * ����    : ��W5500ָ����ַ�Ĵ�����1���ֽ�����
 * ����    : reg:16λ�Ĵ�����ַ
 * ���    : ��
 * ����ֵ  : ��ȡ���Ĵ�����1���ֽ�����
 * ˵��    : ��
 *******************************************************************************/
unsigned char Read_W5500_1Byte(unsigned short reg)
{
    unsigned char i;

    //��W5500��SCSΪ�͵�ƽ
    io_low(W5500_SCS);
    SPI_Send_Short(reg);//ͨ��SPIд16λ�Ĵ�����ַ
    SPI_Send_Byte(FDM1|RWB_READ|COMMON_R);//ͨ��SPIд�����ֽ�,1���ֽ����ݳ���,������,ѡ��ͨ�üĴ���
    i=SPI_Read_Byte();
    io_set(W5500_SCS);
    //��W5500��SCSΪ�ߵ�ƽ
    return i;//���ض�ȡ���ļĴ�������
}

/*******************************************************************************
 * ������  : Read_W5500_SOCK_1Byte
 * ����    : ��W5500ָ���˿ڼĴ�����1���ֽ�����
 * ����    : s:�˿ں�,reg:16λ�Ĵ�����ַ
 * ���    : ��
 * ����ֵ  : ��ȡ���Ĵ�����1���ֽ�����
 * ˵��    : ��
 *******************************************************************************/
unsigned char Read_W5500_SOCK_1Byte(SOCKET s, unsigned short reg)
{
    unsigned char i;

    //��W5500��SCSΪ�͵�ƽ
    io_low(W5500_SCS);

    SPI_Send_Short(reg);//ͨ��SPIд16λ�Ĵ�����ַ
    SPI_Send_Byte(FDM1|RWB_READ|(s*0x20+0x08));//ͨ��SPIд�����ֽ�,1���ֽ����ݳ���,������,ѡ��˿�s�ļĴ���
    i=SPI_Read_Byte();
    io_set(W5500_SCS);
    //��W5500��SCSΪ�ߵ�ƽ
    return i;//���ض�ȡ���ļĴ�������
}

/*******************************************************************************
 * ������  : Read_W5500_SOCK_2Byte
 * ����    : ��W5500ָ���˿ڼĴ�����2���ֽ�����
 * ����    : s:�˿ں�,reg:16λ�Ĵ�����ַ
 * ���    : ��
 * ����ֵ  : ��ȡ���Ĵ�����2���ֽ�����(16λ)
 * ˵��    : ��
 *******************************************************************************/
unsigned short Read_W5500_SOCK_2Byte(SOCKET s, unsigned short reg)
{
    unsigned short i;

    io_low(W5500_SCS);

    SPI_Send_Short(reg);//ͨ��SPIд16λ�Ĵ�����ַ
    SPI_Send_Byte(FDM2|RWB_READ|(s*0x20+0x08));//ͨ��SPIд�����ֽ�,2���ֽ����ݳ���,������,ѡ��˿�s�ļĴ���

    i=SPI_Read_Byte();
    i*=256;
    i+=SPI_Read_Byte();//��ȡ��λ����
    io_set(W5500_SCS);
    //��W5500��SCSΪ�ߵ�ƽ
    return i;//���ض�ȡ���ļĴ�������
}

/*******************************************************************************
 * ������  : Read_SOCK_Data_Buffer
 * ����    : ��W5500�������ݻ������ж�ȡ����
 * ����    : s:�˿ں�,*dat_ptr:���ݱ��滺����ָ��
 * ���    : ��
 * ����ֵ  : ��ȡ�������ݳ���,rx_size���ֽ�
 * ˵��    : ��
 *******************************************************************************/
unsigned short Read_SOCK_Data_Buffer(SOCKET s, unsigned char *dat_ptr)
{
    unsigned short rx_size;
    unsigned short offset, offset1;
    unsigned short i;
    unsigned char j;

    rx_size=Read_W5500_SOCK_2Byte(s,Sn_RX_RSR);
    if(rx_size==0) return 0;//û���յ������򷵻�
    if(rx_size>1460) rx_size=1460;

    offset=Read_W5500_SOCK_2Byte(s,Sn_RX_RD);
    offset1=offset;
    offset&=(S_RX_SIZE-1);//����ʵ�ʵ�������ַ

    io_low(W5500_SCS);

    SPI_Send_Short(offset);//д16λ��ַ
    SPI_Send_Byte(VDM|RWB_READ|(s*0x20+0x18));//д�����ֽ�,N���ֽ����ݳ���,������,ѡ��˿�s�ļĴ���

    if((offset+rx_size)<S_RX_SIZE)//�������ַδ����W5500���ջ������Ĵ���������ַ
    {
        for(i=0;i<rx_size;i++)//ѭ����ȡrx_size���ֽ�����
        {
            j=SPI_Read_Byte();//��ȡ1���ֽ�����
            *dat_ptr=j;//����ȡ�������ݱ��浽���ݱ��滺����
            dat_ptr++;//���ݱ��滺����ָ���ַ����1
        }
    }
    else//�������ַ����W5500���ջ������Ĵ���������ַ
    {
        offset=S_RX_SIZE-offset;
        for(i=0;i<offset;i++)//ѭ����ȡ��ǰoffset���ֽ�����
        {
            j=SPI_Read_Byte();//��ȡ1���ֽ�����
            *dat_ptr=j;//����ȡ�������ݱ��浽���ݱ��滺����
            dat_ptr++;//���ݱ��滺����ָ���ַ����1
        }
        io_set(W5500_SCS);

        io_low(W5500_SCS);

        SPI_Send_Short(0x00);//д16λ��ַ
        SPI_Send_Byte(VDM|RWB_READ|(s*0x20+0x18));//д�����ֽ�,N���ֽ����ݳ���,������,ѡ��˿�s�ļĴ���

        for(;i<rx_size;i++)//ѭ����ȡ��rx_size-offset���ֽ�����
        {
            j=SPI_Read_Byte();//��ȡ1���ֽ�����
            *dat_ptr=j;//����ȡ�������ݱ��浽���ݱ��滺����
            dat_ptr++;//���ݱ��滺����ָ���ַ����1
        }
    }
    io_set(W5500_SCS);

    offset1+=rx_size;//����ʵ��������ַ,���´ζ�ȡ���յ������ݵ���ʼ��ַ
    Write_W5500_SOCK_2Byte(s, Sn_RX_RD, offset1);
    Write_W5500_SOCK_1Byte(s, Sn_CR, RECV);//����������������
    return rx_size;//���ؽ��յ����ݵĳ���
}

/*******************************************************************************
 * ������  : Write_SOCK_Data_Buffer
 * ����    : ������д��W5500�����ݷ��ͻ�����
 * ����    : s:�˿ں�,*dat_ptr:���ݱ��滺����ָ��,size:��д�����ݵĳ���
 * ���    : ��
 * ����ֵ  : ��
 * ˵��    : ��
 *******************************************************************************/
void Write_SOCK_Data_Buffer(SOCKET s, unsigned char *dat_ptr, unsigned short size)
{
    unsigned short offset,offset1;
    unsigned short i;

    //�����UDPģʽ,�����ڴ�����Ŀ��������IP�Ͷ˿ں�
    if((Read_W5500_SOCK_1Byte(s,Sn_MR)&0x0f) != SOCK_UDP)//���Socket��ʧ��
    {
        Write_W5500_SOCK_4Byte(s, Sn_DIPR, UDP_DIPR);//����Ŀ������IP
        Write_W5500_SOCK_2Byte(s, Sn_DPORTR, UDP_DPORT[0]*256+UDP_DPORT[1]);//����Ŀ�������˿ں�
    }

    offset=Read_W5500_SOCK_2Byte(s,Sn_TX_WR);
    offset1=offset;
    offset&=(S_TX_SIZE-1);//����ʵ�ʵ�������ַ

    io_low(W5500_SCS);

    SPI_Send_Short(offset);//д16λ��ַ
    SPI_Send_Byte(VDM|RWB_WRITE|(s*0x20+0x10));//д�����ֽ�,N���ֽ����ݳ���,д����,ѡ��˿�s�ļĴ���

    if((offset+size)<S_TX_SIZE)//�������ַδ����W5500���ͻ������Ĵ���������ַ
    {
        for(i=0;i<size;i++)//ѭ��д��size���ֽ�����
        {
            SPI_Send_Byte(*dat_ptr++);//д��һ���ֽڵ�����
        }
    }
    else//�������ַ����W5500���ͻ������Ĵ���������ַ
    {
        offset=S_TX_SIZE-offset;
        for(i=0;i<offset;i++)//ѭ��д��ǰoffset���ֽ�����
        {
            SPI_Send_Byte(*dat_ptr++);//д��һ���ֽڵ�����
        }
        io_set(W5500_SCS);

        io_low(W5500_SCS);

        SPI_Send_Short(0x00);//д16λ��ַ
        SPI_Send_Byte(VDM|RWB_WRITE|(s*0x20+0x10));//д�����ֽ�,N���ֽ����ݳ���,д����,ѡ��˿�s�ļĴ���

        for(;i<size;i++)//ѭ��д��size-offset���ֽ�����
        {
            SPI_Send_Byte(*dat_ptr++);//д��һ���ֽڵ�����
        }
    }
    io_set(W5500_SCS);

    offset1+=size;//����ʵ��������ַ,���´�д���������ݵ��������ݻ���������ʼ��ַ
    Write_W5500_SOCK_2Byte(s, Sn_TX_WR, offset1);
    Write_W5500_SOCK_1Byte(s, Sn_CR, SEND);//����������������
}

/*******************************************************************************
 * ������  : W5500_Hardware_Reset
 * ����    : Ӳ����λW5500
 * ����    : ��
 * ���    : ��
 * ����ֵ  : ��
 * ˵��    : W5500�ĸ�λ���ű��ֵ͵�ƽ����500us����,������ΧW5500
 *******************************************************************************/
void W5500_Hardware_Reset(void)
{

    io_low(W5500_RST);
    Delay(2000);
    io_set(W5500_RST);
    Delay(2000);
    while((Read_W5500_1Byte(PHYCFGR)&LINK)==0);//�ȴ���̫���������
}

/*******************************************************************************
 * ������  : W5500_Init
 * ����    : ��ʼ��W5500�Ĵ�������
 * ����    : ��
 * ���    : ��
 * ����ֵ  : ��
 * ˵��    : ��ʹ��W5500֮ǰ���ȶ�W5500��ʼ��
 *******************************************************************************/
void W5500_Init(void)
{
    unsigned char i=0;

    Write_W5500_1Byte(MR, RST);//������λW5500,��1��Ч,��λ���Զ���0
    Delay(10);//��ʱ10ms,�Լ�����ú���

    //��������(Gateway)��IP��ַ,Gateway_IPΪ4�ֽ�unsigned char����,�Լ�����
    //ʹ�����ؿ���ʹͨ��ͻ�������ľ��ޣ�ͨ�����ؿ��Է��ʵ��������������Internet
    Write_W5500_nByte(GAR, Gateway_IP, 4);

    //������������(MASK)ֵ,SUB_MASKΪ4�ֽ�unsigned char����,�Լ�����
    //��������������������
    Write_W5500_nByte(SUBR,Sub_Mask,4);

    //����������ַ,PHY_ADDRΪ6�ֽ�unsigned char����,�Լ�����,����Ψһ��ʶ�����豸��������ֵַ
    //�õ�ֵַ��Ҫ��IEEE���룬����OUI�Ĺ涨��ǰ3���ֽ�Ϊ���̴��룬�������ֽ�Ϊ��Ʒ���
    //����Լ�����������ַ��ע���һ���ֽڱ���Ϊż��
    Write_W5500_nByte(SHAR,Phy_Addr,6);

    //���ñ�����IP��ַ,IP_ADDRΪ4�ֽ�unsigned char����,�Լ�����
    //ע�⣬����IP�����뱾��IP����ͬһ�����������򱾻����޷��ҵ�����
    Write_W5500_nByte(SIPR,IP_Addr,4);

    //���÷��ͻ������ͽ��ջ������Ĵ�С���ο�W5500�����ֲ�
    for(i=0;i<8;i++)
    {
        Write_W5500_SOCK_1Byte(i,Sn_RXBUF_SIZE, 0x02);//Socket Rx memory size=2k
        Write_W5500_SOCK_1Byte(i,Sn_TXBUF_SIZE, 0x02);//Socket Tx mempry size=2k
    }

    //��������ʱ�䣬Ĭ��Ϊ2000(200ms)
    //ÿһ��λ��ֵΪ100΢��,��ʼ��ʱֵ��Ϊ2000(0x07D0),����200����
    Write_W5500_2Byte(RTR, 0x07d0);

    //�������Դ�����Ĭ��Ϊ8��
    //����ط��Ĵ��������趨ֵ,�������ʱ�ж�(��صĶ˿��жϼĴ����е�Sn_IR ��ʱλ(TIMEOUT)�á�1��)
    Write_W5500_1Byte(RCR,8);
}

/*******************************************************************************
 * ������  : Detect_Gateway
 * ����    : ������ط�����
 * ����    : ��
 * ���    : ��
 * ����ֵ  : �ɹ�����TRUE(0xFF),ʧ�ܷ���FALSE(0x00)
 * ˵��    : ��
 *******************************************************************************/
unsigned char Detect_Gateway(void)
{
    unsigned char ip_adde[4];
    ip_adde[0]=IP_Addr[0]+1;
    ip_adde[1]=IP_Addr[1]+1;
    ip_adde[2]=IP_Addr[2]+1;
    ip_adde[3]=IP_Addr[3]+1;

    //������ؼ���ȡ���ص�������ַ
    Write_W5500_SOCK_4Byte(0,Sn_DIPR,ip_adde);//��Ŀ�ĵ�ַ�Ĵ���д���뱾��IP��ͬ��IPֵ
    Write_W5500_SOCK_1Byte(0,Sn_MR,MR_TCP);//����socketΪTCPģʽ
    Write_W5500_SOCK_1Byte(0,Sn_CR,OPEN);//��Socket
    Delay(5);//��ʱ5ms

    if(Read_W5500_SOCK_1Byte(0,Sn_SR) != SOCK_INIT)//���socket��ʧ��
    {
        Write_W5500_SOCK_1Byte(0,Sn_CR,CLOSE);//�򿪲��ɹ�,�ر�Socket
        return FALSE;//����FALSE(0x00)
    }

    Write_W5500_SOCK_1Byte(0,Sn_CR,CONNECT);//����SocketΪConnectģʽ

    do
    {
        unsigned char j=0;
        j=Read_W5500_SOCK_1Byte(0,Sn_IR);//��ȡSocket0�жϱ�־�Ĵ���
        if(j!=0)
            Write_W5500_SOCK_1Byte(0,Sn_IR,j);
        Delay(5);//��ʱ5ms
        if((j&IR_TIMEOUT) == IR_TIMEOUT)
        {
            return FALSE;
        }
        else if(Read_W5500_SOCK_1Byte(0,Sn_DHAR) != 0xff)
        {
            Write_W5500_SOCK_1Byte(0,Sn_CR,CLOSE);//�ر�Socket
            return TRUE;
        }
    }while(1);
}

/*******************************************************************************
 * ������  : Socket_Init
 * ����    : ָ��Socket(0~7)��ʼ��
 * ����    : s:����ʼ���Ķ˿�
 * ���    : ��
 * ����ֵ  : ��
 * ˵��    : ��
 *******************************************************************************/
void Socket_Init(SOCKET s)
{
    //���÷�Ƭ���ȣ��ο�W5500�����ֲᣬ��ֵ���Բ��޸�
    Write_W5500_SOCK_2Byte(0, Sn_MSSR, 30);//����Ƭ�ֽ���=30(0x001e)
    //����ָ���˿�
    switch(s)
    {
    case 0:
        //���ö˿�0�Ķ˿ں�
        Write_W5500_SOCK_2Byte(0, Sn_PORT, S0_Port[0]*256+S0_Port[1]);
        //���ö˿�0Ŀ��(Զ��)�˿ں�
        Write_W5500_SOCK_2Byte(0, Sn_DPORTR, S0_DPort[0]*256+S0_DPort[1]);
        //���ö˿�0Ŀ��(Զ��)IP��ַ
        Write_W5500_SOCK_4Byte(0, Sn_DIPR, S0_DIP);

        break;

    case 1:
        break;

    case 2:
        break;

    case 3:
        break;

    case 4:
        break;

    case 5:
        break;

    case 6:
        break;

    case 7:
        break;

    default:
        break;
    }
}

/*******************************************************************************
 * ������  : Socket_Connect
 * ����    : ����ָ��Socket(0~7)Ϊ�ͻ�����Զ�̷���������
 * ����    : s:���趨�Ķ˿�
 * ���    : ��
 * ����ֵ  : �ɹ�����TRUE(0xFF),ʧ�ܷ���FALSE(0x00)
 * ˵��    : ������Socket�����ڿͻ���ģʽʱ,���øó���,��Զ�̷�������������
 *			����������Ӻ���ֳ�ʱ�жϣ��������������ʧ��,��Ҫ���µ��øó�������
 *			�ó���ÿ����һ��,�������������һ������
 *******************************************************************************/
unsigned char Socket_Connect(SOCKET s)
{
    Write_W5500_SOCK_1Byte(s,Sn_MR,MR_TCP);//����socketΪTCPģʽ
    Write_W5500_SOCK_1Byte(s,Sn_CR,OPEN);//��Socket
    Delay(5);//��ʱ5ms
    if(Read_W5500_SOCK_1Byte(s,Sn_SR)!=SOCK_INIT)//���socket��ʧ��
    {
        Write_W5500_SOCK_1Byte(s,Sn_CR,CLOSE);//�򿪲��ɹ�,�ر�Socket
        return FALSE;//����FALSE(0x00)
    }
    Write_W5500_SOCK_1Byte(s,Sn_CR,CONNECT);//����SocketΪConnectģʽ
    return TRUE;//����TRUE,���óɹ�
}

/*******************************************************************************
 * ������  : Socket_Listen
 * ����    : ����ָ��Socket(0~7)��Ϊ�������ȴ�Զ������������
 * ����    : s:���趨�Ķ˿�
 * ���    : ��
 * ����ֵ  : �ɹ�����TRUE(0xFF),ʧ�ܷ���FALSE(0x00)
 * ˵��    : ������Socket�����ڷ�����ģʽʱ,���øó���,�ȵ�Զ������������
 *			�ó���ֻ����һ��,��ʹW5500����Ϊ������ģʽ
 *******************************************************************************/
unsigned char Socket_Listen(SOCKET s)
{
    Write_W5500_SOCK_1Byte(s,Sn_MR,MR_TCP);//����socketΪTCPģʽ
    Write_W5500_SOCK_1Byte(s,Sn_CR,OPEN);//��Socket
    Delay(5);//��ʱ5ms
    if(Read_W5500_SOCK_1Byte(s,Sn_SR)!=SOCK_INIT)//���socket��ʧ��
    {
        Write_W5500_SOCK_1Byte(s,Sn_CR,CLOSE);//�򿪲��ɹ�,�ر�Socket
        return FALSE;//����FALSE(0x00)
    }
    Write_W5500_SOCK_1Byte(s,Sn_CR,LISTEN);//����SocketΪ����ģʽ
    Delay(5);//��ʱ5ms
    if(Read_W5500_SOCK_1Byte(s,Sn_SR)!=SOCK_LISTEN)//���socket����ʧ��
    {
        Write_W5500_SOCK_1Byte(s,Sn_CR,CLOSE);//���ò��ɹ�,�ر�Socket
        return FALSE;//����FALSE(0x00)
    }

    return TRUE;

    //���������Socket�Ĵ򿪺�������������,����Զ�̿ͻ����Ƿ�������������,����Ҫ�ȴ�Socket�жϣ�
    //���ж�Socket�������Ƿ�ɹ����ο�W5500�����ֲ��Socket�ж�״̬
    //�ڷ���������ģʽ����Ҫ����Ŀ��IP��Ŀ�Ķ˿ں�
}

/*******************************************************************************
 * ������  : Socket_UDP
 * ����    : ����ָ��Socket(0~7)ΪUDPģʽ
 * ����    : s:���趨�Ķ˿�
 * ���    : ��
 * ����ֵ  : �ɹ�����TRUE(0xFF),ʧ�ܷ���FALSE(0x00)
 * ˵��    : ���Socket������UDPģʽ,���øó���,��UDPģʽ��,Socketͨ�Ų���Ҫ��������
 *			�ó���ֻ����һ�Σ���ʹW5500����ΪUDPģʽ
 *******************************************************************************/
unsigned char Socket_UDP(SOCKET s)
{
    Write_W5500_SOCK_1Byte(s,Sn_MR,MR_UDP);//����SocketΪUDPģʽ*/
    Write_W5500_SOCK_1Byte(s,Sn_CR,OPEN);//��Socket*/
    Delay(5);//��ʱ5ms
    if(Read_W5500_SOCK_1Byte(s,Sn_SR)!=SOCK_UDP)//���Socket��ʧ��
    {
        Write_W5500_SOCK_1Byte(s,Sn_CR,CLOSE);//�򿪲��ɹ�,�ر�Socket
        return FALSE;//����FALSE(0x00)
    }
    else
        return TRUE;

    //���������Socket�Ĵ򿪺�UDPģʽ����,������ģʽ��������Ҫ��Զ��������������
    //��ΪSocket����Ҫ��������,�����ڷ�������ǰ����������Ŀ������IP��Ŀ��Socket�Ķ˿ں�
    //���Ŀ������IP��Ŀ��Socket�Ķ˿ں��ǹ̶���,�����й�����û�иı�,��ôҲ��������������
}

/*******************************************************************************
 * ������  : W5500_Interrupt_Process
 * ����    : W5500�жϴ���������
 * ����    : ��
 * ���    : ��
 * ����ֵ  : ��
 * ˵��    : ��
 *******************************************************************************/
void W5500_Interrupt_Process(void)
{
    unsigned char i,j;

    IntDispose:

    i=Read_W5500_1Byte(SIR);//��ȡ�˿��жϱ�־�Ĵ���
    if((i & S0_INT) == S0_INT)//Socket0�¼�����
    {
        j=Read_W5500_SOCK_1Byte(0,Sn_IR);//��ȡSocket0�жϱ�־�Ĵ���
        Write_W5500_SOCK_1Byte(0,Sn_IR,j);
        if(j&IR_CON)//��TCPģʽ��,Socket0�ɹ�����
        {
            S0_State|=S_CONN;//��������״̬0x02,�˿�������ӣ�����������������
        }
        if(j&IR_DISCON)//��TCPģʽ��Socket�Ͽ����Ӵ���
        {
            Write_W5500_SOCK_1Byte(0,Sn_CR,CLOSE);//�رն˿�,�ȴ����´�����
            Socket_Init(0);		//ָ��Socket(0~7)��ʼ��,��ʼ���˿�0
            S0_State=0;//��������״̬0x00,�˿�����ʧ��
        }
        if(j&IR_SEND_OK)//Socket0���ݷ������,�����ٴ�����S_tx_process()������������
        {
            S0_Data|=S_TRANSMITOK;//�˿ڷ���һ�����ݰ����
        }
        if(j&IR_RECV)//Socket���յ�����,��������S_rx_process()����
        {
            S0_Data|=S_RECEIVE;//�˿ڽ��յ�һ�����ݰ�
        }
        if(j&IR_TIMEOUT)//Socket���ӻ����ݴ��䳬ʱ����
        {
            Write_W5500_SOCK_1Byte(0,Sn_CR,CLOSE);// �رն˿�,�ȴ����´�����
            S0_State=0;//��������״̬0x00,�˿�����ʧ��
        }
    }

    if(Read_W5500_1Byte(SIR) != 0)
        goto IntDispose;
}
