#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/cpu.h"
#include "../include/menu.h"

#define CHIP_8_SCREEN_WIDTH 64
#define CHIP_8_SCREEN_HEIGHT 32
#define PIXEL_X_SIZE SCREEN_WIDTH/CHIP_8_SCREEN_WIDTH
#define PIXEL_Y_SIZE SCREEN_HEIGHT/CHIP_8_SCREEN_HEIGHT


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
    menu_button buttons[BUTTONS];
    TTF_Text* title;
} menu;

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

/*void input(chip_8* chip, Uint32 scan_code){
    switch(scan_code){
        case SDLK_1:
        case SDLK_2:
        case SDLK_3:
        case SDLK_4:
        case SDLK_Q:
        case SDLK_W:
        case SDLK_E:
        case SDLK_R:
        case SDLK_A:
        case SDLK_S:
        case SDLK_D:
        case SDLK_F:
        case SDLK_Z:
        case SDLK_X:
        case SDLK_C:
        case SDLK_V:
    }
} */

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
        }
        SDL_RenderPresent(window.renderer);
    }

    destroy_buttons(main_menu.buttons, BUTTONS);
    clean(&window);
}


