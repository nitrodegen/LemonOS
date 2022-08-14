#include "../kernel/stdlib.h"
#include "../kernel/stdio.h"
#include <stdint.h>
#include "../kernel/string.h"
#include "./rtl8139.h"
#include "./pci.h"
#include "../kernel/paging.h"
#include "../kernel/ethernet.h"
#define RCR_AAP  (1 << 0) /* Accept All Packets */
#define RCR_APM  (1 << 1) /* Accept Physical Match Packets */
#define RCR_AM   (1 << 2) /* Accept Multicast Packets */
#define RCR_AB   (1 << 3) /* Accept Broadcast Packets */
#define RCR_WRAP (1 << 7) /* Wrap packets too long */

rtl_device_t *rtl8139;
int bus=0;
int func =0 ;
int slot = 0 ;
int io_addr =0 ;
uint8_t mac_addr[6];
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
   //free(packet);

}



void rtl_handler(registers_t *reg){
    kprintf("\nHHE");

  
   uint16_t stat =io_read(io_addr+ISR);
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
   
   //52:54:00:12:34:56

   uint32_t part1 = io_lread(io_addr+0x00);// read first part of mac addr 
   uint32_t part2 = io_lread(io_addr+0x04); // read last part of mac addr
   rtl8139->macaddr[0] = part1 % (part1>>8);
   rtl8139->macaddr[1] = (part1>>8)   % (part1>>16); //11
   rtl8139->macaddr[2] =(part1>>16 )% (part1>>24); // 22 
   rtl8139->macaddr[3] =part1>>24; // 33
   rtl8139->macaddr[4] = part2 %(part2>>8); // 44
   rtl8139->macaddr[5] = part2>>8; // 55

   for(int i =0;i<6;i++){
      mac_addr[i] = rtl8139->macaddr[i];
   }
   
   kprintf("\nMAC: %x : %x : %x : %x : %x : %x",rtl8139->macaddr[0],rtl8139->macaddr[1],rtl8139->macaddr[2],rtl8139->macaddr[3],rtl8139->macaddr[4],rtl8139->macaddr[5]);  
   
}
void rtl8139_send(void * data, uint32_t len){
   uint8_t *val = (uint8_t*)data;

   void *faller = malloc(len+sizeof(ethernet_frame_t));
  _memcpy(faller,data,len+sizeof(ethernet_frame_t));

   io_lwrite(io_addr+TSAD_array[rtl8139->tx_sock],(uint32_t)faller);
   io_lwrite(io_addr+TSD_array[rtl8139->tx_sock++],(uint32_t)len);

   if(rtl8139->tx_sock>3){
      rtl8139->tx_sock=0;
   }
   //free(faller);
   //kprintf("\n[E] RTL8139 packet sent.");
}
void get_mac_addr(uint8_t *buf){
   _memcpy(buf,mac_addr,6);
}

uint32_t
pci_config_read_dword(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset)
{
	uint32_t address =
		((uint32_t)1 << 31)          // enabled
		| ((uint32_t)bus << 16)      // bus number
		| ((uint32_t)slot << 11)     // slot number
		| ((uint32_t)func << 8)      // function number
		| ((uint32_t)offset & 0xfc); // register number

	io_lwrite(PCI_ADDRESS, address);
	return io_bread(PCI_DATA);
}

void rtl8139_init(){

   kprintf("\n********* RTL8139 init **********");

  // char *buff = malloc(8192+16+1500);
   //load RTL8139 in QEMU!! <- done 

   pci_device_t *rtl = pci_find_device(VENDOR_ID,DEVICE_ID);
   bus = rtl->bus;
   slot = rtl->slot;
   func = rtl->func;

   kprintf("\nrtl8139_t: %d  %d  %d",rtl->bus,rtl->slot,rtl->func);
   //enable pci bus mastering ,( for some shit )
   uint32_t rec = pci_read(bus,slot,func,PCI_COMMAND);
   if(!(rec & (1<<2))){
      rec |= (1<<2);
      pci_write(bus,slot,func,PCI_COMMAND,rec);
   }


   uint32_t ret_val = pci_read(bus,slot,func,PCI_BAR0); 

   rtl8139->bar = ret_val& 0x1;
   rtl8139->io_base = ret_val & (~0x3);
   rtl8139->mem_base = ret_val & (~0xf);
   rtl8139->tx_sock = 0;//default sock 
   io_addr = rtl8139->io_base;

   //wake device up from "low power" mode
   io_bwrite(io_addr+0x52,0x0);

   //resetting device
   io_bwrite(io_addr+CMD,0x10);
   while((io_bread(io_addr+CMD) & 0x10)!=0){}
   io_bwrite(io_addr+80,0xC0);

   fetch_mac();
   
   kprintf("\nglobl: %d  %d  %d",bus,slot,func);
   io_bwrite(io_addr+CMD,8 |  4 );

   io_lwrite(io_addr+0x44, ((4<<8)));


   io_lwrite(io_addr+64,  (4<<8));

   io_bwrite(io_addr+80,0x00);
   io_lwrite(io_addr+RBSTART,(unsigned long )rtl8139->rxbuff);

   io_lwrite(io_addr+76,0);

   io_lwrite(io_addr+0x44, 0xf | (1<<7));


   io_bwrite(io_addr+CMD,8 |  4 );

   io_write(io_addr+IMR,32768 | 16384 | 32 | 16  | 64 | 8 | 4 | 2 | 1);


   io_bwrite(io_addr+CMD,0x0c);






   /*   
   rtl8139->rxbuff=malloc(8192+16);
   _memset(rtl8139->rxbuff,0x0,8192+16);


   io_lwrite(io_addr+RBSTART,(uint32_t)rtl8139->rxbuff);
   io_write(io_addr+0x38,0x0);
   io_write(io_addr+0x3a,0x0);



   io_write(io_addr+IMR, 0x0005);// change to 0x00005  

      io_lwrite(io_addr+0x44,0xf | (1 << 7));
   io_bwrite(io_addr+CMD,0x0c);


*/
   uint32_t irq = pci_config_read_dword(bus,slot,func,PCI_INTERRUPT_LINE);
   kprintf("\nRTL IRQ:%d",irq);
   
   register_ihandle(irq+32,rtl_handler);
   
   //fetch_mac();
      
   kprintf("\n****** RTL8139 initialized. *****");
   
   
}