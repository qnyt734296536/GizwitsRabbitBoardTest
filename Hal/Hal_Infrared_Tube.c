#include "Hal_Infrared_Tube.h"

static uint8_t gInfTubeState = 1;
static uint8_t  gTrigger = 0;

/**
*���ú�������״̬
*�͵�ƽ�ڵ����ߵ�ƽû�ڵ�
*/
void Set_InfTube_State(char level)
{
	gInfTubeState = level;
	if(gInfTubeState == 0)
	{
		gTrigger = 1;
	}
	printf("Set infTube levle is %d, gInfState is %d\r\n",level,gInfTubeState);
}


/**
*��ȡ����Թ�״̬
*0��ʾû�ڵ�,1��ʾ�ڵ�
*/
uint8_t Get_InfTube_State(void)
{
	uint8_t state=0;
	state = gInfTubeState==1?0:1;
	return state;
}



/**
*�����Թ��Ƿ񱻴���
*/
uint8_t Get_InfTube_State_Trigger(void)
{
	uint8_t state = 0;
	
	if(gTrigger == 1)
	{
		gTrigger = 0;
		state = 1;
	}
	return state;
}


