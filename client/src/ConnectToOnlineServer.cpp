#include "ConnectToOnlineServer.hpp"
#include "MainMenuScene.hpp"
#include "ConnectingToServerScene.h"

ConnectToOnlineServer::ConnectToOnlineServer(sf::IpAddress a, unsigned short port) : m_offlineAddres(a), m_offlinePort(port)
{
    //ctor
}

ConnectToOnlineServer::~ConnectToOnlineServer()
{
    //dtor
}

void ConnectToOnlineServer::Draw(sf::RenderWindow& rw)
{
    m_gui.draw();
}

void ConnectToOnlineServer::Update(const sf::Time& ur)
{

}

void ConnectToOnlineServer::HandleInput(const sf::Event& event)
{
    m_gui.handleEvent(event);
}

const std::string ConnectToOnlineServer::Name() const
{
    return "Online connection setup scene";
}

bool ConnectToOnlineServer::LoadScene()
{
    /*
    *  Create a TGUI theme
    */
    m_theme = tgui::Theme::create("Media/Widgets/Black.txt");
    auto windowWidth = tgui::bindWidth(m_gui);
    auto windowHeight = tgui::bindHeight(m_gui);
    int windowHeightINT = GameEngine::Instance().getRenderWindow().getSize().y;
    int windowWidthINT = GameEngine::Instance().getRenderWindow().getSize().x;
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
    Title->setTextSize(windowWidthINT*75/1280);
    Title->setTextColor(sf::Color::White);
    Title->setText("Input server Data");
    Title->setPosition((windowWidth/2.0f)-(Title->getSize().x*windowWidth*3.f/1280.f), windowHeight*80.f/720.f);
    m_gui.add(Title);

    /*
    *   Create Server IP label
    */
    tgui::Label::Ptr serverTitle = m_theme->load("Label");
    serverTitle->setAutoSize(true);
    serverTitle->setTextSize(windowWidthINT*30/1280);
    serverTitle->setPosition(windowWidth/2.95f, windowHeight/3.6f);
    serverTitle->setTextColor(sf::Color(211,211,211));
    serverTitle->setText("Server IP:");
    m_gui.add(serverTitle);
    sf::Vector2f titlePos = serverTitle->getPosition();


    /*
    *  Create the text box to input the server ip
    */
    titlePos.y += 45.f*windowHeightINT/720.f;
    tgui::TextBox::Ptr textbox = m_theme->load("TextBox");
    textbox->setSize(windowWidth/3.0967f, windowHeight/21.6f);
    textbox->setPosition(titlePos);
    if(m_offlineAddres != sf::IpAddress::None)
        textbox->setText(StringHelpers::toString(m_offlineAddres));
    m_gui.add(textbox);


    /*
    *   Create Port label
    */
    titlePos.y += 60.f*windowHeightINT/720.f;
    tgui::Label::Ptr portTitle = m_theme->load("Label");
    portTitle->setAutoSize(true);
    portTitle->setTextSize(windowWidthINT*30/1280);
    portTitle->setPosition(titlePos);
    portTitle->setTextColor(sf::Color(211,211,211));
    portTitle->setText("Server UDP port:");
    m_gui.add(portTitle);

    /*
    *  Create the text box to input the server udp port
    */
    titlePos.y += 45.f*windowHeightINT/720.f;
    tgui::TextBox::Ptr serverPortTextBox = m_theme->load("TextBox");
    serverPortTextBox->setSize(windowWidth/3.0967f, windowHeight/21.6f);
    serverPortTextBox->setPosition(titlePos);
    if(m_offlineAddres != sf::IpAddress::None)
        serverPortTextBox->setText(StringHelpers::toString(m_offlinePort));
    m_gui.add(serverPortTextBox);


    /*
    *   Create the connect button and add it to the gui
    */
    titlePos.y += 60.f*windowHeightINT/720.f;
    tgui::Button::Ptr ConnectButton = m_theme->load("Button");
    ConnectButton->setSize(windowWidth/3.0967f, windowHeight/21.6f);
    ConnectButton->setPosition(titlePos);
    ConnectButton->setText("Connect");
    m_gui.add(ConnectButton);
    ConnectButton->connect("pressed", &ConnectToOnlineServer::onConnectClick, this, std::bind(&tgui::TextBox::getText, textbox), std::bind(&tgui::TextBox::getText, serverPortTextBox));


    /*
    *   Create the cancel Button and add it to the gui
    */
    titlePos.y += 60.f*windowHeightINT/720.f;
    tgui::Button::Ptr closeButton = m_theme->load("Button");
    closeButton->setSize(windowWidth/3.0967f, windowHeight/21.6f);
    closeButton->setPosition(titlePos);
    closeButton->setText("Back to Main Menu");
    m_gui.add(closeButton);
    closeButton->connect("pressed", &ConnectToOnlineServer::onClickCancel, this);


    /*
    *   Error msg label
    */
    titlePos.y += 60.f*windowHeightINT/720.f;
    m_error = m_theme->load("Label");
    m_error->setAutoSize(true);
    m_error->setTextSize(windowWidthINT*30/1280);
    m_error->setPosition(titlePos);
    m_error->setTextColor(sf::Color::Red);
    if(m_offlineAddres != sf::IpAddress::None)
        m_error->setText("Server is Offline!");
    else
        m_error->setText("");
    m_gui.add(m_error);

    return true;
}

bool ConnectToOnlineServer::UnloadScene()
{
    return true;
}

void ConnectToOnlineServer::onClickCancel()
{
    GameEngine::Instance().getSceneManager().setActiveScene(*new MainMenuScene());
}

void ConnectToOnlineServer::onConnectClick(std::string ip, std::string port)
{
    sf::IpAddress a(ip);
    try{
        int portInt = std::stoi(port);
        if(a != sf::IpAddress::None)
        {
            GameEngine::Instance().getSceneManager().setActiveScene(*new ConnectingToServerScene(a, (unsigned short)portInt, true));
        } else
        {
            m_error->setText("Invalid Server IP!");
        }
    } catch (const std::exception&)
    {
        m_error->setText("Invalid Server Port!");
    }
}


