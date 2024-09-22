#pragma once


class RenderSystem;
class IPass
{
public:
	IPass() {};
	~IPass() {};

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void Release() = 0;
};