#include "engine.h"
#include "imgui.h"
int main()
{
	ImGui::ShowDemoWindow();
	Engine::getEngine()->start();

	return 1;
}