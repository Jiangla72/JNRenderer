#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<iostream>
#include "Camera.h"
Camera::Camera(float fov, float aspect_ratio, float zNear, float zFar)
{
	m_sCameraData = {};
	m_sCameraData.fov = fov;
	m_sCameraData.aspect_ratio = aspect_ratio;
	m_sCameraData.zNear = zNear;
	m_sCameraData.zFar = zFar;
	m_sCameraData.up = glm::vec3(0.0f, 1.0f, 0.0f);
	m_sCameraData.forward = glm::vec3(0.0f, 0.0f, -1.0f);
	m_bDirty = true;
}

Camera::~Camera()
{
}

void Camera::init()
{
}

void Camera::setFov(float fov)
{
	m_sCameraData.fov = fov;
	m_bDirty = true;
}

void Camera::setAspectRatio(float aspect_ratio)
{
	m_sCameraData.aspect_ratio = aspect_ratio;
	m_bDirty = true;
}

void Camera::setNear(float zNear)
{
	m_sCameraData.zNear = zNear;
	m_bDirty = true;
}

void Camera::setFar(float zFar)
{
	m_sCameraData.zFar = zFar;
	m_bDirty = true;
}

void Camera::setPos(glm::vec3 pos)
{
	m_sCameraData.pos = pos;
	m_bDirty = true;
}

void Camera::setForward(glm::vec3 forward)
{
	m_sCameraData.forward = forward;
	m_bDirty = true;
}

void Camera::setUp(glm::vec3 up)
{
	m_sCameraData.up = up;
	m_bDirty = true;
}

const CameraData& Camera::getCameraData()
{
	if (m_bDirty)
	{
		UpdateCamera();
		m_bDirty = false;
	}
	return m_sCameraData;
}

void Camera::setPitchAndYaw(float fpitch, float fyaw)
{
	m_sCameraData.pitch = fpitch;
	m_sCameraData.yaw = fyaw;
	m_sCameraData.forward.x= cos(glm::radians(m_sCameraData.pitch)) * sin(glm::radians(m_sCameraData.yaw ));
	m_sCameraData.forward.y = sin(glm::radians(m_sCameraData.pitch));
	m_sCameraData.forward.z = cos(glm::radians(m_sCameraData.pitch)) * -cos(glm::radians(m_sCameraData.yaw));
	m_sCameraData.forward = glm::normalize(m_sCameraData.forward);
	m_bDirty = true;
}

void Camera::UpdateCamera()
{
	m_sCameraData.matView = glm::lookAt(m_sCameraData.pos, m_sCameraData.forward + m_sCameraData.pos, m_sCameraData.up);
	m_sCameraData.matProject = glm::perspective(m_sCameraData.fov, m_sCameraData.aspect_ratio, m_sCameraData.zNear, m_sCameraData.zFar);
	m_sCameraData.matViewProject = m_sCameraData.matProject * m_sCameraData.matView;
}

