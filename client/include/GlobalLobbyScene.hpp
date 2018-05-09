#ifndef GLOBALLOBBYSCENE_H
#define GLOBALLOBBYSCENE_H

#include <Scene.hpp>
#include "GameLobbyUpdateNetMessage.hpp"
#include <unordered_map>

class LobbyInfoDisplay
{
public:
    LobbyInfoDisplay(tgui::VerticalLayout::Ptr m_layout, tgui::Theme::Ptr theme, GameLobbyUpdateNetMessage* gameLobbymsg, int width);
    virtual ~LobbyInfoDisplay();

    bool updateStatus(std::string team1_p1, std::string team1_p2, std::string team2_p1, std::string team2_p2);
    const std::string& getIdentifier() const;
    const std::string getTeam1Player1();
    const std::string getTeam1Player2();
    const std::string getTeam2Player1();
    const std::string getTeam2Player2();
    bool isPlayerInLobby(const std::string& dn) const;
    tgui::Panel::Ptr getPanel();
private:
    void onClickJoin(std::string s);
    void onClickSwitch();
    void onPlayClick();

    tgui::Panel::Ptr m_panel;
    tgui::Label::Ptr m_team1_p1;
    tgui::Label::Ptr m_team1_p2;
    tgui::Label::Ptr m_team2_p1;
    tgui::Label::Ptr m_team2_p2;
    tgui::Button::Ptr m_joinButton;
    tgui::Button::Ptr m_switchTeam;
    tgui::Button::Ptr m_play;
    std::string m_identifier;
    std::string m_status;

};

class GlobalLobbyScene : public Scene
{
    public:
        GlobalLobbyScene();
        virtual ~GlobalLobbyScene();

        void Draw(sf::RenderWindow& rw) final override;
        void Update(const sf::Time& ur) final override;
        void HandleInput(const sf::Event& event) final override;
        const std::string Name() const final override;
        void HandleNetworkInput(NetMessage* msg) final override;

    protected:

    private:
        bool LoadScene() final override;
        bool UnloadScene() final override;

        void ScrollPanelScrolling(unsigned int value);
        void OnClickChangeDPN();
        void OnClickCreateLobby();
        void OnClickReturnMenu();

        int m_previousScrollbarValue{0};
        int m_lobbySize{0};
        tgui::VerticalLayout::Ptr m_layout;
        tgui::Scrollbar::Ptr m_scroll;
        std::unordered_map<std::string, LobbyInfoDisplay*> m_lobbies;


};

#endif // GLOBALLOBBYSCENE_H
