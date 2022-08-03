#pragma once
#ifndef STRING_H
#define STRING_H
#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>
#include "./stdio.h"

static inline void* _memset(void*b,int c,size_t len){
    char *src = (char*)b;
    while(len--){
        *src++ = (char)c;
    }
    return b;
}
static inline void* _memcpy(void*dst,void *src,size_t len){
    char *sr = (char*)src;
    char *ds = (char*)dst;
    while(len--){
        *ds++ = *sr++;
    }
    return dst;
}
static inline void clearsc(){
    volatile char* mem=( volatile char*)0xb8000;
    for(int i =0;i<25;i++){
        _memset((void*)mem+(i*160),0xf00,160);
    }


} 
#endif