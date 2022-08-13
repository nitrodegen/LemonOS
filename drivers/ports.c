#include <stdint.h>
unsigned char io_read(uint16_t port) {
    unsigned char result;
   
    asm("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

void io_write(uint16_t port, uint32_t data) {
 
    asm("out %%al, %%dx" : : "a" (data), "d" (port));
}

unsigned short io_wrdread(uint16_t port) {
    unsigned short result;
    asm("in %%dx, %%ax" : "=a" (result) : "d" (port));
    return result;
}

void io_wrdwrite(uint16_t port, uint32_t data) {
    asm("out %%ax, %%dx" : : "a" (data), "d" (port));
}

void io_bwrite(uint32_t port ,uint8_t data){
    asm volatile("outb %1, %0" : : "dN"(port),"a"(data));
}
uint32_t  io_bread(uint32_t port){
    uint8_t res;
    asm volatile("inb  %1, %0" :  "=a"(res): "Nd"(port));
    
    return res;
}
void io_lwrite(uint32_t port ,unsigned long data){
    asm volatile("outl %%eax ,%%dx" : : "dN" (port), "a" (data));
}

uint32_t  io_lread(uint32_t port){
    uint32_t data;
    
    asm volatile("inl %%dx ,%%eax" : "=a" (data) : "dN" (port));
    return data;
}