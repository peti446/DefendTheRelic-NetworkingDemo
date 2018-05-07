#ifndef CHANGEDISPLAYNAMESCENE_HPP
#define CHANGEDISPLAYNAMESCENE_HPP

#include <Scene.hpp>


class ChangeDisplayNameScene : public Scene
{
    public:
        ChangeDisplayNameScene();
        virtual ~ChangeDisplayNameScene();

        void Draw(sf::RenderWindow& rw) final override;
        void Update(const sf::Time& ur) final override;
        void HandleInput(const sf::Event& event) final override;
        const std::string Name() const final override;
        void HandleNetworkInput(NetMessage* msg) final override;
    protected:

    private:
        bool LoadScene() final override;
        bool UnloadScene() final override;

        void onChangeClick(std::string newName);
        void onClickCancel();

        tgui::Label::Ptr m_error;
        tgui::Button::Ptr m_exitButton;
        tgui::Button::Ptr m_changeButton;
        tgui::TextBox::Ptr m_textBox;
        bool m_couldChange{false};
};

#endif // CHANGEDISPLAYNAMESCENE_HPP
