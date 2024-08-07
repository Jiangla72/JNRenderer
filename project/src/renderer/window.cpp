#include "window.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<iostream>
#include"Input.h"
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		Input::setPressedKey(key);
	}
	else if (action == GLFW_RELEASE)
	{
		Input::setReleasedKey(key);
	}

	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window, GL_TRUE);
		break;
	default:
		break;
	}
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{

	if (action == GLFW_PRESS)
	{
		Input::setMouseClicked(button);
	}
	if (action == GLFW_RELEASE)
	{
		Input::setMouseReleased(button);
	}

	if (action == GLFW_PRESS) switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
		std::cout<<"Mosue left button clicked!"<<std::endl;
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		std::cout << "Mosue middle button clicked!" << std::endl;
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		std::cout << "Mosue right button clicked!" << std::endl;
		break;
	default:
		break;

	}
	return;
}
void cursor_position_callback(GLFWwindow* window, double x, double y)
{
	Input::setMouseMove(x, y);
	return;
}
void scroll_callback(GLFWwindow* window, double x, double y)
{
	Input::setMouseScroll(x, y);
	return;
}

void framebuffer_size_callback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
}
Window::Window()
{
}

Window::~Window()
{

}

void Window::init()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	m_pWindow =  glfwCreateWindow(800, 600, "JNRenderer", nullptr, nullptr);
	if (m_pWindow == nullptr)
	{
		std::cout << "Error : failed create window!" << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(m_pWindow);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << " Error : failed load gl func ptrs!" << std::endl;
		return;
	}
	glfwSwapInterval(1);
	glfwSetKeyCallback(m_pWindow, key_callback);
	glfwSetFramebufferSizeCallback(m_pWindow, framebuffer_size_callback);
	glfwSetMouseButtonCallback(m_pWindow, mouse_button_callback);
	glfwSetCursorPosCallback(m_pWindow, cursor_position_callback);
	glfwSetScrollCallback(m_pWindow, scroll_callback);
}

void Window::update()
{

	_ProcessInput(m_pWindow);
	glfwSwapBuffers(m_pWindow);
	glfwPollEvents();
}


void Window::release()
{
	glfwTerminate();
}

bool Window::ShouldClose()
{
	return glfwWindowShouldClose(m_pWindow);
}

void Window::_ProcessInput(GLFWwindow* window)
{
}



