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
	
	//frame = frame > 360 ? 1 : frame + 1.f;
	
	m_pShader->use();
	const std::vector<Model*>& models = scene->getModels();
	
	//GLuint loc = glGetUniformLocation(m_pShader->getProgram(), "frame");
	//if (loc >= 0) // if fail, loc == -1
	//{
	//	glUniform1f(loc, frame);
	//}
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT);
	Camera* camera= scene->getCamera();
	CameraData cameraData = camera->getCameraData();
	unsigned int mvpMatrix = glGetUniformLocation(m_pShader->getProgram(), "mvpMatrix");
	glUniformMatrix4fv(mvpMatrix, 1, GL_FALSE, glm::value_ptr(cameraData.matViewProject));

	unsigned int cameraPos = glGetUniformLocation(m_pShader->getProgram(), "cameraPos");
	glUniform3fv(cameraPos, 1, glm::value_ptr(cameraData.pos));
	for each (auto model in models)
	{
		model->render();
	}
}

