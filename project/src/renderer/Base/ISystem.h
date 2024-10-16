#pragma once
#include "core.h"
#include <memory>
#include <string>
class ISystem
{
public:
	ISystem(const std::string& name = "Layer");
	virtual ~ISystem() = default;

	virtual void OnInit() {}
	virtual void OnUninit() {}
	virtual void OnUpdate() {}
	virtual void OnRender() {}
	virtual void OnGui() {}
	virtual void OnResize(uint32_t renderWidth, uint32_t renderHeight, uint32_t windowWidth, uint32_t windowHeight) {}
	virtual void BeforePresent() {}
};