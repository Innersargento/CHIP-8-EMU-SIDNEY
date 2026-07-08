#include "../include/utils.h"

uint16_t get_address(uint16_t instruction){

    return instruction & NNN;
}

uint16_t get_immediate_number(uint16_t instruction){

    return instruction & NN;
}

uint16_t get_vx(uint16_t instruction){

    return (instruction & X) >> 8;
}

uint16_t get_vy(uint16_t instruction){

    return (instruction & Y) >> 4;
}

uint8_t get_last_nibble(uint16_t instruction){

    return instruction & N;
}

uint8_t get_first_nibble(uint16_t instruction){

    return (instruction & OPCODE) >> 12;
}