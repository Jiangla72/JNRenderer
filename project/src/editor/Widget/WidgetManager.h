#pragma once
#include <vector>

class IWidget;
class WidgetManager
{
private:
	std::vector<IWidget*> m_vecWidgets;
public:
	WidgetManager();
	~WidgetManager();

	void Init();
	void UnInit();
	void Update();
	void OnGui();
private:

};
