#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdlib.h>
#include <stdio.h>

#define WIDTH 480
#define HEIGHT 360

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
} chip_window;

typedef struct {
    SDL_Rect rect;
    int x;
    int y;
    int width;
    int height;
    char* text;
    uint8_t ID;
} menu_button;

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

void init(chip_window* window){

    if(!SDL_Init(SDL_INIT_VIDEO)){
        fprintf(stderr, "SDL ERROR: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    if (!SDL_CreateWindowAndRenderer("CHIP-8-EMU-SIDNEY", WIDTH, HEIGHT, 0, &window->window, &window->renderer)){
        fprintf(stderr, "ERROR: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}

void clean(chip_window* window){

    SDL_DestroyWindow(window->window);
    SDL_DestroyRenderer(window->renderer);

}

void draw_canvas(SDL_Renderer* renderer, SDL_Color color){
    SDL_SetRenderDrawColor(renderer, color.a, color.g, color.g, color.a);
    SDL_RenderClear(renderer);
}

int main(){

    chip_window window;
    init(&window);

    bool exit = false; 

    while(!exit){
        SDL_Event event;

        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_EVENT_QUIT: exit = true; break;
            }
        }

        SDL_RenderPresent(window.renderer);
        draw_canvas(window.renderer, white);
    }

    clean(&window);
}



