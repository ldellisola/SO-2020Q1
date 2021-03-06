// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "include/Curses.h"
#include "include/Syscalls.h"
#include "include/Sem.h"
#include "include/Time.h"
#include "include/Process.h"
#include <stdbool.h>
#include <ctype.h>

/***************************************************************/
/*                          Constantes                         */
/***************************************************************/

#define MAX_SIZE 1000
#define LOOP_TIME 5 // N secs

/***************************************************************/
/*                      Declaraciones                          */
/***************************************************************/

bool isVowel(char c);

/***************************************************************/
/*                 Functiones Publicas                         */
/***************************************************************/


int loop(int argc, char ** argv)
{
    bool flag = true;
    unsigned int startSeconds = GetSeconds();
    unsigned int t1 = 0;
    unsigned int t2 = t1;
    int pid = getpid();
    unsigned int elapsed = 0;
    do
    {
        if ((GetSeconds() - startSeconds) >= 59) { // because when more than 1 minute has elapsed timer explodes
            t1 -= startSeconds;                    // keep the timer upon update
            t2 -= startSeconds;
            startSeconds = GetSeconds();
        }
        else
        {
            t2 = GetSeconds() - startSeconds;
        }

        if (((elapsed % LOOP_TIME) == 0 ) && flag) {
            printf("Loop says: My PID is %d. BRB in %d secs\n", pid, LOOP_TIME);
            flag = false;                           //avoid more than 1 call per second
        } 
        
        if (t2 != t1) {                             // != and not > because of the first if
            flag = true;
            t1 = t2;
            // printf("Elapsed: %d\n", elapsed);
            elapsed++;
        }
        
    } while (1);
    return 0;
}


int cat(int argc, char ** argv){

    int c;
    int index = 0;
    char arr[1000];

    for(int i = 0 ; i < 1000 ; i ++)
        arr[i]  =0;

    while ( (c=readKey()) != -20 && c!=-1 && c !=-2)
    {
        
        if(c >0){
            if(index < 999)
                arr[index++] = (char) c;

            //DEBUG("TEST: %s", arr)
        }

        if( (char) c == '\n'){
            arr[index] = 0;
            printf("CAT: %s",arr);
            index = 0;
        }
    }
    putChar(c);
    printf("\n");
    exit_process();
    return 0;
    
}

int wc(int argc,char ** argv){
    int count=0;
    char c;
    while( (c=readKey()) != -20 && c!=-1 && c !=-2){
        if(c=='\n')
            count++;
    }
    printf("Tiene %d lineas\n",count);
    exit_process();
    return 0;
}

int test_semaforito(int argc,char ** argv) {
    char * semName = "sem1";
    int i = semopen(semName,1);

    if (i == 0) {
        printf("todo ok 1 \2");
    }

    while(1) {
        semwait(semName);
        printf("Hola soy 1 \n");
        sempost(semName);
    }
    return 0;
}

int test_semaforito2(int argc,char ** argv) {
    char * semName = "sem1";
    int i = semopen(semName,1);

    if (i == 0) {
        printf("todo ok 2 \n");
    }

    while(1) {
        semwait(semName);
        printf("Hola soy 2 \n");
        sempost(semName);
    }
    return 0;
}

int filter(int argc,char ** argv) {

    int c;
    int index = 0;
    char arr[1000];

    for(int i = 0 ; i < 1000 ; i ++)
        arr[i]  =0;

    while ( (c=readKey()) != -20 && c!=-1 && c !=-2)
    {
        
        if(c >0){
            if(index < 999) {
                if (!isVowel((char) c)) {
                     arr[index++] = (char) c;
                }
            }
            //DEBUG("TEST: %s", arr)
        }

        if( (char) c == '\n'){
            arr[index] = 0;
            printf("FILTER: %s",arr);
            index = 0;
        }
    }
    putChar(c);
    printf("\n");
    exit_process();
    return 0;
}

bool isVowel(char c) {
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u'
     || c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U';
}