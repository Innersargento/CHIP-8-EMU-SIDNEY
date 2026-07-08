#include "cpu.h"

static const uint8_t font[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void chip_8_init(chip_8* chip){

    *chip = (chip_8){0};

    chip->stack.top = STACK_EMPTY;

    //program counter começa em 0x200 na memória.
    chip->program_counter = START_ADDRESS;

    //Carregar as fontes na memória (antes de 0x200).
    for(int i = 0; i < FONT_START_ADDRESS; i++){
        chip->memory[FONT_START_ADDRESS + i] = font[i]; //050...09F
    }

    //Carregar a rom na memória
    chip_8_load_rom(chip, "IBM");

}

void chip_8_loop(chip_8* chip){

    uint16_t instruction;
    instruction = chip_8_get_instruction(chip);
    chip->program_counter += NEXT_INSTRUCTION; // Pula instrução

    switch(get_first_nibble(instruction)){
        case 0x0: switch(get_address(instruction)){
            case 0x0E0: chip_8_clear_screen(chip); 
                        break;
            case 0x0EE: chip->program_counter = chip_8_stack_pop(chip);
                        break;  
        }; 
        break; 
        case 0x2: chip_8_stack_push(chip, chip->program_counter);
                  chip->program_counter = get_address(instruction);
                  break;

        case 0x1: chip->program_counter = get_address(instruction);
                  break;

        case 0x6: chip->V[get_vx(instruction)] =  get_immediate_number(instruction);
                  break;
        
        case 0x7: chip->V[get_vx(instruction)] +=  get_immediate_number(instruction);
                  break;

        case 0xA: chip->I = get_address(instruction);
                  break;
        case 0xD: chip_8_draw(chip, instruction);
                  break;
    }
}

uint16_t chip_8_get_instruction(chip_8* chip){
    
    uint16_t high = chip->memory[chip->program_counter];
    uint16_t low  = chip->memory[chip->program_counter + 1];
    return (high << 8) | low;
}

void chip_8_load_rom(chip_8* chip, char* file_name){

    FILE* rom = fopen(file_name, "rb");
    
    if (rom == NULL) {
        exit(EXIT_FAILURE);
    }

    if(fseek(rom, 0, SEEK_END)){ // Função fseek(**arquivo**, **offset**, **posição do cursor no arquivo**) 
        fclose(rom);
        exit(EXIT_FAILURE);
    }

    long rom_size = ftell(rom); // ftell retorna a quantidade  

    if(fseek(rom, 0, SEEK_SET)){
        fclose(rom);
        exit(EXIT_FAILURE);
    }

    fread(chip->memory + START_ADDRESS, 1,  rom_size, rom);

    fclose(rom);

}

void chip_8_stack_push(chip_8* chip, uint16_t address){

    if(chip->stack.top >= STACK_SIZE - 1) return;
    
    chip->stack.top++;
    chip->stack.stack[chip->stack.top] = address;
}

uint16_t chip_8_stack_pop(chip_8* chip){

    if(chip->stack.top == STACK_EMPTY) return 0;

    return chip->stack.stack[chip->stack.top--];
}

void chip_8_draw(chip_8* chip, uint16_t instruction){

    uint8_t vx = get_vx(instruction);
    uint8_t vy = get_vy(instruction);
    uint8_t n  = get_last_nibble(instruction);

    chip->V[0xF] = 0;

    uint8_t x = chip->V[vx] % 64;
    uint8_t y = chip->V[vy] % 32;

    for(int i = 0; i < n; ++i){
        uint8_t sprite = chip->memory[chip->I + i];
        
        int py = y + i;
        if(py >= 32) break;

        for(int j = 0; j < 8; ++j){

            int px = x + j;
            if(px >= 64){
                break;
            }
            if(sprite >> (7 - j) & 1 && chip->screen[py][px]){
                chip->screen[py][px] = 0;
                chip->V[0xF] = 1;
            }
            else if(sprite >> (7 - j) & 1 && !(chip->screen[py][px])){
                chip->screen[py][px] = 1;
            }
        }
    }
}

void chip_8_clear_screen(chip_8* chip){
    for(int row = 0; row < 32; ++row){
        for(int col = 0; col < 64; ++col){
            chip->screen[row][col] = 0;
        }
    }
}
