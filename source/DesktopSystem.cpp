module;
#include <SDL.h>
module desktop_system;

DesktopSystem::DesktopSystem()
{
    SDL_Init(SDL_INIT_VIDEO);
}

DesktopSystem::~DesktopSystem()
{
    for (auto& window : windows)
    {
        delete window;
    }
    windows.clear();
}

DesktopWindow* DesktopSystem::create_window(int width, int height)
{
    auto window = new DesktopWindow(width, height);
    windows.append(window);
    return window;
}

DesktopWindow* DesktopSystem::get_window_by_id(uint32_t id) const
{
    for (auto& window : windows)
    {
        if (window->id == id)
            return window;
    }
    return nullptr;
}

void DesktopSystem::close_window(DesktopWindow* window)
{
    windows.remove(window);
    delete window;
}