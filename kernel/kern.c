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

/*
    people are saying i cant build an os. so i will
    i have keyboard and protected mode working for now, and a basic shell
    next order of business is to implement networking ( TCP,UDP, basic downloader)
    then i have to implement multiprocessing
    try to make open,write,
    basic fat format
    web browser shit
    arp 
    udp
    :D

*/

void main() {


    clearsc();
    
    
    isr_install();
    kprintf("\nWelcome to LiberalOS!");
    asm volatile("sti");
    init_keyboard();
    pci_init();
    
    kprintf("\nAllocating page frame for kernel...");
    init_map();
    
    void *v = allocate_page_frame(OS_FUNC,8192);
    kprintf("\nSuccess.");
 
    rtl8139_init();
    
    ethernet_init();
    
    char *hello ="hello\n";
    uint8_t *ip = {192,168,9,109};
    udp_send(ip,59999,5959,(void*)hello,7);
    kprintf("\nConclusion: i need to learn more. i suck at this.");
    


}   