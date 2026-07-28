/**
  ******************************************************************************
  * @file    sensor_poll_task.c
  * @brief   Reads sensor data from the Arduino over I2C and posts it to a
  *          queue. Pure acquisition -- no thresholds, no decisions.
  ******************************************************************************
  */
#include "tasks.h"
#include "cmsis_os.h"

void SensorPollTask(void* arguments){
	for(;;){
		osDelay(100);
	}
}
