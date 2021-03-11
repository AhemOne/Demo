//
//  controller.hpp
//
#ifndef ARDUINO_ARCH_AVR
#ifndef controller_hpp
#define controller_hpp

#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_joystick.h>

class controller {
private:
    int _joystick;
    SDL_GameController * _controller;
    
    void safeControllerClose();
protected:
    bool working();
    
public:
    controller();
    
    bool update();
    
    bool back();
    uint16_t rTrigger();
    uint16_t lTrigger();
    int16_t lAnalog_X();
};

#endif /* controller_hpp */
#endif /* arduino arch */
