#ifndef GLOBALLOBBYSCENE_H
#define GLOBALLOBBYSCENE_H

#include <Scene.hpp>


class GlobalLobbyScene : public Scene
{
    public:
        GlobalLobbyScene();
        virtual ~GlobalLobbyScene();

        void Draw(sf::RenderWindow& rw) final override;
        void Update(const sf::Time& ur) final override;
        void HandleInput(const sf::Event& event) final override;
        const std::string Name() const final override;

    protected:

    private:
        bool LoadScene() final override;
        bool UnloadScene() final override;

        void ScrollPanelScrolling(tgui::Panel::Ptr panel, int value);
        void OnClickChangeDPN();
        void OnClickCreateLobby();
        void OnClickReturnMenu();

        int m_previousScrollbarValue{0};

};

#endif // GLOBALLOBBYSCENE_H
