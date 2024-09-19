#pragma once
#include "Base/core.h"
#include "engine.h"
#include <memory>

class Editor : public Engine
{
public:
	Editor();
	~Editor();
};

std::shared_ptr<Engine> CreateEngine()
{
	return std::make_shared<Editor>();
}