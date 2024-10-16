#pragma once
#include <string>
class IWidget
{
public:
	IWidget();
	~IWidget();

public:
	void OnGui();

public:
	virtual void Init();
	virtual void UnInit();
	virtual void Update();

protected:
	virtual void Paint();
protected:
	std::string m_sWidgetName = "";
	uint32_t m_uWidth = 0;
	uint32_t m_uHeight = 0;
};
