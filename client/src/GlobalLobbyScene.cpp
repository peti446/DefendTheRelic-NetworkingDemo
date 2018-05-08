#include "GlobalLobbyScene.hpp"
#include "MainMenuScene.hpp"
#include "ChangeDisplayNameScene.hpp"
#include "GameLobbyUpdateNetMessage.hpp"
#include "GameLobbyStatusChangedNetMessage.h"
#include "FetchLobbyList.hpp"

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
        case eNetMessageType::eGameLobbyUpdated:
        {
            GameLobbyUpdateNetMessage* cglm = (GameLobbyUpdateNetMessage*)msg;
            if(m_lobbies.find(cglm->Identifier) == m_lobbies.end())
            {
                m_lobbies.insert(std::make_pair(cglm->Identifier, new LobbyInfoDisplay(m_layout, m_theme, cglm, m_lobbySize)));
                m_layout->addSpace(0.1);
                m_scroll->setMaximum(m_layout->getSize().y);
            }
            else
            {
                if (!m_lobbies.at(cglm->Identifier)->updateStatus(cglm->t1_p1, cglm->t1_p2, cglm->t2_p1, cglm->t2_p2))
                {
                    m_layout->remove(m_lobbies.at(cglm->Identifier)->getPanel());
                    int newSize =  m_layout->getSize().y - m_lobbySize;
                    if(newSize < 0)
                        newSize = 0;
                    m_layout->setSize(m_layout->getSize().x, newSize);
                    m_scroll->setMaximum(newSize);
                    delete m_lobbies.at(cglm->Identifier);
                    m_lobbies.erase(cglm->Identifier);
                }
            }

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
    panel->setPosition(windowWidth*40.f/1280.f, windowHeight * ((1.f/3.f)/2.f));
    panel->setSize(windowWidth * (2.f/3.f), windowHeight * (2.f/3.f) + panel->getPosition().y);
    panel->setBackgroundColor(sf::Color(80,80,80,150));
    m_gui.add(panel);

    m_layout = tgui::VerticalLayout::create();
    m_layout->setSize(panel->getSize().x, 0);
    m_lobbySize = panel->getSize().x - 10;
    panel->add(m_layout);

    /*
    *   Create a scroll bar
    */
    m_scroll = m_theme->load("Scrollbar");
    m_scroll->setAutoHide(true);
    m_scroll->setSize(20, panel->getSize().y);
    m_scroll->setPosition(panel->getPosition() + sf::Vector2f(panel->getSize().x, 0));
    m_scroll->setArrowScrollAmount(30);
    m_scroll->setLowValue(panel->getSize().y);
    m_scroll->setMaximum(m_layout->getSize().y);
    m_scroll->connect("ValueChanged", &GlobalLobbyScene::ScrollPanelScrolling, this, std::bind(&tgui::Scrollbar::getValue, m_scroll));
    m_gui.add(m_scroll);

    sf::Vector2f leftPos = panel->getPosition() + sf::Vector2f(panel->getSize().x + 30, 0);
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

    GameEngine::Instance().getNetworkManager().send_tcp(new FetchLobbyList());

    return true;
}

bool GlobalLobbyScene::UnloadScene()
{
    return true;
}


void GlobalLobbyScene::ScrollPanelScrolling(unsigned int value)
{
    int distanceToMove = m_previousScrollbarValue - value;

    // Move all widgets that are inside the panel
    for (auto& widget : m_layout->getWidgets())
        widget->setPosition(widget->getPosition().x, widget->getPosition().y + distanceToMove);

    m_previousScrollbarValue = value;
}

void GlobalLobbyScene::OnClickChangeDPN()
{
    GameEngine::Instance().getSceneManager().setActiveScene(*new ChangeDisplayNameScene());
}

void GlobalLobbyScene::OnClickCreateLobby()
{
    GameEngine::Instance().getNetworkManager().send_tcp(new GameLobbyUpdateNetMessage(true, GameEngine::Instance().getNetworkManager().getDisplayName()));
}

void GlobalLobbyScene::OnClickReturnMenu()
{
    GameEngine::Instance().getNetworkManager().Disconnect();
    GameEngine::Instance().getSceneManager().setActiveScene(*new MainMenuScene());
}


LobbyInfoDisplay::LobbyInfoDisplay(tgui::VerticalLayout::Ptr m_layout, tgui::Theme::Ptr theme, GameLobbyUpdateNetMessage* gameLobbymsg, int width)
{
    int panelHeight = 180;
    m_identifier = gameLobbymsg->Identifier;

    m_panel = theme->load("Panel");
    m_panel->setBackgroundColor(sf::Color(0, 191, 255, 180));
    m_panel->setSize(width,115);


    tgui::Label::Ptr Title = theme->load("Label");
    Title->setAutoSize(true);
    Title->setTextColor(sf::Color::Black);
    Title->setTextSize(18);
    Title->setText("Game Lobby");
    Title->setPosition(width/2.f - (Title->getSize().x/2.f), 15.f);
    m_panel->add(Title);


    tgui::Label::Ptr team1 = theme->load("Label");
    team1->setAutoSize(true);
    team1->setTextColor(sf::Color::Black);
    team1->setText("Attacker team (2 Players MAX)");
    team1->setPosition(20.f, 45.f);
    m_panel->add(team1);

    m_team1_p1 = theme->load("Label");
    m_team1_p1->setAutoSize(true);
    m_team1_p1->setTextColor(sf::Color::Black);
    m_team1_p1->setText(gameLobbymsg->t1_p1);
    m_team1_p1->setPosition(20.f, 75.f);
    m_panel->add(m_team1_p1);

    m_team1_p2 = theme->load("Label");
    m_team1_p2->setAutoSize(true);
    m_team1_p2->setTextColor(sf::Color::Black);
    m_team1_p2->setText(gameLobbymsg->t1_p2);
    m_team1_p2->setPosition(20.f, 115.f);
    m_panel->add(m_team1_p2);


    tgui::Label::Ptr team2 = theme->load("Label");
    team2->setAutoSize(true);
    team2->setText("Defender team (2 Players MAX)");
    team2->setTextColor(sf::Color::Black);
    team2->setPosition(width/2.f, 45.f);
    m_panel->add(team2);

    m_team2_p1 = theme->load("Label");
    m_team2_p1->setAutoSize(true);
    m_team2_p1->setTextColor(sf::Color::Black);
    m_team2_p1->setText(gameLobbymsg->t2_p1);
    m_team2_p1->setPosition(width/2.f, 75.f);
    m_panel->add(m_team2_p1);

    m_team2_p2 = theme->load("Label");
    m_team2_p2->setAutoSize(true);
    m_team2_p2->setTextColor(sf::Color::Black);
    m_team2_p2->setText(gameLobbymsg->t2_p2);
    m_team2_p2->setPosition(width/2.f, 115.f);
    m_panel->add(m_team2_p2);

    m_joinButton = theme->load("Button");
    m_joinButton->setSize(width/3.f, 30);
    m_joinButton->setPosition(width - m_joinButton->getSize().x - 5, panelHeight - 30 - 5);
    m_joinButton->setText("Join!");
    m_joinButton->connect("pressed", &LobbyInfoDisplay::onClickJoin, this, std::bind(tgui::Button::getText, m_joinButton));
    m_panel->add(m_joinButton);

    m_switchTeam = theme->load("Button");
    m_switchTeam->setSize(width/3.f, 30);
    m_switchTeam->setPosition(width - m_switchTeam->getSize().x - (width/3.f) - 15, panelHeight - 30 - 5);
    m_switchTeam->setText("Switch Team");
    m_switchTeam->connect("pressed", &LobbyInfoDisplay::onClickSwitch, this);
    m_switchTeam->hide();
    m_panel->add(m_switchTeam);

    m_layout->add(m_panel);
    m_layout->setFixedSize(m_panel, panelHeight);
    m_layout->setSize(m_layout->getSize() + sf::Vector2f(0, panelHeight + 10));
    updateStatus(gameLobbymsg->t1_p1, gameLobbymsg->t1_p2, gameLobbymsg->t2_p1, gameLobbymsg->t2_p2);
}

LobbyInfoDisplay::~LobbyInfoDisplay()
{

}

bool LobbyInfoDisplay::updateStatus(std::string team1_p1, std::string team1_p2, std::string team2_p1, std::string team2_p2)
{
    if(team1_p1 == "NONE" && team1_p2 == "NONE" && team2_p1 == "NONE" && team2_p2 == "NONE")
    {
        return false;
    }

    m_team1_p1->setText(team1_p1);
    m_team1_p2->setText(team1_p2);
    m_team2_p1->setText(team2_p1);
    m_team2_p2->setText(team2_p2);
    if(team1_p1 != "NONE" && team1_p2 != "NONE" && team2_p1 != "NONE" && team2_p2 != "NONE")
    {
        m_joinButton->disable();
        m_joinButton->setText("Full");
        m_panel->setBackgroundColor(sf::Color(255, 102, 102, 180));
    }
    else if(isPlayerInLobby(GameEngine::Instance().getNetworkManager().getDisplayName()))
    {
        m_joinButton->enable();
        m_switchTeam->show();
        m_joinButton->setText("Exit");
        m_panel->setBackgroundColor(sf::Color(0, 255, 204, 180));

    }
    else
    {
        m_joinButton->enable();
        m_joinButton->setText("Join!");
        m_panel->setBackgroundColor(sf::Color(0, 191, 255, 180));
    }
    return true;
}

tgui::Panel::Ptr LobbyInfoDisplay::getPanel()
{
    return m_panel;
}


const std::string& LobbyInfoDisplay::getIdentifier() const
{
    return m_identifier;
}
bool LobbyInfoDisplay::isPlayerInLobby(const std::string& dn) const
{
    return m_team1_p1->getText() == dn
            || m_team1_p2->getText() == dn
            || m_team2_p1->getText() == dn
            || m_team2_p2->getText() == dn;
}


void LobbyInfoDisplay::onClickJoin(std::string s)
{
    if(s == "Join!")
        GameEngine::Instance().getNetworkManager().send_tcp(new GameLobbyStatusChangedNetMessage(m_identifier, GameLobbyStatusChangedNetMessage::UpdateMsg::eJoin));
    else if(s == "Exit")
        GameEngine::Instance().getNetworkManager().send_tcp(new GameLobbyStatusChangedNetMessage(m_identifier, GameLobbyStatusChangedNetMessage::UpdateMsg::eExit));
}

void LobbyInfoDisplay::onClickSwitch()
{
    GameEngine::Instance().getNetworkManager().send_tcp(new GameLobbyStatusChangedNetMessage(m_identifier, GameLobbyStatusChangedNetMessage::UpdateMsg::eSwitch));
}

