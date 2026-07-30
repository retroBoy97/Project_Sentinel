/**
  ******************************************************************************
  * @file    safety_task.c
  * @brief   Consumes sensor + RFID/keypad data, applies all threshold and
  *          access-control logic, and decides actuator commands and system
  *          state (ARMED/DISARMED/ALARM). Sole decision authority.
  ******************************************************************************
  */
#include "tasks.h"
#include "cmsis_os.h"

void SafetyTask(void* arguments){
	for(;;){
		osDelay(20);
	}
}
