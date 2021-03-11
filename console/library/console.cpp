//
//  console.cpp
//  Car
//
//  Created by Jesse on 27/10/18.
//  Copyright © 2018 Jesse. All rights reserved.
//

#include "console.hpp"

int console::units_width() {
    return window_width() / char_width();
}

int console::units_height() {
    return window_height() / char_height() - _y_offset;
}

void console::nextCursor() {
    _cursor_x++;
    if ( _cursor_x >= units_width() ) {
        line_feed();
    }
}

void console::setCursor(int col, int row) {
    if ( col < 0) col = 0;
    else if (col >= units_width()) col = units_width() - 1;
    _cursor_x = col;
    
    if (row < 0) row = 0;
    else if (row >= units_height()) row = units_height() - 1;
    _cursor_y = row;
}

void console::bell() { // '\a'
    printf("\a");
}

void console::backspace() { // '\b'
    setCursor(_cursor_x - 1, _cursor_y);
}

void console::horizontal_tab() { // '\t'
    if (_cursor_x % 8 == 0) putchar(' ');
    while( _cursor_x % 8 ) putchar(' ');
}

void console::line_feed() { // '\n'
    carriage_return();
    form_feed();
    for (int i = 0; i < units_width(); i++) update_char(ascii(' '), i, _cursor_y);
}

void console::vertical_tab() { // '\v'
    while ( _cursor_y % 6 ) form_feed();
}

void console::form_feed() { // '\f'
    if ( ++_cursor_y >= units_height() ) {
        _cursor_y = 0;
    }
}
    
void console::carriage_return() { // '\r'
    _cursor_x = 0;
}

void console::update_char(void * character, int col, int line) {
    update_chunk(character, col, line + _y_offset, char_width(), char_height());
}

console::console(const char window_name[], int width, int height) : terminal(window_name, width, height) {
    using namespace console_header;
    // console header image
    update_chunk(startupImage, 0, 0,
                 image_width, image_height,
                 char_width(), char_height());
    
    // header message
    _y_offset = 0;
    _cursor_x = (image_width/char_width() + 2);
    _cursor_y = (image_height/char_height())/2 + 1;
    println(header_msg);
    _y_offset = (image_height/char_height() + 2);

    setCursor(0, 0);
}

int console::putchar(int data) {
    switch (data) {
        case '\b':
            //printf("backspace ");
            backspace();
            break;
            
        case '\n':
            //printf("line feed ");
            line_feed();
            break;
            
        case '\r':
            //printf("carriage return ");
            carriage_return();
            break;
            
        case '\t':
            //printf("hor. tab ");
            horizontal_tab();
            break;
            
        case '\v':
            //printf("vert. tab ");
            vertical_tab();
            break;
            
        case '\a':
            //printf("bell ");
            bell();
            break;
            
        case '\f':
            //printf("form feed ");
            form_feed();
            break;
            
        default:
            //printf("%c (%d) ", data, data);
            update_char(ascii(data), _cursor_x, _cursor_y);
            nextCursor();
            break;
    }
    return data;
}

void console::printRandom(const char * data) {
    int i = 0;
    while ((char)data[i] != '\0') {
        putchar(data[i++]);
        //redraw();
        usleep(fgetc(_random) * 1000);
    }
}
