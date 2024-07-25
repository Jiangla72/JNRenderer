#include "Input.h"
#pragma once
#ifdef _JN_ENGINE
#define JNAPI _declspec(dllexport)
#else	
#define JNAPI _declspec(dllimport)
#endif

bool Input::getKey(int key)
{
    return keyHeldEvent[key];
}

bool Input::getPressedKey(int key)
{
    return keyPressEvent[key];
}

bool Input::getReleasedKey(int key)
{
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

void Input::setKey(int key)
{
}
