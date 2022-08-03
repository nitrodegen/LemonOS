#ifndef STDIO_H_
#define STDIO_H_
#include <stdarg.h>
#include <stdint.h>
#include "string.h"

#include "../drivers/keyboard.h"

static int POSX =0;
static int POSY=0;

static void* malloc(size_t n){
    //malloc makes free memory , then just returns address of that
    //i have no idea what i just did, but i gotta say that it works
    //it makes char pointer on address of PREFX, and fills tthat with zeros ,times of n
    //returns address of pointer
    //don't judge me , i did this on my own. (how i understood malloc).
    uint32_t* PREFX;
    *PREFX=(0x1000+0xFFFFF)>>2;
    char *mem = (char*)PREFX;
    for(int i =0;i<n;i++){
        mem[i]=0;
    }    
    void *addr = (void*)mem;
    return addr;
}
//gandalf's version of strlen (mine), returns length of const char* obj
static int _strlen(const char *str){
    int lol =0;
    while(1){
        if(str[lol] == '\0'){
          break;
        }
        lol++;
    }
    return lol;
}
/*
    Kernel VGA display API
        *it should never be used in final system* 
            just in debugging!
*/

static inline void putchar(char c,int fore,int bckg,int x,int y){
    int posx=0;
    int posy=0;

    if(x<0 && y<0){
        posx = POSX;
        posy = POSY;
    }
    else{
        posx = x;
        posy=y;
    }
    uint16_t clr = (bckg<<4) | (fore & 0x0F);
    volatile uint16_t* mem=( volatile uint16_t*)0xb8000+(posy*80+posx);
    *mem=c|(clr<<8);
    if(POSY>80){
        clearsc();
        POSX=0;
        POSY=0;
    }
    if(POSX>=80){
        POSX=0;
        POSY++;
    }
    POSX++;

}

//log is basic function for logging on console, it is not  to be mixed with "printf"
static inline void log_at(const char*str,int fore,int bckg,int x,int y){
    
    if(x <0 && y <0 && fore <0 && bckg <0){
        int len = _strlen(str);
        for(int i =0;i<len;i++){
            putchar(str[i],0xf,0x0,-1,-1);
        }
        POSX++;
    }
    else{
        int len = _strlen(str);
        int x_rise=x;
        for(int i =0;i<len;i++){
            putchar(str[i],fore,bckg,x_rise,y);
            x_rise++;
        }
    }
}
static inline void log(char *msg){
    log_at(msg,-1,-1,-1,-1);
}   


/*
    Public usable Lemon LIBC
        *should be used always, just not when debugging*
*/

static inline void dispchar(char c){
    int posy = POSY;
    int posx = POSX;
    int fore = 0xf;
    int bckg = 0x0;
    uint16_t clr = (bckg<<4) | (fore & 0x0F); //low bit bckg OR operation on low level bit fore
    volatile uint16_t* mem=( volatile uint16_t*)0xb8000+(posy*80+posx);
    *mem=c|(clr<<8);
    if(POSY>80){
        clearsc();
        POSX=0;
        POSY=0;
    }
    if(POSX>=80){
        POSX=0;
        POSY++;
    }
    POSX++;
}

static inline int  del_last_key(){
    int posy = POSY-1;
    int posx = POSX-1;
    int fore = 0x0;
    int bckg = 0x0;
    char c= ' ';
    uint16_t clr = (bckg<<4) | (fore & 0x0F); //low bit bckg OR operation on low level bit fore
    volatile uint16_t* mem=( volatile uint16_t*)0xb8000+(posy*80+posx);
    *mem=c|(clr<<8);
   
}
#endif