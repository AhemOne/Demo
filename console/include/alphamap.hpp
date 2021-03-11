//
//  alphamap.hpp
//  Car
//
//  Created by Jesse on 26/10/18.
//  Copyright © 2018 Jesse. All rights reserved.
//

#ifndef alphamap_hpp
#define alphamap_hpp

#include <stdio.h>

class alphamap {
private:
    const int _width, _height, _depth;
    int _asciimap[255];

public:
    alphamap();
    void * ascii(char input);
    
    int char_width();
    int char_height();
    int char_depth();
};

#endif /* alphamap_hpp */
