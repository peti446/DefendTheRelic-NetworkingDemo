#ifndef GAMEPAUSEMENUSCENE_HPP
#define GAMEPAUSEMENUSCENE_HPP

#include <Scene.hpp>
class GameScene;

class GamePauseMenuScene : public Scene
{
    public:
        GamePauseMenuScene(GameScene* gameScene, std::string who);
        virtual ~GamePauseMenuScene();

        void Draw(sf::RenderWindow& rw) final override;
        void Update(const sf::Time& ur) final override;
        void HandleInput(const sf::Event& event) final override;
        void HandleNetworkInput(NetMessage* msg) final override;
        const std::string Name() const final override;

    protected:

    private:
        bool LoadScene() final override;
        bool UnloadScene() final override;

        void onClickContinue();
        void onClickExit();

        GameScene* m_gameScene;
        std::string m_exitName;
};

#endif // GAMEPAUSEMENUSCENE_HPP
