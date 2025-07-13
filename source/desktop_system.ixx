export module desktop_system;

import Finik.Core;
import desktop_window;

export class DesktopSystem
{
    friend class App;

public:
    DesktopSystem();
    ~DesktopSystem();

    DesktopWindow* create_window(int width, int height);
    DesktopWindow* get_window_by_id(uint32 id) const;
    void close_window(DesktopWindow* window);

    Array<DesktopWindow*> windows;
};