//
//  console.hpp
//  Car
//
//  Created by Jesse on 27/10/18.
//  Copyright © 2018 Jesse. All rights reserved.
//

#ifndef console_hpp
#define console_hpp

#include <stdio.h>
#include <unistd.h>

#include "global.hpp"
#include "terminal.hpp"
#include "stream.hpp"
#include "alphamap.hpp"
#include "console_header.hpp"

class console : public terminal, public Stream, protected alphamap {
private:
    int _cursor_x, _cursor_y;   // cursor position
    int _y_offset;
    
protected:
    int units_width();
    int units_height();
    
    void nextCursor();
    void setCursor(int col, int row);
    void bell();
    void backspace();
    void horizontal_tab();
    void line_feed();
    void vertical_tab();
    void form_feed();
    void carriage_return();
    
    void update_char(void * character, int col, int line);
    
public:
    console(const char window_name[], int width = 640/2, int height = 480/2);
    
    int putchar(int data);
    /* do not keep */
    void printRandom(const char * data);
};

#endif /* console_hpp */
