#pragma once

#include <stdint.h>

unsigned char io_read(uint16_t port);

void io_write(uint16_t port, uint8_t data);

unsigned short io_wrdread(uint16_t port);

void io_wrdwrite(uint16_t port, uint16_t data);
