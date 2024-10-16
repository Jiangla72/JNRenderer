#include "Editor.h"
#include "EngineEntryPoint.h"
#include "EngineInterface.h"
void Editor::_CreateDemoScene()
{
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


