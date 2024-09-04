#include "Input.h"
std::unordered_map<int, bool> Input::keyPressEvent;
std::unordered_map<int, bool> Input::keyReleaseEvent;
std::unordered_map<int, bool> Input::keyHeldEvent;
unsigned int Input::mouseReleased = 0;
unsigned int Input::mouseHeld = 0;
unsigned int Input::mouseClicked = 0;
glm::vec2 Input::mousePosition = { -1.0f,-1.0f };
glm::vec2 Input::mouseMotion = { 0.0f,0.0f };
glm::vec2 Input::scrollOffset = { 0.0f,0.0f };
bool Input::mouseMoving = false;

bool Input::getKey(int key)
{
	if (keyHeldEvent.find(key) == keyHeldEvent.end())
    {
        return false;
    }
    return keyHeldEvent[key];
}

bool Input::getPressedKey(int key)
{
    if (keyPressEvent.find(key) == keyPressEvent.end())
    {
        return false;
    }
    return keyPressEvent[key];
}

bool Input::getReleasedKey(int key)
{
    if (keyReleaseEvent.find(key) == keyReleaseEvent.end())
    {
        return false;
    }
    return keyReleaseEvent[key];
}

bool Input::getMouseClicked(int mouseButton)
{
    unsigned int keyMask = 1 << mouseButton;
	return (mouseClicked & keyMask) != 0;
}

bool Input::getMouseReleased(int mouseButton)
{
    unsigned int keyMask = 1 << mouseButton;
    return (mouseReleased & keyMask) != 0;
}

bool Input::getMouseButton(int mouseButton)
{
    unsigned int keyMask = 1 << mouseButton;
    return (mouseHeld & keyMask) != 0;
}

void Input::setPressedKey(int key)
{
    keyPressEvent[key] = true;
    keyHeldEvent[key] = true;
}

void Input::setReleasedKey(int key)
{
    keyHeldEvent[key] = false;
    keyReleaseEvent[key] = true;
}

void Input::setMouseClicked(int mouseButton)
{
    unsigned int keyMask = 1 << mouseButton;
    mouseClicked |= keyMask;
    mouseHeld |= keyMask;
}

void Input::setMouseReleased(int mouseButton)
{
    unsigned int keyMask = 1 << mouseButton;
    mouseReleased |= keyMask;
    mouseHeld &= ~keyMask;
}

void Input::setMouseMove(float x, float y)
{
    glm::vec2 newPos = { x,y };
    if (mousePosition.x >= 0 && mousePosition.y >= 0)
        mouseMotion = newPos - mousePosition;
    mousePosition = newPos;
    mouseMoving = true;
}

void Input::setMouseScroll(float xOffset, float yOffset)
{
    scrollOffset = { xOffset ,yOffset };
}
