/**
************************************************************
* @file         gizwits_product.c
* @brief        Gizwits control protocol processing, and platform-related       hardware initialization 
* @author       Gizwits
* @date         2017-07-19
* @version      V03030000
* @copyright    Gizwits
* 
* @note         机智云.只为智能硬件而生
*               Gizwits Smart Cloud  for Smart Products
*               链接|增值ֵ|开放|中立|安全|自有|自由|生态
*               www.gizwits.com
*
***********************************************************/

#include <stdio.h>
#include <string.h>
#include "hal_key.h"
#include "gizwits_product.h"
#include "common.h"

#include "delay.h"
#include "Hal_Led.h"
#include "Hal_key.h"
#include "Hal_Buzzer.h"
#include "Hal_Relay.h"
#include "Hal_Light_Resistor.h"
#include "Hal_Dcout.h"
#include "Hal_Rgb_My9291.h"
#include "Hal_Inf_Receiver.h"
#include "Hal_Temp_Hum.h"
#include "Hal_pyr.h"
#include "Hal_Infrared_tube.h"

static uint32_t timerMsCount;
uint8_t aRxBuffer;

/** User area the current device state structure*/
dataPoint_t currentDataPoint;
extern keysTypedef_t keys;

extern TIM_HandleTypeDef htim2;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern TIM_HandleTypeDef htim3;

/**@} */
/**@name Gizwits User Interface
* @{
*/

/**
* @brief Event handling interface

* Description:

* 1. Users can customize the changes in WiFi module status

* 2. Users can add data points in the function of event processing logic, such as calling the relevant hardware peripherals operating interface

* @param [in] info: event queue
* @param [in] data: protocol data
* @param [in] len: protocol data length
* @return NULL
* @ref gizwits_protocol.h
*/
int8_t gizwitsEventProcess(eventInfo_t *info, uint8_t *gizdata, uint32_t len)
{
  uint8_t i = 0;
  dataPoint_t *dataPointPtr = (dataPoint_t *)gizdata;
  moduleStatusInfo_t *wifiData = (moduleStatusInfo_t *)gizdata;
  protocolTime_t *ptime = (protocolTime_t *)gizdata;
  uint8_t rgbflag=0;
#if MODULE_TYPE
  gprsInfo_t *gprsInfoData = (gprsInfo_t *)gizdata;
#else
  moduleInfo_t *ptModuleInfo = (moduleInfo_t *)gizdata;
#endif

  if((NULL == info) || (NULL == gizdata))
  {
    return -1;
  }

  for(i=0; i<info->num; i++)
  {
    switch(info->event[i])
    {
      case EVENT_LED1:
        currentDataPoint.valueLED1 = dataPointPtr->valueLED1;
        GIZWITS_LOG("Evt: EVENT_LED1 %d \n", currentDataPoint.valueLED1);
        if(0x01 == currentDataPoint.valueLED1)
        {
          //user handle
					Control_Led(LED1,1);
        }
        else
        {
          //user handle  
					Control_Led(LED1,0);					
        }
        break;
      case EVENT_LED2:
        currentDataPoint.valueLED2 = dataPointPtr->valueLED2;
        GIZWITS_LOG("Evt: EVENT_LED2 %d \n", currentDataPoint.valueLED2);
        if(0x01 == currentDataPoint.valueLED2)
        {
          //user handle
					Control_Led(LED2,1);
        }
        else
        {
          //user handle    
					Control_Led(LED2,0);
        }
        break;
      case EVENT_LED3:
        currentDataPoint.valueLED3 = dataPointPtr->valueLED3;
        GIZWITS_LOG("Evt: EVENT_LED3 %d \n", currentDataPoint.valueLED3);
        if(0x01 == currentDataPoint.valueLED3)
        {
          //user handle
					Control_Led(LED3,1);
        }
        else
        {
          //user handle    
					Control_Led(LED3,0);
        }
        break;
      case EVENT_LED4:
        currentDataPoint.valueLED4 = dataPointPtr->valueLED4;
        GIZWITS_LOG("Evt: EVENT_LED4 %d \n", currentDataPoint.valueLED4);
        if(0x01 == currentDataPoint.valueLED4)
        {
          //user handle
					Control_Led(LED4,1);
        }
        else
        {
          //user handle    
					Control_Led(LED4,0);
        }
        break;
      case EVENT_buzzer:
        currentDataPoint.valuebuzzer = dataPointPtr->valuebuzzer;
        GIZWITS_LOG("Evt: EVENT_buzzer %d \n", currentDataPoint.valuebuzzer);
        if(0x01 == currentDataPoint.valuebuzzer)
        {
          //user handle
					Control_Buzzer_State(1);
        }
        else
        {
          //user handle  
					Control_Buzzer_State(0);
        }
        break;
      case EVENT_Relay:
        currentDataPoint.valueRelay = dataPointPtr->valueRelay;
        GIZWITS_LOG("Evt: EVENT_Relay %d \n", currentDataPoint.valueRelay);
        if(0x01 == currentDataPoint.valueRelay)
        {
          //user handle
					Control_Relay(1);
        }
        else
        {
          //user handle
					Control_Relay(0);
        }
        break;
      case EVENT_DCout:
        currentDataPoint.valueDCout = dataPointPtr->valueDCout;
        GIZWITS_LOG("Evt: EVENT_DCout %d \n", currentDataPoint.valueDCout);
        if(0x01 == currentDataPoint.valueDCout)
        {
          //user handle
					Set_Dcout_State(1);
        }
        else
        {
          //user handle
					Set_Dcout_State(0);
        }
        break;


      case EVENT_R:
        currentDataPoint.valueR = dataPointPtr->valueR;
        GIZWITS_LOG("Evt:EVENT_R %d\n",currentDataPoint.valueR);
        //user handle
				rgbflag=1;
        break;
      case EVENT_G:
        currentDataPoint.valueG = dataPointPtr->valueG;
        GIZWITS_LOG("Evt:EVENT_G %d\n",currentDataPoint.valueG);
        //user handle
				rgbflag=1;
        break;
      case EVENT_B:
        currentDataPoint.valueB = dataPointPtr->valueB;
        GIZWITS_LOG("Evt:EVENT_B %d\n",currentDataPoint.valueB);
        //user handle
				rgbflag=1;
        break;


      case WIFI_SOFTAP:
        break;
      case WIFI_AIRLINK:
        break;
      case WIFI_STATION:
        break;
      case WIFI_CON_ROUTER:
 
        break;
      case WIFI_DISCON_ROUTER:
 
        break;
      case WIFI_CON_M2M:
 
        break;
      case WIFI_DISCON_M2M:
        break;
      case WIFI_RSSI:
        GIZWITS_LOG("RSSI %d\n", wifiData->rssi);
        break;
      case TRANSPARENT_DATA:
        GIZWITS_LOG("TRANSPARENT_DATA \n");
        //user handle , Fetch data from [data] , size is [len]
        break;
      case WIFI_NTP:
        GIZWITS_LOG("WIFI_NTP : [%d-%d-%d %02d:%02d:%02d][%d] \n",ptime->year,ptime->month,ptime->day,ptime->hour,ptime->minute,ptime->second,ptime->ntp);
        break;
      case MODULE_INFO:
            GIZWITS_LOG("MODULE INFO ...\n");
      #if MODULE_TYPE
            GIZWITS_LOG("GPRS MODULE ...\n");
            //Format By gprsInfo_t
      #else
            GIZWITS_LOG("WIF MODULE ...\n");
            //Format By moduleInfo_t
            GIZWITS_LOG("moduleType : [%d] \n",ptModuleInfo->moduleType);
      #endif
    break;
      default:
        break;
    }
  }

	if(rgbflag==1)
	{
		My9291_Send_Duty(currentDataPoint.valueR, currentDataPoint.valueG, currentDataPoint.valueB,0);
	}
  return 0;
}

/**
* User data acquisition

* Here users need to achieve in addition to data points other than the collection of data collection, can be self-defined acquisition frequency and design data filtering algorithm

* @param none
* @return none
*/
void userHandle(void)
{
 /*
    currentDataPoint.valueInfTube = ;//Add Sensor Data Collection
    currentDataPoint.valueInfPyr = ;//Add Sensor Data Collection
    currentDataPoint.valuetemperature = ;//Add Sensor Data Collection
    currentDataPoint.valuehumidity = ;//Add Sensor Data Collection
    currentDataPoint.valueInfReciverAdd = ;//Add Sensor Data Collection
    currentDataPoint.valueInfReciverCmd = ;//Add Sensor Data Collection
    currentDataPoint.valueLightStr = ;//Add Sensor Data Collection

    */
	
	uint8_t temperature=0, humidity =0;
	uint8_t state=0,infaddr=0,infcmd=0;
	static uint32_t oldTime = 0;
	if(gizGetTimerCount() -oldTime > 3000)
	{
		//温度
		dht11ReadData(&temperature, &humidity);
		currentDataPoint.valuetemperature =temperature;
		currentDataPoint.valuehumidity =humidity ;
		GIZWITS_LOG("temperature is %d,humidity is %d\r\n",temperature, humidity);
		oldTime = gizGetTimerCount();
	}
	
	currentDataPoint.valueInfTube = Get_InfTube_State();//0表示没遮挡
	currentDataPoint.valueInfPyr = Get_Pyr_State();//热释电
	currentDataPoint.valueLightStr =Get_Light_Str_Adc();//光敏电阻
	
	state = Check_Inf_Reciver_Data(&infaddr,&infcmd);
	if(state == 1)
	{
		currentDataPoint.valueInfReciverAdd = infaddr;//Add Sensor Data Collection
    currentDataPoint.valueInfReciverCmd = infcmd;//Add Sensor Data Collection
	}
    
}

/**
* Data point initialization function

* In the function to complete the initial user-related data
* @param none
* @return none
* @note The developer can add a data point state initialization value within this function
*/
void userInit(void)
{
    memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));
    
    /** Warning !!! DataPoint Variables Init , Must Within The Data Range **/ 
    /*
      currentDataPoint.valueLED1 = ;
      currentDataPoint.valueLED2 = ;
      currentDataPoint.valueLED3 = ;
      currentDataPoint.valueLED4 = ;
      currentDataPoint.valuebuzzer = ;
      currentDataPoint.valueRelay = ;
      currentDataPoint.valueDCout = ;
      currentDataPoint.valueR = ;
      currentDataPoint.valueG = ;
      currentDataPoint.valueB = ;
      currentDataPoint.valueInfTube = ;
      currentDataPoint.valueInfPyr = ;
      currentDataPoint.valuetemperature = ;
      currentDataPoint.valuehumidity = ;
      currentDataPoint.valueInfReciverAdd = ;
      currentDataPoint.valueInfReciverCmd = ;
      currentDataPoint.valueLightStr = ;
    */

}


/**
* @brief Millisecond timing maintenance function, milliseconds increment, overflow to zero

* @param none
* @return none
*/
void gizTimerMs(void)
{
    timerMsCount++;
}

/**
* @brief Read millisecond count

* @param none
* @return millisecond count
*/
uint32_t gizGetTimerCount(void)
{
    return timerMsCount;
}

/**
* @brief MCU reset function

* @param none
* @return none
*/
void mcuRestart(void)
{
    __set_FAULTMASK(1);
    HAL_NVIC_SystemReset();
}

/**@} */

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART1 and Loop until the end of transmission */
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
 
  return ch;
}

/**
  * @brief  Period elapsed callback in non blocking mode 
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim==&htim2)
	{
			keyHandle((keysTypedef_t *)&keys);
			gizTimerMs();
	}
	if(htim==&htim3)
	{
		Add_Inf_Receiver_Time();
	}
}

/**
* @brief Timer TIM3 init function

* @param none
* @return none
*/
void timerInit(void)
{
	HAL_TIM_Base_Start_IT(&htim2);
}

/**
  * @brief  This function handles USART IDLE interrupt.
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef*UartHandle)  
{  
    if(UartHandle->Instance == USART2)  
    {  
				gizPutData((uint8_t *)&aRxBuffer, 1);

        HAL_UART_Receive_IT(&huart2, (uint8_t *)&aRxBuffer, 1);//开启下一次接收中断  
    }  
}  

/**
* @brief USART init function

* Serial communication between WiFi modules and device MCU
* @param none
* @return none
*/
void uartInit(void)
{
	HAL_UART_Receive_IT(&huart2, (uint8_t *)&aRxBuffer, 1);//开启下一次接收中断  
}

/**
* @brief Serial port write operation, send data to WiFi module
*
* @param buf      : buf address
* @param len      : buf length
*
* @return : Return effective data length;-1，return failure
*/
int32_t uartWrite(uint8_t *buf, uint32_t len)
{
		uint8_t crc[1] = {0x55};
    uint32_t i = 0;
	
    if(NULL == buf)
    {
        return -1;
    }

    for(i=0; i<len; i++)
    {
        HAL_UART_Transmit_IT(&huart2, (uint8_t *)&buf[i], 1);
				while (huart2.gState != HAL_UART_STATE_READY);//Loop until the end of transmission

        if(i >=2 && buf[i] == 0xFF)
        {
						HAL_UART_Transmit_IT(&huart2, (uint8_t *)&crc, 1);
						while (huart2.gState != HAL_UART_STATE_READY);//Loop until the end of transmission
        }
    }

#ifdef PROTOCOL_DEBUG
    GIZWITS_LOG("MCU2WiFi[%4d:%4d]: ", gizGetTimerCount(), len);
    for(i=0; i<len; i++)
    {
        GIZWITS_LOG("%02x ", buf[i]);

        if(i >=2 && buf[i] == 0xFF)
        {
            GIZWITS_LOG("%02x ", 0x55);
        }
    }
    GIZWITS_LOG("\n");
#endif
		
		return len;
}  
