#ifndef GAMESCENE_HPP
#define GAMESCENE_HPP

#include <Scene.hpp>
#include <vector>
#include <unordered_map>

class Entity;
class PlayerEntity;


class GameScene : public Scene
{
    public:
        GameScene();
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
        void InstanciateBullet(PlayerEntity& whoIsShooting, float speed);

        std::vector<Entity*> m_bullets;
        std::vector<Entity*> m_Activebullets;
        std::unordered_map<std::string ,Entity*> m_players;
};

#endif // GAMESCENE_HPP
