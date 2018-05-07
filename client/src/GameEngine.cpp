#include "GameEngine.hpp"
#include "Logger.hpp"
#include "Scene.hpp"
#include "MainMenuScene.hpp"
#include "StringHelpers.hpp"

const sf::Time GameEngine::TimePerFrame = sf::seconds(1.f/60.f);

GameEngine::GameEngine()
{
    //ctor
}

GameEngine::~GameEngine()
{
    //dtor
}

GameEngine& GameEngine::Instance()
{
    static GameEngine* m_singleton;
    if(m_singleton == nullptr)
    {
        m_singleton = new GameEngine();
    }
    return *m_singleton;
}

void GameEngine::run()
{
    //Make sure we can only run it once
    if(m_GameWindow.isOpen())
    {
        Log(l_CRITICAL) << "Somthing tryed to run the game again, but it is already running";
        return;
    }

    Log(l_DEBUG) << "Setting up Game to run";
    m_GameWindow.create(sf::VideoMode(1280,720), "Defend the Relic", sf::Style::Titlebar | sf::Style::Close);
    //Set up statistics text
    m_Font.loadFromFile("Media/Fonts/Sansation.ttf");
    m_StatisticsText.setFont(m_Font);
    m_StatisticsText.setPosition(5.f, 5.f);
    m_StatisticsText.setCharacterSize(10);
    Log(l_DEBUG) << "Creating first scene";
    //Set up data the first scene
    m_sceneManager.setActiveScene(*new MainMenuScene());
    //Run the game loop
    GameLoop();
}

void GameEngine::stop()
{
    m_GameWindow.close();
}


void GameEngine::GameLoop()
{
    Log(l_DEBUG) << "Starting Game Loop";
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    while (m_GameWindow.isOpen())
    {
        //Update statistics numbers
        sf::Time elapsedTime = clock.restart();
        timeSinceLastUpdate += elapsedTime;
        //Get the current active scene form the scene manager
        Scene* s = m_sceneManager.getCurrentActiveScene();
        //Scene is null call aux renderer and handler so the app does not stall#
        //Could directly exit, but im keeping it open and leave it up to the user to decide
        if(s == nullptr)
        {
            UpdateStatistics(elapsedTime);
            auxDraw();
            auxEventHandler();
            continue;
        }

        //At this point scene is not null so just execute the normal game loop, using the scenes functions.
        //Updatate once for every #tickintervalNumber seconds, based on the time that has passed since the last update was executed(timeSinceLastUpdate)
        while (timeSinceLastUpdate > TimePerFrame)
        {
            timeSinceLastUpdate -= TimePerFrame;

            sf::Event e;
            while (m_GameWindow.pollEvent(e))
            {
                //If we detect that we need to exit close the program
                if(e.type == sf::Event::Closed)
                {
                    stop();
                    continue;
                }
                //Send a handle message to the scene
                s->HandleInput(e);
            }
            //Update the scene
            s->Update(TimePerFrame);
        }

        //Update the statistics
        UpdateStatistics(elapsedTime);
        //Clear and render the scene/statistics info
        m_GameWindow.clear();
        s->Draw(m_GameWindow);
        m_GameWindow.draw(m_StatisticsText);
        m_GameWindow.display();
        //Delete any stored scenes from here one the game loop will not use the scene that it acquired at the begining
        m_sceneManager.garbageCollection();
    }
    Log(l_DEBUG) << "Exiting Game Loop";
    CleanUp();
}

void GameEngine::CleanUp()
{
    Log(l_DEBUG) << "Cleaning up the game before exiting";
    //Make sure we delete the singleton!
    static GameEngine* m_singleton;
    delete m_singleton;
    m_singleton = nullptr;
}
void GameEngine::auxEventHandler()
{
    sf::Event e;
    while (m_GameWindow.pollEvent(e))
    {
        if(e.type == sf::Event::Closed)
        {
            stop();
            return;
        }
    }
}

void GameEngine::auxDraw()
{
    m_GameWindow.clear();
    m_GameWindow.draw(m_StatisticsText);
    m_GameWindow.display();
    m_sceneManager.garbageCollection();
}


void GameEngine::UpdateStatistics(sf::Time elapsedTime)
{
    m_StatisticsUpdateTime += elapsedTime;
    m_StatisticsNumFrames++;

    if (m_StatisticsUpdateTime >= sf::seconds(1.0f))
    {
        m_StatisticsText.setString(
            "Frames / Second = " + StringHelpers::toString(m_StatisticsNumFrames) + "\n" +
            "Time / Update = " + StringHelpers::toString((m_StatisticsUpdateTime.asMicroseconds() / m_StatisticsNumFrames)*0.001f) + "ms");

        m_StatisticsUpdateTime -= sf::seconds(1.0f);
        m_StatisticsNumFrames = 0;
    }
}

SceneManager& GameEngine::getSceneManager()
{
    return m_sceneManager;
}

sf::RenderWindow& GameEngine::getRenderWindow()
{
    return m_GameWindow;
}
Network& GameEngine::getNetworkManager()
{
    return m_net;
}


