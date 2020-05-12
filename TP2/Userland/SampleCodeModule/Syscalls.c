#include "../Include/Syscalls.h"
extern void _exit();
extern void sleep_process();
extern void get_pid(int * ret);



int getpid(){
	int ret;
	get_pid(&ret);
	return ret;
}

void exit_process(){
	_exit();
	__asm__("hlt");

}

void BlockAndSwitchProcess(int * pid){
	block_process(pid);
	__asm__("hlt");
}

void SleepUntilUserInput(){
	sleep_process();


}