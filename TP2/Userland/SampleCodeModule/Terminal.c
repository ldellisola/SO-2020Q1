// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdarg.h>
#include "include/Syscalls.h"
#include "include/Terminal.h"
#include "include/Curses.h"
#include "include/Pipes.h"
#include "include/Commands.h"
#include "include/String.h"
#include <stdlib.h>
#include <stdbool.h>
#include "include/Exec.h"
#include "include/test_util.h"
#include "include/Process.h"
#include "include/Sem.h"

extern void __ForceTimerTick__();

/***************************************************************/
/*                        Constantes                           */
/***************************************************************/

#define MAXBUFFER (600)
#define MAXPIPES 5
/***************************************************************/
/*                         Estructuras                         */
/***************************************************************/

typedef struct
{
    char *process;
    bool isBackground;
    int argc;
    char *argv[20];
    unsigned long hash;

} ParsedCommand_t;

typedef int (*processFunction)(int, char **);

typedef struct
{
    processFunction function;
    const char *name;
    bool isProcess;
    unsigned long hash;
    const char *description;
    bool isSO;

} Command_t;

/***************************************************************/
/*                         Declaraciones                       */
/***************************************************************/

void clearArray(char *arr, int size);
void overwriteArray(char *src, char *dest);
void printTerminal();
int interpretCommand();
void overwriteArrayUpTo(char *src, char *dest, char c);
void InitializeTerminal();
unsigned long sdbm(char *str);
int help(int argc, char **argv);

/***************************************************************/
/*                         Variables                           */
/***************************************************************/

static char TerminalType[MAXBUFFER];
static unsigned int TypeIndex = 0;

Command_t commands[] = {
    {.function = loop, .name = "loop", .isProcess = true, .description = "Infinite loop", .isSO = true},
    {.function = test_mm, .name = "testMem", .isProcess = true, .description = "Process that tests our memory manager implementation.",.isSO = true},
    {.function = test_processes, .name = "testProcess", .isProcess = true, .description = "Process that tests our process implementation.",.isSO = true},
    {.function = test_prio, .name = "testPrio", .isProcess = true, .description = "Process that tests our priorities implementation.",.isSO = true},
    {.function = test_sync, .name = "testSync", .isProcess = true, .description = "Process that tests our semaphore sync implementation.",.isSO = true},
    {.function = test_no_sync, .name = "testNoSync", .isProcess = true, .description = "Process that tests our semaphore no sync implementation.",.isSO = true},
    {.function = cat, .name = "cat", .isProcess = true, .description = "Imprime en STDOOUT lo que viene por STDIN",.isSO = true},
    {.function = wc, .name = "wc", .isProcess = true, .description = "Count the lines of what it reads through its STDIN",.isSO = true},
    {.function = filter, .name="filter", .isProcess = true, .description = "Prints without vowels of what it reads through its STDIN",.isSO = true},
    {.function = phylo, .name="phylo", .isProcess = true, .description = "Problem of the eating thinking people",.isSO = true},
    {.function = test_semaforito, .name="s1", .isProcess = true, .description = "Prueba un semaforo 1",.isSO = false},
    {.function = test_semaforito2, .name="s2", .isProcess = true, .description = "Prueba un semaforo 2",.isSO = false},
    

    {.function = help, .name = "help", .isProcess = false, .description = "It enumerates all the commands available on this shell\nIf there's an argument, it will tell you the funcition of that command.", .isSO = true},
    {.function = time, .name = "time", .isProcess = false, .description = "It shows the current date and time.", .isSO = false},
    {.function = infoReg, .name = "infoReg", .isProcess = false, .description = "It prints on screen the actual value of the registers.", .isSO = false},
    {.function = invalidOpcode, .name = "invalidOpcode", .isProcess = false, .description = "It is a test to validate the INVALID OPCODE exception.", .isSO = false},
    {.function = clearConsole, .name = "clear", .isProcess = false, .description = "It clears the screen.", .isSO = true},
    {.function = quotient, .name = "quotient", .isProcess = false, .description = "It calculates the quotient of the division of [Argument_1] by [Argument_2]. This can be used to test the DIVISION BY ZERO exception.", .isSO = false},
    {.function = malloc_test, .name = "malloctest", .isProcess = false, .description = "Our very own memory manager test.", .isSO = false},
    {.function = printMemoryState, .name = "mem", .isProcess = false, .description = "It prints on screen the first 32 bytes of memory from any given memory position.", .isSO = true},
    {.function = killProcess, .name = "kill", .isProcess = false, .description = "It kills any process by its PID. It won't kill the terminal.", .isSO = true},
    {.function = niceProcess, .name = "nice", .isProcess = false, .description = "It changes the priority of a process, with 1 beign more priority and 3 less priority.", .isSO = true},
    {.function = blockProcess, .name = "block", .isProcess = false, .description = "It blocks a process by its PID. It won't block the terminal.", .isSO = true},
    {.function = ProcessState, .name = "ps", .isProcess = false, .description = "It prints in the terminal information about all the processes that are currently running.", .isSO = true},
    {.function = pipe, .name = "pipe", .isProcess = false, .description = "It prints in the terminal information about all the pipes that currently exists.", .isSO = true},
    {.function = semInfo, .name = "sem", .isProcess = false, .description = "It prints in the terminal information about all the current semaphores that currently exists.", .isSO = true},
    //{.function = testSem, .name = "testSem", .isProcess = false, .description = "Our very own function that tests our semaphores implementation."},
    {.function = testPipes, .name = "testPipes", .isProcess = false, .description = "Our very own function that tests our pipes implementation.", .isSO = false},

    {.function = NULL, .name = NULL, .isProcess = false, .description = NULL}};


/***************************************************************/
/*                      Funciones Publicas                     */
/***************************************************************/

int runTerminal()
{
    InitializeTerminal();
    clearArray(TerminalType, MAXBUFFER);
    TypeIndex = 0;
    printf("Hey! You can use \"help\" to check for our commands...\n");
    printf(">>>>  ");
    int exit = 0;
            //SleepUntilUserInput();

    do
    {
        int key = readKey();
        if (key > 0)
        {
            if (key == 8)
            {
                if (TypeIndex > 0)
                {
                    TerminalType[--TypeIndex] = 0;
                    RemoveLastCharFromDisplay();
                }
            }
            else
            {
                TerminalType[TypeIndex++] = key;
                printTerminal();

                if (key == '\n')
                {
                    exit = interpretCommand();
                    clearArray(TerminalType, MAXBUFFER);
                    TypeIndex = 0;
                    printf(">>>>  ");
                    //SleepUntilUserInput();

                }
            }
        }
    } while (!exit);

    return exit;
}

/***************************************************************/
/*                      Funciones Privadas                     */
/***************************************************************/

unsigned long sdbm(char *str)
{
    unsigned long hash = 0;
    int c;
    while ((c = *(str++)))
        hash = c + (hash << 6) + (hash << 16) - hash;

    return hash;
}

void InitializeTerminal()
{

    for (int i = 0; commands[i].function != NULL; i++)
    {
        commands[i].hash = sdbm((char *)commands[i].name);
    }
}

int help(int argc, char **argv)
{
    if (argc == 0)
    {
        int i = 0;
        printf("FOR ARIEL:\n");    
        while (commands[i].name != NULL)
        {
            if(commands[i].isSO)
                printf("%s    |  %s\n", commands[i].isProcess ? "Proceso" : "Comando", commands[i].name);
            i++;
        }
        i = 0;
        printf("NOT FOR ARIEL:\n");    
        while (commands[i].name != NULL)
        {
            if(!commands[i].isSO)
            printf("%s    |  %s\n", commands[i].isProcess ? "Proceso" : "Comando", commands[i].name);
            i++;
        }
    }
    else
    {
        unsigned long hash = sdbm(argv[0]);
        for (int i = 0; commands[i].name != NULL; i++)
        {
            if (hash == commands[i].hash)
            {
                printf("%s      |  %s\n", commands[i].isProcess ? "Proceso" : "Comando", commands[i].name);
                printf("           %s\n", commands[i].description);
                return 0;
            }
        }

        printfError("%s  Unkown command\n",argv[0]);
    }
    return 0;
}

void ProcessCommandString(char *command, ParsedCommand_t * cmd)
{   char *currentPart;
    int index = 0;
    cmd->isBackground = false;
    cmd->process = NULL;

    if (command[strlen(command) - 1] == '\n')
        command[strlen(command) - 1] = 0;

        
    while ((currentPart = strtok(&command, ' ')) != NULL && index < 20)
    {
        if (cmd->process == NULL)
        {
            cmd->process = currentPart;
            continue;
        }

        if (currentPart[0] == '&')
        {
            cmd->isBackground = true;
            break;
        }
        cmd->argv[index++] = currentPart;
    }

    cmd->argc = index;
    cmd->hash = sdbm(cmd->process);
}


void createCommand(int i,ParsedCommand_t * parsedCommand,int fdw,int fdr){
    if (commands[i].isProcess)
    {
        exec((char *)commands[i].name, parsedCommand->isBackground, commands[i].function,fdr,fdw,parsedCommand->argc, parsedCommand->argv);
    }
    else
    {
        commands[i].function(parsedCommand->argc, parsedCommand->argv);
    }
    
}

int interpretCommand()
{
    ParsedCommand_t parsedCommand[MAXPIPES];
    int cant_process = 0;
    char * command= TerminalType;
    char * currentPart;

    if (command[strlen(command) - 1] == '\n')
        command[strlen(command) - 1] = 0;

    while((currentPart = strtok(&command, '|')) != NULL && cant_process < MAXPIPES){
        ProcessCommandString(currentPart, &parsedCommand[cant_process]);
        cant_process++;
    }
    //es fd y el i del array de comands
    int fd[cant_process][3];
    fd[0][0]=-1;
    fd[0][1]=-1;
    fd[0][2]=-1;

    //los fd
    if(cant_process>1){
        char name[20];
        uint64_t auxiliar;
        int fdr;
        int fdw;
        for(int i=0;i<cant_process-1;i++){
            auxiliar=GetUniform((uint32_t)100);
            IntToString(name,20,auxiliar);
            fdr=openPipe(name,READ);
            fdw=openPipe(name,WRITE);
            fd[i][WRITE]=fdw;
            fd[i+1][READ]=fdr;
            //pongo -1 el 3
            fd[i][2]=-1;
        }
        fd[cant_process-1][WRITE]=-1;
        fd[cant_process-1][2]=-1;
    }

    int i;
    int count=0;
    for (i = 0; commands[i].name != NULL && count<cant_process;i++)
    {
        for(int j=0;j<cant_process;j++){
            if(commands[i].hash==parsedCommand[j].hash){
                fd[j][2]=i;
                count++;
            }
        }
    }

    if (count<cant_process)
    {
        int j=0;
        while(fd[j][2]!=-1){
        j++;
        if(fd[j][0]!=-1)
        closePipes(fd[j][0]);
        }

        printfError("%s: command not found \n",parsedCommand[j].process);
    }
    else{
        for(int i=0;i<cant_process;i++){
            int aux=fd[i][2];
            createCommand(aux,&parsedCommand[i],fd[i][WRITE],fd[i][READ]);
        }
    }


    __ForceTimerTick__();


    return 0;
}

void printTerminal()
{

    if (TerminalType[0])
        putChar(TerminalType[TypeIndex - 1]);
}

void clearArray(char *arr, int size)
{

    for (int i = 0; i < size; i++)
        arr[i] = 0;
}

void overwriteArray(char *src, char *dest)
{
    overwriteArrayUpTo(src, dest, 0);
}

void overwriteArrayUpTo(char *src, char *dest, char c)
{
    clearArray(dest, MAXBUFFER);
    int i;
    for (i = 0; i < MAXBUFFER && src[i] != 0 && src[i] != '\n' && src[i] != c; i++)
        dest[i] = src[i];
    if (i != MAXBUFFER)
    {
        dest[i] = 0;
    }
}
