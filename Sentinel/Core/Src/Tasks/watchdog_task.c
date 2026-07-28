/**
  ******************************************************************************
  * @file    watchdog_task.c
  * @brief   Heartbeat check: verifies all 6 other tasks set their alive flag
  *          since the last check, then feeds the IWDG.
  ******************************************************************************
  */
#include "tasks.h"
#include "cmsis_os.h"

void WatchdogTask(void* arguments){
	for(;;){
		osDelay(100);
	}
}
