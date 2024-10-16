#pragma once
#include "stdInt.h"

class RenderSystem;
class IPass
{
public:
	uint32_t m_uWidth = 0;
	uint32_t m_uHeight = 0;
public:
	IPass() {};
	~IPass() {};

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void Release() = 0;
	virtual void OnResize(uint32_t renderWidth, uint32_t renderHeight) = 0;
};