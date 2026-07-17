#include "tasks.h"
#include "cmsis_os.h"

void WatchdogTask(void* arguments){
	for(;;){
		osDelay(100);
	}
}
