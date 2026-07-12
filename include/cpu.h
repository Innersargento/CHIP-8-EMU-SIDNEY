#ifndef CPU_H
#define CPU_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../include/utils.h"
#include <time.h>
#include <SDL3/SDL.h>

#define START_ADDRESS       0x200
#define FONT_START_ADDRESS   0x50
#define MEMORY_SIZE          4096
#define FONT_SIZE              80
#define NEXT_INSTRUCTION    0x002
#define STACK_SIZE             16
#define STACK_EMPTY            -1
#define HEIGHT                 32
#define WIDTH                  64
#define TIMER_HZ               60
#define TIMER_INTERVAL_MS      (1000.0 / TIMER_HZ)   // ~16.666 ms por tique
#define CPU_HZ                660
#define CYCLES_PER_TICK        (CPU_HZ / TIMER_HZ)   // = 11 instruções por tique
#define MAX_ACCUMULATOR_MS    250.0


typedef struct {
    uint16_t stack[STACK_SIZE];
    int top;
} stack_memory;

typedef struct {

    uint16_t program_counter;
    uint16_t I;
    uint8_t timer;
    uint8_t sound;
    uint8_t memory[MEMORY_SIZE];
    uint8_t V[16];
    uint8_t last_key;
    bool key_state;
    bool screen[HEIGHT][WIDTH];
    stack_memory stack;

}chip_8;

uint16_t chip_8_get_instruction(chip_8* chip);

uint16_t chip_8_stack_pop(chip_8* chip);

uint8_t chip_8_get_font_address(uint8_t value);

bool chip_8_load_rom(chip_8* chip, const char* file_path);

void chip_8_draw(chip_8* chip, uint16_t instruction);

void chip_8_clear_screen(chip_8* chip);

void chip_8_update_timers(chip_8* chip);

void chip_8_stack_push(chip_8* chip, uint16_t address);

void chip_8_init(chip_8* chip);

void chip_8_loop(chip_8* chip);

#endif