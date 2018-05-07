#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <queue>
#include <deque>
class Scene;

class SceneManager
{
    public:
        SceneManager();
        virtual ~SceneManager();

        Scene* setActiveScene(Scene& newScene, bool deleteOldScene = true);
        Scene* getCurrentActiveScene();
        void deleteScene(Scene* scene);
        void deleteAllScenes();
        void garbageCollection();

    protected:

    private:
        Scene* m_CurrentActiveScene;
        std::deque<Scene*> m_backgroundScenes;
        std::queue<Scene*> m_sceneToDelete;
};

#endif // SCENEMANAGER_H
