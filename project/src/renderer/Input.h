#pragma once
#include<unordered_map>
#include "glm/glm.hpp"
#include "core.h"
class JNAPI Input
{
private:
	static std::unordered_map<int, bool> keyPressEvent;
	static std::unordered_map<int, bool> keyReleaseEvent;
	static std::unordered_map<int, bool> keyHeldEvent;
	static unsigned int mouseReleased;
	static unsigned int mouseHeld;
	static unsigned int mouseClicked;
public:
	static glm::vec2 mousePosition;
	static glm::vec2 mouseMotion;
	static glm::vec2 scrollOffset;
	static bool mouseMoving;
public:
	Input() {};
	~Input() {};
public:
	static bool getKey(int key);
	static bool getPressedKey(int key);
	static bool getReleasedKey(int key);
	static bool getMouseClicked(int mouseButton);
	static bool getMouseReleased(int mouseButton);
	static bool getMouseButton(int mouseButton);

	static void setPressedKey(int key);
	static void setReleasedKey(int key);
	static void setMouseClicked(int mouseButton);
	static void setMouseReleased(int mouseButton);
	static void setMouseMove(float x, float y);
	static void setMouseScroll(float xOffset, float yOffset);
};