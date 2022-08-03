

#pragma once
#ifndef TOOLS_H
#define TOOLS_h
#include <stdint.h>
#include "./stdio.h"
#include <stdarg.h>
#define low_16(address) (uint16_t)((address) & 0xFFFF)
#define high_16(address) (uint16_t)(((address) >> 16) & 0xFFFF)

extern void cleanafter();

static inline void kpanic(){
    POSX=0;
    POSY++;
    log("Kernel panic - not syncing - ");
    cleanafter();
}

void itoa(int n ,char str[]);
int atoi(char *str);

/*
void reverse(char str[]){
    int i =0;
    char c;
    int j = _strlen(str)-1;
    while(i<j){        
        c = str[j];
        str[j] = str[i];
        str[i]= c;
        i++;
        j--;
    }
}
*/

#endif