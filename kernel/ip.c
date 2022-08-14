
#include "stdio.h"
#include "./ethernet.h"

#include "./paging.h"
#include "./ip.h"
#include "./udp.h"




uint8_t my_ip[]=  { 10,0,2,15};

uint8_t zero_h[]=  { 0,0,0,0,0,0};


uint16_t checksum(ip_packet_t *packet){

    int len = sizeof(ip_packet_t) /2;
    uint16_t *part = (uint16_t*) packet;
    uint8_t *part2 = (uint8_t*) packet;
    uint32_t sum =0; 
    for(int i =0;i<len;i++){
        sum+=ntohs(part[i]);
    }

    uint32_t carry = sum >> 16;
    sum = sum & 0x0000ffff;
    sum = sum + carry;
    uint16_t ret = ~sum;
    return ret;
}

void ip_send(uint8_t *dst_ip,void *data ,int len ){
    int arp = 3;
    uint8_t *cpy = dst_ip;

    ip_packet_t * packet = malloc(sizeof(ip_packet_t)+len);
    _memset(packet,0,sizeof(ip_packet_t));
    packet->version = IP_IPV4;
    packet->ihl = 5;
    packet->tos = 0;
    packet->len= sizeof(ip_packet_t)+len;
    packet->id = 0;
    packet->flags = 0;
    packet->fragment_offset_high =0;
    packet->fragment_offset_low =0;
    packet->ttl = 64;
    packet->protocol = PROTOCOL_UDP;
    //missing: get ip from dhcp

    dst_ip[0] = 192;
    dst_ip[1] = 168;
    dst_ip[2] = 9;
    dst_ip[3] = 109;

    uint8_t my_ip[]=  { 10,0,2,15};

    kprintf("\nhehe: %d",dst_ip[1]);
    _memcpy(packet->src_ip,my_ip,4);
    _memcpy(packet->dst_ip,dst_ip,4);
    void *datapack =(void*)packet+packet->ihl*4;
    _memcpy(datapack,data,len);

    *((uint8_t*)(&packet->version_ptr)) = htonb(*((uint8_t*)(&packet->version_ptr)),4);
    *((uint8_t*)(packet->flags_fragment_ptr)) = htonb(*((uint8_t*)(packet->flags_fragment_ptr)),3);

    packet->len = htons(sizeof(ip_packet_t)+len);


    packet->header_checksum =0;
    packet->header_checksum = htons(checksum(packet));


    uint8_t dst_mac[6];

    while(!arp_search(dst_mac,dst_ip)){
        if(arp != 0){
            arp--;
            arp_send_packet(zero_h,dst_ip);
        }
    }
    kprintf("\nMAC %d  %d  %d  %d  %d  %d   ",dst_mac[0],dst_mac[1],dst_mac[2],dst_mac[3],dst_mac[4],dst_mac[5]);
    
    ethernet_send(dst_mac,(uint8_t*)packet,htons(packet->len),ETHERNET_TYPE_IP);
    kprintf("\n[E] Sent IP Packet.");

    free(packet);

}

void ip_handle(ip_packet_t *packet, int length){
    *((uint8_t*)(&packet->version_ptr)) = ntohb(*((uint8_t*)(&packet->version_ptr)), 4);
    *((uint8_t*)(packet->flags_fragment_ptr)) = ntohb(*((uint8_t*)(packet->flags_fragment_ptr)), 3);
    if(packet->version = IP_IPV4){
        void *ptr =(void*)packet+packet->ihl*4;
        if(packet->protocol == PROTOCOL_UDP){
            udp_handle(ptr);
        }

    }
}