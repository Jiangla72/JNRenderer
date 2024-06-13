#include "window.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<iostream>
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
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << " Error : failed load gl func ptrs!" << std::endl;
		return;
	}
	glfwMakeContextCurrent(m_pWindow);
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



