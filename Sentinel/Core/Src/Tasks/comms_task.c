/**
  ******************************************************************************
  * @file    comms_task.c
  * @brief   Bidirectional gateway to the Pi: sends SENSOR_DATA/ALARM_EVENT/
  *          STATUS_UPDATE, receives COMMAND frames and replies with ACK.
  ******************************************************************************
  */
#include "tasks.h"
#include "cmsis_os.h"

void CommsTask(void* arguments){
	for(;;){
		osDelay(500);
	}
}
