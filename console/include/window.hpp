//
//  window.hpp
//  Car
//
//  Created by Jesse on 26/10/18.
//  Copyright © 2018 Jesse. All rights reserved.
//

#ifndef window_hpp
#define window_hpp

#include <stdio.h>
#include <SDL2/SDL.h>
#include <thread>

#include "global.hpp"
#include "stream.hpp"
#include "interval.hpp"

class window : public frameTimer {
private:
    const int _width, _height;
    SDL_Window * _window;
    SDL_Renderer * _renderer;
    SDL_Texture * _texture;
    
protected:
    SDL_Surface * _surface;

    FILE * _random;
    
public:
    std::mutex m_surface;
    
    window(const char * windowName, unsigned int window_flags, unsigned int render_flags, unsigned int screen_col_pixels, unsigned int screen_row_pixels, unsigned int framerate = 25);
    ~window();

    virtual void redraw();
    
    int window_width();
    int window_height();
};


#endif /* window_hpp */
