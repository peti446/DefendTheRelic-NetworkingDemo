#include "ConnectingToServerScene.h"
#include <chrono>

ConnectingToServerScene::ConnectingToServerScene(sf::IpAddress ip, unsigned short port): m_ip(ip), m_port(port)
{

}

ConnectingToServerScene::~ConnectingToServerScene()
{
}

void ConnectingToServerScene::Draw(sf::RenderWindow& rw)
{
    m_gui.draw();
}

void ConnectingToServerScene::Update(const float ur)
{
    if(m_connectToServer.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
    {
        //Load game scene
        return;
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
    int windowWidth = GameEngine::Instance().getRenderWindow().getSize().x;
    int windowHeight = GameEngine::Instance().getRenderWindow().getSize().y;
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
    tgui::Label::Ptr Title = m_theme->load("Label");
    Title->setAutoSize(true);
    Title->setTextSize(windowWidth*75/1280);
    Title->setTextColor(sf::Color::White);
    Title->setText("Connecting to server... Please wait.");
    Title->setPosition((windowWidth/2.0f)-(Title->getSize().x*windowWidth*3.5f/1280.f), windowHeight/2.f);
    this->m_gui.add(Title);

    m_connectToServer = std::async(std::launch::async, &Network::ConnectToServer, &GameEngine::Instance().getNetworkManager(), m_ip, m_port, 4);

    return true;
}

bool ConnectingToServerScene::UnloadScene()
{
    return true;
}
