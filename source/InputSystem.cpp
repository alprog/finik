module;
#include "SDL.h"
module input_system;

InputSystem::InputSystem()
{
    SDL_Init(SDL_INIT_EVENTS | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER);
}