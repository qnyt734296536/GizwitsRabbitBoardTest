#ifndef _HAL_BUZZER_H_
#define _HAL_BUZZER_H_

#include "common.h"


/**
*��ʼ��������
*/
void Init_Buzzer(void);

/**
*���Ʒ������켸��
*/
void Control_Buzzer(char num, char ms);

void Control_Buzzer_State(uint8_t state);


void Test_Buzzer(void);

#endif

