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
osThreadId_t defaultTaskHandle;
osThreadId_t SpeedRegTaskHandle;
osThreadId_t PositionRegTaskHandle;
osThreadId_t TimerGameTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
   
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartSpeedRegTask(void *argument);
void StartPositionRegTask(void *argument);
void StartTimerGameTask(void *argument);

extern void MX_LWIP_Init(void);
extern void MX_USB_DEVICE_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */
osKernelInitialize();

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
  const osThreadAttr_t defaultTask_attributes = {
    .name = "defaultTask",
    .priority = (osPriority_t) osPriorityNormal,
    .stack_size = 128
  };
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* definition and creation of SpeedRegTask */
  const osThreadAttr_t SpeedRegTask_attributes = {
    .name = "SpeedRegTask",
    .priority = (osPriority_t) osPriorityRealtime,
    .stack_size = 128
  };
  SpeedRegTaskHandle = osThreadNew(StartSpeedRegTask, NULL, &SpeedRegTask_attributes);

  /* definition and creation of PositionRegTask */
  const osThreadAttr_t PositionRegTask_attributes = {
    .name = "PositionRegTask",
    .priority = (osPriority_t) osPriorityRealtime1,
    .stack_size = 128
  };
  PositionRegTaskHandle = osThreadNew(StartPositionRegTask, NULL, &PositionRegTask_attributes);

  /* definition and creation of TimerGameTask */
  const osThreadAttr_t TimerGameTask_attributes = {
    .name = "TimerGameTask",
    .priority = (osPriority_t) osPriorityNormal,
    .stack_size = 128
  };
  TimerGameTaskHandle = osThreadNew(StartTimerGameTask, NULL, &TimerGameTask_attributes);

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
void StartDefaultTask(void *argument)
{
  /* init code for LWIP */
  //MX_LWIP_Init();

  /* init code for USB_DEVICE */
  //MX_USB_DEVICE_Init();
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartSpeedRegTask */
/**
* @brief Function implementing the SpeedRegTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartSpeedRegTask */
void StartSpeedRegTask(void *argument)
{
  /* USER CODE BEGIN StartSpeedRegTask */
  /* Infinite loop */
  for(;;)
  {
	F_GPIO_ToogleLedRed();
    osDelay(1000);
  }
  /* USER CODE END StartSpeedRegTask */
}

/* USER CODE BEGIN Header_StartPositionRegTask */
/**
* @brief Function implementing the PositionRegTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartPositionRegTask */
void StartPositionRegTask(void *argument)
{
  /* USER CODE BEGIN StartPositionRegTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(20);
  }
  /* USER CODE END StartPositionRegTask */
}

/* USER CODE BEGIN Header_StartTimerGameTask */
/**
* @brief Function implementing the TimerGameTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTimerGameTask */
void StartTimerGameTask(void *argument)
{
  /* USER CODE BEGIN StartTimerGameTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(500);
  }
  /* USER CODE END StartTimerGameTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
