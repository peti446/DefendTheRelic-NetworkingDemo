#ifndef GAMESCENE_HPP
#define GAMESCENE_HPP

#include <Scene.hpp>
#include <vector>


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

        std::vector<int>
};

#endif // GAMESCENE_HPP
