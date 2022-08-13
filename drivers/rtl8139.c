#include "../kernel/stdlib.h"
#include "../kernel/stdio.h"
#include <stdint.h>
#include "../kernel/string.h"
#include "./rtl8139.h"
#include "./pci.h"
#include "../kernel/paging.h"
#include "../kernel/ethernet.h"

rtl_device_t *rtl8139={0};

uint8_t TSAD_array[4] = {0x20, 0x24, 0x28, 0x2C};
uint8_t TSD_array[4] = {0x10, 0x14, 0x18, 0x1C};

uint32_t packet_ptr;


void rtl8139_receive(){
   uint16_t *t = (uint16_t*) (rtl8139->rxbuff+packet_ptr);

   uint16_t length = *(t+1);

   t=t+2;

   void* packet = malloc(length);
   _memcpy(packet,t,length);
   ethernet_handle(packet,length);
   packet_ptr=(packet_ptr+length+4+3)  & RX_READ_POINTER_MASK;
   if(packet_ptr> RX_BUF_SIZE){
      packet_ptr-=RX_BUF_SIZE;
   }
   io_write(rtl8139->io_base+CAPR,packet_ptr-0x10);
   free(packet);

}

void rtl_handler(registers_t *reg){

   uint16_t stat =io_read(rtl8139->io_base+ISR);

   if(stat & TOK){
      kprintf("\n[E] Packet sent.");
   }
   if(stat & ROK){
      kprintf("\n[E] New packet received.");
      rtl8139_receive();
   }
   io_write(rtl8139->io_base+0x3e,0x5);//clear bits and reset device.
   
}

void fetch_mac(){

   uint32_t part1 = io_lread(rtl8139->io_base+0x00);// read first part of mac addr 
   uint32_t part2 = io_lread(rtl8139->io_base+0x04); // read last part of mac addr

   rtl8139->macaddr[0] = part1 % (part1>>8);
   rtl8139->macaddr[1] = part1>>8 & part1>>24; //11
   rtl8139->macaddr[2] =part1>>16 & part1>>24; // 22 
   rtl8139->macaddr[3] =part1>>24; // 33
   rtl8139->macaddr[4] = part2>>8 & part2; // 44
   rtl8139->macaddr[5] = part2>>8; // 55
   
   kprintf("\nRTL8139 MAC:   %x : %x : %x : %x : %x : %x",rtl8139->macaddr[0],rtl8139->macaddr[1],rtl8139->macaddr[2],rtl8139->macaddr[3],rtl8139->macaddr[4],rtl8139->macaddr[5]);  
   
}
void rtl8139_send(void * data, uint32_t len){
  
   void *faller = malloc(len+sizeof(ethernet_frame_t));
  _memcpy(data,faller,len+sizeof(ethernet_frame_t));
   io_lwrite(rtl8139->io_base+TSAD_array[rtl8139->tx_sock],(uint32_t)faller);
   io_lwrite(rtl8139->io_base+TSD_array[rtl8139->tx_sock++],(uint32_t)len);

   if(rtl8139->tx_sock>3){
      rtl8139->tx_sock=0;
   }

   

}
void get_mac_addr(uint8_t *buf){
   _memcpy(buf,rtl8139->macaddr,6);
}
void rtl8139_init(){

   //load RTL8139 in QEMU!! <- done 

   pci_device_t *rtl = pci_find_device(VENDOR_ID,DEVICE_ID);
   uint32_t ret_val = pci_read(rtl->bus,rtl->slot,rtl->func,PCI_BAR0); 
   

   rtl8139->bar = ret_val& 0x1;
   rtl8139->io_base = ret_val & (~0x3);
   rtl8139->mem_base = ret_val & (~0xf);
   rtl8139->tx_sock = 0;//default sock 
   
   //enable pci bus mastering ,( for some shit )
   uint32_t rec = pci_read(rtl->bus,rtl->slot,rtl->func,PCI_COMMAND);
   if(!(rec & (1<<2))){
      rec |= (1<<2);
      pci_write(rtl->bus,rtl->slot,rtl->func,PCI_COMMAND,rec);
   }
   //wake device up from "low power" mode
   io_bwrite(rtl8139->io_base+0x52,0x0);

   uint32_t ioaddr = rtl8139->io_base;

   //resetting device
   io_bwrite(ioaddr+CMD,0x10);
   while((io_bread(ioaddr+CMD) & 0x10)!=0){}

   
   //device buffer (RX)
   rtl8139->rxbuff = malloc(8192+16+1500); // how do we do malloc good?

   
   _memset(rtl8139->rxbuff,0x0,8192+16+1500);
   
   io_lwrite(ioaddr+RBSTART,(int)rtl8139->rxbuff);
   
   io_lwrite(ioaddr+IMR,0x0005);
   
   io_lwrite(ioaddr+0x44,0xf | (1 << 7));
   io_lwrite(ioaddr+CMD,0x0c);

   uint32_t irq = pci_read(rtl->bus,rtl->slot,rtl->func,PCI_INTERRUPT_LINE);

   register_ihandle(irq+32,rtl_handler);
   
   fetch_mac();
   
   kprintf("\nRTL8139 initialized.");
   
}