#include "ConnectingToServerScene.h"
#include <chrono>
#include "MainMenuScene.hpp"
#include "ConnectToOnlineServer.hpp"
#include "GameLobbyScene.hpp"

ConnectingToServerScene::ConnectingToServerScene(sf::IpAddress ip, unsigned short port,  bool retrunOnline): m_ip(ip), m_port(port), m_returnToOnline(retrunOnline)
{

}

ConnectingToServerScene::~ConnectingToServerScene()
{
}

void ConnectingToServerScene::Draw(sf::RenderWindow& rw)
{
    m_gui.draw();
}

void ConnectingToServerScene::Update(const sf::Time& ur)
{
    if(m_connectToServer.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
    {
        if(m_connectToServer.get())
        {
            GameEngine::Instance().getSceneManager().setActiveScene(*new GameLobbyScene());
            //Change to game lobby scene
        }
        else
        {
            //Change to main screen
            if(m_returnToOnline)
                GameEngine::Instance().getSceneManager().setActiveScene(*new ConnectToOnlineServer(m_ip, m_port));
            else
                GameEngine::Instance().getSceneManager().setActiveScene(*new MainMenuScene());
        }
        return;
    }
    m_secondsPassed += ur.asSeconds();
    if(m_secondsPassed >= 1.0f)
    {
        m_secondsPassed = 0;
        std::string t = Title->getText();
        if(t.size() == m_textSize){
            m_textDir = 1;
        } else if(t.size() == m_textSize+3){
            m_textDir = -1;
        }

        if(m_textDir == 1)
        {
            t += ".";
        }else
        {
            t = t.substr(0, t.size()-1);
        }

        Title->setText(t);
    }
}

void ConnectingToServerScene::HandleInput(const sf::Event& event)
{
    m_gui.handleEvent(event);
}

const std::string ConnectingToServerScene::Name() const
{
    return "Connecting To Server Loading Scene";
}

bool ConnectingToServerScene::LoadScene()
{

    /*
    *  Create a TGUI theme
    */
    m_theme = tgui::Theme::create("Media/Widgets/Black.txt");
    auto windowWidth = tgui::bindWidth(m_gui);
    auto windowHeight = tgui::bindHeight(m_gui);
    /*
    *  Import background image and apply it.
    */
    tgui::Picture::Ptr background = tgui::Picture::create("Media/Textures/MainMenu/Background.jpg");
    background->setSize(windowWidth, windowHeight);
    m_gui.add(background);

    /*
    *   Create tiitle Label and add it to the gui
    *   Position it in the middle of the screen
    */
    Title = m_theme->load("Label");
    Title->setAutoSize(true);
    Title->setTextSize(GameEngine::Instance().getRenderWindow().getSize().x*50/1280);
    Title->setTextColor(sf::Color::White);
    Title->setText("Connecting to server... Please wait.");
    m_textDir = 1;
    m_textSize = Title->getText().toAnsiString().size();
    Title->setPosition((windowWidth/2.0f)-(Title->getSize().x*windowWidth*4.5f/1280.f), windowHeight/2.f);
    m_gui.add(Title);

    m_connectToServer = std::async(std::launch::async, &Network::ConnectToServer, &GameEngine::Instance().getNetworkManager(), m_ip, m_port, 4);

    return true;
}

bool ConnectingToServerScene::UnloadScene()
{
    return true;
}
