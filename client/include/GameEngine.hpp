#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <SFML/Graphics.hpp>
#include "SceneManager.hpp"
#include "Network.h"

class GameEngine
{
    public:
        //Delete copy and move constructors
        GameEngine(GameEngine const& c) = delete;
        void operator=(GameEngine const& c) = delete;
        virtual ~GameEngine();

        void run();
        void stop();

        SceneManager& getSceneManager();
        sf::RenderWindow& getRenderWindow();
        Network& getNetworkManager();

        static GameEngine& Instance();
    protected:

    private:
        //Make consturctor private as for the singleton
        GameEngine();

        void GameLoop();
        void CleanUp();
        void auxEventHandler();
        void auxDraw();
        void UpdateStatistics(sf::Time elapsedTime);

        //Game core variables
        sf::RenderWindow m_GameWindow;
        SceneManager m_sceneManager;
        Network m_net;

        //Statistis info
        static const sf::Time TimePerFrame;
        sf::Text m_StatisticsText;
        sf::Time m_StatisticsUpdateTime;
        size_t m_StatisticsNumFrames{0};
        sf::Font m_Font;
};

#endif // GAMEENGINE_H
