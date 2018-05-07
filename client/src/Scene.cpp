#include "Scene.hpp"
Scene::Scene(): m_gui()
{
}

Scene::~Scene()
{
}
void Scene::Load()
{
    if(m_loaded)
    {
        Log(l_WARN) << "Scene " << Name() << " was already loaded, cannot load a scene twice.";
        return;
    }
    m_gui.setWindow(GameEngine::Instance().getRenderWindow());
    m_gui.setView(GameEngine::Instance().getRenderWindow().getView());
    if(LoadScene()){

        Log(l_DEBUG) << "Loaded Scene " << Name();
        m_loaded = true;
        return;
    }

    Log(l_WARN) << "Scene " << Name() << " could not been loaded successfully";
}

void Scene::Unload()
{
    if(!m_loaded)
    {
         Log(l_WARN) << "Scene " << Name() << " was already unloaded, cannot load a scene twice.";
        return;
    }
    if(UnloadScene())
    {
        Log(eLogType::l_DEBUG) << "Unloaded Scene" << Name();
        m_loaded = false;
        return;
    }

    Log(l_WARN) << "Scene " << Name() << " could not been unloaded successfully";
}

bool Scene::isLoaded() const
{
    return m_loaded;
}


