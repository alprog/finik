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
    ImGui_ImplSDL2_InitForD3D(window.get_impl());

    RenderSystem& render_system = Single::Get<RenderSystem>();
    render_system.ImguiInitHelper();
}

Gui::~Gui()
{
    ImGui::SetCurrentContext(impl);
    ImGui_ImplDX12_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void Gui::set_context()
{
    ImGui::SetCurrentContext(impl);
}

void Gui::prepare()
{
    // Start the Dear ImGui frame
    ImGui_ImplDX12_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

    for (auto& view : views)
    {
        view->draw();
    }

    static bool show_demo_window = true;
    //static bool show_another_window = false;
    //ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    //// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    ImGui::ShowMetricsWindow();

    //// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    //{
    //    static float f = 0.0f;
    //    static int counter = 0;

    //    ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

    //    static bool selected[3] = { false, false, false };

    //    ImGui::Selectable("main.c", &selected[0]);
    //    ImGui::SameLine();
    //    ImGui::Button("Link 2");

    //    ImGui::SetNextItemAllowOverlap();
    //    ImGui::Selectable("Link 2", &selected[1]);
    //    ImGui::SameLine();
    //    ImGui::Button("Link 2##1");

    //    ImGui::SetNextItemAllowOverlap();
    //    ImGui::Selectable("Hello.h", &selected[2]);
    //    ImGui::SameLine();
    //    ImGui::Button("Link 2");

    //    ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
    //    ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
    //    ImGui::Checkbox("Another Window", &show_another_window);

    //    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
    //    ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

    //    static bool open = true;
    //    if (open)
    //    {
    //        ImGui::Begin("efef", &open);
    //        ImGui::Button("Show popup");
    //        ImGui::End();
    //    }

    //    if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
    //        counter++;
    //    ImGui::SameLine();
    //    ImGui::Text("counter = %d", counter);

    //    ImGui::Begin("Issue #1453");
    //    ImGui::BeginChild("test", ImVec2(100, 100));
    //    if (ImGui::BeginPopupContextWindow())
    //    {
    //        if (ImGui::Selectable("Clear"))
    //        {
    //        }
    //        ImGui::EndPopup();
    //    }
    //    ImGui::EndChild();
    //    ImGui::End();

    //    //ImGui::BeginPopup("child 1", 0);
    //    //ImGui::Button("child 1");
    //    //ImGui::EndPopup();\

    //    //ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    //    ImGui::End();
    //}

    //// 3. Show another simple window.
    //if (show_another_window)
    //{
    //    ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
    //    ImGui::Text("Hello from another window!");
    //    if (ImGui::Button("Close Me"))
    //        show_another_window = false;
    //    ImGui::End();
    //}

    // Rendering
    ImGui::Render();
}

void Gui::render(ID3D12GraphicsCommandList* command_list)
{
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), command_list);
}
