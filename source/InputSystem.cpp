module;
#include "SDL3/SDL.h"
module InputSystem;

InputSystem::InputSystem()
{
    SDL_Init(SDL_INIT_EVENTS | SDL_INIT_JOYSTICK | SDL_INIT_GAMEPAD);
}
