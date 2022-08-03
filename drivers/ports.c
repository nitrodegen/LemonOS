#include <stdint.h>
unsigned char io_read(uint16_t port) {
    unsigned char result;
   
    asm("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

void io_write(uint16_t port, uint8_t data) {
 
    asm("out %%al, %%dx" : : "a" (data), "d" (port));
}

unsigned short io_wrdread(uint16_t port) {
    unsigned short result;
    asm("in %%dx, %%ax" : "=a" (result) : "d" (port));
    return result;
}

void io_wrdwrite(uint16_t port, uint16_t data) {
    asm("out %%ax, %%dx" : : "a" (data), "d" (port));
}
