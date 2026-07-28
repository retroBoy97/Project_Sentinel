/**
  ******************************************************************************
  * @file    actuator_task.c
  * @brief   Drives relay, servo, stepper, buzzer and LEDs based on commands
  *          from SafetyTask. Pure execution -- no decisions made here.
  ******************************************************************************
  */
#include "tasks.h"
#include "cmsis_os.h"

void ActuatorTask(void* arguments){
	for(;;){
		osDelay(100);
	}
}
