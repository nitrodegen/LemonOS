#include "./stdlib.h"
#include "./stdio.h"
#include <stdint.h>
#include "./string.h"
#include "../drivers/rtl8139.h"
#include "../drivers/pci.h"
#include "./paging.h"
#include "./ethernet.h"
#include "./ip.h"

int curr_table;
int table_size;

uint8_t broadcast_mac[] = {0xff,0xff,0xff,0xff,0xff,0xff};
arp_table_entry_t arp_table[512];



uint16_t  ntohs(uint16_t netshort){
    uint32_t first = *((uint8_t*)(&netshort));
    uint32_t second = *((uint8_t*)(&netshort)+1);
    return (first << 8) | (second);
}


uint16_t  htons(uint16_t netshort){
    uint32_t first = *((uint8_t*)(&netshort));
    uint32_t second = *((uint8_t*)(&netshort)+1);
    return (first << 8) | (second);
}
uint8_t ntohb(uint8_t byte, int num_bits) {
    num_bits = 8-num_bits;

    uint8_t t = byte<< (8-num_bits);
    return  t | ( byte>>num_bits);  
}
uint8_t htonb(uint8_t byte, int num_bits) {
    uint8_t t = byte<< (8-num_bits);
    return  t | ( byte>>num_bits);
}



void ethernet_send(uint8_t dstmac[6] , uint8_t *data,int len , uint16_t protocol){
    uint8_t srcmac[6];
    get_mac_addr(srcmac);
    ethernet_frame_t *frame = (ethernet_frame_t*)malloc(sizeof(ethernet_frame_t)+len);
    void *frame_data = (void*)frame+sizeof(ethernet_frame_t);
    
    _memcpy(frame->src_addr,srcmac,6);
    _memcpy(frame->dst_addr,dstmac,6);
    
    _memcpy(frame_data,data,len);

    frame->type = htons(protocol);
    //kprintf("\n%d" ,protocol);
    rtl8139_send(frame,sizeof(ethernet_frame_t)+len);
    //free(frame); // tip for me in the future : this will fuck up sending.

    
}
//TODO: implement send packet arp


void arp_send_packet(uint8_t mac[6], uint8_t * ip){
    arp_packet_t *packet = malloc(sizeof(arp_packet_t));

    get_mac_addr(packet->src_mac);
   
    packet->src_ip[0] = 10 ;
    packet->src_ip[1] = 2 ;
    packet->src_ip[2] = 0 ;
    packet->src_ip[3] = 15 ;
    _memcpy(packet->dst_ip,ip,4);
    _memcpy(packet->dst_mac,mac,6);
    packet->opcode = htons(ARP_REQUEST);
    packet->ip_len = 4;
    packet->mac_len =6 ; 
    packet->hardware_type = htons(HARDWARE_TYPE_ETHERNET);
    packet->protocol = htons(ETHERNET_TYPE_IP);
    
    ethernet_send(broadcast_mac,(uint8_t*)packet,sizeof(arp_packet_t),ETHERNET_TYPE_ARP);
    
   free(packet);//could cause problems

}
void arp_handle(arp_packet_t* packet,int len){
    char dst_ip[4];
    char dst_mac[6];
    _memcpy(dst_ip,packet->src_ip,4);
    _memcpy(dst_mac,packet->src_mac,6);
    if(ntohs(packet->opcode) == ARP_REQUEST){
        kprintf("\n[E] ARP Request received.");
        uint32_t my_ip =  0x0e02000a;
        
        if(_memcmp(packet->dst_ip,&my_ip,4)){
            get_mac_addr(packet->src_mac);
            packet->src_ip[0] = 10 ;
            packet->src_ip[1] = 2 ;
            packet->src_ip[2] = 0 ;
            packet->src_ip[3] = 15 ;
            //tried to hardcode ip to: 192.168.9.120

            _memcpy(packet->dst_mac,dst_mac,6);
            _memcpy(packet->dst_ip,dst_ip,4);
            packet->opcode = htons(ARP_REPLY);

            packet->mac_len = 6;
            packet->ip_len = 4;
            packet->hardware_type = htons(HARDWARE_TYPE_ETHERNET);
            packet->protocol = htons(ETHERNET_TYPE_IP);
            ethernet_send(dst_mac,packet,sizeof(arp_packet_t),ETHERNET_TYPE_ARP);

        }
        
        
    }
    _memcpy(&arp_table[curr_table].ip_addr , dst_ip,4);
    _memcpy(&arp_table[curr_table].mac_addr , dst_mac,6);
    if(table_size < 512){
        table_size++;
    }
    if(table_size>=512){
        table_size =0;
    }

}



void ethernet_handle(ethernet_frame_t *packet , uint16_t length){
    void *data = (void*)packet+sizeof(ethernet_frame_t);
    int datalen = length - sizeof(ethernet_frame_t);
    if(ntohs(packet->type) == ETHERNET_TYPE_ARP){
        
        arp_handle(data,length);
    }
    if(ntohs(packet->type) == ETHERNET_TYPE_IP){
        kprintf("\nIP packet not implemented .");
        ip_handle(data,length);
    }
}
void arp_lookup(uint8_t *mac,uint8_t *ip){
    _memcpy(&arp_table[curr_table].ip_addr,ip,4);
    _memcpy(&arp_table[curr_table].mac_addr,mac,6);

    if(table_size < 512){
        table_size++;
    }
    if(table_size>=512){
        table_size=0;
    }

}

int arp_search(uint8_t *ret, uint8_t *ip ){
    uint32_t *entry =*((uint32_t*)(ip));
    for(int i =0;i<512;i++){
        if(arp_table[i].ip_addr == entry){
            _memcpy(ret,&arp_table[i].mac_addr,6);
            return 1;
        }
    }
    return 0;
}
void arp_init(){

    uint8_t mac[6];
    uint8_t ip[4];
    _memset(&mac,0xff,6);
    _memset(&ip,0xff,4);
    arp_lookup(mac,ip);
}

void ethernet_init(){
    arp_init();
}




