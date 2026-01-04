#include "SettingWidget.h"
#include "imgui.h"
SettingWidget::SettingWidget()
{
}

SettingWidget::~SettingWidget()
{
}

void SettingWidget::Init()
{
	m_sWidgetName = "Setting";
    m_uWidth = 100;
    m_uHeight = 300;
}

void SettingWidget::UnInit()
{
}

void SettingWidget::Update()
{
}

void SettingWidget::Paint()
{
    //static float f = 0.0f;
    //static int counter = 0;
    //ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
    ////ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
    ////ImGui::Checkbox("Another Window", &show_another_window);

    //ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
    //ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    //ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

    //if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
    //    counter++;
    //ImGui::SameLine();
    //ImGui::Text("counter = %d", counter);
    //ImGuiIO& io = ImGui::GetIO(); (void)io;
    //ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
}
