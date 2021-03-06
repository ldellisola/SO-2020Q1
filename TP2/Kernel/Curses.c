// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "include/Curses.h"
#include <String.h>
#include <ConsoleDriver.h>

/***************************************************************/
/*                         CONSTANTES                          */
/***************************************************************/

#define MAXBUFFER 500

/***************************************************************/
/*                   Funciones Publicas                        */
/***************************************************************/

void printf(const char *format, ...)
{
    char string[MAXBUFFER];
    for (int i = 0; i < MAXBUFFER; i++)
        *(string + i) = 0;
    va_list args;
    va_start(args, format);
    snprintf(string, MAXBUFFER, (char *)format, args);
    va_end(args);

    printLine(string);
}

void printfColor(int fontColor, int backgroundColor, const char *format, ...)
{
    char string[MAXBUFFER];
    for (int i = 0; i < MAXBUFFER; i++)
        *(string + i) = 0;
    va_list args;
    va_start(args, format);
    snprintf(string, MAXBUFFER, (char *)format, args);
    va_end(args);

    ColorChar colorString[strlen(string)];
    int i;
    for (i = 0; string[i] != 0; i++)
    {
        colorString[i].ch = string[i];
        colorString[i].fontColor = fontColor;
        colorString[i].backgroundColor = backgroundColor;
    }
    colorString[i].ch = 0;

    printLineColor(colorString);
}

void putChar(char ch)
{
    printChar(ch);
}

void putCharColor(char ch, int fontColor, int backgroundColor)
{
    ColorChar temp;
    temp.ch = ch;
    temp.fontColor = fontColor;
    temp.backgroundColor = backgroundColor;
    printCharColor(temp);
}
