#pragma once
#include <stdint.h>
#include "stdio.h"
#include "stdlib.h"
#include <stddef.h>
#include <stdbool.h>
#define FREE_TO_USE 1
#define OS_FUNC 2

#define MMAP_GET_NUM 0
#define MMAP_GET_ADDR 1
#define PAGE_SIZE 4096
#define MEMORY_RESERVED 4
//Both tables contain 1024 4-byte entries, making them 4 KiB each. In the page directory, each entry points to a page table. In the page table, each entry points to a 4 KiB physical page frame. 
//On the x86, the MMU maps memory through a series of tables, two to be exact.They are the paging directory (PD), and the paging table (PT).


//we need page directories (1024 of them) , and they contain 1024 page tables, which contain 1024 pages 
//1024 everything (some people say 512)


typedef struct{
    uint32_t type;
    uint32_t phaddr;
    uint32_t len;
    bool used;
}mmap_entry;
int GETADDR();
void init_map();
void* allocate_page_frame(uint32_t type,int length);

int deallocate_page_frame(void *addr);
void* malloc(size_t size);
void free(void *ptr);
