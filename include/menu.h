#ifndef MENU_H

#define MENU_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdlib.h>
#include <stdio.h>

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 360
#define FONT_PATH "assets/font/Arial.ttf"
#define FONT_SIZE 15
#define BUTTONS 1
#define BUTTON_WIDTH 200
#define BUTTON_HEIGHT 100


typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;
    TTF_TextEngine* engine;
} chip_window;

typedef struct {
    char* text;
    TTF_Text* label;
    int x;
    int y;
    int width;
    int height;
    uint8_t ID;
} menu_button;



void init(chip_window* window);

void clean(chip_window* window);

void draw_canvas(SDL_Renderer* renderer, SDL_Color color);

void render_button(chip_window* window, menu_button* button, SDL_Color offset, SDL_Color color, SDL_Color text_color);

void render_buttons(chip_window* window, menu_button* button, SDL_Color offset, SDL_Color color, SDL_Color text_color, int count);

uint8_t button_pressed(menu_button* buttons, float x, float y, int count);

bool init_button(chip_window* window, menu_button* button, SDL_Color text_color);

bool init_buttons(chip_window* window, menu_button* buttons, SDL_Color text_color, int count);

void destroy_buttons(menu_button* buttons, int count);

#endif