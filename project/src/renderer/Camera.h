#pragma once
#include "core.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

struct CameraData
{
	float fov;
	float aspect_ratio;
	float zNear;
	float zFar;
	glm::vec3 pos;
	glm::vec3 forward;
	glm::vec3 up;
	glm::mat4x4 matView;
	glm::mat4x4 matProject;
	glm::mat4x4 matViewProject;
	bool isOrtho;

	CameraData() 
	{
		fov = 45.0f;
		aspect_ratio = 1.0f;
		zNear = 1.0f;
		zFar = 1000.0f;
		pos = { 0,0,10 };
		forward = { 0,0,1 };
		up = { 0,1,0 };
		matView = glm::mat4x4(1);
		matProject = glm::mat4x4(1);
		matViewProject = glm::mat4x4(1);
		isOrtho = false;
	}
};

class JNAPI Camera
{
private:
	CameraData m_sCameraData;
	bool m_bDirty = true;
public:
	Camera(float fov,float aspect_ratio,float zNear,float zFar);
	~Camera();
public:
	void init();
	// glm::lookat glm::perspective glm::ortho
	void setFov(float fov);
	void setAspectRatio(float aspect_ratio);
	void setNear(float zNear);
	void setFar(float zFar);
	void setPos(glm::vec3 pos);
	void setForward(glm::vec3 forward);
	void setUp(glm::vec3 up);

	const CameraData& getCameraData();

private:
	void UpdateCamera();
};