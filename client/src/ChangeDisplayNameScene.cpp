#include "ChangeDisplayNameScene.hpp"
#include "GlobalLobbyScene.hpp"
#include "MainMenuScene.hpp"
#include "DisplayNameUpdate.hpp"

ChangeDisplayNameScene::ChangeDisplayNameScene()
{
    //ctor
}

ChangeDisplayNameScene::~ChangeDisplayNameScene()
{
    //dtor
}


void ChangeDisplayNameScene::Draw(sf::RenderWindow& rw)
{
    m_gui.draw();
}

void ChangeDisplayNameScene::Update(const sf::Time& ur)
{
}

void ChangeDisplayNameScene::HandleNetworkInput(NetMessage* msg)
{
    if(msg->getType() == eNetMessageType::eDisplayNameUpdate)
    {
        GameEngine::Instance().getNetworkManager().HandleChangeDN((DisplayNameUpdate*)msg);
        GameEngine::Instance().getSceneManager().setActiveScene(*new GlobalLobbyScene());
    }
}


void ChangeDisplayNameScene::HandleInput(const sf::Event& event)
{
    m_gui.handleEvent(event);
}

const std::string ChangeDisplayNameScene::Name() const
{
    return "Change Display Name";
}

bool ChangeDisplayNameScene::LoadScene()
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
    Title->setText("Change Name");
    Title->setPosition((windowWidth/2.0f)-(Title->getSize().x*windowWidth*2.0f/720.f), windowHeight*25.f/720.f);
    m_gui.add(Title);
    sf::Vector2f titlePos = Title->getPosition();

    /*
    *  Create the text box to input the new name
    */
    titlePos.y += 45.f*windowHeightINT/720.f;
    tgui::TextBox::Ptr textbox = m_theme->load("TextBox");
    textbox->setSize(windowWidth/3.0967f, windowHeight/21.6f);
    textbox->setPosition(titlePos);
    textbox->setText(GameEngine::Instance().getNetworkManager().getDisplayName());
    m_gui.add(textbox);


    /*
    *   Create the Change button and add it to the gui
    */
    titlePos.y += 60.f*windowHeightINT/720.f;
    m_changeButton = m_theme->load("Button");
    m_changeButton->setSize(windowWidth/3.0967f, windowHeight/21.6f);
    m_changeButton->setPosition(titlePos);
    m_changeButton->setText("Change");
    m_gui.add(m_changeButton);
    m_changeButton->connect("pressed", &ChangeDisplayNameScene::onChangeClick, this, std::bind(&tgui::TextBox::getText, textbox));

    /*
    *   Create the cancel Button and add it to the gui
    */
    titlePos.y += 60.f*windowHeightINT/720.f;
    tgui::Button::Ptr closeButton = m_theme->load("Button");
    closeButton->setSize(windowWidth/3.0967f, windowHeight/21.6f);
    closeButton->setPosition(titlePos);
    closeButton->setText("Back to Lobby");
    m_gui.add(closeButton);
    closeButton->connect("pressed", &ChangeDisplayNameScene::onClickCancel, this);

    /*
    *   Error msg label
    */
    titlePos.y += 60.f*windowHeightINT/720.f;
    m_error = m_theme->load("Label");
    m_error->setAutoSize(true);
    m_error->setTextSize(windowWidthINT*30/1280);
    m_error->setPosition(titlePos);
    m_error->setTextColor(sf::Color::Red);
    m_gui.add(m_error);

    return true;
}

bool ChangeDisplayNameScene::UnloadScene()
{
    return true;
}
void ChangeDisplayNameScene::onChangeClick(std::string newName)
{
    //m_changeButton->disable();
    //m_exitButton->disable();
    //m_textBox->disable();
    m_couldChange = GameEngine::Instance().getNetworkManager().send_tcp(new DisplayNameUpdate(newName));
    if(!m_couldChange)
    {
        m_error->setText("Could not start the server!");
       // m_changeButton->enable();
       // m_exitButton->enable();
        m_exitButton->setText("Return to Main Menu");
      //  m_textBox->enable();
    } else
    {
        m_error->setText("Changing name ...");
    }
}

void ChangeDisplayNameScene::onClickCancel()
{
    if(!m_couldChange)
    {
        GameEngine::Instance().getSceneManager().setActiveScene(*new MainMenuScene());
    }
    GameEngine::Instance().getSceneManager().setActiveScene(*new GlobalLobbyScene());
}


