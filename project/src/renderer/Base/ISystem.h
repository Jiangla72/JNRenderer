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
	virtual void OnPresent() {}
protected:
	std::string m_DebugName;
};