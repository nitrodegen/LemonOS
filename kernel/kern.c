#include <stdint.h>
#include "stdio.h"
#include "../cpu/idt.h"
#include "../cpu/isr.h"
#include "../cpu/timer.h"
#include "../drivers/keyboard.h"
#include "../drivers/pci.h"
#include "../drivers/rtl8139.h"
#include "stdlib.h"
#include "paging.h"
#include "./ethernet.h"
#include "udp.h"

void main() {

    //goal for this OS: boot to chatroom and instantly send and receive shit.
    //plug that usb , YOUR OWN OS! , NO NEED TO INSTALL, CONNECTED TO MY SERVER AND CAN CONTROL MY PC
    init_map();

    asm volatile("cli");
    isr_install();  
    asm volatile("sti");
    void *v = allocate_page_frame(OS_FUNC,PAGE_SIZE);
    clearsc();
    init_keyboard();
    pci_init();
    rtl8139_init(); 
    
    kprintf("\nWelcome to LiberalOS!");
  


}   