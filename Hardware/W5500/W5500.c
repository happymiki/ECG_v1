/**********************************************************************************
 * �ļ���  ��W5500.c
 * ����    ��W5500 ����������         
 * ��汾  ��ST_v3.5
**********************************************************************************/
#include "W5500.h"	



bool W5500_Socket0_Sendding = false;
Uint8PacketQueue W5500_Socket0_SendPacketQueue = {NULL, NULL, CreatUint8PacketNode, Free_W5500_PacketNoedItem};


/***************----- ��������������� -----***************/
uint8_t Gateway_IP[4]={192,168,1,1};//����IP��ַ 
uint8_t Sub_Mask[4]={255,255,255,0};	//�������� 
uint8_t Phy_Addr[6]={0x0C, 
0x29,0xAB,0x7C,0x00,0x01};	//�����ַ(MAC) 
uint8_t IP_Addr[4]={192,168,1,142};	//����IP��ַ 

uint8_t UDP_Target_IPR[4]={192,168,1,105};	//UDP(�㲥)ģʽ,Ŀ������IP��ַ
uint8_t UDP_Target_Port[2]={0x7B,0x17};	//UDP(�㲥)ģʽ,Ŀ�������˿ں�

Socket socket_0 = {
    0,
    {192,168,1,142},
    5000,
//    {192,168,1,102},
    {115,159,203,38},
    31511,
    TCP_CLIENT,
    S_BREAK,
    S_TRANSMITOK,
    &W5500_Socket0_SendPacketQueue,
    false
};

/***************----- �˿����ݻ����� -----***************/
uint8_t Rx_Buffer[S_RX_SIZE];	//�˿ڽ������ݻ�����

uint8_t W5500_Interrupt;	//W5500�жϱ�־(0:���ж�,1:���ж�)



/*******************************************************************************
* ������  : W5500_GPIO_Configuration
* ����    : W5500 GPIO��ʼ������
* ����    : ��
* ���    : ��
* ����ֵ  : ��
* ˵��    : ��
*******************************************************************************/
void W5500_GPIO_Configuration(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	EXTI_InitTypeDef  EXTI_InitStructure;	

   RCC_APB2PeriphClockCmd(W5500_SCS_RCC_Periph | W5500_RST_RCC_Periph
                | W5500_INT_RCC_Periph | RCC_APB2Periph_AFIO, ENABLE); 
	/* W5500_RST���ų�ʼ������(PC5) */
	GPIO_InitStructure.GPIO_Pin  = W5500_RST_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(W5500_RST_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(W5500_RST_PORT, W5500_RST_PIN);
	
	/* W5500_INT���ų�ʼ������(PC4) */	
	GPIO_InitStructure.GPIO_Pin = W5500_INT_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(W5500_INT_PORT, &GPIO_InitStructure);
		
	/* Connect EXTI Line4 to PC4 */
	GPIO_EXTILineConfig(W5500_INT_SOURCE_PORT, W5500_INT_SOURCE_PIN);

	/* PC4 as W5500 interrupt input */
	EXTI_InitStructure.EXTI_Line = W5500_INT_EXTI_LINE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}

/*******************************************************************************
* ������  : W5500_NVIC_Configuration
* ����    : W5500 ���������ж����ȼ�����
* ����    : ��
* ���    : ��
* ����ֵ  : ��
* ˵��    : ��
*******************************************************************************/
void W5500_NVIC_Configuration(void)
{
    NVIC_IRQChannel_Configuration_Set(W5500_INT_EXTI_IRQn,1,2,ENABLE);
}

/*******************************************************************************
* ������  : W5500_SPI_Send_Short
* ����    : SPI����2���ֽ�����(16λ)
* ����    : dat:�����͵�16λ����
* ���    : ��
* ����ֵ  : ��
* ˵��    : ��
*******************************************************************************/
void W5500_SPI_Send_Short(uint16_t dat)
{
	W5500_SPI_Send_Byte(dat>>8);//д���ݸ�λ
	W5500_SPI_Send_Byte(dat);	//д���ݵ�λ
}
void W5500_SCS_High(void)
{
    GPIO_SetBits(W5500_SCS_PORT,W5500_SCS_PIN); //��W5500��SCSΪ�ߵ�ƽ
}
void W5500_SCS_Low(void)
{
    GPIO_ResetBits(W5500_SCS_PORT,W5500_SCS_PIN); //��W5500��SCSΪ�ߵ�ƽ
}
/*******************************************************************************
* ������  : Write_W5500_1Byte
* ����    : ͨ��SPI��ָ����ַ�Ĵ���д1���ֽ�����
* ����    : reg:16λ�Ĵ�����ַ,dat:��д�������
* ���    : ��
* ����ֵ  : ��
* ˵��    : ��
*******************************************************************************/
void Write_W5500_1Byte(uint16_t reg, uint8_t dat)
{
	W5500_SCS_Low();//��W5500��SCSΪ�͵�ƽ

	W5500_SPI_Send_Short(reg);//ͨ��SPIд16λ�Ĵ�����ַ
	W5500_SPI_Send_Byte(FDM1|RWB_WRITE|COMMON_R);//ͨ��SPIд�����ֽ�,1���ֽ����ݳ���,д����,ѡ��ͨ�üĴ���
	W5500_SPI_Send_Byte(dat);//д1���ֽ�����

	W5500_SCS_High(); //��W5500��SCSΪ�ߵ�ƽ
}

/*******************************************************************************
* ������  : Write_W5500_2Byte
* ����    : ͨ��SPI��ָ����ַ�Ĵ���д2���ֽ�����
* ����    : reg:16λ�Ĵ�����ַ,dat:16λ��д�������(2���ֽ�)
* ���    : ��
* ����ֵ  : ��
* ˵��    : ��
*******************************************************************************/
void Write_W5500_2Byte(uint16_t reg, uint16_t dat)
{
	W5500_SCS_Low();//��W5500��SCSΪ�͵�ƽ
		
	W5500_SPI_Send_Short(reg);//ͨ��SPIд16λ�Ĵ�����ַ
	W5500_SPI_Send_Byte(FDM2|RWB_WRITE|COMMON_R);//ͨ��SPIд�����ֽ�,2���ֽ����ݳ���,д����,ѡ��ͨ�üĴ���
	W5500_SPI_Send_Short(dat);//д16λ����

	W5500_SCS_High(); //��W5500��SCSΪ�ߵ�ƽ
}

/*******************************************************************************
* ������  : Write_W5500_nByte
* ����    : ͨ��SPI��ָ����ַ�Ĵ���дn���ֽ�����
* ����    : reg:16λ�Ĵ�����ַ,*dat_ptr:��д�����ݻ�����ָ��,size:��д������ݳ���
* ���    : ��
* ����ֵ  : ��
* ˵��    : ��
*******************************************************************************/
void Write_W5500_nByte(uint16_t reg, uint8_t *dat_ptr, uint16_t size)
{
	uint16_t i;

	W5500_SCS_Low();//��W5500��SCSΪ�͵�ƽ	
		
	W5500_SPI_Send_Short(reg);//ͨ��SPIд16λ�Ĵ�����ַ
	W5500_SPI_Send_Byte(VDM|RWB_WRITE|COMMON_R);//ͨ��SPIд�����ֽ�,N���ֽ����ݳ���,д����,ѡ��ͨ�üĴ���

	for(i=0;i<size;i++)//ѭ������������size���ֽ�����д��W5500
	{
		W5500_SPI_Send_Byte(*dat_ptr++);//дһ���ֽ�����
	}

	W5500_SCS_High(); //��W5500��SCSΪ�ߵ�ƽ
}

/*******************************************************************************
* ������  : Write_W5500_SOCK_1Byte
* ����    : ͨ��SPI��ָ���˿ڼĴ���д1���ֽ�����
* ����    : s:�˿ں�,reg:16λ�Ĵ�����ַ,dat:��д�������
* ���    : ��
* ����ֵ  : ��
* ˵��    : ��
*******************************************************************************/
void Write_W5500_SOCK_1Byte(uint8_t socket_index, uint16_t reg, uint8_t dat)
{
	W5500_SCS_Low();//��W5500��SCSΪ�͵�ƽ	
		
	W5500_SPI_Send_Short(reg);//ͨ��SPIд16λ�Ĵ�����ַ
	W5500_SPI_Send_Byte(FDM1|RWB_WRITE|(socket_index*0x20+0x08));//ͨ��SPIд�����ֽ�,1���ֽ����ݳ���,д����,ѡ��˿�s�ļĴ���
	W5500_SPI_Send_Byte(dat);//д1���ֽ�����

	W5500_SCS_High(); //��W5500��SCSΪ�ߵ�ƽ
}

/*******************************************************************************
* ������  : Write_W5500_SOCK_2Byte
* ����    : ͨ��SPI��ָ���˿ڼĴ���д2���ֽ�����
* ����    : s:�˿ں�,reg:16λ�Ĵ�����ַ,dat:16λ��д�������(2���ֽ�)
* ���    : ��
* ����ֵ  : ��
* ˵��    : ��
*******************************************************************************/
void Write_W5500_SOCK_2Byte(uint8_t socket_index, uint16_t reg, uint16_t dat)
{
	W5500_SCS_Low();//��W5500��SCSΪ�͵�ƽ
			
	W5500_SPI_Send_Short(reg);//ͨ��SPIд16λ�Ĵ�����ַ
	W5500_SPI_Send_Byte(FDM2|RWB_WRITE|(socket_index*0x20+0x08));//ͨ��SPIд�����ֽ�,2���ֽ����ݳ���,д����,ѡ��˿�s�ļĴ���
	W5500_SPI_Send_Short(dat);//д16λ����

	W5500_SCS_High(); //��W5500��SCSΪ�ߵ�ƽ
}

/*******************************************************************************
* ������  : Write_W5500_SOCK_4Byte
* ����    : ͨ��SPI��ָ���˿ڼĴ���д4���ֽ�����
* ����    : s:�˿ں�,reg:16λ�Ĵ�����ַ,*dat_ptr:��д���4���ֽڻ�����ָ��
* ���    : ��
* ����ֵ  : ��
* ˵��    : ��
*******************************************************************************/
void Write_W5500_SOCK_4Byte(uint8_t socket_index, uint16_t reg, uint8_t *dat_ptr)
{
	W5500_SCS_Low();//��W5500��SCSΪ�͵�ƽ
			
	W5500_SPI_Send_Short(reg);//ͨ��SPIд16λ�Ĵ�����ַ
	W5500_SPI_Send_Byte(FDM4|RWB_WRITE|(socket_index*0x20+0x08));//ͨ��SPIд�����ֽ�,4���ֽ����ݳ���,д����,ѡ��˿�s�ļĴ���

	W5500_SPI_Send_Byte(*dat_ptr++);//д��1���ֽ�����
	W5500_SPI_Send_Byte(*dat_ptr++);//д��2���ֽ�����
	W5500_SPI_Send_Byte(*dat_ptr++);//д��3���ֽ�����
	W5500_SPI_Send_Byte(*dat_ptr++);//д��4���ֽ�����

	W5500_SCS_High(); //��W5500��SCSΪ�ߵ�ƽ
}

/*******************************************************************************
* ������  : Read_W5500_1Byte
* ����    : ��W5500ָ����ַ�Ĵ�����1���ֽ�����
* ����    : reg:16λ�Ĵ�����ַ
* ���    : ��
* ����ֵ  : ��ȡ���Ĵ�����1���ֽ�����
* ˵��    : ��
*******************************************************************************/
uint8_t Read_W5500_1Byte(uint16_t reg)
{
	uint8_t i;

	W5500_SCS_Low();//��W5500��SCSΪ�͵�ƽ
			
	W5500_SPI_Send_Short(reg);//ͨ��SPIд16λ�Ĵ�����ַ
	W5500_SPI_Send_Byte(FDM1|RWB_READ|COMMON_R);//ͨ��SPIд�����ֽ�,1���ֽ����ݳ���,������,ѡ��ͨ�üĴ���

	i=W5500_SPI_Receive_Data();
	W5500_SPI_Send_Byte(0x00);//����һ��������
	i=W5500_SPI_Receive_Data();//��ȡ1���ֽ�����

	W5500_SCS_High();//��W5500��SCSΪ�ߵ�ƽ
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
uint8_t Read_W5500_SOCK_1Byte(uint8_t socket_index, uint16_t reg)
{
	uint8_t i;

	W5500_SCS_Low();//��W5500��SCSΪ�͵�ƽ
			
	W5500_SPI_Send_Short(reg);//ͨ��SPIд16λ�Ĵ�����ַ
	W5500_SPI_Send_Byte(FDM1|RWB_READ|(socket_index*0x20+0x08));//ͨ��SPIд�����ֽ�,1���ֽ����ݳ���,������,ѡ��˿�s�ļĴ���

	i=W5500_SPI_Receive_Data();
	W5500_SPI_Send_Byte(0x00);//����һ��������
	i=W5500_SPI_Receive_Data();//��ȡ1���ֽ�����

	W5500_SCS_High();//��W5500��SCSΪ�ߵ�ƽ
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
uint16_t Read_W5500_SOCK_2Byte(uint8_t socket_index, uint16_t reg)
{
	uint16_t i;

	W5500_SCS_Low();//��W5500��SCSΪ�͵�ƽ
			
	W5500_SPI_Send_Short(reg);//ͨ��SPIд16λ�Ĵ�����ַ
	W5500_SPI_Send_Byte(FDM2|RWB_READ|(socket_index*0x20+0x08));//ͨ��SPIд�����ֽ�,2���ֽ����ݳ���,������,ѡ��˿�s�ļĴ���

	i=W5500_SPI_Receive_Data();
	W5500_SPI_Send_Byte(0x00);//����һ��������
	i=W5500_SPI_Receive_Data();//��ȡ��λ����
	W5500_SPI_Send_Byte(0x00);//����һ��������
	i = i<<8;
	i+=W5500_SPI_Receive_Data();//��ȡ��λ����

	W5500_SCS_High();//��W5500��SCSΪ�ߵ�ƽ
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
uint16_t Read_SOCK_Data_Buffer(Socket* socket, uint8_t *dat_ptr)
{
	uint16_t rx_size;
	uint16_t offset, offset1;
	uint16_t i;
	uint8_t j;

	rx_size=Read_W5500_SOCK_2Byte(socket->socket_index,Sn_RX_RSR);
	if(rx_size==0) return 0;//û���յ������򷵻�
	if(rx_size>1460) rx_size=1460;

	offset=Read_W5500_SOCK_2Byte(socket->socket_index,Sn_RX_RD);
	offset1=offset;
	offset&=(S_RX_SIZE-1);//����ʵ�ʵ������ַ

	W5500_SCS_Low();//��W5500��SCSΪ�͵�ƽ

	W5500_SPI_Send_Short(offset);//д16λ��ַ
	W5500_SPI_Send_Byte(VDM|RWB_READ|(socket->socket_index*0x20+0x18));//д�����ֽ�,N���ֽ����ݳ���,������,ѡ��˿�s�ļĴ���
	j=W5500_SPI_Receive_Data();
	
	if((offset+rx_size)<S_RX_SIZE)//�������ַδ����W5500���ջ������Ĵ���������ַ
	{
		for(i=0;i<rx_size;i++)//ѭ����ȡrx_size���ֽ�����
		{
			W5500_SPI_Send_Byte(0x00);//����һ��������
			j=W5500_SPI_Receive_Data();//��ȡ1���ֽ�����
			*dat_ptr=j;//����ȡ�������ݱ��浽���ݱ��滺����
			dat_ptr++;//���ݱ��滺����ָ���ַ����1
		}
	}
	else//�������ַ����W5500���ջ������Ĵ���������ַ
	{
		offset=S_RX_SIZE-offset;
		for(i=0;i<offset;i++)//ѭ����ȡ��ǰoffset���ֽ�����
		{
			W5500_SPI_Send_Byte(0x00);//����һ��������
			j=W5500_SPI_Receive_Data();//��ȡ1���ֽ�����
			*dat_ptr=j;//����ȡ�������ݱ��浽���ݱ��滺����
			dat_ptr++;//���ݱ��滺����ָ���ַ����1
		}
		W5500_SCS_High(); //��W5500��SCSΪ�ߵ�ƽ

		W5500_SCS_Low();//��W5500��SCSΪ�͵�ƽ

		W5500_SPI_Send_Short(0x00);//д16λ��ַ
		W5500_SPI_Send_Byte(VDM|RWB_READ|(socket->socket_index*0x20+0x18));//д�����ֽ�,N���ֽ����ݳ���,������,ѡ��˿�s�ļĴ���
		j=W5500_SPI_Receive_Data();

		for(;i<rx_size;i++)//ѭ����ȡ��rx_size-offset���ֽ�����
		{
			W5500_SPI_Send_Byte(0x00);//����һ��������
			j=W5500_SPI_Receive_Data();//��ȡ1���ֽ�����
			*dat_ptr=j;//����ȡ�������ݱ��浽���ݱ��滺����
			dat_ptr++;//���ݱ��滺����ָ���ַ����1
		}
	}
	W5500_SCS_High(); //��W5500��SCSΪ�ߵ�ƽ

	offset1+=rx_size;//����ʵ�������ַ,���´ζ�ȡ���յ������ݵ���ʼ��ַ
	Write_W5500_SOCK_2Byte(socket->socket_index, Sn_RX_RD, offset1);
	Write_W5500_SOCK_1Byte(socket->socket_index, Sn_CR, RECV);//����������������
	return rx_size;//���ؽ��յ����ݵĳ���
}

/*******************************************************************************
* ������  : W5500_Send_Socket_Data
* ����    : ������д��W5500�����ݷ��ͻ�����
* ����    : s:�˿ں�,*dat_ptr:���ݱ��滺����ָ��,size:��д�����ݵĳ���
* ���    : ��
* ����ֵ  : ��
* ˵��    : ��
*******************************************************************************/
void W5500_Send_Socket_Data(Socket* socket)
{
    if(socket->send_Packet_Queue_Handle->head)
    {
        uint16_t offset,offset1;
        uint16_t i;
        uint8_t* packet;
        uint16_t packetLength;
        Uint8PacketNode* uint8PacketNodePointer;
        uint8PacketNodePointer = Uint8PacketQueuePop(socket->send_Packet_Queue_Handle);
        packet = uint8PacketNodePointer->packet;
        packetLength = uint8PacketNodePointer->packetLength;
        socket->packet_Sendding = true;
        //�����UDPģʽ,�����ڴ�����Ŀ��������IP�Ͷ˿ں�
        if((Read_W5500_SOCK_1Byte(socket->socket_index,Sn_MR)&0x0f) != SOCK_UDP)//���Socket��ʧ��
        {		
            Write_W5500_SOCK_4Byte(socket->socket_index, Sn_DIPR, UDP_Target_IPR);//����Ŀ������IP  		
            Write_W5500_SOCK_2Byte(socket->socket_index, Sn_DPORTR, ((uint16_t)UDP_Target_Port[0]<<8)+UDP_Target_Port[1]);//����Ŀ�������˿ں�				
        }

        offset=Read_W5500_SOCK_2Byte(socket->socket_index,Sn_TX_WR);
        offset1=offset;
        offset&=(S_TX_SIZE-1);//����ʵ�ʵ������ַ

        W5500_SCS_Low();//��W5500��SCSΪ�͵�ƽ

        W5500_SPI_Send_Short(offset);//д16λ��ַ
        W5500_SPI_Send_Byte(VDM|RWB_WRITE|(socket->socket_index*0x20+0x10));//д�����ֽ�,N���ֽ����ݳ���,д����,ѡ��˿�s�ļĴ���

        if((offset+packetLength)<S_TX_SIZE)//�������ַδ����W5500���ͻ������Ĵ���������ַ
        {
            for(i=0;i<packetLength;i++)//ѭ��д��size���ֽ�����
            {
                W5500_SPI_Send_Byte(*packet++);//д��һ���ֽڵ�����		
            }
        }
        else//�������ַ����W5500���ͻ������Ĵ���������ַ
        {
            offset=S_TX_SIZE-offset;
            for(i=0;i<offset;i++)//ѭ��д��ǰoffset���ֽ�����
            {
                W5500_SPI_Send_Byte(*packet++);//д��һ���ֽڵ�����
            }
            W5500_SCS_High(); //��W5500��SCSΪ�ߵ�ƽ

            W5500_SCS_Low();//��W5500��SCSΪ�͵�ƽ

            W5500_SPI_Send_Short(0x00);//д16λ��ַ
            W5500_SPI_Send_Byte(VDM|RWB_WRITE|(socket->socket_index*0x20+0x10));//д�����ֽ�,N���ֽ����ݳ���,д����,ѡ��˿�s�ļĴ���

            for(;i<packetLength;i++)//ѭ��д��size-offset���ֽ�����
            {
                W5500_SPI_Send_Byte(*packet++);//д��һ���ֽڵ�����
            }
        }
        W5500_SCS_High(); //��W5500��SCSΪ�ߵ�ƽ

        offset1+=packetLength;//����ʵ�������ַ,���´�д���������ݵ��������ݻ���������ʼ��ַ
        Write_W5500_SOCK_2Byte(socket->socket_index, Sn_TX_WR, offset1);
        Write_W5500_SOCK_1Byte(socket->socket_index, Sn_CR, SEND);//����������������
        socket->send_Packet_Queue_Handle->FreePacketNoedItem(uint8PacketNodePointer);
    }
    else
    {
        socket->packet_Sendding = false;
    }
}


/*******************************************************************************
* ������  : W5500_Hardware_Reset
* ����    : Ӳ����λW5500
* ����    : ��
* ���    : ��
* ����ֵ  : ��
* ˵��    : W5500�ĸ�λ���ű��ֵ͵�ƽ����500us����,��������W5500
*******************************************************************************/
void W5500_Hardware_Reset(void)
{
	GPIO_ResetBits(W5500_RST_PORT, W5500_RST_PIN);//��λ��������
	W5500_Delay_ms(50);
	GPIO_SetBits(W5500_RST_PORT, W5500_RST_PIN);//��λ��������
	W5500_Delay_ms(200);
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
    uint8_t socket_index;
    Socket* socket = &socket_0;
    printf("W5500 reset.\r\n");
	W5500_GPIO_Configuration();	//W5500 GPIO��ʼ������
    W5500_NVIC_Configuration(); //W5500 �ж�����
    W5500_SPI_Configuration();  //W5500 SPI��ʼ������
    W5500_Hardware_Reset();		//Ӳ����λW5500
    printf("W5500 Ethernet link\r\n");
	Write_W5500_1Byte(MR, RST);//�����λW5500,��1��Ч,��λ���Զ���0
	W5500_Delay_ms(10);//��ʱ10ms,�Լ�����ú���
	//��������(Gateway)��IP��ַ,Gateway_IPΪ4�ֽ�uint8_t����,�Լ����� 
	//ʹ�����ؿ���ʹͨ��ͻ�������ľ��ޣ�ͨ�����ؿ��Է��ʵ��������������Internet
	Write_W5500_nByte(GAR, Gateway_IP, 4);
	//������������(MASK)ֵ,SUB_MASKΪ4�ֽ�uint8_t����,�Լ�����
	//��������������������
	Write_W5500_nByte(SUBR,Sub_Mask,4);		
	
	//���������ַ,PHY_ADDRΪ6�ֽ�uint8_t����,�Լ�����,����Ψһ��ʶ�����豸�������ֵַ
	//�õ�ֵַ��Ҫ��IEEE���룬����OUI�Ĺ涨��ǰ3���ֽ�Ϊ���̴��룬�������ֽ�Ϊ��Ʒ���
	//����Լ����������ַ��ע���һ���ֽڱ���Ϊż��
	Write_W5500_nByte(SHAR,Phy_Addr,6);		

	//���ñ�����IP��ַ,IP_ADDRΪ4�ֽ�uint8_t����,�Լ�����
	//ע�⣬����IP�����뱾��IP����ͬһ�����������򱾻����޷��ҵ�����
	Write_W5500_nByte(SIPR,IP_Addr,4);		
	
	//���÷��ͻ������ͽ��ջ������Ĵ�С���ο�W5500�����ֲ�
	for(socket_index=0;socket_index<8;socket_index++)
	{
		Write_W5500_SOCK_1Byte(socket_index,Sn_RXBUF_SIZE, 0x02);//Socket Rx memory size=2k
		Write_W5500_SOCK_1Byte(socket_index,Sn_TXBUF_SIZE, 0x02);//Socket Tx mempry size=2k
	}

	//��������ʱ�䣬Ĭ��Ϊ2000(200ms) 
	//ÿһ��λ��ֵΪ100΢��,��ʼ��ʱֵ��Ϊ2000(0x07D0),����200����
	Write_W5500_2Byte(RTR, 0x07d0);

	//�������Դ�����Ĭ��Ϊ8�� 
	//����ط��Ĵ��������趨ֵ,�������ʱ�ж�(��صĶ˿��жϼĴ����е�Sn_IR ��ʱλ(TIMEOUT)�á�1��)
	Write_W5500_1Byte(RCR,8);

	//�����жϣ��ο�W5500�����ֲ�ȷ���Լ���Ҫ���ж�����
	//IMR_CONFLICT��IP��ַ��ͻ�쳣�ж�,IMR_UNREACH��UDPͨ��ʱ����ַ�޷�������쳣�ж�
	//������Socket�¼��жϣ�������Ҫ���
    printf("W5500 IRQn init.\r\n");
	Write_W5500_1Byte(IMR,IM_IR7 | IM_IR6);
	Write_W5500_1Byte(SIMR,S0_IMR);
	Write_W5500_SOCK_1Byte(socket->socket_index, Sn_IMR, IMR_SENDOK | IMR_TIMEOUT | IMR_RECV | IMR_DISCON | IMR_CON);
    printf("W5500 detect Gateway.\r\n");
    Detect_Gateway(socket);	//������ط�����
    printf("W5500 socket init.\r\n");
	Socket_Init(socket);		//ָ��Socket(0~7)��ʼ��,��ʼ���˿�0
    printf("W5500 Inited.\r\n");
    if(socket->socket_Mode==TCP_CLIENT)//TCP�ͻ���ģʽ 
    {
        while(Socket_Connect(socket)!=TRUE)W5500_Delay_ms(5);;
        printf("TCP socket connected.\r\n");
    }
}

/*******************************************************************************
* ������  : Detect_Gateway
* ����    : ������ط�����
* ����    : ��
* ���    : ��
* ����ֵ  : �ɹ�����TRUE(0xFF),ʧ�ܷ���FALSE(0x00)
* ˵��    : ��
*******************************************************************************/
uint8_t Detect_Gateway(Socket* socket)
{
	uint8_t ip_adde[4];
	ip_adde[0]=socket->local_IP[0]+1;
	ip_adde[1]=socket->local_IP[1]+1;
	ip_adde[2]=socket->local_IP[2]+1;
	ip_adde[3]=socket->local_IP[3]+1;

	//������ؼ���ȡ���ص������ַ
	Write_W5500_SOCK_4Byte(socket->socket_index,Sn_DIPR,ip_adde);//��Ŀ�ĵ�ַ�Ĵ���д���뱾��IP��ͬ��IPֵ
	Write_W5500_SOCK_1Byte(socket->socket_index,Sn_MR,MR_TCP);//����socketΪTCPģʽ
	Write_W5500_SOCK_1Byte(socket->socket_index,Sn_CR,OPEN);//��Socket	
	W5500_Delay_ms(5);//��ʱ5ms 	
	
	if(Read_W5500_SOCK_1Byte(socket->socket_index,Sn_SR) != SOCK_INIT)//���socket��ʧ��
	{
		Write_W5500_SOCK_1Byte(socket->socket_index,Sn_CR,CLOSE);//�򿪲��ɹ�,�ر�Socket
		return FALSE;//����FALSE(0x00)
	}

	Write_W5500_SOCK_1Byte(socket->socket_index,Sn_CR,CONNECT);//����SocketΪConnectģʽ						

	do
	{
		u8 j=0;
		j=Read_W5500_SOCK_1Byte(socket->socket_index,Sn_IR);//��ȡSocket0�жϱ�־�Ĵ���
		if(j!=0)
		Write_W5500_SOCK_1Byte(socket->socket_index,Sn_IR,j);
		W5500_Delay_ms(5);//��ʱ5ms 
		if((j&IR_TIMEOUT) == IR_TIMEOUT)
		{
			return FALSE;	
		}
		else if(Read_W5500_SOCK_1Byte(socket->socket_index,Sn_DHAR) != 0xff)
		{
			Write_W5500_SOCK_1Byte(socket->socket_index,Sn_CR,CLOSE);//�ر�Socket
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
void Socket_Init(Socket* socket)
{
	//���÷�Ƭ���ȣ��ο�W5500�����ֲᣬ��ֵ���Բ��޸�	
	Write_W5500_SOCK_2Byte(socket->socket_index, Sn_MSSR, 1460);//����Ƭ�ֽ���=1460(0x5b4)
	//����ָ���˿�
	switch(socket->socket_index)
	{
		case 0:
			//���ö˿�0�Ķ˿ں�
            socket->local_Port = ((uint16_t)rand()%65535);
            printf("W5500 Port: %d\r\n",socket->local_Port);
			Write_W5500_SOCK_2Byte(socket->socket_index, Sn_PORT, socket->local_Port);
			//���ö˿�0Ŀ��(Զ��)�˿ں�
			Write_W5500_SOCK_2Byte(socket->socket_index, Sn_DPORTR,socket->target_Port);
			//���ö˿�0Ŀ��(Զ��)IP��ַ
			Write_W5500_SOCK_4Byte(socket->socket_index, Sn_DIPR, socket->target_IP);			
			
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
* ������  : W5500_Socket_Set
* ����    : W5500�˿ڳ�ʼ������
* ����    : ��
* ���    : ��
* ����ֵ  : ��
* ˵��    : �ֱ�����4���˿�,���ݶ˿ڹ���ģʽ,���˿�����TCP��������TCP�ͻ��˻�UDPģʽ.
*			�Ӷ˿�״̬�ֽ�Socket_State�����ж϶˿ڵĹ������
*******************************************************************************/
bool W5500_Socket_Set_Default(Socket* socket)
{
	if(socket->port_State==0)//�˿�0��ʼ������
	{
		if(socket->socket_Mode==TCP_SERVER)//TCP������ģʽ 
		{
			if(Socket_Listen(socket)==TRUE)
            {
				socket->port_State=S_INIT;
                return true;
            }
		}
		else if(socket->socket_Mode==TCP_CLIENT)//TCP�ͻ���ģʽ 
		{
			if(Socket_Connect(socket)==TRUE)
            {
				socket->port_State=S_INIT;
                return true;
            }
		}
		else//UDPģʽ 
		{
			if(Socket_UDP(socket)==TRUE)
            {
				socket->port_State=S_INIT|S_CONN;
                return true;
            }
		}
        socket->port_State=0;
        return false;
	}
    return true;
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
uint8_t Socket_Connect(Socket* socket)
{
	Write_W5500_SOCK_1Byte(socket->socket_index,Sn_MR,MR_TCP);//����socketΪTCPģʽ
    Write_W5500_SOCK_1Byte(socket->socket_index,Sn_CR,CLOSE);//�򿪲��ɹ�,�ر�Socket
	Write_W5500_SOCK_1Byte(socket->socket_index,Sn_CR,OPEN);//��Socket
	W5500_Delay_ms(5);//��ʱ10ms
	if(Read_W5500_SOCK_1Byte(socket->socket_index,Sn_SR)!=SOCK_INIT)//���socket��ʧ��
	{
		Write_W5500_SOCK_1Byte(socket->socket_index,Sn_CR,CLOSE);//�򿪲��ɹ�,�ر�Socket
		return FALSE;//����FALSE(0x00)
	}
	Write_W5500_SOCK_1Byte(socket->socket_index,Sn_CR,CONNECT);//����SocketΪConnectģʽ
    socket->port_State=S_INIT;
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
uint8_t Socket_Listen(Socket* socket)
{
	Write_W5500_SOCK_1Byte(socket->socket_index,Sn_MR,MR_TCP);//����socketΪTCPģʽ 
	Write_W5500_SOCK_1Byte(socket->socket_index,Sn_CR,OPEN);//��Socket	
	W5500_Delay_ms(5);//��ʱ5ms
	if(Read_W5500_SOCK_1Byte(socket->socket_index,Sn_SR)!=SOCK_INIT)//���socket��ʧ��
	{
		Write_W5500_SOCK_1Byte(socket->socket_index,Sn_CR,CLOSE);//�򿪲��ɹ�,�ر�Socket
		return FALSE;//����FALSE(0x00)
	}	
	Write_W5500_SOCK_1Byte(socket->socket_index,Sn_CR,LISTEN);//����SocketΪ����ģʽ	
	W5500_Delay_ms(5);//��ʱ5ms
	if(Read_W5500_SOCK_1Byte(socket->socket_index,Sn_SR)!=SOCK_LISTEN)//���socket����ʧ��
	{
		Write_W5500_SOCK_1Byte(socket->socket_index,Sn_CR,CLOSE);//���ò��ɹ�,�ر�Socket
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
uint8_t Socket_UDP(Socket* socket)
{
	Write_W5500_SOCK_1Byte(socket->socket_index,Sn_MR,MR_UDP);//����SocketΪUDPģʽ*/
	Write_W5500_SOCK_1Byte(socket->socket_index,Sn_CR,OPEN);//��Socket*/
	W5500_Delay_ms(5);//��ʱ5ms
	if(Read_W5500_SOCK_1Byte(socket->socket_index,Sn_SR)!=SOCK_UDP)//���Socket��ʧ��
	{
		Write_W5500_SOCK_1Byte(socket->socket_index,Sn_CR,CLOSE);//�򿪲��ɹ�,�ر�Socket
		return FALSE;//����FALSE(0x00)
	}
	else
		return TRUE;

	//���������Socket�Ĵ򿪺�UDPģʽ����,������ģʽ��������Ҫ��Զ��������������
	//��ΪSocket����Ҫ��������,�����ڷ�������ǰ����������Ŀ������IP��Ŀ��Socket�Ķ˿ں�
	//���Ŀ������IP��Ŀ��Socket�Ķ˿ں��ǹ̶���,�����й�����û�иı�,��ôҲ��������������
}

/*******************************************************************************
* ������  : W5500_INT_EXTI_IRQHandler
* ����    : W5500�ж����жϷ�����(W5500 INT�����жϷ�����)
* ����    : ��
* ���    : ��
* ����ֵ  : ��
* ˵��    : ��
*******************************************************************************/
void W5500_INT_EXTI_IRQHandler(void)
{
	if(EXTI_GetITStatus(W5500_INT_EXTI_LINE) != RESET)
	{
		EXTI_ClearITPendingBit(W5500_INT_EXTI_LINE);
		W5500_Interrupt=1;
	}
}

/*���ڷ�װ���ṩ����push������FIFO�Ľӿ�
*
*/
void W5500_Push_Socket0_SendDataIntoFIFO(uint8_t *Socket_SendBuff, uint16_t DataSendLength)//���ڷ�װ���ṩ����push��FIFO�Ľӿ�
{
    Socket* socket = &socket_0;
    Uint8PacketQueuePushStreamData(socket->send_Packet_Queue_Handle, Socket_SendBuff, DataSendLength);
    if(socket->packet_Sendding == false)
    {
        W5500_Send_Socket_Data(socket);
    }
}

void Free_W5500_PacketNoedItem(Uint8PacketNode* uint8PacketNodePointer)
{
    if(!uint8PacketNodePointer)return;
    if(uint8PacketNodePointer->packetBlock)
    {
        free(uint8PacketNodePointer->packetBlock);
        uint8PacketNodePointer->packetBlock = NULL;
    }
    free(uint8PacketNodePointer);
}


/*******************************************************************************
* ������  : W5500_Interrupt_Process
* ����    : W5500�жϴ��������
* ����    : ��
* ���    : ��
* ����ֵ  : ��
* ˵��    : ��
*******************************************************************************/
void W5500_Interrupt_Process(void)
{
	uint8_t IRQ_sign;
    Socket* socket;
    while(1)
    {
        W5500_Interrupt=0;//�����жϱ�־
        IRQ_sign = Read_W5500_1Byte(IR);//��ȡ�жϱ�־�Ĵ���
        Write_W5500_1Byte(IR, (IRQ_sign&0xf0));//��д����жϱ�־

        if((IRQ_sign & CONFLICT) == CONFLICT)//IP��ַ��ͻ�쳣����
        {
             //�Լ���Ӵ���
        }

        if((IRQ_sign & UNREACH) == UNREACH)//UDPģʽ�µ�ַ�޷������쳣����
        {
            //�Լ���Ӵ���
        }

        IRQ_sign=Read_W5500_1Byte(SIR);//��ȡ�˿��жϱ�־�Ĵ���	
        if((IRQ_sign & S0_INT) == S0_INT)//Socket0�¼����� 
        {
            socket = &socket_0;
            IRQ_sign=Read_W5500_SOCK_1Byte(socket->socket_index,Sn_IR);//��ȡSocket0�жϱ�־�Ĵ���
            Write_W5500_SOCK_1Byte(socket->socket_index,Sn_IR,IRQ_sign);
            if(IRQ_sign&IR_CON)//��TCPģʽ��,Socket0�ɹ����� 
            {
                socket->port_State|=S_CONN;//��������״̬0x02,�˿�������ӣ�����������������
            }
            if(IRQ_sign&IR_DISCON)//��TCPģʽ��Socket�Ͽ����Ӵ���
            {
                Write_W5500_SOCK_1Byte(socket->socket_index,Sn_CR,CLOSE);//�رն˿�,�ȴ����´����� 
                Socket_Init(socket);		//ָ��Socket(0~7)��ʼ��,��ʼ���˿�0
                socket->port_State=S_BREAK;//��������״̬0x00,�˿�����ʧ��
            }
            if(IRQ_sign&IR_SEND_OK)//Socket0���ݷ������,�����ٴ�����S_tx_process()������������ 
            {
                socket->send_receive_State|=S_TRANSMITOK;//�˿ڷ���һ�����ݰ����
            }
            if(IRQ_sign&IR_RECV)//Socket���յ�����,��������S_rx_process()���� 
            {
                socket->send_receive_State|=S_RECEIVE;//�˿ڽ��յ�һ�����ݰ�
            }
            if(IRQ_sign&IR_TIMEOUT)//Socket���ӻ����ݴ��䳬ʱ���� 
            {
                Write_W5500_SOCK_1Byte(socket->socket_index,Sn_CR,CLOSE);// �رն˿�,�ȴ����´����� 
                socket->port_State=S_BREAK;//��������״̬0x00,�˿�����ʧ��
            }
        }
        if(Read_W5500_1Byte(SIR) == 0) 
            break;
    }
}

bool W5500_Daemon_Process(void)
{
    Socket* socket = &socket_0;
    if(W5500_Socket_Set_Default(socket) == false)//W5500�˿ڳ�ʼ������
    {
        return false;
    }
    if(W5500_Interrupt)//����W5500�ж�		
    {
        W5500_Interrupt_Process();//W5500�жϴ��������
    }
    if(socket->port_State == S_BREAK)
    {
        if(socket->socket_Mode==TCP_CLIENT)//TCP�ͻ���ģʽ 
        {
            while(Socket_Connect(socket)!=TRUE)W5500_Delay_ms(10);
        }
    }
    if((socket->send_receive_State & S_RECEIVE) == S_RECEIVE)//���Socket0���յ�����
    {
        socket->send_receive_State&=~S_RECEIVE;
        W5500_Socket_Receive_Process(socket);//W5500���ղ����ͽ��յ�������
    }
    if(socket->port_State == (S_INIT|S_CONN))
    {
        if((socket->send_receive_State & S_TRANSMITOK) == S_TRANSMITOK)
        {
            socket->send_receive_State&=~S_TRANSMITOK;
            if(socket->send_Packet_Queue_Handle->head)
            {
                W5500_Send_Socket_Data(socket);
            }
            else
            {
                socket->packet_Sendding = false;
            }
        }
    }
 
    return true;
}

/*******************************************************************************
* ������  : W5500_Socket_Receive_Process
* ����    : W5500���ղ�������յ�������
* ����    : s:�˿ں�
* ���    : ��
* ����ֵ  : ��
* ˵��    : �������ȵ���S_rx_process()��W5500�Ķ˿ڽ������ݻ�������ȡ����,
*			Ȼ�󽫶�ȡ�����ݴ�Rx_Buffer������Temp_Buffer���������д���
*			������ϣ������ݴ�Temp_Buffer������Tx_Buffer������������S_tx_process()
*			�������ݡ�
*******************************************************************************/
void W5500_Socket_Receive_Process(Socket* socket)
{
	uint16_t size;
    uint8_t* Tx_Buffer;
	size=Read_SOCK_Data_Buffer(socket, Rx_Buffer);
    Tx_Buffer = (uint8_t*)malloc(sizeof(uint8_t)*size);
	memcpy(Tx_Buffer, Rx_Buffer, size);
    Uint8PacketQueuePushStreamData(socket->send_Packet_Queue_Handle, Tx_Buffer, size);
	W5500_Send_Socket_Data(socket);
    free(Tx_Buffer);
}

