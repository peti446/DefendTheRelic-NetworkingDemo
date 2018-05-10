#include "GamePauseMenuScene.hpp"
#include "GameScene.hpp"
#include "GlobalLobbyScene.hpp"
#include "PlayerExitMatch.hpp"

GamePauseMenuScene::GamePauseMenuScene(GameScene* gameScene, std::string who) : m_gameScene(gameScene), m_exitName(who)
{
    //ctor
}

GamePauseMenuScene::~GamePauseMenuScene()
{
    //dtor
}

void GamePauseMenuScene::Draw(sf::RenderWindow& rw)
{
    m_gameScene->Draw(rw);
    m_gui.draw();
}

void GamePauseMenuScene::Update(const sf::Time& ur)
{
    m_gameScene->Update(ur);
}

void GamePauseMenuScene::HandleInput(const sf::Event& event)
{
    m_gui.handleEvent(event);
}

void GamePauseMenuScene::HandleNetworkInput(NetMessage* msg)
{
    m_gameScene->HandleNetworkInput(msg);
}

const std::string GamePauseMenuScene::Name() const
{
    return "Game Pause Menu";
}

bool GamePauseMenuScene::LoadScene()
{
    /*
    *  Import Theme and create gui for the config window.
    */
    m_theme = tgui::Theme::create("Media/Widgets/Black.txt");
    int windowWidth = GameEngine::Instance().getRenderWindow().getSize().x;
    int windowHeight = GameEngine::Instance().getRenderWindow().getSize().y;
    /*
    *  Import background image and apply it.
    */
    tgui::Picture::Ptr background = tgui::Picture::create("Media/Textures/MainMenu/PauseBackground.png");
    background->setSize(windowWidth, windowHeight);
    m_gui.add(background);

    /*
    *   Create the Continue Button and add it to the gui
    *   Size: windowWidth/3.0967 x windowHeight/21.6
    *   Position: x->windowWidth/2.95, y->windowHeight/3.6
    */
    tgui::Button::Ptr PlayButton = m_theme->load("Button");
    PlayButton->setSize(windowWidth/3.0967, windowHeight/21.6);
    PlayButton->setPosition(windowWidth/2.95, windowHeight/3.6);
    PlayButton->setText("Continue");
    m_gui.add(PlayButton);
    PlayButton->connect("pressed", &GamePauseMenuScene::onClickContinue, this);


    /*
    *   Create the Return to main menu button and add it to the gui
    *   Size: windowWidth/3.0967 x windowHeight/21.6
    *   Position: x->windowWidth/2.95, y->windowHeight/2.827
    */
    tgui::Button::Ptr closeButton = m_theme->load("Button");
    closeButton->setSize(windowWidth/3.0967, windowHeight/21.6);
    closeButton->setPosition(windowWidth/2.95, windowHeight/2.827);
    closeButton->setText("Return to Main Menu");
    m_gui.add(closeButton);
    closeButton->connect("pressed", &GamePauseMenuScene::onClickExit, this);
    return true;
}

bool GamePauseMenuScene::UnloadScene()
{
    return true;
}

void GamePauseMenuScene::onClickContinue()
{
    GameEngine::Instance().getSceneManager().setActiveScene(*m_gameScene);
}

void GamePauseMenuScene::onClickExit()
{
    GameEngine::Instance().getNetworkManager().send_tcp(new PlayerExitMatch(m_exitName));
    GameEngine::Instance().getSceneManager().setActiveScene(*new GlobalLobbyScene());
}

