#include "EndGameMenuScene.hpp"
#include "GlobalLobbyScene.hpp"
#include "GameScene.hpp"
#include "PlayerExitMatch.hpp"

EndGameMenuScene::EndGameMenuScene(GameScene* gameScene, std::string winningTeam, sf::Uint8 t1Kills, sf::Uint8 t2kills) : m_gameScene(gameScene), m_winningTeam(winningTeam), m_t1_kills(t1Kills), m_t2_kills(t2kills)
{
    //ctor
}

EndGameMenuScene::~EndGameMenuScene()
{
    //dtor
}

void EndGameMenuScene::Draw(sf::RenderWindow& rw)
{
    m_gameScene->Draw(rw);
    m_gui.draw();
}

void EndGameMenuScene::Update(const sf::Time& ur)
{
    m_gameScene->Update(ur);
}

void EndGameMenuScene::HandleInput(const sf::Event& event)
{
    m_gui.handleEvent(event);
}

void EndGameMenuScene::HandleNetworkInput(NetMessage* msg)
{
    m_gameScene->HandleNetworkInput(msg);
}

const std::string EndGameMenuScene::Name() const
{
    return "Ending screen";
}

bool EndGameMenuScene::LoadScene()
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
    *   Create tiitle Label and add it to the gui
    *   Position it in the middle of the screen
    */
    tgui::Label::Ptr Title = m_theme->load("Label");
    Title->setAutoSize(true);
    Title->setTextSize(GameEngine::Instance().getRenderWindow().getSize().x*75/1280);
    Title->setTextColor(sf::Color::White);
    Title->setText("Team " + m_winningTeam + " wins!");
    Title->setPosition((windowWidth/2.0f)-(Title->getSize().x*windowWidth*3.5f/1280.f), windowHeight*80.f/720.f);
    m_gui.add(Title);

    tgui::Label::Ptr KillsT1 = m_theme->load("Label");
    KillsT1->setAutoSize(true);
    KillsT1->setTextColor(sf::Color::White);
    KillsT1->setText("Team 1 kills:" + StringHelpers::toString(m_t1_kills));
    KillsT1->setPosition((windowWidth/2.0f)-(KillsT1->getSize().x*windowWidth*3.5f/1280.f), windowHeight*130.f/720.f);
    m_gui.add(KillsT1);

    tgui::Label::Ptr KillsT2 = m_theme->load("Label");
    KillsT2->setAutoSize(true);
    KillsT2->setTextColor(sf::Color::White);
    KillsT2->setText("Team 2 kills:" + StringHelpers::toString(m_t2_kills));
    KillsT2->setPosition((windowWidth/2.0f)-(KillsT2->getSize().x*windowWidth*3.5f/1280.f), windowHeight*160.f/720.f);
    m_gui.add(KillsT2);

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
    closeButton->connect("pressed", &EndGameMenuScene::onClickExit, this);
    return true;
}

bool EndGameMenuScene::UnloadScene()
{
    return true;
}


void EndGameMenuScene::onClickExit()
{
    GameEngine::Instance().getNetworkManager().send_tcp(new PlayerExitMatch(GameEngine::Instance().getNetworkManager().getDisplayName()));
    GameEngine::Instance().getSceneManager().setActiveScene(*new GlobalLobbyScene());
}
