#include "tasks.h"
#include "cmsis_os.h"

void DisplayTask(void* arguments){
	for(;;){
		osDelay(100);
	}
}
