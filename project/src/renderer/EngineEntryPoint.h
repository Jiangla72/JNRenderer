#include "engine.h"
#include "imgui.h"

int main(int argc, char** argv)
{
	//ImGui::ShowDemoWindow();
	CreateEngine();
	Engine::GetEngine()->start();

	/*delete engine;
	engine = nullptr;*/
	return 1;
}