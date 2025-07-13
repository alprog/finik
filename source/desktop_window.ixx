module;
#include "windows.h"
struct SDL_Window;
using WindowImpl = SDL_Window;
export module desktop_window;

import Finik.Core;
import swap_chain;
import gui;
import scene;

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