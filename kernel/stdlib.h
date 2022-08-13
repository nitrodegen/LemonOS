

#pragma once
#ifndef TOOLS_H
#define TOOLS_h
#include <stdint.h>
#include "./stdio.h"
#include <stdarg.h>
#define low_16(address) (uint16_t)((address) & 0xFFFF)
#define high_16(address) (uint16_t)(((address) >> 16) & 0xFFFF)
#define STACK 0x9000
extern void cleanafter();
static inline void kpanic(){
    
    kprintf("\nKernel panic - not syncing -");
    cleanafter();
}

void  itoh(char *buff , unsigned int num , int base);
uint32_t strtol(char *buf);
void itoa(int n ,char str[]);
int atoi(char *str);
uint32_t _memcmp(void* b1,void *b2 , int n );

#endif