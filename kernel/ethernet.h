#pragma once
#include "./stdlib.h"
#include "./stdio.h"
#include <stdint.h>
#include "./string.h"
#include "./paging.h"
#include "arp.h"



#define ETHERNET_TYPE_ARP 0x0806
#define ETHERNET_TYPE_IP  0x0800

#define HARDWARE_TYPE_ETHERNET 0x01


typedef struct ethframe {
    uint8_t src_addr[6];
    uint8_t dst_addr[6];
    uint16_t type;
    uint8_t data[];
}__attribute__((packed)) ethernet_frame_t;


uint16_t ntohs(uint16_t netshort);
uint16_t htons(uint16_t netshort);

uint8_t htonb(uint8_t byte, int num_bits);
uint8_t ntohb(uint8_t byte, int num_bits);


void ethernet_handle(ethernet_frame_t * packet, uint16_t length);
void ethernet_send(uint8_t *dstmac , uint8_t *data,int len , uint16_t protocol);

void ethernet_init();