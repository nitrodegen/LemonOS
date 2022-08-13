#include "../kernel/stdio.h"
#include "../kernel/string.h"
#include "../kernel/stdlib.h"
#include "../cpu/idt.h"
#include "../cpu/isr.h"
#include "./pci.h"
//Buses are numbered 0-255. Slots are numbered 0-31. Functions are numbered 0-7.
pci_opt_t irqs[32];
void main_handler(registers_t *reg){
    kprintf("\npci interrupt.");
}


int pci_read(int bus, int slot ,int func, int type){
        
    int temp =0; 
    int address  = (int )((bus<<16) | (slot<<11) | (func<<8) | (type & 0xFC) | ((uint32_t)0x80000000)) ;
    
    io_lwrite(PCI_ADDRESS,address);
    temp =(io_lread(PCI_DATA)>>((type & 2)*8)) &0xFFFF;
    return temp ; 
}

pci_device_t* find_dev(int bus,int slot,int func){
    
    pci_device_t *device={0};
    _memset(device,0,4096);

    device->bus = bus;
    device->slot = slot;
    device->func = func;

    int vendi = pci_read(bus,slot,func,PCI_VENDOR_ID);
    int devi =  pci_read(bus,slot,func,PCI_DEVICE_ID);
   // dev.vendor = vendi;
    //dev.device = devi;
    device->vendor = vendi;
    device->device = devi;
    return device;
}

//Buses are numbered 0-255. Slots are numbered 0-31. Functions are numbered 0-7
pci_device_t* pci_find_device(uint32_t vendor,uint32_t device_id){
    /*
        we have to scan through all buses, devices, functions , slots

        so lets do it.
        //first i will search through all slots;
    */
   pci_device_t* final_device ={0};
   for(int bus =0;bus<MAX_BUS;bus++){
        for(int slot =0;slot<DEVICE_PER_BUS;slot++){
            for(int func = 0 ;func<FUNCTION_PER_DEVICE;func++){
                pci_device_t *hardware = find_dev(bus,slot,func);
                if(hardware->vendor == vendor && hardware->device== device_id){
                    final_device = hardware;
                    
                    return hardware;
                }

            }
        }
        //if bus =0 per every slot , loop through functions 
   }
   return final_device;
}
void pci_search_all(){
    clearsc();
    reset_args();
    for(int bus =0;bus<MAX_BUS;bus++){
        for(int slot =0;slot<DEVICE_PER_BUS;slot++){
            for(int func = 0 ;func<FUNCTION_PER_DEVICE;func++){
                pci_device_t *hardware = find_dev(bus,slot,func);
                if(hardware->vendor != 0xFFFF && hardware->device !=0xFFFF){
                    kprintf("\n Device [*] Vendor ID : 0x%x    Device ID: 0x%x ",hardware->vendor,hardware->device);
                }
            }
        }
        //if bus =0 per every slot , loop through functions 
   }
}

void pci_init(){
    
    pci_device_t* device = pci_find_device(INTEL_HUB_VENDOR,INTEL_HUB_DEVICE);
 
    kprintf("\n[*] Intel USB HUB Vendor:%d    DeviceID:%d",device->vendor,device->device);
    if(device->vendor == INTEL_HUB_VENDOR && device->device == INTEL_HUB_DEVICE){
        kprintf("\nPCI Initialized successfully.");
        
    }
    else{
        kprintf("\nPCI failed to initialize.");
    }

}
void pci_write(int bus , int slot, int func ,int type,int value){
    int address  = (int )((bus<<16) | (slot<<11) | (func<<8) | (type & 0xFC) | ((uint32_t)0x80000000)) ;
    io_lwrite(PCI_ADDRESS,address);
    io_lwrite(PCI_DATA,value);
}