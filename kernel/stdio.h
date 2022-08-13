
#ifndef STDIO_H_
#define STDIO_H_
#include <stdarg.h>
#include <stdint.h>
#include <stdarg.h>

#include "string.h"

#include "../drivers/keyboard.h"



void reset_args();

//gandalf's version of strlen (mine), returns length of const char* obj
int _strlen(const char *str);
/*
    Kernel VGA display API
        *it should never be used in final system* 
            just in debugging!
*/
void putchar(char c,int fore,int bckg,int x,int y);
//log is basic function for logging on console, it is not  to be mixed with "printf"
void log_at(const char*str,int fore,int bckg,int x,int y);
void log(char *msg); 

int kprintf( char* msg, ...);



/*
    Public usable Lemon LIBC
        *should be used always, just not when debugging*
*/

void dispchar(char c);

int  del_last_key();
#endif