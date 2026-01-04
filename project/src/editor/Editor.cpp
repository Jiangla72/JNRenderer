#include "Editor.h"
#include "EngineEntryPoint.h"
#include "EngineInterface.h"
void Editor::_CreateDemoScene()
{
	//std::string pathModel = "D:\\Workspace\\JNRenderer\\JNRenderer\\models\\spot\\spot_triangulated_good.obj";
	std::string pathModel = "G:\\JNRenderer\\JNRenderer\\models\\spot\\spot_triangulated_good.obj";
    std::shared_ptr<Model> bunny = ResourceManager::GetResource<Model>(pathModel);
    
    // Scale down the model to fix the "too large" issue
	bunny->SetScale(glm::vec3(0.75f)); 
	// Center it
	bunny->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	// Rotate to face camera (optional, depending on model)
	bunny->SetRotation(glm::vec3(0.0f, 180.0f, 0.0f));

	JNLOGINFO("Create Model at path : {} , with mesh count : {}", pathModel, bunny->m_vecMesh.size());

	std::shared_ptr<Scene> scene = std::make_shared <Scene>();
	scene->init();
	scene->Add(bunny);
	
	scene->AddCube(1.0f, glm::vec3(-2.0f, 0.0f, 0.0f));
	scene->AddSphere(0.5f, 32, 16, glm::vec3(2.0f, 0.0f, 0.0f));

	// Add a floor plane for reference
	scene->AddPlane(20.0f, 20.0f, glm::vec3(0.0f, -0.5f, 0.0f));

	// Point Light: Closer and brighter for the scaled model
	Light* pointLight = new Light();
    pointLight->position = { 2.0f, 2.0f, 2.0f, 1.0f }; 
    pointLight->intensity = { 50.0f, 50.0f, 50.0f, 0.0f }; 
    pointLight->color = { 1.0f, 0.95f, 0.85f, 0.0f };

    // Directional Light: Fill light
    Light* dirLight = new Light();
    dirLight->position = { -2.0f, 4.0f, -1.0f, 0.0f }; 
    dirLight->intensity = { 10.0f, 10.0f, 10.0f, 0.0f }; 
    dirLight->color = { 0.95f, 0.95f, 1.0f, 0.0f }; 

    // Spot Light: Highlight
    Light* spotLight = new Light();
    spotLight->position = { 0.0f, 3.0f, 3.0f, 1.0f }; 
    spotLight->intensity = { 80.0f, 80.0f, 80.0f, 0.0f };
    spotLight->color = { 1.0f, 0.9f, 0.6f, 0.0f };

    scene->Add(pointLight);
    scene->Add(dirLight);
    scene->Add(spotLight);

    SceneManager::AddScene("1", scene);
    SceneManager::SetActiveScene("1");
}
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
    m_pWidgetManager = std::make_shared<WidgetManager>();
    m_pWidgetManager->Init();
    //just for test
    _CreateDemoScene();
}

void Editor::_update()
{
	Engine::_update();
    m_pWidgetManager->Update();
}

void Editor::_onGui()
{
	Engine::_onGui();
    m_pWidgetManager->OnGui();
}
