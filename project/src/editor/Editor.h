#pragma once
#include "Base/core.h"
#include "engine.h"
#include <memory>
class Texture;
class Editor : public Engine
{
private:
	std::shared_ptr<Texture> m_pTexture = nullptr;

public:
	Editor();
	~Editor();
protected:
	void _init() override;
	void _update() override;
	void _onGui() override;
};

void CreateEngine()
{
	new Editor();
}