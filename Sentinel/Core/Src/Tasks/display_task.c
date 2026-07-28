/**
  ******************************************************************************
  * @file    display_task.c
  * @brief   Updates the 1602 LCD with the current system status.
  ******************************************************************************
  */
#include "tasks.h"
#include "cmsis_os.h"

void DisplayTask(void* arguments){
	for(;;){
		osDelay(100);
	}
}
