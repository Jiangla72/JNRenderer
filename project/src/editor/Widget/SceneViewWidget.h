#pragma once
#include "IWidget.h"
#include <memory>
class Texture;
class SceneViewWidget: public IWidget
{
public:
	SceneViewWidget();
	~SceneViewWidget();

public:
	virtual void Init();
	virtual void UnInit();
	virtual void Update();

protected:
	virtual void Paint();
private:
	std::shared_ptr<Texture> m_pTexture = nullptr;
};
