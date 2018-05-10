#ifndef ENDGAMEMENUSCENE_HPP
#define ENDGAMEMENUSCENE_HPP

#include <Scene.hpp>
class GameScene;

class EndGameMenuScene : public Scene
{
    public:
        EndGameMenuScene(GameScene* gameScene, std::string winningTeam, sf::Uint8 t1Kills, sf::Uint8 t2kills);
        virtual ~EndGameMenuScene();

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
        std::string m_winningTeam;
        sf::Uint8 m_t1_kills;
        sf::Uint8 m_t2_kills;
};

#endif // ENDGAMEMENUSCENE_HPP
