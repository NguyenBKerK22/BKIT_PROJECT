#ifndef __CRC16_H__
#define __CRC16_H__

#include <stdint.h>
#include <stdlib.h>

extern uint16_t crc16_lookup_table[256];

uint16_t crc16(const uint8_t *data, uint16_t num_bytes);

#endif /* __CRC16_H__ */
