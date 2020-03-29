#include "shmHelper.h"

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Shared memory
#include <sys/shm.h>
#include <sys/mman.h>
#include <fcntl.h>

#include <sys/stat.h>

#include <sys/types.h>
#include <unistd.h>



SHMData_t shmCreate(const char * shmName,  size_t shmSize ){

    SHMData_t data;

    strcpy(data.name,shmName);
    data.size = shmSize;

    data.fd = shm_open(shmName,O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR | S_IROTH | S_IWOTH);

    if(data.fd == -1){
        perror("Creating Shared Memory");
        exit(-1);
    }


    if(ftruncate(data.fd,shmSize) == -1){
        perror("Truncating Shared Memory");
        exit(-1);
    }

    data.map = (char *) mmap(NULL,shmSize, PROT_READ | PROT_WRITE, MAP_SHARED,data.fd,0);
    if(data.map == (void *) -1){
        perror("Mapping Shared memory to virtual memory");
        exit(-1);
    }

    // Indice para escribir y leer a memoria compartida
    data.readIndex = 0;
    data.writeIndex = 0;

    return data;
}

void shmDestroy(SHMData_t * data){

    if(munmap((*data).map,(*data).size) == -1){
        perror("Deleting Mapped Memory");
    }

    if(shm_unlink((*data).name) == -1){
        perror("Unlinking Shared Memory");
    }

    if(close((*data).fd) == -1){
        perror("Closing Shared Memory File Descriptor");
    }
}


void shmWrite(char * buffer, int bSize, SHMData_t * data){

    for(int i = 0 ; i < bSize ; i++){
            data->map[data->writeIndex++] = buffer[i];
        }
        data->map[data->writeIndex] = 0;
}


 int shmRead(char * buffer, int bSize,SHMData_t * data ){
     int i;

     for(i = 0 ; i < bSize && data->map[data->readIndex] != 0; i++){
         buffer[i] = data->map[data->readIndex++];
     }
     buffer[i] = 0;

    return i;
}

