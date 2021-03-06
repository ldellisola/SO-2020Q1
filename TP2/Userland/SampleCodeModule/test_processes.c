// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "include/test_util.h"
#include "include/Exec.h"
#include "include/Syscalls.h"
#include "include/Curses.h"
#include "include/Process.h"



//TO BE INCLUDED
int  endless_loop(int argc,char ** argv){
  while(1);
  return 0;
}

uint32_t my_create_process(char * name){
    int status=1;//background
  return exec(name,status,endless_loop,-1,-1,0,NULL);
}

uint32_t my_kill(uint32_t pid){
    int process=pid;
    //devuelve en el param que le mando la respuesta
    process_manager(1,(void *)&process,0,0);
  return process;  
}

uint32_t my_block(uint32_t pid){
    int process=pid;
    //devuelve en el param que le mando la respuesta
    	process_manager(3,(void *)&process,0,0);
    return process;
}

uint32_t my_unblock(uint32_t pid){
  int process=pid;
    //devuelve en el param que le mando la respuesta
    process_manager(3,(void *)&process,0,0);
    return process;
}

#define MAX_PROCESSES 12 //Should be around 80% of the the processes handled by the kernel

enum State {ERROR, RUNNING, BLOCKED, KILLED};

typedef struct P_rq{
  uint32_t pid;
  enum State state;
}p_rq;

int test_processes(int argc,char ** argv){
  p_rq p_rqs[MAX_PROCESSES];
  uint8_t rq;
  uint8_t alive = 0;
  uint8_t action;

  while (1){
    // Create MAX_PROCESSES processes
    for(rq = 0; rq < MAX_PROCESSES; rq++){
      p_rqs[rq].pid = my_create_process("endless_loop");  // TODO: Port this call as required

      if (p_rqs[rq].pid == -1){                           // TODO: Port this as required
        printf("Error creating process\n");               // TODO: Port this as required
        exit_process();
        return 0;
      }else{
        p_rqs[rq].state = RUNNING;
        alive++;
      }
    }
    // Randomly kills, blocks or unblocks processes until every one has been killed
    while (alive > 0){

      for(rq = 0; rq < MAX_PROCESSES; rq++){
        action = GetUniform(2) % 2; 

        switch(action){
          case 0:
            if (p_rqs[rq].state == RUNNING || p_rqs[rq].state == BLOCKED){
              if (my_kill(p_rqs[rq].pid) == -1){          // TODO: Port this as required
                printf("Error killing process\n");        // TODO: Port this as required
                exit_process();
                return 0;
              }
              p_rqs[rq].state = KILLED; 
              alive--;
            }
            break;

          case 1:
            if (p_rqs[rq].state == RUNNING){
              if(my_block(p_rqs[rq].pid) == -1){          // TODO: Port this as required
                printf("Error blocking process\n");       // TODO: Port this as required
                exit_process();
                return 0;
              }
              p_rqs[rq].state = BLOCKED; 
            }
            break;
        }
      }
      // Randomly unblocks processes
      for(rq = 0; rq < MAX_PROCESSES; rq++)
        if (p_rqs[rq].state == BLOCKED && GetUniform(2) % 2){
          if(my_unblock(p_rqs[rq].pid) == -1){            // TODO: Port this as required
            printf("Error unblocking process\n");         // TODO: Port this as required
            exit_process();
            return 0;
          }
          p_rqs[rq].state = RUNNING; 
        }
    } 
  }
  exit_process();
  return 0;
}