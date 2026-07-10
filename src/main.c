#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/cpu.h"
#include "../include/menu.h"

#define CHIP_8_SCREEN_WIDTH   64
#define CHIP_8_SCREEN_HEIGHT  32
#define PIXEL_X_SIZE          SCREEN_WIDTH/CHIP_8_SCREEN_WIDTH
#define PIXEL_Y_SIZE          SCREEN_HEIGHT/CHIP_8_SCREEN_HEIGHT


SDL_Color white = {
    .r = 255,
    .g = 255,
    .b = 255,
    .a = SDL_ALPHA_OPAQUE
};

SDL_Color black = {
    .r = 0,
    .g = 0,
    .b = 0,
    .a = SDL_ALPHA_OPAQUE
};

typedef struct {
    menu_button  buttons[BUTTONS];
    TTF_Text*    title;
} menu;

typedef enum {
    NOT_PRESSED,
    PRESSED
} key_state;

void screen_render(chip_8* chip, SDL_Renderer* renderer, SDL_Color color){
    for(int row = 0; row < 32; ++row ){
        for(int col = 0; col < 64; ++col){
            SDL_FRect rect = (SDL_FRect){ col * PIXEL_X_SIZE, row * PIXEL_Y_SIZE, PIXEL_X_SIZE, PIXEL_Y_SIZE };
            if(chip->screen[row][col]){
                SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
}

uint8_t input(Uint32 scan_code){
    switch(scan_code){
        case SDLK_1: return 0x01; break;
        case SDLK_2: return 0x02; break;
        case SDLK_3: return 0x03; break;
        case SDLK_4: return 0x0F; break;
        case SDLK_Q: return 0x04; break;
        case SDLK_W: return 0x05; break;
        case SDLK_E: return 0x06; break;
        case SDLK_R: return 0x0D; break;
        case SDLK_A: return 0x07; break;
        case SDLK_S: return 0x08; break;
        case SDLK_D: return 0x09; break;
        case SDLK_F: return 0x0E; break;
        case SDLK_Z: return 0x0A; break;
        case SDLK_X: return 0x00; break;
        case SDLK_C: return 0x0B; break;
        case SDLK_V: return 0x0C; break;
    }
}

int main(){

    chip_window window;
    chip_8 chip;
    menu main_menu;

    init(&window);
    chip_8_init(&chip);

    main_menu.buttons[0] = (menu_button){"Load ROM", NULL, SCREEN_WIDTH/2, SCREEN_HEIGHT/4, BUTTON_WIDTH, BUTTON_HEIGHT, 0x01};
    main_menu.title = TTF_CreateText(window.engine, window.font, "CHIP-8-EMU", 0);
    int text_w, text_h;
    TTF_GetTextSize(main_menu.title, &text_w, &text_h);

    rom_request req;
    req.path[0] = '\0';
    SDL_SetAtomicInt(&req.ready, 0);
    bool rom_loaded = false;

    init_buttons(&window, main_menu.buttons, white, BUTTONS);

    bool exit = false; 

    while(!exit){
        SDL_Event event;

        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_EVENT_QUIT: exit = true; break;
                case SDL_EVENT_MOUSE_BUTTON_DOWN: uint8_t ID = button_pressed(main_menu.buttons, event.motion.x, event.motion.y, BUTTONS);
                switch(ID){
                    case 0x01: open_rom_dialog(&window, &req); break;
                };
                break;
                case SDL_EVENT_KEY_DOWN: 
                    chip.last_key = input(event.key.scancode); 
                    chip.key_state = PRESSED;
                    break; 

            }
        }
        if (SDL_GetAtomicInt(&req.ready)) {
            SDL_SetAtomicInt(&req.ready, 0);
            rom_loaded = chip_8_load_rom(&chip, req.path);
            if (!rom_loaded)
                fprintf(stderr, "Falha ao carregar ROM: %s\n", req.path);
        }
        draw_canvas(window.renderer, black);
        if(!rom_loaded){
            render_buttons(&window, main_menu.buttons, white, black, white, BUTTONS);
            TTF_DrawRendererText(main_menu.title, SCREEN_WIDTH/2 - text_w/2, 10);
        } else {
            chip_8_loop(&chip);
            screen_render(&chip, window.renderer, white);
            chip.key_state = NOT_PRESSED;
        }
        SDL_RenderPresent(window.renderer);
    }

    destroy_buttons(main_menu.buttons, BUTTONS);
    clean(&window);
}


