#ifndef _HAL_INF_RECEIVER_H_
#define _HAL_INF_RECEIVER_H_


#include "common.h"


#define INF_RECEIVER_PIN GPIO_PIN_2
#define INF_RECEIVER_GPIO_TYPE GPIOD


void Pro_Inf_Receiver_Pin_Eint(char level);


/**
*��ȡ�������ͷ����
*������,����1,������Ч
*/
uint8_t Check_Inf_Reciver_Data(uint8_t *addr,uint8_t * cmd);

uint8_t Test_Inf_Receiver(void);

/**�����Ƿ񱻴���*/
uint8_t Test_Ligh_Garde_Trigger(void);



/**
*���Ӻ�����յĶ�ʱʱ��
*/
void Add_Inf_Receiver_Time(void);

#endif

