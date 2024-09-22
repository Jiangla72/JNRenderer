#include "ImGuiSystem.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "engine.h"
#include "Window/window.h"
#include <glad/glad.h>
#include <gl/GL.h>

ImGuiSystem::ImGuiSystem()
{
}

ImGuiSystem::~ImGuiSystem()
{
}

void ImGuiSystem::OnInit()
{

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    auto window = Engine::GetEngine()->GetSystem<Window>();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window->GetGLFWWindow(), true);
#ifdef __EMSCRIPTEN__
    ImGui_ImplGlfw_InstallEmscriptenCanvasResizeCallback("#canvas");
#endif
    ImGui_ImplOpenGL3_Init(JN_GLSL_VERSION);

}

void ImGuiSystem::OnUninit()
{
}

void ImGuiSystem::OnUpdate()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();


}

void ImGuiSystem::OnGui()
{
   // ImGui::ShowDemoWindow();
}

void ImGuiSystem::BeforePresent()
{
    ImGui::Render();
   

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
