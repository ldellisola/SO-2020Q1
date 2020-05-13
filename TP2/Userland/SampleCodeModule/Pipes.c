#include "../Include/Pipes.h"
#include "../Include/Syscalls.h"
#include "../Include/MemManager.h"

int openPipe(char * name,actions action){
  int fd;
  pipes(0,(void*)name,(int)action,&fd);
  return fd;
}

int readPipe(int pipe,char * buffer,int bufferSize){
  int ans=bufferSize;
  read(pipe,buffer,&ans,NULL,NULL);
  return ans;
}

int writePipe(int pipe,char * buffer){
  int ans;
  write(pipe,buffer,&ans,NULL,NULL);
  return ans;
}


int closePipes(int pipe){
  int ans=pipe;
  pipes(1,(void *)&ans,NULL,NULL);
  return ans;
}

void pipe(){
  pipes(2,NULL,NULL,NULL);
}
