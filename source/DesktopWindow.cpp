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

DesktopWindow::DesktopWindow(int width, int height)
    : width{width}
    , height{height}
{
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_MAXIMIZED);
    impl = SDL_CreateWindow("title", 0, 0, window_flags);
    SDL_GetWindowSize(impl, &this->width, &this->height);

    id = SDL_GetWindowID(impl);

    SDL_PropertiesID properties = SDL_GetWindowProperties(impl);
    hwnd = (HWND)SDL_GetPointerProperty(properties, SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr);

    swap_chain = new SwapChain(*this);
    gui = new Gui(*this);

    setIcon();
}

void DesktopWindow::setIcon()
{
    ByteBlob blob("assets/icon.png");
    Ptr image = Images::loadPng(blob, ImageOrigin::TopLeft);
    int32 w = image->width;
    int32 h = image->height;
    SDL_Surface* icon = SDL_CreateSurfaceFrom(w, h, SDL_PIXELFORMAT_RGBA32, image->data, w * 4);
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
    camera.FieldOfView = PI / 2.0f;
    camera.calcViewMatrix();
    camera.calcProjectionMatrix();

    if (scene != nullptr)
    {
        auto context = Single::Get<RenderSystem>().getRenderContext();
        scene->render(*context, camera, Matrix::Identity, Vector2::Zero, RenderPass::Geometry);
    }
}
