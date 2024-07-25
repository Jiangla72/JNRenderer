#pragma once
#include<unordered_map>

class Input
{
	static std::unordered_map<int, bool> keyPressEvent;
	static std::unordered_map<int, bool> keyReleaseEvent;
	static std::unordered_map<int, bool> keyHeldEvent;
	static unsigned int mouseReleased;
	static unsigned int mouseHeld;
	static unsigned int mouseClicked;
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

	static void setKey(int key);
	static void setPressedKey(int key);
	static void setReleasedKey(int key);
	static void setMouseClicked(int mouseButton);
	static void setMouseReleased(int mouseButton);
	static void setMouseButton(int mouseButton);
};