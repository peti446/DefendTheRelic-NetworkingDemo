#ifndef GAMELOBBYSCENE_H
#define GAMELOBBYSCENE_H

#include <Scene.hpp>

class GameLobbyScene : public Scene
{
    public:
        GameLobbyScene();
        virtual ~GameLobbyScene();

        void Draw(sf::RenderWindow& rw) final override;
        void Update(const sf::Time& ur) final override;
        void HandleInput(const sf::Event& event) final override;
        const std::string Name() const final override;

    protected:

    private:
        bool LoadScene() final override;
        bool UnloadScene() final override;
};

#endif // GAMELOBBYSCENE_H
