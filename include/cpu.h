#ifndef CPU_H
#define CPU_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../include/utils.h"
#include <time.h>
#include <SDL3/SDL.h>

#define START_ADDRESS 0x200
#define FONT_START_ADDRESS 0x50
#define MEMORY_SIZE 4096
#define FONT_SIZE 80
#define NEXT_INSTRUCTION 0x002
#define STACK_SIZE 16
#define STACK_EMPTY -1
#define HEIGHT 32
#define WIDTH 64

typedef struct {
    uint16_t stack[STACK_SIZE];
    int top;
} stack_memory;

typedef struct {
    uint16_t program_counter;
    //Salvar endereços aqui.
    uint16_t I;
    stack_memory stack;
    uint8_t timer;
    uint8_t sound;
    uint8_t memory[MEMORY_SIZE];
    //16 registradores V0...VF
    uint8_t V[16]; 
    bool screen[HEIGHT][WIDTH];

}chip_8;


//Funções essenciais
void chip_8_init(chip_8* chip);

void chip_8_loop(chip_8* chip);

uint16_t chip_8_get_instruction(chip_8* chip);

bool chip_8_load_rom(chip_8* chip, const char* file_path);

void chip_8_stack_push(chip_8* chip, uint16_t address);

uint16_t chip_8_stack_pop(chip_8* chip);

void chip_8_draw(chip_8* chip, uint16_t instruction);

void chip_8_clear_screen(chip_8* chip);

#endif