#include "cpu.h"

int main(){
    chip_8 chip;

    chip_8_init(&chip);

    for(int i = 0; i < 20; i++){
        chip_8_loop(&chip);
        for(int row = 0; row < HEIGHT; ++row ){
            printf("\n");
            for(int col = 0; col < WIDTH; ++col){
                if(chip.screen[row][col] == true)
                    printf("S");
                else {
                    printf(" ");
                }
            }
        }
    }
}