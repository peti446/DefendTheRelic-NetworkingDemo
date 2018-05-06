#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <SFML/Graphics.hpp>

class GameEngine
{
    public:
        GameEngine();
        virtual ~GameEngine();

        void run();

    protected:

    private:
        void GameLoop();

        void UpdateStatistics(sf::Time elapsedTime);

        static const sf::Time m_TimePerFrame;
        sf::Text m_StatisticsText;
        sf::Time m_StatisticsUpdateTime;
        sf::Window m_GameWindow;
};

#endif // GAMEENGINE_H
