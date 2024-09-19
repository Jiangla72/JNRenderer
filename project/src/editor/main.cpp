#include "engine.h"
#include "imgui.h"

int main(int argc, char** argv)
{
	//ImGui::ShowDemoWindow();
	auto engine = CreateEngine();
	engine->start();

	return 1;
}