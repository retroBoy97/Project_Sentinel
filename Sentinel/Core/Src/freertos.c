/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "tasks.h"

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

const osThreadAttr_t sensorPollTask_attributes = {
		.name = "SensorPollTask",
		.stack_size = 128 * 4,
		.priority = (osPriority_t) osPriorityNormal,
};

const osThreadAttr_t localSensorTask_attributes = {
		.name = "LocalSensorTask",
		.stack_size = 128 * 4,
		.priority = (osPriority_t) osPriorityNormal,
};

const osThreadAttr_t safetyTask_attributes = {
		.name = "SafetyTask",
		.stack_size = 128 * 4,
		.priority = (osPriority_t) osPriorityHigh,
};

const osThreadAttr_t actuatorTask_attributes = {
		.name = "ActuatorTask",
		.stack_size = 128 * 4,
		.priority = (osPriority_t) osPriorityNormal,
};

const osThreadAttr_t displayTask_attributes = {
		.name = "DisplayTask",
		.stack_size = 128 * 4,
		.priority = (osPriority_t) osPriorityLow,
};

const osThreadAttr_t commsTask_attributes = {
		.name = "CommsTask",
		.stack_size = 128 * 4,
		.priority = (osPriority_t) osPriorityNormal,
};

const osThreadAttr_t watchdogTask_attributes = {
		.name = "WatchdogTask",
		.stack_size = 128 * 4,
		.priority = (osPriority_t) osPriorityHigh,
};

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

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
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  osThreadId_t actuatorTaskHandle = osThreadNew(ActuatorTask, NULL, &actuatorTask_attributes);
  osThreadId_t commsTaskHandle = osThreadNew(CommsTask, NULL, &commsTask_attributes);
  osThreadId_t displayTaskHandle = osThreadNew(DisplayTask, NULL, &displayTask_attributes);
  osThreadId_t localSensorTaskHandle = osThreadNew(LocalSensorTask, NULL, &localSensorTask_attributes);
  osThreadId_t safetyTaskHandle = osThreadNew(SafetyTask, NULL, &safetyTask_attributes);
  osThreadId_t sensorPollTaskHandle = osThreadNew(SensorPollTask, NULL, &sensorPollTask_attributes);
  osThreadId_t watchdogTaskHandle = osThreadNew(WatchdogTask, NULL, &watchdogTask_attributes);
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

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
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

