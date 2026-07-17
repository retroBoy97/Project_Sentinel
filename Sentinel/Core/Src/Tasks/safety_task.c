#include "tasks.h"
#include "cmsis_os.h"

void SafetyTask(void* arguments){
	for(;;){
		osDelay(100);
	}
}
