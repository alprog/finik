module;
#include "windows.h"
struct SDL_Window;
using WindowImpl = SDL_Window;
export module DesktopWindow;

import Finik.Core;
import SwapChain;
import Gui;
import Scene;

// for intellisense

export class DesktopWindow
{
public:
    DesktopWindow(int width, int height);
    ~DesktopWindow();

    void renderScene();
    void setIcon();

    WindowImpl* get_impl()
    {
        return impl;
    }

    unsigned int id;
    HWND hwnd;
    SwapChain* swap_chain;
    Scene* scene;
    Gui* gui;

    int width;
    int height;

private:
    WindowImpl* impl;
};
