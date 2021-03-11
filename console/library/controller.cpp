//
//  controller.cpp
//  Car Mac
//
//  Created by Jesse on 8/8/18.
//  Copyright © 2018 Jesse. All rights reserved.
//
#ifndef ARDUINO_ARCH_AVR
#include "../include/controller.hpp"

void controller::safeControllerClose() {
    SDL_GameControllerClose(_controller);
    _controller = NULL;
    _joystick = -1;
}

controller::controller() {
    _controller = NULL;
    _joystick = -1;
}

bool controller::working() {
    if (_controller) {
        if (SDL_GameControllerGetAttached(_controller)) {
            return true;
        } else {
            fprintf(stderr, "Error: Controller has been disconnected\n");
            safeControllerClose();
        }
    } else {
        int i;
        SDL_JoystickUpdate();
        for (i = 0; i < SDL_NumJoysticks(); i++) {
            if (SDL_IsGameController(i)) {
                // attempt to open joystick as controller
                _controller = SDL_GameControllerOpen(i);
                fprintf(stderr, "Note: Controller opened at %d\n", i);
                break;
            }
            fprintf(stderr, "Note: No Controller at %d\n", i);
        }
        
        if (_controller) {
            _joystick = i;
            fprintf(stderr, "Controller connected...\n");
            return working();
        } else if (_joystick == -1) {
            _joystick = -2;
            fprintf(stderr, "Error: No controller connected\n");
        }
    }
    return false;
}

bool controller::update() {
    if (!working()) return false;
    SDL_GameControllerUpdate();
    return true;
}

bool controller::back() {
    return SDL_GameControllerGetButton(_controller, SDL_CONTROLLER_BUTTON_BACK);
}

uint16_t controller::rTrigger() {
    return SDL_GameControllerGetAxis(_controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
}

uint16_t controller::lTrigger() {
    return SDL_GameControllerGetAxis(_controller, SDL_CONTROLLER_AXIS_TRIGGERLEFT);
}

int16_t controller::lAnalog_X() {
    return SDL_GameControllerGetAxis(_controller, SDL_CONTROLLER_AXIS_LEFTX);
}

#endif
