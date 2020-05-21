#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stdlib.h>
#include <stdint.h>


#define FD_STDOUT 				(0x01)
#define FD_STDERR 				(0x02)
#define FD_STDIN 				(0x03)
#define FD_SPEAKER 				(0x04)
#define FD_SQUARES 				(0x05)
#define FD_MEMORY 				(0x06)
#define FD_REGISTERS			(0x07)
#define FD_DEVICE_INFO  		(0x08)
#define FD_TIMER				(0x09)
#define FD_TIME					(0x0A)
#define FD_STDOUT_COLOR			(0x0B)

#define DELETE_CURRENT_CHAR 1
#define DELETE_ALL_DISPLAY 3

extern void _write(int fd, void * first, void *second, void * third, void * forth );

extern void _read(int fd, void * first, void *second, void * third, void * forth);

int read(int fd,char * buffer, uint64_t size);

int write(int fd, char * buffer);

extern void delete(int fd, void * first, void *second, void * third, void * forth);

extern 	void _malloc(int increment, void ** resp);

extern  void _free(void ** address);

extern void memory_state(void ** first, void ** last, void ** next);

extern void create_process(char * name,int * state,void * function);

extern void kill_process(int * pid);


extern void nice_process(int * pid,int prior);

void BlockAndSwitchProcess(int * pid);

extern void block_process(int * pid);

void SleepUntilUserInput();


void exit_process();

extern void ps();


int getpid();

extern void sem(int fd,void * first,void * second);

extern void pipes(int fd, void * name, void * action, void * pipe);
#endif