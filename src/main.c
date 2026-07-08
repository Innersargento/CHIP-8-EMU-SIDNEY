#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdlib.h>
#include <stdio.h>

#define WIDTH 480
#define HEIGHT 360
#define FONT_PATH "assets/font/Arial.ttf"
#define FONT_SIZE 15
#define BUTTONS 3


typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;
    TTF_TextEngine* engine;
} chip_window;

typedef struct {
    char* text;
    int x;
    int y;
    int width;
    int height;
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

    if(TTF_Init() < 0){
        fprintf(stderr, "SDL ERROR: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    if (!SDL_CreateWindowAndRenderer("CHIP-8-EMU-SIDNEY", WIDTH, HEIGHT, 0, &window->window, &window->renderer)){
        fprintf(stderr, "ERROR: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    window->font = TTF_OpenFont(FONT_PATH, FONT_SIZE);

    if(window->font == NULL){
        fprintf(stderr, "ERROR: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    window->engine = TTF_CreateRendererTextEngine(window->renderer);

    if(window->engine == NULL){
        fprintf(stderr, "ERROR: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}

void clean(chip_window* window){

    SDL_DestroyRenderer(window->renderer);
    SDL_DestroyWindow(window->window);
    TTF_DestroyRendererTextEngine(window->engine);
    TTF_CloseFont(window->font);
    TTF_Quit();
    SDL_Quit();

}

void draw_canvas(SDL_Renderer* renderer, SDL_Color color){
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(renderer);
}

void render_button(chip_window* window, menu_button* button, SDL_Color offset, SDL_Color color, SDL_Color text_color){

    SDL_FRect rect = (SDL_FRect){button->x - button->width/2, button->y - button->height/2, button->width, button->height};

    TTF_Text* textObj = TTF_CreateText(window->engine, window->font, button->text, 0);
    TTF_SetTextColor(textObj, text_color.r, text_color.g, text_color.b, text_color.a);
    
    SDL_SetRenderDrawColor(window->renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(window->renderer, &rect);

    int text_w, text_h;
    TTF_GetTextSize(textObj, &text_w, &text_h);


    SDL_SetRenderDrawColor(window->renderer, offset.r , offset.g , offset.b , offset.a);
    SDL_RenderRect(window->renderer, &rect);

    TTF_DrawRendererText(textObj, button->x - text_w/2 , button->y - text_h/2);

}

void render_buttons(chip_window* window, menu_button* button, SDL_Color offset, SDL_Color color, SDL_Color text_color, int count){
    for(int i = 0; i < count; ++i){
        render_button(window, &button[i], offset, color, text_color);
    }
}

int main(){

    chip_window window;
    init(&window);
    menu_button buttons[BUTTONS];

    buttons[0] = (menu_button){"Load ROM", WIDTH/2, HEIGHT/5, 200, 100, 0x01};
    buttons[1] = (menu_button){"Settings", WIDTH/2, HEIGHT/2, 200, 100, 0x02};
    buttons[2] = (menu_button){"Exit", WIDTH/2, HEIGHT/1.8, 200, 100, 0x03};

    bool exit = false; 

    while(!exit){
        SDL_Event event;

        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_EVENT_QUIT: exit = true; break;
            }
        }

        draw_canvas(window.renderer, black);
        render_buttons(&window, buttons, white, black, white, BUTTONS);
        SDL_RenderPresent(window.renderer);
    }

    clean(&window);
}



