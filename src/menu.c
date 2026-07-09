#include "../include/menu.h"

void init(chip_window* window){

    if(!SDL_Init(SDL_INIT_VIDEO)){
        fprintf(stderr, "SDL ERROR: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    if(TTF_Init() < 0){
        fprintf(stderr, "SDL ERROR: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    if (!SDL_CreateWindowAndRenderer("CHIP-8-EMU-SIDNEY", SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window->window, &window->renderer)){
        fprintf(stderr, "ERROR: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    window->font = TTF_OpenFont(FONT_PATH, UI_FONT_SIZE);

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
    
    SDL_SetRenderDrawColor(window->renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(window->renderer, &rect);

    int text_w, text_h;
    TTF_GetTextSize(button->label, &text_w, &text_h);

    SDL_SetRenderDrawColor(window->renderer, offset.r , offset.g , offset.b , offset.a);
    SDL_RenderRect(window->renderer, &rect);

    TTF_DrawRendererText(button->label, button->x - text_w/2 , button->y - text_h/2);

}

void render_buttons(chip_window* window, menu_button* button, SDL_Color offset, SDL_Color color, SDL_Color text_color, int count){
    for(int i = 0; i < count; ++i){
        render_button(window, &button[i], offset, color, text_color);
    }
}

uint8_t button_pressed(menu_button* buttons, float x, float y, int count){
    for(int i = 0; i < count; ++i){
        float left = buttons[i].x - buttons[i].width  / 2.0f;
        float top  = buttons[i].y - buttons[i].height / 2.0f;

        if(x >= left && x <= left + buttons[i].width &&
           y >= top  && y <= top  + buttons[i].height){
            return buttons[i].ID;
        }
    }
    return 0x00;
}

bool init_button(chip_window* window, menu_button* button, SDL_Color text_color){

    button->label = TTF_CreateText(window->engine, window->font, button->text, 0);

    if(button->label == NULL){
        fprintf(stderr, "TTF ERROR: %s\n", SDL_GetError());
        return false;
    }

    TTF_SetTextColor(button->label, text_color.r, text_color.g, text_color.b, text_color.a);

    return true;
}

bool init_buttons(chip_window* window, menu_button* buttons, SDL_Color text_color, int count){
    for(int i = 0; i < count; ++i){
        if(!init_button(window, &buttons[i], text_color)) return false;
    }
    return true;
}

void destroy_buttons(menu_button* buttons, int count){
    for(int i = 0; i < count; ++i){
        TTF_DestroyText(buttons[i].label);
        buttons[i].label = NULL;
    }
}

static void SDLCALL rom_dialog_callback(void *userdata, const char * const *filelist, int filter){

    (void)filter;
    rom_request *req = (rom_request *)userdata;

    if (filelist == NULL) {
        fprintf(stderr, "DIALOG ERROR: %s\n", SDL_GetError());
        return;
    }
    if (filelist[0] == NULL) { 
        return;
    }

    SDL_strlcpy(req->path, filelist[0], sizeof(req->path));
    SDL_SetAtomicInt(&req->ready, 1);
}

void open_rom_dialog(chip_window *window, rom_request *req) {
    SDL_ShowOpenFileDialog(rom_dialog_callback, req, window->window, rom_filters, SDL_arraysize(rom_filters), NULL, false);              
}