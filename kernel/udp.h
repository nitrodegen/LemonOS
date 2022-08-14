#include "./stdio.h"
#include "./ethernet.h"
#include "./ip.h"
#include "./paging.h"

#ifndef UDP_H
#define UDP_H

typedef struct udp_packet {
    uint16_t src_port;
    uint16_t dst_port;
    uint16_t length;
    uint16_t checksum;
    uint16_t data[]

} __attribute__((packed)) udp_packet_t ;


void udp_send(uint16_t *dst_ip , uint16_t src_port , uint16_t dst_port ,void *data ,int len);
void udp_handle(udp_packet_t * packet);

#endif 
