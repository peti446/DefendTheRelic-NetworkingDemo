#ifndef GLOBALLOBBYSCENE_H
#define GLOBALLOBBYSCENE_H

#include <Scene.hpp>
#include "CreateGameLobbyNetMessage.hpp"
#include <unordered_map>

class LobbyInfoDisplay
{
public:
    LobbyInfoDisplay() =default;
    LobbyInfoDisplay(tgui::VerticalLayout::Ptr m_layout, tgui::Theme::Ptr theme, CreateGameLobbyNetMessage* gameLobbymsg, int width);
    virtual ~LobbyInfoDisplay();

    void updateStatus(std::string team1_p1, std::string team1_p2, std::string team2_p1, std::string team2_p2);
    const std::string& getIdentifier() const;
private:
    void onClickJoin();

    tgui::Label::Ptr m_team1_p1;
    tgui::Label::Ptr m_team1_p2;
    tgui::Label::Ptr m_team2_p1;
    tgui::Label::Ptr m_team2_p2;
    tgui::Button::Ptr m_joinButton;
    std::string m_identifier;

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

        void ScrollPanelScrolling(tgui::Panel::Ptr panel, int value);
        void OnClickChangeDPN();
        void OnClickCreateLobby();
        void OnClickReturnMenu();

        int m_previousScrollbarValue{0};
        int m_lobbySize{0};
        tgui::VerticalLayout::Ptr m_layout;
        std::unordered_map<std::string, LobbyInfoDisplay> m_lobbies;


};

#endif // GLOBALLOBBYSCENE_H
