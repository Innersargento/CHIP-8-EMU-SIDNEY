#ifndef UTILS_H
#define UTILS_H

#define NNN    0x0FFF
#define NN     0x00FF
#define N      0X000F
#define Y      0x00F0
#define X      0x0F00
#define OPCODE 0xF000

#include <stdint.h>

uint16_t  get_address(uint16_t instruction);

uint16_t  get_immediate_number(uint16_t instruction);

uint16_t  get_vx(uint16_t instruction);

uint16_t  get_vy(uint16_t instruction);

uint8_t   get_last_nibble(uint16_t instruction);

uint8_t   get_first_nibble(uint16_t instruction);

#endif