#include "timer.h"
#include "../drivers/ports.h"
#include "../kernel/stdio.h"
#include "isr.h"

uint32_t tick=0;

static void timer_callback(registers_t *r){
    tick++;
    log("ticking..");
}

void init_timer(uint32_t freq){
    register_ihandle(IRQ0,timer_callback);
    uint32_t divide = 1193180/freq;
    uint8_t low =(uint8_t)(divide&0xFF);
    uint16_t high =(uint16_t)((divide>>8)&0xFF);

    io_write(0x43,0x36);//command port
    io_write(0x40,low);
    io_write(0x40,high);

}
