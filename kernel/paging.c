#include <stdint.h>
#include "stdio.h"
#include "stdlib.h"
#include "paging.h"
#include <stddef.h>
#include <stdbool.h>
mmap_entry* hdr;
int dir_index=0;
//this function allocates first parameters of mmap_entry to be somewhere after kernel starts.
void init_map( ){
    hdr=(mmap_entry*)0x1000+8192; // 2MB (im predicting it is the end of kernel :D ) 
    hdr->len=1995904;
    hdr->type = MEMORY_RESERVED;
    hdr->phaddr= 0x1000;//0x1e8480-0x1000
    hdr->used = true;
}

int GETADDR(){
    return hdr->phaddr;
}
void* allocate_page_frame(uint32_t type,int length){
    length+=sizeof(mmap_entry);
    uint32_t address = hdr->phaddr;
    uint32_t end= address+hdr->len;

    mmap_entry* addr = (mmap_entry*)((void*)end);
    addr->phaddr = end;
    addr->len = length;
    addr->type = type;
    addr->used= true;

    //remember for later :D  
    hdr->phaddr = end;
    hdr->used = true;
    hdr->len = length;

    return (void*)end;
}


int deallocate_page_frame(void *addr){

    mmap_entry* d = (mmap_entry*)addr;
    hdr->phaddr = d->phaddr-d->len;
    hdr->len = 0;
    hdr->used = false;
    hdr->type = FREE_TO_USE;
    
    d->used=false;
    d->phaddr=0;
    d->len = 0;
    d->type =FREE_TO_USE;
    
   
    char*ptr = (char*)addr+sizeof(mmap_entry*)+2;//testing freed block
    ptr[0] = 'x';
    if(_strlen(ptr) == 1 && ptr[0] == 'x'){
        return 1;
    }
    else{
        return -1;
    }

}

void *malloc(size_t size){
    uint32_t last_addr = hdr->phaddr+hdr->len;
    int stat= 0 ;
    uint32_t chosen=0;
    bool found=false;
    if(hdr->used == true){
        uint32_t check = last_addr;
        mmap_entry* entry = (mmap_entry*) ((void*)check);
        if(entry->used == false){
            stat++;
        }
        check = last_addr+size;
        mmap_entry*test= (mmap_entry*)((void*)check);
        if(test->used ==false){
            stat++;

        }
        if(stat == 2 ){
            chosen = last_addr;

        }

    }
    else{
        found = true;
    }
    if(found == true){
        _memset(&last_addr,0,size+sizeof(mmap_entry));
        void *v = allocate_page_frame(OS_FUNC,size);
        return (void*)v+sizeof(mmap_entry)+4;
    }
}
void free(void *ptr){
    //deallocate page frame , and make it zero . done .
    int l = hdr->len;
    int stat = deallocate_page_frame(ptr);
    if(stat == 1){
        char *addr = (char*)ptr;
        _memset(addr,0,l);

    }
    else{
        kpanic();
    }
}