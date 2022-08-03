#include <stdint.h>
#include "stdio.h"
#include "../cpu/idt.h"
#include "../cpu/isr.h"
#include "../cpu/timer.h"
#include "../drivers/keyboard.h"

void main() {
    
    clearsc();
    isr_install();
    log_at("Welcome to Lemon OS!",0xe,0x0,29,10);
    log_at("Note: Marko duvaj ga",0x9,0x0,29    ,4);
    log_at("Press Enter to start 'sour shell'",0x3,0x0,22,15);
    //asm volatile("int $3");//testing interrupts
    asm volatile("sti");
    init_keyboard();
   
}   