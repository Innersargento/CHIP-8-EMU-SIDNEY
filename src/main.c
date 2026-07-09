#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/menu.h"


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

int main(){

    chip_window window;
    init(&window);
    menu_button buttons[BUTTONS];

    buttons[0] = (menu_button){"Load ROM", NULL, SCREEN_WIDTH/2, SCREEN_HEIGHT/5, BUTTON_WIDTH, BUTTON_HEIGHT, 0x01};

    init_buttons(&window, buttons, white, BUTTONS);

    bool exit = false; 

    while(!exit){
        SDL_Event event;

        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_EVENT_QUIT: exit = true; break;
                case SDL_EVENT_MOUSE_BUTTON_DOWN: printf("Button ID: %x", button_pressed(buttons, event.button.x, event.button.y, BUTTONS)); break;
            }
        }

        draw_canvas(window.renderer, black);
        render_buttons(&window, buttons, white, black, white, BUTTONS);
        SDL_RenderPresent(window.renderer);
    }

    destroy_buttons(buttons, BUTTONS);
    clean(&window);
}



