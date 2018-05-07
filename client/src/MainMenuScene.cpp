#include "MainMenuScene.hpp"

MainMenuScene::MainMenuScene()
{
}

MainMenuScene::~MainMenuScene()
{
}

void MainMenuScene::Draw(sf::RenderWindow& rw)
{
    m_gui.draw();
}

void MainMenuScene::Update(const float ur)
{
}

void MainMenuScene::HandleInput(const sf::Event& event)
{
    m_gui.handleEvent(event);
}

const std::string MainMenuScene::Name() const
{
    return "Main Menu";
}

bool MainMenuScene::LoadScene()
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
    Title->setText("~ Defend the Relic ~");
    Title->setPosition((windowWidth/2.0f)-(Title->getSize().x*windowWidth*3.5f/1280.f), windowHeight*80.f/720.f);
    this->m_gui.add(Title);

    /*
    *   Create the Single Button and add it to the gui
    */
    tgui::Button::Ptr PlayButton = m_theme->load("Button");
    PlayButton->setSize(windowWidth/3.0967f, windowHeight/21.6f);
    PlayButton->setPosition(windowWidth/2.95f, windowHeight/3.6f);
    PlayButton->setText("Singleplayer");
    this->m_gui.add(PlayButton);
    //PlayButton->connect("pressed", &MainMenuScene::newSnakeGame, this);

    sf::Vector2f bs = PlayButton->getPosition();

    /*
    *   Create the Lan Server Button and add it to the gui
    */
    bs.y += 60.f*windowHeight/720.f;
    tgui::Button::Ptr LanServer = m_theme->load("Button");
    LanServer->setSize(windowWidth/3.0967f, windowHeight/21.6f);
    LanServer->setPosition(bs.x, bs.y);
    LanServer->setText("LAN Server");
    m_gui.add(LanServer);
    //closeButton->connect("pressed", [&](){ GameEngine::Instance().stop(); });

    /*
    *   Create the Online Button and add it to the gui
    */
    bs.y += 60.f*windowHeight/720.f;
    tgui::Button::Ptr OnlineButton = m_theme->load("Button");
    OnlineButton->setSize(windowWidth/3.0967f, windowHeight/21.6f);
    OnlineButton->setPosition(bs.x, bs.y);
    OnlineButton->setText("Online Server");
    m_gui.add(OnlineButton);
    //closeButton->connect("pressed", [&](){ GameEngine::Instance().stop(); });

    /*
    *   Create the Option Button and add it to the gui
    */
    bs.y += 60.f*windowHeight/720.f;
    tgui::Button::Ptr OptionButton = m_theme->load("Button");
    OptionButton->setSize(windowWidth/3.0967f, windowHeight/21.6f);
    OptionButton->setPosition(bs.x, bs.y);
    OptionButton->setText("Options");
    m_gui.add(OptionButton);
    //closeButton->connect("pressed", [&](){ GameEngine::Instance().stop(); });

    /*
    *   Create the Close Button and add it to the gui
    */
    bs.y += 60.f*windowHeight/720.f;
    tgui::Button::Ptr closeButton = m_theme->load("Button");
    closeButton->setSize(windowWidth/3.0967f, windowHeight/21.6f);
    closeButton->setPosition(bs.x, bs.y);
    closeButton->setText("Exit");
    m_gui.add(closeButton);
    closeButton->connect("pressed", [&](){ GameEngine::Instance().stop(); });
    return true;
}

bool MainMenuScene::UnloadScene()
{
    return true;
}
