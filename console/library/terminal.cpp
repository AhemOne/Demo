//
//  terminal.cpp
//  Car
//
//  Created by Jesse on 27/10/18.
//  Copyright © 2018 Jesse. All rights reserved.
//

#include "terminal.hpp"

terminal::terminal(const char window_name[], int screen_width, int screen_height) : window(window_name, WINDOW_FLAGS, RENDERER_FLAGS, screen_width, screen_height) {
}

void terminal::update_chunk(void * pixels, int col_position, int row_position, int chunk_width, int chunk_height, int offset_x, int offset_y) {
    SDL_Surface * _image = SDL_CreateRGBSurface(0, chunk_width, chunk_height, 24, 0x0000FF, 0x00FF00, 0xFF0000, 0);
    memcpy(_image->pixels, pixels, chunk_width * chunk_height * 3);
    
    SDL_Rect input, screen;
    input.h = chunk_height;
    input.w = chunk_width;
    input.x = 0;
    input.y = 0;
    screen.h = window_height();
    screen.w = window_width();
    screen.x = chunk_width * col_position + offset_x;
    screen.y = chunk_height * row_position + offset_y;
    
    m_surface.lock();
    SDL_BlitSurface(_image, &input, _surface, &screen);
    m_surface.unlock();
    SDL_FreeSurface(_image);
}
