#include "./stdio.h"
#include "./ethernet.h"
#include "./ip.h"
#include "./paging.h"
#include "./udp.h"

void udp_send(uint16_t *dst_ip , uint16_t src_port , uint16_t dst_port ,void *data ,int len){
    dst_ip[0] = 192;
    dst_ip[1] = 168;
    dst_ip[2] = 9;
    dst_ip[3] = 109;



    kprintf("\n  %d" ,dst_ip[0]);

    int leng = sizeof(udp_packet_t)+len;

    udp_packet_t *packet = malloc(leng);

    _memset(packet,0,leng);

    packet->src_port = htons(src_port);
    packet->dst_port = htons(dst_port);
    packet->length= htons(leng);
    packet->checksum = 0;// BRO IF IT IS OPTIONAL , SUCK IT.
    kprintf("\n UDP :%s",data); //debugging 
    _memcpy((void*)packet+sizeof(udp_packet_t),data,len);
    kprintf("\n  %d   %d " ,dst_ip[0],packet->dst_port);


    ip_send(dst_ip,packet,leng);
    free(packet);
}


void udp_handle(udp_packet_t*packet){

    uint16_t port = htons(packet->dst_port);
    uint16_t ip = htons(packet->length);

    uint16_t data = packet->data;

    kprintf("\nReceived UDP packet. port: %d  ",port);
}

