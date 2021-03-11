//
//  terminal.hpp
//  Car
//
//  Created by Jesse on 27/10/18.
//  Copyright © 2018 Jesse. All rights reserved.
//

#ifndef terminal_hpp
#define terminal_hpp

#include <stdio.h>

#include "global.hpp"
#include "window.hpp"

class terminal : public window {
private:
    
protected:
    void update_chunk(void * pixels, int col_position, int row_position, int chunk_width, int chunk_height, int offset_x = 0, int offset_y = 0);
        
public:
    terminal(const char window_name[], int screen_width, int screen_height);
};
#endif /* terminal_hpp */
