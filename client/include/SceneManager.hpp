#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <list>

class SceneManager
{
    public:
        SceneManager();
        virtual ~SceneManager();

        //void changeActiveScene(Scene* newScene);
        //void changeActiveScene(Scene* newScene, bool deleteOldScene);
        //Scene* getCurrentActiveScene();
        //void deleteAllScenes();
       //void garbageSceneColection();

    protected:

    private:
        //std::list<Scene*> m_activeScenes;
        //std::list<Scene*> m_sceneToDelete;
};

#endif // SCENEMANAGER_H
