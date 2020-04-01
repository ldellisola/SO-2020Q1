#include "semaphoreHelper.h"

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
// Shared memory
#include <sys/mman.h>
#include <fcntl.h>

#include <sys/stat.h>

SemData_t semaphoreSetUp(const char *name)
{

    SemData_t data;

    strncpy(data.name, name, 500);

    for (int i = 0; data.name[i] != 0; i++)
        if (data.name[i] == '\n')
            data.name[i] = 0;


    char path[500];

    sprintf(path, "/dev/shm%s",data.name );

    sem_unlink(name);
    data.id = sem_open(data.name, O_CREAT | O_RDWR, 0777, 0); //S_IRUSR | S_IWUSR | S_IROTH | S_IWOTH , 0);


    

    if (data.id == SEM_FAILED)
    {
        
        perror("Creating Semaphore");

        if (access(path, F_OK) != -1)
    {
        printf("File: %s Doesnt exist\n", path);
    }else{
            printf("File: %s Exist!\n",path);
        }
        exit(-1);
    }

    return data;
}

SemData_t semaphoreOpen(const char *name)
{

    SemData_t data;

    strncpy(data.name, name, 500);

    for (int i = 0; data.name[i] != 0; i++)
        if (data.name[i] == '\n')
            data.name[i] = 0;

                    char path[500];

        sprintf(path, "/dev/shm%s", data.name);

    data.id = sem_open(data.name, O_RDWR, 0777, 0); // S_IRUSR | S_IWUSR | S_IROTH | S_IWOTH , 0);

    if (data.id == SEM_FAILED)
    {
        perror("Creating Semaphore");


        if (access(path, F_OK) != -1)
        {
            printf("File: %s Doesnt exist\n", path);
        }else{
            printf("File: %s Exist!\n",path);
        }
        exit(-1);
    }

    return data;
}

void semaphoreDestroy(SemData_t *data)
{

    if (sem_close(data->id) != 0)
    {
        perror("Closing Semaphore");
        exit(-1);
    }
    if (sem_unlink(data->name) < 0)
    {
        perror("Unlinking Semaphore");
        exit(-1);
    }
}

void SemaphoreWait(SemData_t *data)
{
    sem_wait(data->id);
}

void SemaphorePost(SemData_t *data)
{
    sem_post(data->id);
}