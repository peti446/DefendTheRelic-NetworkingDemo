#include "GlobalLobbyScene.hpp"
#include "MainMenuScene.hpp"
#include "ChangeDisplayNameScene.hpp"

GlobalLobbyScene::GlobalLobbyScene()
{
    //ctor
}

GlobalLobbyScene::~GlobalLobbyScene()
{
    //dtor
}

void GlobalLobbyScene::Draw(sf::RenderWindow& rw)
{
    m_gui.draw();
}

void GlobalLobbyScene::Update(const sf::Time& ur)
{

}

void GlobalLobbyScene::HandleInput(const sf::Event& event)
{
    m_gui.handleEvent(event);
}


void GlobalLobbyScene::HandleNetworkInput(NetMessage* msg)
{
    switch(msg->getType())
    {
        case eNetMessageType::eGetAllGameLobbyInfo:
        {
            break;
        }
        case eNetMessageType::eCreateGameLobby:
        {
            std::string s = ((CreateGameLobbyNetMessage*)msg)->Identifier;
            m_lobbies[s] = (LobbyInfoDisplay(m_layout, m_theme, (CreateGameLobbyNetMessage*)msg, m_lobbySize));
            m_layout->addSpace(0.1);
            Log() << "Created";
            break;
        }
        case eNetMessageType::eGameLobbyUpdated:
        {
            break;
        }
        case eNetMessageType::eJoinGameLobby:
        {
            break;
        }
    }
    delete msg;
}


const std::string GlobalLobbyScene::Name() const
{
    return "Main Lobby";
}

bool GlobalLobbyScene::LoadScene()
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
    Title->setText("Lobby");
    Title->setPosition((windowWidth/2.0f)-(Title->getSize().x), windowHeight*25.f/720.f);
    m_gui.add(Title);


    /*
    *   Create panel for the lobbies
    */
    tgui::Panel::Ptr panel = m_theme->load("Panel");
    panel->setSize(windowWidth * (2.f/3.f), windowHeight * (2.f/3.f));
    panel->setPosition(windowWidth*40.f/1280.f, windowHeight * ((1.f/3.f)/2.f));
    panel->setBackgroundColor(sf::Color(80,80,80,150));
    m_gui.add(panel);

    m_layout = tgui::VerticalLayout::create();
    m_layout->setSize(panel->getSize());
    m_layout->setPosition(panel->getPosition());
    m_lobbySize = panel->getSize().x - 10;
    m_gui.add(m_layout);

    /*
    *   Create a scroll bar
    */
    tgui::Scrollbar::Ptr scrollbar = m_theme->load("Scrollbar");
    scrollbar->setAutoHide(true);
    scrollbar->setSize(20, panel->getSize().y);
    scrollbar->setPosition(panel->getPosition() + sf::Vector2f(panel->getSize().x, 0));
    scrollbar->setArrowScrollAmount(30);
    scrollbar->setLowValue(panel->getSize().y);
    scrollbar->setMaximum(panel->getSize().y);
    scrollbar->connect("valuechanged", &GlobalLobbyScene::ScrollPanelScrolling, this, panel, std::bind(&tgui::Scrollbar::getValue, scrollbar));
    m_gui.add(scrollbar);

    sf::Vector2f leftPos = panel->getPosition() + sf::Vector2f(panel->getSize().x + 10, 0);
    int leftSizeX = windowWidthINT - leftPos.x - 15;
    /*
    *   Create display name tgui
    */

    tgui::Label::Ptr displayLabel = m_theme->load("Label");
    displayLabel->setPosition(leftPos);
    displayLabel->setAutoSize(true);
    displayLabel->setText("Display Name:  ");
    m_gui.add(displayLabel);

    /*
    *   Create Display name text
    */
    tgui::Label::Ptr displayName = m_theme->load("Label");
    displayName->setPosition(leftPos + sf::Vector2f(displayLabel->getSize().x,0));
    displayName->setAutoSize(true);
    displayName->setText(GameEngine::Instance().getNetworkManager().getDisplayName());
    m_gui.add(displayName);


    /*
    *   Create the Close Button and
    */
    leftPos.y += 30;
    tgui::Button::Ptr ChangeDisplayName = m_theme->load("Button");
    ChangeDisplayName->setPosition(leftPos);
    ChangeDisplayName->setSize(leftSizeX, windowHeight/21.6f);
    ChangeDisplayName->setText("Change Name");
    ChangeDisplayName->connect("pressed", &GlobalLobbyScene::OnClickChangeDPN, this);
    m_gui.add(ChangeDisplayName);


    /*
    *   Creates the create lobby button
    */
    leftPos.y += 50;
    tgui::Button::Ptr CreateLobbyBtn = m_theme->load("Button");
    CreateLobbyBtn->setPosition(leftPos);
    CreateLobbyBtn->setSize(leftSizeX, windowHeight/21.6f);
    CreateLobbyBtn->setText("Create Lobby");
    CreateLobbyBtn->connect("pressed", &GlobalLobbyScene::OnClickCreateLobby, this);
    m_gui.add(CreateLobbyBtn);

    /*
    *   Return to main menu button
    */
    leftPos.y += 50;
    tgui::Button::Ptr Exit = m_theme->load("Button");
    Exit->setPosition(leftPos);
    Exit->setSize(leftSizeX, windowHeight/21.6f);
    Exit->setText("Return to Main Menu");
    Exit->connect("pressed", &GlobalLobbyScene::OnClickReturnMenu, this);
    m_gui.add(Exit);

    //GameEngine::Instance().getNetworkManager().send_tcp();

    return true;
}

bool GlobalLobbyScene::UnloadScene()
{
    return true;
}


void GlobalLobbyScene::ScrollPanelScrolling(tgui::Panel::Ptr panel, int value)
{
    int distanceToMove = m_previousScrollbarValue - value;

    // Move all widgets that are inside the panel
    for (auto& widget : panel->getWidgets())
        widget->setPosition(widget->getPosition().x, widget->getPosition().y + distanceToMove);

    m_previousScrollbarValue = value;
}

void GlobalLobbyScene::OnClickChangeDPN()
{
    GameEngine::Instance().getSceneManager().setActiveScene(*new ChangeDisplayNameScene());
}

void GlobalLobbyScene::OnClickCreateLobby()
{
    GameEngine::Instance().getNetworkManager().send_tcp(new CreateGameLobbyNetMessage(true, GameEngine::Instance().getNetworkManager().getDisplayName()));
}

void GlobalLobbyScene::OnClickReturnMenu()
{
    GameEngine::Instance().getNetworkManager().Disconnect();
    GameEngine::Instance().getSceneManager().setActiveScene(*new MainMenuScene());
}


LobbyInfoDisplay::LobbyInfoDisplay(tgui::VerticalLayout::Ptr m_layout, tgui::Theme::Ptr theme, CreateGameLobbyNetMessage* gameLobbymsg, int width)
{
    int panelHeight = 150;
    m_identifier = gameLobbymsg->Identifier;

    tgui::Panel::Ptr panel = theme->load("Panel");
    panel->setBackgroundColor(sf::Color(0, 191, 255, 180));
    panel->setSize(width,115);


    tgui::Label::Ptr Title = theme->load("Label");
    Title->setAutoSize(true);
    Title->setText("Game Lobby");
    Title->setPosition(width/2.f - (Title->getSize().x/2.f), 15.f);
    panel->add(Title);


    tgui::Label::Ptr team1 = theme->load("Label");
    team1->setAutoSize(true);
    team1->setText("Attacker team (2 Players MAX)");
    team1->setPosition(20.f, 45.f);
    panel->add(team1);

    m_team1_p1 = theme->load("Label");
    m_team1_p1->setAutoSize(true);
    m_team1_p1->setText(gameLobbymsg->t1_p1);
    m_team1_p1->setPosition(20.f, 75.f);
    panel->add(m_team1_p1);

    m_team1_p2 = theme->load("Label");
    m_team1_p2->setAutoSize(true);
    m_team1_p2->setText(gameLobbymsg->t1_p2);
    m_team1_p2->setPosition(20.f, 115.f);
    panel->add(m_team1_p2);


    tgui::Label::Ptr team2 = theme->load("Label");
    team2->setAutoSize(true);
    team2->setText("Defender team (2 Players MAX)");
    team2->setPosition(width/2.f, 45.f);
    panel->add(team2);

    m_team2_p1 = theme->load("Label");
    m_team2_p1->setAutoSize(true);
    m_team2_p1->setText(gameLobbymsg->t2_p1);
    m_team2_p1->setPosition(width/2.f, 75.f);
    panel->add(m_team2_p1);

    m_team2_p1 = theme->load("Label");
    m_team2_p1->setAutoSize(true);
    m_team2_p1->setText(gameLobbymsg->t2_p2);
    m_team2_p1->setPosition(width/2.f, 115.f);
    panel->add(m_team2_p1);

    m_joinButton = theme->load("Button");
    m_joinButton->setSize(width/3.f, 30);
    m_joinButton->setPosition(width - m_joinButton->getSize().x - 5, panelHeight - 5);
    m_joinButton->setText("Join!");
    m_joinButton->connect("pressed", &LobbyInfoDisplay::onClickJoin, this);
    panel->add(m_joinButton);

    if(m_team1_p1->getText() != "NONE" && m_team1_p2->getText() != "NONE" && m_team2_p1->getText() != "NONE" && m_team2_p2->getText() != "NONE")
    {
        m_joinButton->disable();
        m_joinButton->setText("Full");
    } else
    {
        m_joinButton->enable();
        m_joinButton->setText("Join!");
    }

    m_layout->add(panel);
}

LobbyInfoDisplay::~LobbyInfoDisplay()
{

}

void LobbyInfoDisplay::updateStatus(std::string team1_p1, std::string team1_p2, std::string team2_p1, std::string team2_p2)
{
    m_team1_p1->setText(team1_p1);
    m_team1_p2->setText(team1_p2);
    m_team2_p1->setText(team2_p1);
    m_team2_p2->setText(team2_p2);
    if(team1_p1 != "NONE" && team1_p2 != "NONE" && team2_p1 != "NONE" && team2_p2 != "NONE")
    {
        m_joinButton->disable();
        m_joinButton->setText("Full");
    } else
    {
        m_joinButton->enable();
        m_joinButton->setText("Join!");
    }
}

const std::string& LobbyInfoDisplay::getIdentifier() const
{
    return m_identifier;
}

void LobbyInfoDisplay::onClickJoin()
{

}
