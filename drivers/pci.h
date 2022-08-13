#ifndef PCI_H
#define PCI_H
#include "../kernel/stdio.h"
#include "../kernel/string.h"
#include "../kernel/stdlib.h"
#include "../cpu/idt.h"
#include "../cpu/isr.h"
#include "./ports.h"


// Ports
#define INTEL_HUB_VENDOR 0x8086
#define INTEL_HUB_DEVICE 0x1237
#define PCI_ADDRESS  0xCF8
#define PCI_DATA     0xCFC

// Config Address Register

// Offset
#define PCI_VENDOR_ID            0x00
#define PCI_DEVICE_ID            0x02
#define PCI_COMMAND              0x04
#define PCI_STATUS               0x06
#define PCI_REVISION_ID          0x08
#define PCI_PROG_IF              0x09
#define PCI_SUBCLASS             0x0a
#define PCI_CLASS                0x0b
#define PCI_CACHE_LINE_SIZE      0x0c
#define PCI_LATENCY_TIMER        0x0d
#define PCI_HEADER_TYPE          0x0e
#define PCI_BIST                 0x0f
#define PCI_BAR0                 0x10
#define PCI_BAR1                 0x14
#define PCI_BAR2                 0x18
#define PCI_BAR3                 0x1C
#define PCI_BAR4                 0x20
#define PCI_BAR5                 0x24
#define PCI_INTERRUPT_LINE       0x3C
#define PCI_SECONDARY_BUS        0x09

// Device type
#define PCI_HEADER_TYPE_DEVICE  0
#define PCI_HEADER_TYPE_BRIDGE  1
#define PCI_HEADER_TYPE_CARDBUS 2
#define PCI_TYPE_BRIDGE 0x0604
#define PCI_TYPE_SATA   0x0106
#define PCI_NONE 0xFFFF
#define MAX_BUS 256

#define DEVICE_PER_BUS           32
#define FUNCTION_PER_DEVICE      32 // should be  from 0 to 7 (0-7)

typedef struct{
    int vendor;
    int device;
    int bus;
    int slot;
    int func;
    int type;
    void* handler;
}pci_opt_t;

typedef struct{
    int vendor;
    int device;
    int bus;
    int slot;
    int func;
}pci_device_t;

void main_handler(registers_t *reg);
void pci_init();
int pci_read(int bus , int slot, int func ,int type);
void pci_write(int bus , int slot, int func ,int type,int value);

pci_device_t* pci_find_device(uint32_t vendor,uint32_t device_id);
void pci_search_all();
pci_device_t* find_dev(int bus,int slot,int func);

#endif
