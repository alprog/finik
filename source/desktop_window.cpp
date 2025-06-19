module;
#include <SDL.h>
#include <SDL_syswm.h>
module desktop_window;

import render_system;
import swap_chain;
import scene;
import camera;
import gui;
import images;

DesktopWindow::DesktopWindow(int width, int height)
    : width{width}
    , height{height}
{
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_MAXIMIZED);
    impl = SDL_CreateWindow("title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0, 0, window_flags);
    SDL_GetWindowSize(impl, &this->width, &this->height);

    id = SDL_GetWindowID(impl);

    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(impl, &wmInfo);
    hwnd = (HWND)wmInfo.info.win.window;

    swap_chain = new SwapChain(*this);
    gui = new Gui(*this);

    setIcon();
}

void DesktopWindow::setIcon()
{
    Image* image = Images::loadPng("assets/icon.png");
    int32 w = image->width;
    int32 h = image->height;
    SDL_Surface* icon = SDL_CreateRGBSurfaceFrom(image->data, w, h, 32, w * 4,
        0x000000FF, // Rmask
        0x0000FF00, // Gmask
        0x00FF0000, // Bmask
        0xFF000000  // Amask
    );

    SDL_SetWindowIcon(impl, icon);
}

DesktopWindow::~DesktopWindow()
{
    delete gui;
    delete swap_chain;
    SDL_DestroyWindow(impl);
}

void DesktopWindow::renderScene()
{
    static Camera camera;
    camera.position = Vector3(0, 1, -3);
    camera.lookAt = Vector3::Zero;
    camera.FieldOfView = std::numbers::pi / 2.0f;
    camera.calcViewMatrix();
    camera.calcProjectionMatrix();

    if (scene != nullptr)
    {
        auto context = Single::Get<RenderSystem>().getRenderContext();
        scene->render(*context, camera, Matrix::Identity, RenderPass::Geometry);
    }
}