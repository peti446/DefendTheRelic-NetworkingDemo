#include "SceneManager.hpp"
#include "Logger.hpp"
#include "Scene.hpp"
#include <algorithm>

SceneManager::SceneManager() : m_CurrentActiveScene(nullptr)
{
}

SceneManager::~SceneManager()
{
    deleteAllScenes();
}

Scene* SceneManager::setActiveScene(Scene& newScene, bool deleteOldScene)
{
    Scene* current_active = getCurrentActiveScene();
    std::string s = (current_active != nullptr) ? current_active->Name() : "none";
    Log(l_DEBUG) << "Changing from scene " <<  s << " to " << newScene.Name();
    newScene.Load();
    m_CurrentActiveScene = &newScene;
    auto i = std::find(m_backgroundScenes.begin(), m_backgroundScenes.end(), &newScene);
    if(i != m_backgroundScenes.end())
    {
        m_backgroundScenes.erase(i);
    }

    if(current_active == nullptr || deleteOldScene)
    {
       deleteScene(current_active);
       return nullptr;
    }

    Log(l_DEBUG) << "Added scene " << current_active->Name() << " to the background scenes";

    m_backgroundScenes.push_back(current_active);
    return current_active;
    return nullptr;
}

Scene* SceneManager::getCurrentActiveScene()
{
    return m_CurrentActiveScene;
}

void SceneManager::deleteScene(Scene* scene)
{
    if(scene == nullptr)
        return;
    Log(l_DEBUG) << "Marking scene " << scene->Name() << " to be deleted";
    m_sceneToDelete.push(scene);
}

void SceneManager::deleteAllScenes()
{
    Log(eLogType::l_WARN) << "Deleting all scenes...";
    deleteScene(m_CurrentActiveScene);
    while(!m_backgroundScenes.empty())
    {
        deleteScene(m_backgroundScenes.front());
        m_backgroundScenes.pop_front();
    }
    garbageCollection();
}

void SceneManager::garbageCollection()
{
    while(!m_sceneToDelete.empty())
    {
        Scene* s = m_sceneToDelete.front();
        s->Unload();
        m_sceneToDelete.pop();
        delete s;
    }
}
