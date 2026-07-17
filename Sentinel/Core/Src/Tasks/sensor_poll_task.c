#include "tasks.h"
#include "cmsis_os.h"

void SensorPollTask(void* arguments){
	for(;;){
		osDelay(100);
	}
}
