#include "SceneViewWidget.h"
#include "EngineInterface.h"
#include "imgui.h"
SceneViewWidget::SceneViewWidget()
{
}

SceneViewWidget::~SceneViewWidget()
{
}

void SceneViewWidget::Init()
{
	m_sWidgetName = "SceneView";
	m_pTexture = std::make_shared<Texture>(500, 600, JNF_RGBA);
	Engine::GetEngine()->GetSystem<RenderSystem>()->SetRenderTarget(m_pTexture, 500, 600);
    m_uWidth = 500;
    m_uHeight = 600;
}

void SceneViewWidget::UnInit()
{
}

void SceneViewWidget::Update()
{
	float deltaTime = Timer::GetDeltaTime();
    glm::vec3 moveDir = { 0.0f,0.0f,0.0f };
    float cameraSpeed = 0.2f * deltaTime;
    Camera* l_Camera = SceneManager::GetActiveScene()->getCamera();
    CameraData cameraData = l_Camera->getCameraData();
    glm::vec3 cameraPos = cameraData.pos;
    if (Input::getKey(JN_KEY_UP) || Input::getKey(JN_KEY_W))
    {
        moveDir += cameraData.forward * (1.0f);
    }
    if (Input::getKey(JN_KEY_LEFT) || Input::getKey(JN_KEY_A))
    {
        moveDir += glm::normalize(glm::cross(cameraData.forward, cameraData.up)) * (-1.0f);
    }
    if (Input::getKey(JN_KEY_DOWN) || Input::getKey(JN_KEY_S))
    {
        moveDir += cameraData.forward * (-1.0f);
    }
    if (Input::getKey(JN_KEY_RIGHT) || Input::getKey(JN_KEY_D))
    {
        moveDir += glm::normalize(glm::cross(cameraData.forward, cameraData.up)) * (1.0f);
    }
    if (Input::getKey(JN_KEY_Q))
    {
        moveDir += cameraData.up * (1.0f);
    }
    if (Input::getKey(JN_KEY_E))
    {
        moveDir += cameraData.up * (-1.0f);
    }
    cameraPos += cameraSpeed * moveDir;
    l_Camera->setPos(cameraPos);

    if (Input::mouseMoving && Input::getMouseButton(1))
    {
        float xoffset = Input::mouseMotion.x;
        float yoffset = -Input::mouseMotion.y;
        float sensitivity = 0.05f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;
        cameraData.pitch += yoffset;
        cameraData.yaw += xoffset;
        l_Camera->setPitchAndYaw(cameraData.pitch + yoffset, cameraData.yaw + xoffset);
        Input::mouseMoving = false;
    }
}

void SceneViewWidget::Paint()
{
	glm::vec2 uv_min = { 0.0f, 1.0f };                 // bottom-left
	glm::vec2 uv_max = { 1.0f, 0.0f };                 // top-right
	glm::vec4 tint_col = { 1.0f, 1.0f, 1.0f, 1.0f };   // No tint
	glm::vec4 border_col = { 1.0f, 1.0f, 1.0f, 0.5f }; // 50% opaque white

    glm::vec2 ContentSize = ImGui::GetContentRegionAvail();
	glm::vec2 viewportSize = { ContentSize.x, ContentSize.y };
 
    Engine::GetEngine()->Resize(viewportSize.x, viewportSize.y, -1, -1);
	ImGui::Image((ImTextureID)m_pTexture->GetHandle(), ContentSize, uv_min, uv_max);
}
