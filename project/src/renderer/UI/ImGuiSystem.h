#pragma once
#include "Base/core.h"
#include "Base/ISystem.h"

class JNAPI ImGuiSystem : public ISystem
{
public:
	ImGuiSystem();
	~ImGuiSystem();

public:
	virtual void OnInit();
	virtual void OnUninit();
	virtual void OnUpdate();
	virtual void OnGui();
	virtual void BeforePresent();
};