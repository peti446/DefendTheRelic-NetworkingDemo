#ifndef MAINMENUSCENE_H
#define MAINMENUSCENE_H

#include <Scene.hpp>


class MainMenuScene : public Scene
{
    public:
        MainMenuScene();
        virtual ~MainMenuScene();

        void Draw(sf::RenderWindow& rw) final override;
        void Update(const sf::Time& ur) final override;
        void HandleInput(const sf::Event& event) final override;
        const std::string Name() const final override;
    protected:

    private:
        bool LoadScene() final override;
        bool UnloadScene() final override;

        void OnClickLanServer();
        void onClickOnlineServer();
};

#endif // MAINMENUSCENE_H
