//
//  window.cpp
//  Car
//
//  Created by Jesse on 26/4 /18.
//  Copyright © 21 8 Jesse. All rights reserved.
//

#include "window.hpp"

window::window(const char * windowName, unsigned int window_flags, unsigned int render_flags, unsigned int screen_col_pixels, unsigned int screen_row_pixels, unsigned int framerate) :
frameTimer(framerate),
_width(screen_col_pixels),
_height(screen_row_pixels),
_window(SDL_CreateWindow(windowName, 0, 0, _width, _height, window_flags)),
_renderer(SDL_CreateRenderer(_window, -1, render_flags)),
_surface(SDL_CreateRGBSurface(0, _width, _height, 24, 0x0000FF, 0x00FF00, 0xFF0000, 0))
{
    //enable_debug(Window);
    _random = fopen("/dev/random", "r");
    
    debug("%s creat%s\n", windowName, (_window && _renderer && _surface)?"ed":"ion error");
}

void window::redraw() {
    m_surface.lock();
    _texture = SDL_CreateTextureFromSurface(_renderer, _surface);
    m_surface.unlock();
    
    SDL_RenderClear(_renderer);
    SDL_RenderCopy(_renderer, _texture, NULL, NULL);
    SDL_RenderPresent(_renderer);
    
    SDL_DestroyTexture(_texture);
}

window::~window() {
    SDL_FreeSurface(_surface);
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
    enable_debug(Window);
    debug("Window has been closed\n");
}

int window::window_width() {
    return _width;
}

int window::window_height() {
    return _height;
}
