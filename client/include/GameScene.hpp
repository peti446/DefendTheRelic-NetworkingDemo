#ifndef GAMESCENE_HPP
#define GAMESCENE_HPP

#include <Scene.hpp>
#include <vector>
#include <unordered_map>

class Entity;
class BulletEntity;
class PlayerEntity;


class GameScene : public Scene
{
    public:
        GameScene(std::string t1_p1, std::string t1_p2, std::string t2_p1, std::string t2_p2, std::string gameID);
        virtual ~GameScene();

        void Draw(sf::RenderWindow& rw) final override;
        void Update(const sf::Time& ur) final override;
        void HandleNetworkInput(NetMessage* msg) final override;
        void HandleInput(const sf::Event& event) final override;
        const std::string Name() const final override;

    protected:

    private:
        bool LoadScene() final override;
        bool UnloadScene() final override;
        bool InstanciateBullet(PlayerEntity& whoIsShooting, float speed);
        void handlePlayerInput(sf::Keyboard::Key key, bool pressed);


        std::string m_t1_p1;
        std::string m_t1_p2;
        std::string m_t2_p1;
        std::string m_t2_p2;
        std::string m_gameID;
        std::vector<BulletEntity*> m_bullets;
        std::vector<BulletEntity*> m_Activebullets;
        std::unordered_map<std::string, PlayerEntity*> m_players;
};

#endif // GAMESCENE_HPP
