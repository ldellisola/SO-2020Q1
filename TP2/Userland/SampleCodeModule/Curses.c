#include "../Include/Curses.h"
#include "../Include/String.h"
#include "../Include/Syscalls.h"
#include "../Include/deviceInfo.h"
#include <stdint.h>
#include <stdbool.h>

typedef struct{
    char ch;
    int fontColor;
    int backgroundColor;
    int x;
    int y;
}ColorChar;


#define MAXBUFFER 1000




void clearConsole()
{
    delete(FD_STDOUT,DELETE_ALL_DISPLAY,NULL,NULL,NULL);
}


void printf(const char * format, ...){
    char string[MAXBUFFER];
    for(int i=0;i<MAXBUFFER;i++)
        *(string+i)=0;
    va_list args;
	va_start(args,format);
    snprintf(string,MAXBUFFER,(char*)format,args);
    va_end(args);

    write(FD_STDOUT,string);
}

void printfColorAt(const char * format,int fontColor, int backgroundColor,int x,int y,...){
    unsigned int charWidth;
    getCharWidth(&charWidth);
    char string[MAXBUFFER];
    for(int i=0;i<MAXBUFFER;i++)
        *(string+i)=0;
    va_list args;
	va_start(args,format);
    snprintf(string,MAXBUFFER,(char*)format,args);
    va_end(args);

    ColorChar colorString[strlen(string)];
    int i;
    int posX=x;
    int posY=y;
    for(i = 0; string[i]!=0;i++,posX+=charWidth){
        colorString[i].ch = string[i];
        colorString[i].fontColor = fontColor;
        colorString[i].backgroundColor = backgroundColor;
        colorString[i].x=posX;
        colorString[i].y=posY;

    }
        colorString[i].ch = 0;

        _write(FD_STDOUT_COLOR,colorString,0,0,0);

}


void printfError(const char * format,...){
    char string[MAXBUFFER];
    for(int i=0;i<MAXBUFFER;i++)
        *(string+i)=0;
    va_list args;
	va_start(args,format);
    snprintf(string,MAXBUFFER,(char*)format,args);
    va_end(args);

    _write(FD_STDERR,string,0,0,0);

}



void RemoveLastCharFromDisplay(){
    delete(FD_STDOUT,DELETE_CURRENT_CHAR,NULL,NULL,NULL);
}


void putChar( char ch)
{
    char buff[2]={0,0};
    buff[0]=ch;
    write(FD_STDOUT,buff);

}


int readKey()
{
    static char buff[2];
    int ans=read(FD_STDIN,buff,2);
    if(ans==-1 || ans ==-2)
	    return ans;
    char retValue = buff[0];
    buff[0] = 0;
    return retValue;
}
void * memset(void * destiation, int32_t c, uint64_t length) {
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destiation;

	while(length--)
		dst[length] = chr;

	return destiation;
}
