#include "tasks.h"
#include "cmsis_os.h"

void CommsTask(void* arguments){
	for(;;){
		osDelay(100);
	}
}
