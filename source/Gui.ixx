module;
class DesktopWindow;
struct ImGuiContext;
using GuiImpl = ImGuiContext;
struct ID3D12GraphicsCommandList;
export module Gui;

import Finik.Core;
import View;

export class Gui
{
public:
    Gui(DesktopWindow& window);
    Gui::~Gui();

    void prepare();
    void render(ID3D12GraphicsCommandList* command_list);
    void set_context();

private:
    GuiImpl* impl;
    DesktopWindow& window;

public:
    Array<UniquePtr<View>> views;
};
