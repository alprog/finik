module;
class DesktopWindow;
struct ImGuiContext;
using GuiImpl = ImGuiContext;
export module Gui;

import Finik.Core;
import View;
import RenderSystem;

export class Gui
{
public:
    Gui(DesktopWindow& window);
    Gui::~Gui();

    void prepare();
    void render(CommandList& list);
    void set_context();

private:
    GuiImpl* impl;
    DesktopWindow& window;

public:
    Array<UniquePtr<View>> views;
};
