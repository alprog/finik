module Gui;

import Imgui;
import RenderSystem;
import DesktopWindow;

Gui::Gui(DesktopWindow& window)
    : window{window}
{
    // Setup Dear ImGui context
    impl = ImGui::CreateContext();
    ImGui::SetCurrentContext(impl);

    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.ConfigViewportsNoDecoration = false;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForD3D(window.get_impl());

    RenderSystem& render_system = Single::Get<RenderSystem>();
    render_system.ImguiInitHelper();
}

Gui::~Gui()
{
    ImGui::SetCurrentContext(impl);
    ImGui_ImplDX12_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
}

void Gui::set_context()
{
    ImGui::SetCurrentContext(impl);
}

void Gui::prepare()
{
    ImGui_ImplSDL3_NewFrame();
    ImGui_ImplDX12_NewFrame();
    
    ImGui::NewFrame();
    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

    for (auto& view : views)
    {
        view->draw();
    }

    static bool show_demo_window = true;
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    ImGui::ShowMetricsWindow();

    ImGui::Render(); // prepare draw data
}

void Gui::render(CommandList& list)
{
    auto command_list = list.listImpl.Get();
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), command_list);
}
