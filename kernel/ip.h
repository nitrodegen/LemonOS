#pragma once
#include "stdio.h"
#include "./ethernet.h"
#include "./paging.h"

#ifndef IP_H
#define IP_H
#define IP_IPV4 4

#define IP_PACKET_NO_FRAGMENT 2
#define IP_IS_LAST_FRAGMENT 4

#define PROTOCOL_UDP 17
#define PROTOCOL_TCP 6

typedef struct{
    char version_ptr[0];
    uint16_t version;
    uint8_t tos;
    uint16_t len;
    uint16_t id;
    uint16_t ihl;
    uint8_t ttl;
    uint32_t src_ip[4];
    uint32_t dst_ip[4];
    char flags_fragment_ptr[0];
    uint8_t flags:3;
    uint8_t fragment_offset_high:5;
    uint8_t fragment_offset_low;
    uint8_t protocol;
    uint16_t header_checksum;
    char data[];
}ip_packet_t;

uint16_t checksum(ip_packet_t *packet);

void get_ip_str(char *str, uint8_t *ip);

void ip_send(uint8_t *dst_ip,void *data,int len);

void ip_handle(ip_packet_t *packet,int len );// we are obviously providing ip_packet_t to our handle , to handle and decode that packet.



#endif