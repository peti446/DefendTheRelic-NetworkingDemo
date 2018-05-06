#include "GameEngine.hpp"

const sf::Time Game::TimePerFrame = sf::seconds(1.f/60.f);

GameEngine::GameEngine()
{
    //ctor
}

GameEngine::~GameEngine()
{
    //dtor
}

void GameEngine::run()
{

    GameLoop();
}

void GameEngine::GameLoop()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    while (m_GameWindow.isOpen())
    {
        sf::Time elapsedTime = clock.restart();
        timeSinceLastUpdate += elapsedTime;
        while (timeSinceLastUpdate > TimePerFrame)
        {
            timeSinceLastUpdate -= TimePerFrame;

            processEvents();
            update(TimePerFrame);
        }

        updateStatistics(elapsedTime);
        render();
    }
}

void Game::updateStatistics(sf::Time elapsedTime)
{
    m_StatisticsUpdateTime += elapsedTime;
    m_StatisticsNumFrames += 1;

    if (m_StatisticsUpdateTime >= sf::seconds(1.0f))
    {
        m_StatisticsText.setString(
            "Frames / Second = " + StringHelpers::toString(mStatisticsNumFrames) + "\n" +
            "Time / Update = " + StringHelpers::toString((mStatisticsUpdateTime.asMicroseconds() / mStatisticsNumFrames)*0.001f) + "ms");

        m_StatisticsUpdateTime -= sf::seconds(1.0f);
        m_StatisticsNumFrames = 0;
    }
}
