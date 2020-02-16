/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     
#include "F_Odometry.h"
#include "F_LCD.h"
#include "usart.h"

#include "A_com.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId MotionRegTaskHandle;
osThreadId OdometryRegTaskHandle;
osThreadId SensorsTaskHandle;
osThreadId DebugTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
   
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void StartMotionRegTask(void const * argument);
void StartOdometryRegTask(void const * argument);
void StartSensorsTask(void const * argument);
void StartDebugTask(void const * argument);

extern void MX_LWIP_Init(void);
extern void MX_USB_DEVICE_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];
  
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}                   
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of MotionRegTask */
  osThreadDef(MotionRegTask, StartMotionRegTask, osPriorityHigh, 0, 128);
  MotionRegTaskHandle = osThreadCreate(osThread(MotionRegTask), NULL);

  /* definition and creation of OdometryRegTask */
  osThreadDef(OdometryRegTask, StartOdometryRegTask, osPriorityRealtime, 0, 128);
  OdometryRegTaskHandle = osThreadCreate(osThread(OdometryRegTask), NULL);

  /* definition and creation of SensorsTask */
  osThreadDef(SensorsTask, StartSensorsTask, osPriorityNormal, 0, 128);
  SensorsTaskHandle = osThreadCreate(osThread(SensorsTask), NULL);

  /* definition and creation of DebugTask */
  osThreadDef(DebugTask, StartDebugTask, osPriorityNormal, 0, 128);
  DebugTaskHandle = osThreadCreate(osThread(DebugTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* init code for LWIP */
  MX_LWIP_Init();

  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN StartDefaultTask */
  tcpecho_init();

  /* Infinite loop */
  for(;;)
  {
	  F_GPIO_ToogleLedRed();
    osDelay(500);
    HAL_Delay(2);
    F_GPIO_SetLed5(FALSE);	// Flaf OFF
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartMotionRegTask */
/**
* @brief Function implementing the MotionRegTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartMotionRegTask */
void StartMotionRegTask(void const * argument)
{
  /* USER CODE BEGIN StartMotionRegTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(50);
    F_GPIO_SetLed2(TRUE);	// Flag ON
    HAL_Delay(1);
    F_GPIO_SetLed2(FALSE);	// Flaf OFF
  }
  /* USER CODE END StartMotionRegTask */
}

/* USER CODE BEGIN Header_StartOdometryRegTask */
/**
* @brief Function implementing the OdometryRegTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartOdometryRegTask */
void StartOdometryRegTask(void const * argument)
{
  /* USER CODE BEGIN StartOdometryRegTask */
	F_Odometry_RegTask_Handler(argument);
  /* Infinite loop */
  for(;;)
  {
    osDelay(10);
  }
  /* USER CODE END StartOdometryRegTask */
}

/* USER CODE BEGIN Header_StartSensorsTask */
/**
* @brief Function implementing the SensorsTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartSensorsTask */
void StartSensorsTask(void const * argument)
{
  /* USER CODE BEGIN StartSensorsTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(200);
    F_GPIO_SetLed3(TRUE);	// Flag ON
    HAL_Delay(30);
    F_GPIO_SetLed3(FALSE);
  }
  /* USER CODE END StartSensorsTask */
}

/* USER CODE BEGIN Header_StartDebugTask */
/**
* @brief Function implementing the DebugTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartDebugTask */
void StartDebugTask(void const * argument)
{
  /* USER CODE BEGIN StartDebugTask */
	//F_UART_DebugTask_Handler(argument);
	//F_LCD_DebugTask_Handler(argument);
  /* Infinite loop */
  for(;;)
  {
    osDelay(400);
//    F_GPIO_SetLed4(TRUE);	// Flag ON
//    HAL_Delay(30);
//    F_GPIO_SetLed4(FALSE);
  }
  /* USER CODE END StartDebugTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
