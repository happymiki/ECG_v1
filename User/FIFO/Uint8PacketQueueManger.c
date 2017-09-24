#include "Uint8PacketQueueManger.h"


/*����һ�����Ͱ��Ķ���
*����һ���������ָ��
*/
Uint8PacketQueue* CreatUint8PacketQueue(Uint8PacketNode* (*CreatUint8PacketNode)(uint8_t* packet, void* packetBlock))
{
    Uint8PacketQueue*  Uint8PacketQueueHandle = NULL;
    Uint8PacketQueueHandle = (Uint8PacketQueue*)malloc(sizeof(Uint8PacketQueue));//��Ϊȫ�ֵĹ�����ж���ֻ����һ�Σ����ͷ�
    if(Uint8PacketQueueHandle == NULL)
    {
        return NULL;
    }
    Uint8PacketQueueHandle->head = NULL;
    Uint8PacketQueueHandle->last = NULL;
    Uint8PacketQueueHandle->CreatUint8PacketNode = CreatUint8PacketNode;
    return Uint8PacketQueueHandle;
}



/*��һ�����Ͱ��ڵ�ѹ���������
*Uint8PacketQueueHandle:Ҫѹ��Ĺ����ָ��
*uint8PacketNodePointer:���Ͱ��ڵ�
*/
void Uint8PacketQueuePush(Uint8PacketQueue* Uint8PacketQueueHandle,Uint8PacketNode* uint8PacketNodePointer)
{
    if(!uint8PacketNodePointer || !Uint8PacketQueueHandle)return;
    if(Uint8PacketQueueHandle->head == NULL)
    {
        Uint8PacketQueueHandle->head = uint8PacketNodePointer;
    }
    else
    {
        Uint8PacketQueueHandle->last->next = uint8PacketNodePointer;
    }
    uint8PacketNodePointer->next = NULL;
    Uint8PacketQueueHandle->last = uint8PacketNodePointer;
}
/*ͨ�������ݻ������ݿ鴴��һ�����ڵ㣬���ڼ�����еȲ���
*packet:���ݰ�
*packetBlock:���ݿ�
*�����½��İ��ڵ�ָ��
*
*/
Uint8PacketNode* CreatUint8PacketNode(uint8_t* packet, void* packetBlock)
{
    Uint8PacketNode* uint8PacketNodePointer;
    
    uint8PacketNodePointer = (Uint8PacketNode*)malloc(sizeof(Uint8PacketNode));
    if(!uint8PacketNodePointer)return NULL;
    
    uint8PacketNodePointer->packet = packet;
    uint8PacketNodePointer->packetLength = 0;
    uint8PacketNodePointer->packetBlock = packetBlock;
    uint8PacketNodePointer->next = NULL;
    uint8PacketNodePointer->index = 0;
    uint8PacketNodePointer->resendCount = 0;
    uint8PacketNodePointer->resendTime = 0;
    return uint8PacketNodePointer;
}

Uint8PacketNode* SetUint8PacketNodeLength(Uint8PacketNode* uint8PacketNodePointer,uint16_t packetLength)
{
    if(!uint8PacketNodePointer)return NULL;
    uint8PacketNodePointer->packetLength = packetLength;
    return uint8PacketNodePointer;
}

/*��һ�����ݰ��ṹ��ڵ�ѹ���������
*Uint8PacketQueueHandle:Ҫѹ��Ĺ����ָ��
*packetBlock:���ݰ�ָ��
*
*
*/
void Uint8PacketQueuePushBlock(Uint8PacketQueue* Uint8PacketQueueHandle,void* packetBlock)
{
    Uint8PacketNode* uint8PacketNodePointer;
    if(!Uint8PacketQueueHandle || !packetBlock)return;
    uint8PacketNodePointer = Uint8PacketQueueHandle->CreatUint8PacketNode(NULL, packetBlock);
    Uint8PacketQueuePush(Uint8PacketQueueHandle, uint8PacketNodePointer);
}

/*��һ�����ݰ��ڵ�ѹ���������
*Uint8PacketQueueHandle:Ҫѹ��Ĺ����ָ��
*packet:���ݰ�ָ��
*
*
*/
void Uint8PacketQueuePushStreamData(Uint8PacketQueue* Uint8PacketQueueHandle,uint8_t* packet,uint16_t packetLength)
{
    Uint8PacketNode* uint8PacketNodePointer;
    if(!Uint8PacketQueueHandle || !packet)return;
    uint8PacketNodePointer = Uint8PacketQueueHandle->CreatUint8PacketNode(packet, NULL);
    if(!uint8PacketNodePointer)return;
    SetUint8PacketNodeLength(uint8PacketNodePointer, packetLength);
    Uint8PacketQueuePush(Uint8PacketQueueHandle, uint8PacketNodePointer);
}

/*�ӷ��Ͱ������е���һ���ڵ�
*Uint8PacketQueueHandle:Ҫ�����Ĺ����ָ�롣
*����ֵΪ�ڵ�ָ�롣
*
*
*/
Uint8PacketNode* Uint8PacketQueuePop(Uint8PacketQueue* Uint8PacketQueueHandle)
{
    Uint8PacketNode* uint8PacketNodePointer;
    if(Uint8PacketQueueHandle->head == NULL)
    {
        return NULL;
    }
    uint8PacketNodePointer = Uint8PacketQueueHandle->head;
    Uint8PacketQueueHandle->head = Uint8PacketQueueHandle->head->next;
    uint8PacketNodePointer->next = NULL;
    return uint8PacketNodePointer;  
}


/*�ͷ�һ�����ڵ�Ŀռ�
*uint8PacketNodePointer��Ҫ�ͷŵ���ڵ�ָ��
*
*/
void FreePacketNoedItem(Uint8PacketNode* uint8PacketNodePointer)
{
    if(!uint8PacketNodePointer)return;
    if(uint8PacketNodePointer->packet)
    {
        free(uint8PacketNodePointer->packet);
        uint8PacketNodePointer->packet = NULL;
    }
    if(uint8PacketNodePointer->packetBlock)
    {
        free(uint8PacketNodePointer->packetBlock);
        uint8PacketNodePointer->packetBlock = NULL;
    }
    free(uint8PacketNodePointer);
}


/*ɾ�������е�һ��
*PacketQueueHandle��������ָ��
*uint8PacketNodePointer��Ҫɾ������ڵ�ָ��
*uint8PacketNodePreviousPointer��Ҫɾ�����ǰһ��Ľڵ�ָ��
*/
void DeletPacketQueueCurrentItem(Uint8PacketQueue* PacketQueueHandle, Uint8PacketNode** uint8PacketNodePointer, Uint8PacketNode** uint8PacketNodePreviousPointer)
{
    if(!PacketQueueHandle || !*uint8PacketNodePointer)return;
    if(!*uint8PacketNodePreviousPointer)         //��һ�����  �൱��Ϊͷ�ڵ�
    {
        *uint8PacketNodePreviousPointer = *uint8PacketNodePointer;
        *uint8PacketNodePointer = (*uint8PacketNodePointer)->next;
        PacketQueueHandle->FreePacketNoedItem(*uint8PacketNodePreviousPointer);      
        *uint8PacketNodePreviousPointer = NULL;
        PacketQueueHandle->head = *uint8PacketNodePointer;
    }
    else
    {
        (*uint8PacketNodePreviousPointer)->next = (*uint8PacketNodePointer)->next;
        PacketQueueHandle->FreePacketNoedItem(*uint8PacketNodePreviousPointer);        
        *uint8PacketNodePointer = (*uint8PacketNodePreviousPointer)->next;
        if(!*uint8PacketNodePointer)            //��һ�����  �൱��Ϊβ�ڵ�
        {
            PacketQueueHandle->last = *uint8PacketNodePreviousPointer;
        }
    }
}



/*ͨ�������ж϶�����ÿһ���Ƿ�Ҫɾ��
*PacketQueueHandle��������ָ��
*PacketQueueItemCondition��һ���ж���������ָ�룬����һ���ڵ㣬���ж��Ƿ���Ҫɾ������������򷵻�True��
*
*/
void DeletPacketQueueConditionalItem(Uint8PacketQueue* PacketQueueHandle, bool (*PacketQueueItemCondition)(Uint8PacketNode* uint8PacketNodePointer))
{
    Uint8PacketNode* uint8PacketNodePointer = NULL;
    Uint8PacketNode* uint8PacketNodePreviousPointer = NULL;
    if(!PacketQueueHandle || !PacketQueueItemCondition)return;
    uint8PacketNodePointer = PacketQueueHandle->head;
    
    while(uint8PacketNodePointer)
    {
        if(PacketQueueItemCondition(uint8PacketNodePointer))
        {
            DeletPacketQueueCurrentItem(PacketQueueHandle, &uint8PacketNodePointer, &uint8PacketNodePreviousPointer);//��ΪҪ���ⲿ��ָ�����Ҳ����Ӱ�죬��˴˴�����Ϊָ��ָ���ָ�롣
            continue;
        }
        uint8PacketNodePreviousPointer = uint8PacketNodePointer;
        uint8PacketNodePointer = uint8PacketNodePointer->next;
    }
}





