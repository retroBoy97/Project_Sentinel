/**
  ******************************************************************************
  * @file    local_sensor_task.c
  * @brief   Reads RFID and keypad directly and posts raw values to a queue.
  *          Pure acquisition -- SafetyTask makes all decisions.
  ******************************************************************************
  */
#include "tasks.h"
#include "cmsis_os.h"

void LocalSensorTask(void* arguments){
	for(;;){
		osDelay(100);
	}
}
