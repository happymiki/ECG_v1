#include "CommunicationConfig.h"



//ʹ�ø��ֵ�ַ���籾�ء��ػ����㲥�����ص�ַ
//�ն˵�ַΪ1H���ڵ��ַӦ���䲻ͬ
#ifdef __TERMINAL_ON__
    const uint16_t Protocol_LocalhostAddress = 0x0003;
#else
    #ifdef __TERMINAL_OFF__
    const uint16_t Protocol_LocalhostAddress = 0x0004;
    #endif
#endif

const uint16_t Protocol_LoopbackAddress = 0x0000;
const uint16_t Protocol_BroadcastAddress = 0xFFFF;
const uint16_t Protocol_GatewayAddress = 0x0001;








