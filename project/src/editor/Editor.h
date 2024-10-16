#pragma once
#include "Base/core.h"
#include "engine.h"
#include <memory>
#include "Widget/WidgetManager.h"
class Texture;
class Editor : public Engine
{
private:
	void _CreateDemoScene();
public:
	Editor();
	~Editor();
protected:
	void _init() override;
	void _update() override;
	void _onGui() override;
private:
	std::shared_ptr<WidgetManager> m_pWidgetManager = nullptr;
};

void CreateEngine()
{
	new Editor();
}