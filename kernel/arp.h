
#include "./ethernet.h"
#include "./stdio.h"
#include <stdint.h>
#include <stddef.h>



#define ARP_REQUEST 1
#define ARP_REPLY 2

typedef struct {
    uint16_t hardware_type;
    uint16_t protocol;
    uint8_t mac_len;
    uint8_t ip_len;
    uint16_t opcode;
    uint8_t src_mac[6];
    uint8_t src_ip[4];
    uint8_t dst_mac[6];
    uint8_t dst_ip[4];

} __attribute__((packed)) arp_packet_t;

typedef struct  {
    uint32_t* ip_addr;
    uint32_t* mac_addr; // DO NOT USE uint64_t (NO MEMORY AND NO SUPPORT FOR IT!) (NOT A 64BIT SYSTEM! THANKS.) - gavrilo
    
} arp_table_entry_t;

void arp_send_packet(uint8_t * mac, uint8_t * ip);
void arp_handle(arp_packet_t* packet , int len );
void arp_lookup(uint8_t *mac,uint8_t *ip);
int arp_search(uint8_t *ret, uint8_t *ip);
void arp_init();




