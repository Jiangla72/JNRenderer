#pragma once
#include "IWidget.h"
class SettingWidget : public IWidget
{
public:
	SettingWidget();
	~SettingWidget();

public:
	virtual void Init();
	virtual void UnInit();
	virtual void Update();

protected:
	virtual void Paint();
private:

};
