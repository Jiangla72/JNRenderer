#include "Editor.h"
#include "EngineEntryPoint.h"
#include "EngineInterface.h"
Editor::Editor()
{
}

Editor::~Editor()
{
}

void Editor::_init()
{
    bool info = RegisterSystem<ImGuiSystem>();
    Engine::_init();

	std::string pathModel = "D:\\Workspace\\JNRenderer\\JNRenderer\\models\\spot\\spot_triangulated_good.obj";
	std::shared_ptr<Model> bunny = ResourceManager::GetResource<Model>(pathModel);
	JNLOGINFO("Create Model at path : {} , with mesh count : {}", pathModel, bunny->m_vecMesh.size());

	std::shared_ptr<Scene> scene = std::make_shared <Scene>();
	scene->init();
	scene->Add(bunny);
	Light* light = new Light();
	light->position = { 20, 0, 50 ,0 };
	light->intensity = { 500, 500, 500 ,0 };
	light->color = { 0.2, 0, 0.21 ,0 };
	Light* light1 = new Light();
	light1->position = { 0, 0, 50 ,0 };
	light1->intensity = { 500, 500, 500 ,0 };
	light1->color = { 0.53, 0.12, 0.8 ,0 };

	Light* light2 = new Light();
	light2->position = { 20, 0, 50 ,0 };
	light2->intensity = { 500, 500, 500 ,0 };
	light2->color = { 0.6, 1, 0.6 ,0 };

	scene->Add(light);
	scene->Add(light1);
	scene->Add(light2);

	SceneManager::AddScene("1", scene);
	SceneManager::SetActiveScene("1");

	m_pTexture = std::make_shared<Texture>(500, 500, JNF_RGBA);
    GetSystem<RenderSystem>()->SetRenderTarget(m_pTexture, 500, 500);
}

void Editor::_update()
{
	Engine::_update();
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

void Editor::_onGui()
{
	Engine::_onGui();
	//ImGui::ShowDemoWindow();
    static float f = 0.0f;
    static int counter = 0;
    ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

    ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
    //ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
    //ImGui::Checkbox("Another Window", &show_another_window);

    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

    if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        counter++;
    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::End();


    ImGui::Begin("Scene View");
    glm::vec2 uv_min = { 0.0f, 1.0f };                 // bottom-left
    glm::vec2 uv_max = { 1.0f, 0.0f };                 // top-right
    glm::vec4 tint_col = { 1.0f, 1.0f, 1.0f, 1.0f };   // No tint
    glm::vec4 border_col = { 1.0f, 1.0f, 1.0f, 0.5f }; // 50% opaque white
    ImGui::Image((ImTextureID)m_pTexture->GetHandle(), glm::vec2(500, 500), uv_min, uv_max);
    ImGui::End();
}

