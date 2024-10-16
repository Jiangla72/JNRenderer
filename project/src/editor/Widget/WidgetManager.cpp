#include "WidgetManager.h"
#include "IWidget.h"
#include "SceneViewWidget.h"
#include "SettingWidget.h"
WidgetManager::WidgetManager()
{
}

WidgetManager::~WidgetManager()
{
}

void WidgetManager::Init()
{

	m_vecWidgets.push_back(new SettingWidget());
	m_vecWidgets.push_back(new SceneViewWidget());

	for (auto widget : m_vecWidgets)
	{
		widget->Init();
	}
}

void WidgetManager::UnInit()
{
	for (auto widget : m_vecWidgets)
	{
		widget->UnInit();
	}
}

void WidgetManager::Update()
{
	for (auto widget:m_vecWidgets)
	{
		widget->Update();
	}
}

void WidgetManager::OnGui()
{
	for (auto widget : m_vecWidgets)
	{
		widget->OnGui();
	}
}
