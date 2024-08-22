#include "Renderer.h"
#include"Triangle.h"
#include"Scene.h"
#include"Camera.h"
#include"Model.h"
#include "Mesh.h"
#include<array>
#include <algorithm>
#include "Input.h"
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "Light.h"
Renderer::Renderer()
{
}

Renderer::~Renderer()
{
   
}

void Renderer::init()
{
	m_pShader = new Shader("D:\\Workspace\\JNRenderer\\JNRenderer\\shaders\\defaultShader.vsh", "D:\\Workspace\\JNRenderer\\JNRenderer\\shaders\\defaultShader.fsh");
	m_pShader->init();

	//light.position = { 20, 0, 20 };
	//light.intensity = { 500, 500, 500};
	//GLuint uboLight;
	//glGenBuffers(1, &uboLight);
	//glBindBuffer(GL_UNIFORM_BUFFER, uboLight);
	//glBufferData(GL_UNIFORM_BUFFER, 32, &light, GL_STATIC_DRAW);
	//glBindBuffer(GL_UNIFORM_BUFFER, 0);

	//GLuint lightIndex = glGetUniformBlockIndex(m_pShader->getProgram(), "Light");
	//glUniformBlockBinding(m_pShader->getProgram(), lightIndex, 0);
	//glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboLight);
	//glUseProgram(shaderProgram);
//#pragma region Create VAO
//	float vertices[] = {
//	-0.5f, -0.28867f, 0.0f,
//	 0.5f, -0.28867f, 0.0f,
//	 0.0f,  0.57773f, 0.0f
//	};
//	glGenVertexArrays(1, &VAO);
//	glGenBuffers(1, &VBO);
//
//	glBindVertexArray(VAO);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(0);
//#pragma endregion

}

void Renderer::render(std::shared_ptr<Scene> scene)
{
	
	//frame = frame > 360 ? 1 : frame + 1.f;s
	
	 m_pShader->use();
	 const std::vector<Model*>& models = scene->getModels();
	 const std::vector<Light*>& lights = scene->getLights();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Camera* camera= scene->getCamera();
	CameraData cameraData = camera->getCameraData();
	unsigned int mvpMatrix = glGetUniformLocation(m_pShader->getProgram(), "mvpMatrix");
	glUniformMatrix4fv(mvpMatrix, 1, GL_FALSE, glm::value_ptr(cameraData.matViewProject));

	unsigned int cameraPos = glGetUniformLocation(m_pShader->getProgram(), "cameraPos");
	glUniform3fv(cameraPos, 1, glm::value_ptr(cameraData.pos));

	struct LightInfoForGPU
	{
		Light light[8];
		int nCount = 0;
		glm::ivec3 padding = glm::ivec3(0);
	} lightInfo;
	for (auto light : lights)
	{
		lightInfo.light[(int)lightInfo.nCount] = *light;
		lightInfo.nCount++;
		if (lightInfo.nCount > 7)
			break;
	}
	m_pShader->setUniformBuffer("LightUBO", (void*)&lightInfo, sizeof(LightInfoForGPU));
	for(auto model : models)
	{
		model->render();
	}
}

