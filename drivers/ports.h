#pragma once

#include <stdint.h>

unsigned char io_read(uint16_t port);

void io_write(uint16_t port, uint8_t data);

unsigned short io_wrdread(uint16_t port);

void io_wrdwrite(uint16_t port, uint16_t data);
uint32_t  io_bread(uint32_t port);
void io_bwrite(uint32_t port ,uint8_t data);
void io_lwrite(uint32_t port ,unsigned long data);

uint32_t  io_lread(uint32_t port);
