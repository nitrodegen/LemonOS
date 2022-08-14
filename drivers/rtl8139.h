#include "../kernel/stdlib.h"
#include "../kernel/stdio.h"
#include <stdint.h>
#include "../kernel/string.h"
#include "./pci.h"
#define u16 uint16_t
//registers:
#define MAC0_5 0x00 
#define MAC0_7  0x08
#define RBSTART 0x30
#define CMD 0x37
#define IMR 0x3c
#define ISR 0x3e
#define NUMBER_TX 4
//The PCI vendor ID is 0x10EC and the device ID is 0x8139.
#define VENDOR_ID 0x10EC
#define DEVICE_ID 0x8139
#define Bit(shift)         (1 << shift)
#define ETH_ZLEN  60 // Min. octets in frame sans FC
static int max_interrupt_work = 20;
static int multicast_filter_limit = 32;
#define RX_BUF_LEN_IDX  2     // 0=8K, 1=16K, 2=32K, 3=64K
#define TX_BUF_SIZE 1536
#define TX_FIFO_THRESH 256  // In bytes, rounded down to 32 byte units
#define RX_FIFO_THRESH  4   // Rx buffer level before first PCI xfer
#define RX_DMA_BURST    4   // Maximum PCI burst, '4' is 256 bytes
#define TX_DMA_BURST    4   // Calculate as 16 << val
#define TX_TIMEOUT  (6*HZ)
#define PKT_BUF_SZ    1536
#define RX_BUF_SIZE 8192
#define CAPR 0x38
#define RX_READ_POINTER_MASK (~3)
#define ROK                 (1<<0)
#define RER                 (1<<1)
#define TOK     (1<<2)
#define TER     (1<<3)
#define TX_TOK  (1<<15)
static u16 COMMAND_BUFFER_EMPTY = Bit(0);
static u16 COMMAND_TX_ENABLE = Bit(2);
static u16 COMMAND_RX_ENABLE = Bit(3);
static u16 COMMAND_RESET = Bit(4);

static  u16 INTERRUPT_MASK_RX_OK = Bit(0);
static  u16 INTERRUPT_MASK_RX_ERROR = Bit(1);
static  u16 INTERRUPT_MASK_TX_OK = Bit(2);
enum RTL8139_registers {
  MAG0             = 0x00,       // Ethernet hardware address
  MAR0             = 0x08,       // Multicast filter
  TxStatus0        = 0x10,       // Transmit status (Four 32bit registers)
  TxAddr0          = 0x20,       // Tx descriptors (also four 32bit)
  RxBuf            = 0x30,
  RxEarlyCnt       = 0x34,
  RxEarlyStatus    = 0x36,
  ChipCmd          = 0x37,
  RxBufPtr         = 0x38,
  RxBufAddr        = 0x3A,
  IntrMask         = 0x3C,
  IntrStatus       = 0x3E,
  TxConfig         = 0x40,
  RxConfig         = 0x44,
  Timer            = 0x48,        // A general-purpose counter
  RxMissed         = 0x4C,        // 24 bits valid, write clears
  Cfg9346          = 0x50,
  Config0          = 0x51,
  Config1          = 0x52,
  FlashReg         = 0x54,
  GPPinData        = 0x58,
  GPPinDir         = 0x59,
  MII_SMI          = 0x5A,
  HltClk           = 0x5B,
  MultiIntr        = 0x5C,
  TxSummary        = 0x60,
  MII_BMCR         = 0x62,
  MII_BMSR         = 0x64,
  NWayAdvert       = 0x66,
  NWayLPAR         = 0x68,
  NWayExpansion    = 0x6A,

  // Undocumented registers, but required for proper operation
  FIFOTMS          = 0x70,        // FIFO Control and test
  CSCR             = 0x74,        // Chip Status and Configuration Register
  PARA78           = 0x78,
  PARA7c           = 0x7c,        // Magic transceiver parameter register
};
// The IO base address and the IRQ number for the device is taken from PCI configuration.


typedef struct{

    uint32_t bar;
    uint32_t io_base;
    uint32_t mem_base;
    int eeprom;
    char *rxbuff;//receiver buffer (buffer where we receive our packets)
    int tx_sock;
    unsigned int rx_config;
    uint32_t macaddr[6];

}rtl_device_t;

void fetch_mac();
void get_mac_addr(uint8_t *buf);
void rtl8139_receive();
void rtl8139_send(void * data,uint32_t len);
static void rtl_handler(registers_t *reg);
void rtl8139_init();
uint32_t get_addr();

