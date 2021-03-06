// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "include/MemManager.h"
#include "include/Curses.h"
#include "include/test_util.h"
#include "include/Process.h"

#define MAX_BLOCKS 128
#define MAX_MEMORY 150000 //Should be around 80% of memory managed by the MM

typedef struct MM_rq{
  void *address;
  uint32_t size;
}mm_rq;

int test_mm(int argc,char ** argv){
  mm_rq mm_rqs[MAX_BLOCKS];
  uint8_t rq;
  uint32_t total;

    //DEBUG("TEST MEM%s","")


  while (1){

    


    rq = 0;
    total = 0;
    // Request as many blocks as we can
    while(rq < MAX_BLOCKS && total < MAX_MEMORY){
      mm_rqs[rq].size = GetUniform(MAX_MEMORY - total - 1) + 1;
      mm_rqs[rq].address =malloc(mm_rqs[rq].size); // TODO: Port this call as required
        
      total += mm_rqs[rq].size;
      rq++;
    }

    // Set
    uint32_t i;
    for (i = 0; i < rq; i++){
      if (mm_rqs[i].address != NULL)
        memset(mm_rqs[i].address, i, mm_rqs[i].size); // TODO: Port this call as required
        
    }
    // Check
    for (i = 0; i < rq; i++){
      if (mm_rqs[i].address != NULL){
        if(!memcheck(mm_rqs[i].address, i, mm_rqs[i].size)){
          printf("ERROR!\n");
        }
      }
    }    
    // Free
    for (i = 0; i < rq; i++){
      if (mm_rqs[i].address != NULL)
        free(mm_rqs[i].address);  // TODO: Port this call as required   
    }
  }
  return 0; 
}

/*int main(){
  test_mm();
  return 0;
}*/
