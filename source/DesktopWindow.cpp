module;
#include <SDL3/SDL.h>
module DesktopWindow;

import RenderSystem;
import SwapChain;
import Scene;
import Camera;
import Gui;
import Images;
import ByteBlob;

void FlushSDLEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {}
}

DesktopWindow::DesktopWindow(int width, int height)
    : width{width}
    , height{height}
{
    SDL_WindowFlags window_flags = (SDL_WindowFlags)( SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_MAXIMIZED);
    impl = SDL_CreateWindow("title", 1000, 800, window_flags);
    id = SDL_GetWindowID(impl);
    SDL_GetWindowSize(impl, &this->width, &this->height);

    setIcon(); 
    
    SDL_PropertiesID properties = SDL_GetWindowProperties(impl);
    hwnd = (HWND)SDL_GetPointerProperty(properties, SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr);
    swap_chain = new SwapChain(*this);
    gui = new Gui(*this);
}

void DesktopWindow::setIcon()
{
    ByteBlob blob("assets/icon.png");
    Ptr image = Images::loadPng(blob, ImageOrigin::TopLeft);
    int32 w = image->width;
    int32 h = image->height;
    SDL_Surface* icon = SDL_CreateSurfaceFrom(w, h, SDL_PIXELFORMAT_RGBA32, image->data, w * 4);
    SDL_SetWindowIcon(impl, icon);   

    FlushSDLEvents(); // This needed for taskbar icon
}

DesktopWindow::~DesktopWindow()
{
    delete gui;
    delete swap_chain;
    SDL_DestroyWindow(impl);
}
