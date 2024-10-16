#include "IWidget.h"
#include "imgui.h"

IWidget::IWidget()
{
}

IWidget::~IWidget()
{
}

void IWidget::Init()
{
}

void IWidget::UnInit()
{
}

void IWidget::Update()
{
}

void IWidget::OnGui()
{
	ImGui::SetNextWindowSize({float(m_uWidth),float(m_uHeight)});
	ImGui::Begin(m_sWidgetName.c_str());
	glm::vec2 WindowSize = ImGui::GetWindowSize();
	m_uWidth = WindowSize.x;
	m_uHeight = WindowSize.y;
	Paint();
	ImGui::End();
}

void IWidget::Paint()
{
}
