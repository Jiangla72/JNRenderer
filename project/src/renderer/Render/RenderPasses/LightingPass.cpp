#include "LightingPass.h"
#include "Base/Config.h"
#include "Render/ShaderModule.h"
#include "Render/Texture.h"
#include "Render/Camera.h"
#include "Render/RenderSystem.h"
#include "Render/Light.h"
#include "Scene/Scene.h"
#include <glad/glad.h>
#include <algorithm>
#include <string>
#include <glm/glm.hpp>

LightingPass::LightingPass() {}

LightingPass::~LightingPass() {}

void LightingPass::Init()
{
	// ����������ɫ��
	m_pLightingShader = ShaderModule::GetShaderModule(
		Config::GetShaderPath("lighting", "vertex").c_str(),
		Config::GetShaderPath("lighting", "fragment").c_str()
	);

	// ����ȫ���ı���
	float quadVertices[] = {
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};
	glGenVertexArrays(1, &m_uQuadVAO);
	glGenBuffers(1, &m_uQuadVBO);
	glBindVertexArray(m_uQuadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_uQuadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glBindVertexArray(0);
}

void LightingPass::Update() {}

void LightingPass::SetGBuffer(std::shared_ptr<Texture> gBuffer1, std::shared_ptr<Texture> gBuffer2, std::shared_ptr<Texture> gBuffer3, std::shared_ptr<Texture> gBuffer4)
{
	m_gBuffer1 = gBuffer1;
	m_gBuffer2 = gBuffer2;
	m_gBuffer3 = gBuffer3;
	m_gBuffer4 = gBuffer4;
}

void LightingPass::SetShadowMap(unsigned int shadowMap, const glm::mat4& lightSpaceMatrix)
{
	m_uShadowMap = shadowMap;
	m_lightSpaceMatrix = lightSpaceMatrix;
}

void LightingPass::Render()
{
	unsigned int fbo = RenderSystem::GetMainFBO();
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	
	RenderContext& renderContext = RenderSystem::GetRenderContext();
	// 保证视口与主FBO一致，避免上一Pass改变视口导致拉伸
	if (renderContext.renderWidth > 0 && renderContext.renderHeight > 0) {
		glViewport(0, 0, renderContext.renderWidth, renderContext.renderHeight);
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	m_pLightingShader->use();

	// ��RenderContext��ȡ���µ�G-Buffer����Ӱ����
	
	
	// �� G-Buffer ����
	if (renderContext.m_gBuffer1) 
		renderContext.m_gBuffer1->BindTexture(0);
	if (renderContext.m_gBuffer2) 
		renderContext.m_gBuffer2->BindTexture(1);
	if (renderContext.m_gBuffer3) 
		renderContext.m_gBuffer3->BindTexture(2);
	if (renderContext.m_gBuffer4) 
		renderContext.m_gBuffer4->BindTexture(3);

	// �������� Uniform
	m_pLightingShader->useTexture("gPosition", 0);
	m_pLightingShader->useTexture("gNormal", 1);
	m_pLightingShader->useTexture("gAlbedo", 2);
	m_pLightingShader->useTexture("gMetallicRoughness", 3);

	// ����Ӱ��ͼ
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, renderContext.m_uShadowMap);
	m_pLightingShader->useTexture("shadowMap", 4);
	m_pLightingShader->setUniformMatrix4fv("lightSpaceMatrix", renderContext.m_lightSpaceMatrix);

	// ������Ⱦ���������ù�Դ��Ϣ
// ȾùԴϢ
	if (renderContext.camera) {
		glm::vec3 viewPos = glm::vec3(renderContext.camera->getCameraData().pos);
		m_pLightingShader->setUniform3fv("viewPos", viewPos);
	}

	if (renderContext.scene)
	{
		const std::vector<Light*>& lights = renderContext.scene->getLights();
		int numLights = std::min((int)lights.size(), 10);
		m_pLightingShader->setUniform1i("numLights", numLights);

		for (int i = 0; i < numLights; ++i)
		{
			std::string iStr = std::to_string(i);

			glm::vec3 pos = glm::vec3(lights[i]->position);
			// Combine color and intensity
			glm::vec3 color = glm::vec3(lights[i]->color) * glm::vec3(lights[i]->intensity);

			m_pLightingShader->setUniform3fv("lightPositions[" + iStr + "]", pos);
			m_pLightingShader->setUniform3fv("lightColors[" + iStr + "]", color);
		}
	}

	glBindVertexArray(m_uQuadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void LightingPass::Release()
{
	if (m_uQuadVAO)
		glDeleteVertexArrays(1, &m_uQuadVAO);
	if (m_uQuadVBO) 
		glDeleteBuffers(1, &m_uQuadVBO);
}

void LightingPass::OnResize(uint32_t renderWidth, uint32_t renderHeight)
{
}
